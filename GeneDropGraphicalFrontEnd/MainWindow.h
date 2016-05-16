#pragma once
#include <qmainwindow.h>
#include "ProgramOptions.h"


class MainWindow :
	public QMainWindow {
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

	void runWithCurrentOptions();

signals:
	void message(const QString& what);

private:
	QWidget* centralWidget;

	void buildDefaultUI();

	void run(const ProgramOptions& options);

	OptionsWidget* settingsWidget;
};

