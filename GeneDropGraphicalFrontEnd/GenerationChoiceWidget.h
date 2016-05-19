#pragma once
#include <QWidget>
#include <vector>
#include <string>


class GenerationChoiceWidget : public QWidget {
	Q_OBJECT
public:
	GenerationChoiceWidget(std::vector<std::string>& toSet, const std::string& pedigreeFileName, QWidget* parent);
	~GenerationChoiceWidget();

	std::vector<std::string> chooseFromPedigree(const std::string& pedigreeFileName, const std::vector<std::string>& alreadySelected);

	static std::string generationVectorToString(const std::vector<std::string>& vec);
	static std::vector<std::string> stringToGenerationVector(const std::string& string);

signals:
	void message(const QString& what);
};

