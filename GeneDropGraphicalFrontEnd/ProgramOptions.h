#pragma once
#include <limits>
#include <string>
#include <QWidget>


class ProgramOptions {
public:
	ProgramOptions();
	~ProgramOptions();

	static const int maximumNumberOfRuns;
	static const int maximumNumberOfThreads;

	unsigned int numberOfRuns = 1;
	unsigned int numberOfThreads = 1;
	std::string pedigreeFileName;
	std::string genotypeFileName;
	std::string lociFileName;

	std::string outputDirectory;
	std::vector<std::string> generationsToPrint;
};


class OptionsWidget : public QWidget {
	Q_OBJECT
public:
	OptionsWidget(QWidget* parent);
	~OptionsWidget();

	ProgramOptions options;

signals:
	void message(const QString& what);
};