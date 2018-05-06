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

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// Main application class
//////////////////////////////////////////////////////////////////////////
class Application {

	friend class ImguiManager;
	friend class AssetManager;

	// Locked constuctor, destructor, assign operator
	JE_THIS_IS_STATIC_CLASS(Application)

public:

	static void Run(bool _imgui);

private:

	//////////////////////////////////////////////////////////////////////////
	// Init data for application
	//////////////////////////////////////////////////////////////////////////
	struct InitData
	{
		std::string	m_title;		// Title
		bool		m_isFullScreen;	// Flag of fullscr
		int			m_width;		// Width size
		int			m_height;		// Height size
	};

	static bool	Initialize();
	static void	Update();
	static void	Close();

	static bool InitSDL();
	static void CloseSDL();

	static void EditorUpdate(const float _dt);

	// Private variables
	static InitData			m_Data;		// Window config
	static SDL_Window*		m_pWindow;	// SDL window
	static SDL_Event		m_pEvent;	// SDL Event
	static SDL_GLContext	m_pContext;	// SDL GL context
	static SDL_Surface*		m_pSurface; // SDL screen surface
	static int				m_buffers, m_samples;
	static bool				m_IMGUI;
};

using APP = Application;

JE_END
