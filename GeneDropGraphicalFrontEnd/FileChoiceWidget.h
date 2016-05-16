#pragma once
#include <QWidget>
#include <string>


class FileChoiceWidget :
	public QWidget {
	Q_OBJECT;
public:
	enum Type {
		OPEN_DIR,
		SAVE_FILE,
		OPEN_FILE
	};

	FileChoiceWidget(std::string& toSet, Type type, QWidget* parent);
	~FileChoiceWidget();

private:
	std::string& toSet;
	Type type;
};

