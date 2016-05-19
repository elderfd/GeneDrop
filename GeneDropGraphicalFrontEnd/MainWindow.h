#pragma once
#include <qmainwindow.h>
#include "ProgramOptions.h"


class QTextEdit;


class MainWindow :
	public QMainWindow {
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

	void runWithCurrentOptions();

	void printMessage(const QString& message);

signals:
	void message(const QString& what);

private:
	QWidget* centralWidget;

	void buildDefaultUI();

	void run(const ProgramOptions& options);

	OptionsWidget* settingsWidget;

	QTextEdit* outputWindow;
};

