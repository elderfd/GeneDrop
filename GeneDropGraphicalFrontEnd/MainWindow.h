#pragma once
#include <qmainwindow.h>


class MainWindow :
	public QMainWindow {
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

signals:
	void message(const QString& what);

private:
	QWidget* centralWidget;

	void buildDefaultUI();

	void run(const ProgramOptions& options);

	QString chooseFile() const;
};

