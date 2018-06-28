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
		std::string	m_title;		// Title
		std::string m_icon;			// Icon directory
		bool		m_isFullScreen;	// Flag of fullscr
		int			m_width;		// Width size
		int			m_height;		// Height size
	};

public:

	static void Run(bool _imgui);
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
	static AppData			m_Data;					// Window config
	static SDL_Window*		m_pWindow;				// SDL window
	static SDL_Event		m_pEvent;				// SDL Event
	static SDL_GLContext	m_pContext;				// SDL GL context
	static SDL_Surface		*m_pSurface, *m_pIcon;	// SDL screen surface
	static int				m_buffers, m_samples;
	static bool				m_IMGUI, m_openCMD;
};

using APP = Application;

jeEnd
