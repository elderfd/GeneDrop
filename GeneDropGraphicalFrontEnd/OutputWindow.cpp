#include "OutputWindow.h"
#include <QDateTime>


OutputWindow::OutputWindow(QWidget* parent) : QTextEdit(parent) {
	setReadOnly(true);
}


OutputWindow::~OutputWindow() {}


void OutputWindow::writeWithStamp(const QString& text) {
	append(timeStamp() + text);
}


QString OutputWindow::timeStamp() const {
	return QDateTime::currentDateTime().toString("dd/MM HH:mm:ss - ");
}