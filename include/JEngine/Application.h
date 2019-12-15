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

#include "Macro.h"
#include <string>
#include "SDL.h"

jeBegin

struct MAStats;
struct MAConfig;

//////////////////////////////////////////////////////////////////////////
// Main application class
//////////////////////////////////////////////////////////////////////////
class Application {

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
	};

public:

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
};

using APP = Application;

jeEnd
