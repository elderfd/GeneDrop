#include "ProgramOptions.h"
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include "FileChoiceWidget.h"
#include "GenerationChoiceWidget.h"
#include <QFrame>
#include <QCheckBox>


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

	auto genotypeFileWidget = new FileChoiceWidget(options.genotypeFileName, FileChoiceWidget::OPEN_FILE, this);
	layout->addWidget(genotypeFileWidget, 1, 1);

	auto pedigreeFileWidget = new FileChoiceWidget(options.pedigreeFileName, FileChoiceWidget::OPEN_FILE, this);
	layout->addWidget(pedigreeFileWidget, 2, 1);

	auto lociFileWidget = new FileChoiceWidget(options.lociFileName, FileChoiceWidget::OPEN_FILE, this);
	layout->addWidget(lociFileWidget, 3, 1);

	layout->setColumnMinimumWidth(2, 20);

	layout->addWidget(new QLabel("Output directory:"), 0, 3);
	auto outputFolderWidget = new FileChoiceWidget(options.outputDirectory, FileChoiceWidget::OPEN_DIR, this);
	layout->addWidget(outputFolderWidget, 0, 4);

	auto generationChoiceWidget = new GenerationChoiceWidget(options.generationsToPrint, options.pedigreeFileName, this);
	layout->addWidget(generationChoiceWidget, 1, 3, 1, 2);

	connect(generationChoiceWidget, &GenerationChoiceWidget::message, [this](const QString& message) {
		emit this->message(message);
	});

	auto advancedButton = new QCheckBox("Show advanced options", this);

	layout->addWidget(advancedButton, 4, 0, 1, 4);

	auto advancedArea = new QFrame(this);
	advancedArea->setVisible(false);
	layout->addWidget(advancedArea, 5, 0, 1, 2);

	auto advancedLayout = new QGridLayout(advancedArea);

	auto numberOfThreadsSpinner = new QSpinBox(this);
	numberOfThreadsSpinner->setMaximum(ProgramOptions::maximumNumberOfThreads);
	numberOfThreadsSpinner->setMinimum(1);
	connect(numberOfThreadsSpinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int val) {
		options.numberOfThreads = val;
	});
	advancedLayout->addWidget(new QLabel("Number of threads:"), 0, 0);
	advancedLayout->addWidget(numberOfThreadsSpinner, 0, 1);

	connect(advancedButton, &QCheckBox::stateChanged, advancedArea, &QFrame::setVisible);
}


OptionsWidget::~OptionsWidget() {}