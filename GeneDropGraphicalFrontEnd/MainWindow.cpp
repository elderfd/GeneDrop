#include "MainWindow.h"
#include "ProgramOptions.h"
#include "SimulationManager.h"
#include <QFileDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>


MainWindow::MainWindow() {
	buildDefaultUI();
}


MainWindow::~MainWindow() {}


void MainWindow::buildDefaultUI() {
	centralWidget = new QWidget(this);
	auto layout = new QVBoxLayout(centralWidget);

	auto settingsWidget = new OptionsWidget(this);

	layout->addWidget(settingsWidget);

	auto goButton = new QPushButton("Run");

	layout->addWidget(goButton);

	auto outputWindow = new QTextEdit(this);

	connect(this, &MainWindow::message, outputWindow, &OutputWindow::)

	layout->addWidget(outputWindow);

	this->setCentralWidget(centralWidget);
}


void MainWindow::run(const ProgramOptions& options) {
	SimulationManager simManager;

	try {
		simManager.buildPedigreeFromFile(options.pedigreeFileName);
		simManager.buildStartingStateFromFiles(options.lociFileName, options.genotypeFileName);
	} catch (std::exception& e) {
		
		std::cout << e.what() << std::endl;
		return;
	}
}