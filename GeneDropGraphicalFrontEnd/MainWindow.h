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
	QPushButton* goButton;

	bool simulationsRunning = false;
};

