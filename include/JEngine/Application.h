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

	friend class Core;

public:

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

	static void EditorUpdate(const float _dt);

	// Locked functions
	Application() = delete; 
	~Application() = delete;
	Application(const Application& /*app*/) = delete;
	void operator=(const Application& /*app*/) = delete;

	// Private variables
	static InitData			m_Data;		// Window config
	static SDL_Window*		m_pWindow;	// SDL window
	static SDL_Event		m_pEvent;	// SDL Event
	static SDL_GLContext	m_pContext;	// SDL GL context
	static SDL_Surface*		m_pSurface; // SDL screen surface
	static int				m_buffers, m_samples;
};

using APP = Application;

JE_END
