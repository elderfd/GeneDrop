#include "MainWindow.h"
#include "SimulationManager.h"
#include <QFileDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include "OutputMaker.h"
#include <sstream>
#include <thread>


MainWindow::MainWindow() {
	buildDefaultUI();
}


MainWindow::~MainWindow() {}


void MainWindow::buildDefaultUI() {
	centralWidget = new QWidget(this);
	auto layout = new QVBoxLayout(centralWidget);

	settingsWidget = new OptionsWidget(this);

	layout->addWidget(settingsWidget);

	auto goButton = new QPushButton("Run");

	layout->addWidget(goButton);

	auto outputWindow = new QTextEdit(this);
	outputWindow->setReadOnly(true);

	connect(this, &MainWindow::message, [outputWindow](const QString& what) {
		auto message = SimulationManager::makeTimeStamp() + ": " + what.toStdString();
		outputWindow->append(QString(message.c_str()));
	});
	connect(goButton, &QPushButton::pressed, this, &MainWindow::runWithCurrentOptions);

	layout->addWidget(outputWindow);

	auto clearButton = new QPushButton("Clear output window", this);
	connect(clearButton, &QPushButton::pressed, outputWindow, &QTextEdit::clear);
	layout->addWidget(clearButton);

	this->setCentralWidget(centralWidget);
}


void MainWindow::run(const ProgramOptions& options) {
	SimulationManager simManager;

	try {
		simManager.buildPedigreeFromFile(options.pedigreeFileName);
		simManager.buildStartingStateFromFiles(options.lociFileName, options.genotypeFileName);

		std::string outputFileName = options.outputDirectory + "/Output(" + SimulationManager::makeTimeStamp() + ").csv";
		OutputMaker out;
		int reportEvery = 50;

		if (options.generationsToPrint.size() > 0) out.printOnlyCertainGenerations(options.generationsToPrint);

		if (out.open(outputFileName)) {
			try {
				for (unsigned int i = 0; i < options.numberOfRuns; i++) {
					auto result = simManager.getRealisation();

					out << result;

					if ((i + 1) % reportEvery == 0 || i == options.numberOfRuns - 1) {
						std::stringstream message;
						message << "Done " << i + 1 << " runs out of " << options.numberOfRuns << std::endl;
						emit this->message(QString(message.str().c_str()));
					}
				}
			} catch (std::exception& e) {
				emit this->message(QString(e.what()));
			}

			out.close();
		}
	} catch (std::exception& e) {
		emit this->message(QString(e.what()));
		return;
	}
}


void MainWindow::runWithCurrentOptions() {
	std::thread t(&MainWindow::run, this, settingsWidget->options);
	t.detach();
}