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

<<<<<<< HEAD
#include <macros.hpp>
#include <string>
#include <SDL.h>

jeBegin

=======
#include "Macro.h"
#include <string>
#include "SDL.h"

jeBegin

struct MAStats;
struct MAConfig;

>>>>>>> 4af9948ac99f35dbd94753136ac865176a80e124
//////////////////////////////////////////////////////////////////////////
// Main application class
//////////////////////////////////////////////////////////////////////////
class Application {

<<<<<<< HEAD
	// application data structure
	struct AppData {

		std::string	title;	// window title
		std::string icon;	// app icon directory
		bool isFullscreen = false;
		int	width = 800;
		int	height = 600;
=======
	friend class ImguiManager;
	friend class AssetManager;

	// Locked constuctor, destructor, assign operator
	jeStaticClassDeclaration(Application)

	//////////////////////////////////////////////////////////////////////////
	// Init data for application
	//////////////////////////////////////////////////////////////////////////
	struct AppData
	{
		std::string	title;			// Title
		std::string icon;			// Icon directory
		bool		isFullscreen;	// Flag of fullscr
		int			width;			// Width size
		int			height;			// Height size
>>>>>>> 4af9948ac99f35dbd94753136ac865176a80e124
	};

public:

<<<<<<< HEAD
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
=======
	static void Run(bool imgui);
	static void CreateConsole();
	static void CloseConsole();

	static void		ActivateVSync(bool on);
	static AppData	GetAppData();

private:

	static bool	Initialize();
	static void	Update();
	static void	Close();

	static bool InitSDL();
	static void CloseSDL();

	static void EditorUpdate(float dt);

	// Private variables
	static AppData			data_;					// Window config
	static SDL_Window*		pWindow_;				// SDL window
	static SDL_Event		pEvent_;				// SDL Event
	static SDL_GLContext	pContext_;				// SDL GL context
	static SDL_Surface		*pSurface_, *pIcon_;	// SDL screen surface
	static bool				activateIMGUI_, openCMD_;
>>>>>>> 4af9948ac99f35dbd94753136ac865176a80e124
};

using APP = Application;

jeEnd
