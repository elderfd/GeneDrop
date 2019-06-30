#pragma once
#include <QString>
#include <QTextEdit>


class OutputWindow :
	public QTextEdit {
	Q_OBJECT
public:
	OutputWindow(QWidget* parent);
	~OutputWindow();

	void writeWithStamp(const QString& text);

protected:
	QString timeStamp() const;
};

