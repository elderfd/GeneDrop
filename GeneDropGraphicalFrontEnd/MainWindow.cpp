#include "MainWindow.h"
#include <QPushButton>
#include <QVBoxLayout>


MainWindow::MainWindow() {
	buildDefaultUI();
}


MainWindow::~MainWindow() {}


void MainWindow::buildDefaultUI() {
	centralWidget = new QWidget(this);
	auto layout = new QVBoxLayout(centralWidget);

	auto controlLayout = new QHBoxLayout();

	

	layout->addLayout(controlLayout);

	auto goButton = new QPushButton("Run");

	layout->addWidget(goButton);

}
