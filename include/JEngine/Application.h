/******************************************************************************/
/*!
\file   Application.h
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2017/08/23(yy/mm/dd)

\description
Contains Application's class and member functions

*/
/******************************************************************************/
#pragma once

#include <macros.hpp>
#include <string>
#include <SDL.h>

jeBegin

//////////////////////////////////////////////////////////////////////////
// Main application class
//////////////////////////////////////////////////////////////////////////
class Application {

	// application data structure
	struct AppData {

		std::string	title;	// window title
		std::string icon;	// app icon directory
		bool isFullscreen = false;
		int	width = 800;
		int	height = 600;
	};

public:

	static void run();
	static void quit();

	static AppData get_appdata();
	static void set_screensize(int w, int h);
	static void set_title(const std::string& title);
	static void set_fullscreen(bool isFullScreen);

private:

	static bool initialize();
	static void update();
	static void close();

	static void rendering_thread();

	// Private variables
	static AppData data_;
	static SDL_Window* window_;				// SDL window
	static SDL_Event event_;				// SDL Event
	static SDL_GLContext context_;			// SDL GL context
	static SDL_Surface *surface_, *icon_;	// SDL screen surface
	static bool run_;

	// Prevent to clone this class
	Application() = delete;
	~Application() = delete;
	Application(Application&&) = delete;
	Application(const Application&) = delete;
	Application& operator= (Application&&) = delete;
	Application& operator= (const Application&) = delete;
};

using APP = Application;

jeEnd
