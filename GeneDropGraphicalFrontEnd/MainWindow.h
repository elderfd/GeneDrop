#pragma once
#include <qmainwindow.h>
#include "ProgramOptions.h"


class QTextEdit;
class QPushButton;


class MainWindow :
	public QMainWindow {
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

	void runWithCurrentOptions();

	void printMessage(const QString& message);

	void disallowSimulations();
	void allowSimulations();

	void stopSimulations();

signals:
	void message(const QString& what);
	void simulationsStarted();
	void simulationsStopped();

private:
	QWidget* centralWidget;

	void buildDefaultUI();

	void run(const ProgramOptions& options);

	OptionsWidget* settingsWidget;

	QTextEdit* outputWindow;
	QPushButton* goButton, *stopButton;

	bool simulationsRunning = false;
	bool stopDemanded = false;
};

