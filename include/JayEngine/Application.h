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
#include "SDL2\SDL.h"

NS_JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// Main application class
//////////////////////////////////////////////////////////////////////////
class Application {

public:

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

	Application(const InitData& _data);
	
	static bool Initialize();
	static void Update();
	static void Close();

private:

	// Locked functions
	~Application() {};
	Application(const Application& /*app*/) {};
	void operator=(const Application& /*app*/) {};

	// Private variables
	static InitData		m_pData;	// Window config
	static SDL_Window*	m_pWindow;	// SDL window
	static SDL_Event	m_pEvent;	// SDL Event
};

NS_JE_END