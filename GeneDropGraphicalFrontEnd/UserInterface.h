#pragma once
#include "application.h"
#include "view.h"
#include <Awesomium/WebCore.h>
#include <Awesomium/STLHelpers.h>
#include "MethodDispatcher.h"
#ifdef _WIN32
#include <Windows.h>
#endif

class UserInterface : public Application::Listener
{
public:
	UserInterface();
	virtual ~UserInterface();

	void run() {
		app_->run();
	}

	// Inherited from Application::Listener
	virtual void OnLoaded() {
		view_ = View::Create(500, 300);

		bindMethods(view_->web_view());

		// TODO: Clean up data paths
		Awesomium::WebURL url(Awesomium::WSLit("file:///E:/Work/GeneDrop/NewVersion/GeneDropGraphicalFrontEnd/mainView.html"));
		view_->web_view()->LoadURL(url);
	}

	void bindMethods(Awesomium::WebView* webView)
	{
		Awesomium::JSValue result = webView->CreateGlobalJavascriptObject(interfaceName);

		if (result.IsObject())
		{
			Awesomium::JSObject& app = result.ToObject();

			dispatcher.Bind(app, Awesomium::WSLit("go"), JSDelegate(this, &UserInterface::onGoButtonPressed));
		}

		view_->web_view()->set_js_method_handler(&dispatcher);
	}

	void onGoButtonPressed(Awesomium::WebView* caller, const Awesomium::JSArray& args)
	{
		go();
	}

	void go();

	void printToMainOutputBox(std::string message);

	// Inherited from Application::Listener
	virtual void OnUpdate() {

	}

	// Inherited from Application::Listener
	virtual void OnShutdown() {
	}

protected:
	Application* app_;
	View* view_;
	MethodDispatcher dispatcher;

	static const Awesomium::WebString interfaceName;
};