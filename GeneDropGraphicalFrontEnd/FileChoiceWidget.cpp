#include "FileChoiceWidget.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <functional>


FileChoiceWidget::FileChoiceWidget(std::string& toSet, Type type, QWidget* parent) : toSet(toSet), QWidget(parent) {
	auto layout = new QHBoxLayout(this);

	auto input = new QLineEdit(this);
	auto button = new QPushButton("Choose file", this);

	layout->addWidget(input);
	layout->addWidget(button);

	std::function<QString(void)> chooseFunc = nullptr;

	switch (type) {
		case OPEN_DIR:
			chooseFunc = [this]() {
				return QFileDialog::getExistingDirectory(this);
			};
			break;
		case SAVE_FILE:
			chooseFunc = [this]() {
				return QFileDialog::getSaveFileName(this);
			};
			break;
		case OPEN_FILE:
			chooseFunc = [this]() {
				return QFileDialog::getOpenFileName(this);
			};
			break;
	}

	connect(button, &QPushButton::clicked, [this, &toSet, input, chooseFunc]() {
		auto fileName = chooseFunc();
		if (!fileName.isEmpty()) {
			toSet = fileName.toStdString();
			input->setText(fileName);
		}
	});
	
	connect(input, &QLineEdit::textChanged, [this, &toSet, input]() {
		toSet = input->text().toStdString();
	});
}


FileChoiceWidget::~FileChoiceWidget() {}
