#include "UserInterface.h"

const Awesomium::WebString UserInterface::interfaceName = Awesomium::WSLit("appInterface");

UserInterface::UserInterface() : app_(Application::Create()), view_(0)
{
	app_->set_listener(this);
	
}


UserInterface::~UserInterface()
{
	if (view_)
		app_->DestroyView(view_);
	if (app_)
		delete app_;
}


void UserInterface::printToMainOutputBox(std::string message)
{
	auto convertedString = Awesomium::WSLit(message.c_str());

	// Grab the window object from the page
	Awesomium::JSValue window = view_->web_view()->ExecuteJavascriptWithResult(Awesomium::WSLit("window"), Awesomium::WSLit(""));

	// Call some javascript to print out the message
	if (window.IsObject())
	{
		Awesomium::JSArray args;
		args.Push(convertedString);

		window.ToObject().Invoke(Awesomium::WSLit("outputText"), args);
	}
}


void UserInterface::go()
{
	printToMainOutputBox("Building simulations.\n");
	
	SimulationManagerFactory managerFactory;

	// TODO: Get input data

	std::string pedigreeFileName = "pedigree.csv", genotypeFileName = "founders.csv", lociFileName = "loci.csv";
	int numberOfRuns = 1;
	int numberOfThreads = 1;

	// Set up an appropriate simulation state
	SimulationManager simManager = managerFactory.createFromSimpleInput(pedigreeFileName, genotypeFileName, lociFileName, numberOfRuns, numberOfThreads);

	// Verify the input
	Maybe<std::string> error = simManager.verifySimulationPrototype();

	if (error)
	{
		// Push out any error message
		printToMainOutputBox(error.value());
	}
	else
	{
		printToMainOutputBox("Running simulations. \n");

		// Do some sims
		simManager.run();

		// Get some output
		std::string outputFileName = "Output(" + SimulationManager::makeTimeStamp() + ").csv";

		simManager.outputResultsToFile(outputFileName);

		printToMainOutputBox("Simulations done.\n");
	}
}
