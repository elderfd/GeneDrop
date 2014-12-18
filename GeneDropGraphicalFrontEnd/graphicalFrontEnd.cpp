#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>

int main(int argc, char *argv[])
{
	// Initialise the web core
	Awesomium::WebConfig coreConfig;
	Awesomium::WebCore* core = Awesomium::WebCore::Initialize(coreConfig);

	// Clean up the webcore
	Awesomium::WebCore::Shutdown();

	return 0;
}