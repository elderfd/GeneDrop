#include "ProgramOptions.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

const int ProgramOptions::maximumNumberOfRuns = std::numeric_limits<int>::max();
const int ProgramOptions::maximumNumberOfThreads = 8;


ProgramOptions::ProgramOptions() {}


ProgramOptions::~ProgramOptions() {}


OptionsWidget::OptionsWidget(QWidget* parent) : QWidget(parent) {
	auto layout = new QGridLayout(this);

	layout->addWidget(new QLabel("Number of runs:"), 0, 0);
	layout->addWidget(new QLabel("Genotype file:"), 1, 0);
	layout->addWidget(new QLabel("Pedigree file:"), 2, 0);
	layout->addWidget(new QLabel("Loci file:"), 3, 0);

	auto numberOfRunsSpinner = new QSpinBox(this);
	numberOfRunsSpinner->setMaximum(ProgramOptions::maximumNumberOfRuns);
	numberOfRunsSpinner->setMinimum(1);
	connect(numberOfRunsSpinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int val) {
		options.numberOfRuns = val;
	});

	layout->addWidget(numberOfRunsSpinner, 0, 1);

	auto genotypeFileButton = new QPushButton("Choose file", this);
	connect(genotypeFileButton, &QPushButton::clicked, [this]() {
		auto fileName = QFileDialog::getOpenFileName(this);
		if (!fileName.isEmpty()) options.genotypeFileName = fileName.toStdString();
	});

	layout->addWidget(genotypeFileButton, 1, 1);

	auto pedigreeFileButton = new QPushButton("Choose file", this);
	connect(pedigreeFileButton, &QPushButton::clicked, [this]() {
		auto fileName = QFileDialog::getOpenFileName(this);
		if (!fileName.isEmpty()) options.pedigreeFileName = fileName.toStdString();
	});

	layout->addWidget(pedigreeFileButton, 2, 1);

	auto lociFileButton = new QPushButton("Choose file", this);
	connect(lociFileButton, &QPushButton::clicked, [this]() {
		auto fileName = QFileDialog::getOpenFileName(this);
		if (!fileName.isEmpty()) options.lociFileName = fileName.toStdString();
	});

	layout->addWidget(lociFileButton, 3, 1);

	auto advancedButton = new QPushButton("Advanced", this);

	layout->addWidget(advancedButton, 4, 0, 1, 2);
}


OptionsWidget::~OptionsWidget() {}