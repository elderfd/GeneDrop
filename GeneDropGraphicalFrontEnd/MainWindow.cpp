#include "MainWindow.h"
#include "SimulationManager.h"
#include <QFileDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include "OutputMaker.h"
#include <sstream>
#include <thread>
#include <mutex>
#include <QScrollBar>


MainWindow::MainWindow() {
	buildDefaultUI();
}


MainWindow::~MainWindow() {}


void MainWindow::buildDefaultUI() {
	centralWidget = new QWidget(this);
	auto layout = new QVBoxLayout(centralWidget);

	settingsWidget = new OptionsWidget(this);

	layout->addWidget(settingsWidget);

	goButton = new QPushButton("Run");

	layout->addWidget(goButton);

	outputWindow = new QTextEdit(this);
	outputWindow->setReadOnly(true);

	connect(settingsWidget, &OptionsWidget::message, this, &MainWindow::printMessage);
	connect(goButton, &QPushButton::pressed, this, &MainWindow::runWithCurrentOptions);

	layout->addWidget(outputWindow);

	auto clearButton = new QPushButton("Clear output window", this);
	connect(clearButton, &QPushButton::pressed, outputWindow, &QTextEdit::clear);
	layout->addWidget(clearButton);

	connect(this, &MainWindow::simulationsStarted, this, &MainWindow::disallowSimulations, Qt::BlockingQueuedConnection);
	connect(this, &MainWindow::simulationsStopped, this, &MainWindow::allowSimulations, Qt::BlockingQueuedConnection);
	connect(this, &MainWindow::message, this, &MainWindow::printMessage);

	this->setCentralWidget(centralWidget);
}


void MainWindow::run(const ProgramOptions& options) {
	if (simulationsRunning) return;
	
	simulationsRunning = true;
	emit simulationsStarted();

	SimulationManager simManager;

	unsigned int numberOfRunsComplete = 0;
	int numberOfRunningThreads = 0;

	std::mutex outputMutex;

	try {
		simManager.build(options.pedigreeFileName, options.lociFileName, options.genotypeFileName);

		std::string outputFileName = options.outputDirectory + "/Output(" + SimulationManager::makeTimeStamp() + ").csv";
		int reportEvery = 50;

		auto runAndWrite = [&]() {
			auto result = simManager.getRealisation();

			OutputMaker out;

			{
				std::lock_guard<std::mutex> guard(outputMutex);

				if (out.open(outputFileName, numberOfRunsComplete)) {
					if (options.generationsToPrint.size() > 0) out.printOnlyCertainGenerations(options.generationsToPrint);

					out << result;

					if ((numberOfRunsComplete + 1) % reportEvery == 0 || numberOfRunsComplete == options.numberOfRuns - 1) {
						std::stringstream messageStr;
						messageStr << "Done " << numberOfRunsComplete + 1 << " runs out of " << options.numberOfRuns << std::endl;
						emit message(QString(messageStr.str().c_str()));
					}

					out.close();
				}

				++numberOfRunsComplete;
			}

			--numberOfRunningThreads;
		};

		while (numberOfRunsComplete < options.numberOfRuns) {
			if (numberOfRunningThreads < (int)options.numberOfThreads) {
				++numberOfRunningThreads;
				std::thread newThread(runAndWrite);
				newThread.detach();
			}
		}

	} catch (std::exception& e) {
		emit message(QString(e.what()));
		return;
	}

	emit simulationsStopped();
	simulationsRunning = false;
}


void MainWindow::runWithCurrentOptions() {
	std::thread t(&MainWindow::run, this, settingsWidget->options);
	t.detach();
}


void MainWindow::printMessage(const QString& what) {
	auto message = SimulationManager::makeTimeStamp() + ": " + what.toStdString();
	
	bool moveToEnd = false;
	if (outputWindow->verticalScrollBar()->value() == outputWindow->verticalScrollBar()->maximum()) {
		moveToEnd = true;
	}
	
	outputWindow->append(QString(message.c_str()));
	if(moveToEnd) outputWindow->moveCursor(QTextCursor::End);
}


void MainWindow::disallowSimulations() {
	goButton->setEnabled(false);
}


void MainWindow::allowSimulations() {
	goButton->setEnabled(true);
}