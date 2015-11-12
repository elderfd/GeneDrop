#pragma once
#include <qmainwindow.h>


class MainWindow :
	public QMainWindow {
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

private:
	QWidget* centralWidget;

	void buildDefaultUI();
};

