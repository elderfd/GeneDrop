#include "GenerationChoiceWidget.h"
#include <QGridLayout>
#include <QLineEdit>
#include "SimulationManager.h"
#include <QListWidget>
#include <QStringList>
#include <QCheckBox>
#include <QDialog>
#include <QLabel>
#include <QPushButton>


GenerationChoiceWidget::GenerationChoiceWidget(std::vector<std::string>& toSet, const std::string& pedigreeFileName, QWidget* parent) : QWidget(parent) {
	auto layout = new QGridLayout(this);
	auto label = new QLabel("Generations to output:", this);
	auto textInput = new QLineEdit(this);
	auto checkBox = new QCheckBox("Output all generations", this);
	auto chooseButton = new QPushButton("Choose from pedigree", this);

	connect(checkBox, &QCheckBox::stateChanged, [textInput, chooseButton, &toSet](int state) {
		textInput->setEnabled(!state);
		chooseButton->setEnabled(!state);

		if (!state) {
			toSet.clear();
		} else {
			toSet = GenerationChoiceWidget::stringToGenerationVector(textInput->text().toStdString());
		}
	});

	connect(chooseButton, &QPushButton::pressed, [this, &pedigreeFileName, &toSet, textInput]() {
		auto chosen = this->chooseFromPedigree(pedigreeFileName, toSet);
		toSet = chosen;

		textInput->setText(QString(GenerationChoiceWidget::generationVectorToString(chosen).c_str()));
	});

	connect(textInput, &QLineEdit::editingFinished, [textInput, &toSet]() {
		toSet = GenerationChoiceWidget::stringToGenerationVector(textInput->text().toStdString());
	});

	checkBox->setChecked(true);

	layout->addWidget(checkBox, 0, 0, 1, 3);
	layout->addWidget(label, 1, 0);
	layout->addWidget(textInput, 1, 1);
	layout->addWidget(chooseButton, 1, 2);
}


GenerationChoiceWidget::~GenerationChoiceWidget() {}


std::vector<std::string> GenerationChoiceWidget::chooseFromPedigree(const std::string& pedigreeFileName, const std::vector<std::string>& alreadySelected) {
	std::vector<std::string> retVec;

	try {
		auto pedigree = SimulationManager::buildPedigreeFromFile(pedigreeFileName);
		auto allGenerations = pedigree.getAllGenerations();

		QStringList choices;
		for (const auto& gen : allGenerations) {
			// Remove the "empty" founder generation
			if(!gen.empty()) {
				choices.append(QString(gen.c_str()));
			}
		}

		QDialog dialog(this);
		dialog.setWindowTitle("Choose from pedigree");
		dialog.setSizeGripEnabled(true);
		QVBoxLayout l(&dialog);
		QListWidget choiceWidget(&dialog);
		choiceWidget.setSelectionMode(QAbstractItemView::MultiSelection);
		l.addWidget(&choiceWidget);
		choiceWidget.insertItems(0, choices);

		for (auto i = 0; i < choiceWidget.count(); i++) {
			auto item = choiceWidget.item(i);

			if (std::find(alreadySelected.begin(), alreadySelected.end(), item->text().toStdString()) != alreadySelected.end()) {
				item->setSelected(true);
			}
		}

		QHBoxLayout buttonL;

		QPushButton okButton("Ok");
		buttonL.addWidget(&okButton);

		QPushButton cancelButton("Cancel");
		buttonL.addWidget(&cancelButton);

		connect(&okButton, &QPushButton::pressed, &dialog, &QDialog::accept);
		connect(&cancelButton, &QPushButton::pressed, &dialog, &QDialog::reject);

		l.addLayout(&buttonL);

		int returnValue = dialog.exec();

		if (returnValue == QDialog::Accepted) {
			for (auto& index : choiceWidget.selectionModel()->selectedIndexes()) {
				retVec.push_back(index.data().toString().toStdString());
			}
		} else {
			retVec = alreadySelected;
		}
	} catch (std::exception& e) {
		emit message(QString(e.what()));
	}

	return retVec;
}


std::string GenerationChoiceWidget::generationVectorToString(const std::vector<std::string>& vec) {
	std::string ret;
	bool begin = true;

	for (const auto& subStr : vec) {
		if (begin) {
			begin = false;
		} else {
			ret += ", ";
		}
		ret += subStr;
	}

	return ret;
}


std::vector<std::string> GenerationChoiceWidget::stringToGenerationVector(const std::string& string) {
	std::vector<std::string> tokens;

	if (!string.empty()) {
		std::size_t start = 0, end = 0;

		while ((end = string.find(",", start)) != std::string::npos) {
			std::string tok = string.substr(start, end - start);

			auto leftMost = string.find_first_not_of(" \n\r\t");

			if (leftMost != std::string::npos) {
				auto rightMost = string.find_last_not_of(" \n\r\t");
				tok = tok.substr(leftMost, rightMost - leftMost);
				tokens.push_back(tok);
			}

			start = end + 1;
		}
		tokens.push_back(string.substr(start));
	}
	
	return tokens;
}
