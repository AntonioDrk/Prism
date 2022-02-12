// Precompiled headers
#include "pch.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <collection.h>
#include "MyEngine.h"
#include "Timer.h"
#include "Input.h"


//Common namespaces
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;
using namespace DX;

// Class definition of the core "framework" of the app
ref class App sealed : public IFrameworkView
{
	bool isWindowClosed;
	bool isWindowVisible;
	BasicGameEngine BGEngine;
	
public:

	// Function called by Windows
	virtual void Initialize(CoreApplicationView^ AppView)
	{
		// set the OnActivated function to handle to Activated "event"
		AppView->Activated += ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated);

		CoreApplication::Suspending += ref new EventHandler<SuspendingEventArgs^>(this, &App::OnSuspended);
		CoreApplication::Resuming += ref new EventHandler<Object^>(this, &App::OnResumed);

		isWindowClosed = false;
		isWindowVisible = true;
	}

	virtual void SetWindow(CoreWindow^ Window)
	{
		// Bin relevant events of the main window
		Window->PointerPressed += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::OnPointerPressed);
		Window->PointerMoved += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::OnPointerMoved);
		Window->PointerWheelChanged += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &App::OnPointerWheelChanged);
		Window->VisibilityChanged += ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &App::OnVisibilityChanged);
		Window->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &App::OnKeyDown);
		Window->KeyUp += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &App::OnKeyUp);

		Window->Closed += ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &App::OnWindowClosed);
	}

	virtual void Load(String^ EntryPoint){}
	virtual void Run()
	{
		BGEngine.Initialize();

		// Obtaining the current window
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();

		// C++/CX
		Timer^ timer = ref new Timer();
		
		// Repeat event processing until window closes
		while (!isWindowClosed)
		{
			if (isWindowVisible) 
			{
				timer->Update();
				// Run ProcessEvents() to dispatch events and process them all 
				// if there are any present, then return to this loop
				Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
				BGEngine.Update(timer->Total, timer->Delta);
				BGEngine.Render();
				/*OutputDebug(timer->Delta.ToString() + L"\n");
				OutputDebug(timer->Total.ToString() + L"\n");*/
			}
			else 
			{
				Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
			}
		}
	}

	virtual void Uninitialize(){}

	// "Event" that is called when the application window is ready to be activated
	void OnActivated(CoreApplicationView^ CoreAppView, IActivatedEventArgs^ Args)
	{
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		Window->Activate();
		isWindowVisible = true;
	}

	void OnWindowClosed(CoreWindow^ Window, CoreWindowEventArgs^ Args)
	{
		isWindowClosed = true;
	}

	void OnVisibilityChanged(CoreWindow^ Window, VisibilityChangedEventArgs^ Args)
	{
		isWindowVisible = Args->Visible;
	}

	void OnPointerPressed(CoreWindow^ Window, PointerEventArgs^ Args)
	{
		// Test code to see that the event is called
		int px = int(Args->CurrentPoint->Position.X);
		int py = int(Args->CurrentPoint->Position.Y);
		MessageDialog Dialog("Hey did you just click on your mouse on " + px + " " +
							 py + " ? No way!Awesome.", "Announcement!");
		Dialog.ShowAsync();	
	}

	// On movement of the mouse pointer
	void OnPointerMoved(CoreWindow^ Window, PointerEventArgs^ Args){}

	// On Scroll Wheel Action
	void OnPointerWheelChanged(CoreWindow^ Window, PointerEventArgs^ Args)
	{
		int Wheel = Args->CurrentPoint->Properties->MouseWheelDelta;
		if (Wheel > 0)
		{
			MessageDialog Dialog("Wheel Up!", "Announcement!");
			Dialog.ShowAsync();
		}
		else if(Wheel < 0)
		{
			MessageDialog Dialog("Wheel Down!", "Announcement!");
			Dialog.ShowAsync();
		}
	}

	void OnKeyDown(CoreWindow^ Window, KeyEventArgs^ Args)
	{
		Input::SetKeyDown(Args->VirtualKey);
	}

	void OnKeyUp(CoreWindow^ Window, KeyEventArgs^ Args)
	{
		Input::SetKeyUp(Args->VirtualKey);
	}

	// TODO: Save state of the application here !
	void OnSuspended(Object^ Sender, SuspendingEventArgs^ Args)
	{
		MessageDialog Dialog("I went into suspended mode . . .", "Announcement!");
		Dialog.ShowAsync();
	}
	
	// Called when application is resumed by the OS
	void OnResumed(Object^ Sender, Object^ Args)
	{
		MessageDialog Dialog("I went into suspended mode . . .", "Announcement!");
		Dialog.ShowAsync();
	}

};

// class definition that creates an instance of our core framework class
ref class AppSource sealed : IFrameworkViewSource
{
public:
	virtual IFrameworkView^ CreateView()
	{
		return ref new App();	// Create an App class and return it
	}
};

[MTAThread] // define the main() as a multi-threaded-apartment function
int main(Array<String^>^ args)
{
	CoreApplication::Run(ref new AppSource());
	return 0;
}