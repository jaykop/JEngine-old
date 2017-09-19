#pragma once

#include "GL/glew.h"
#define GLEW_STATIC

#include "Macro.h"
NS_JE_BEGIN

class GLManager {

public:

	static bool initSDL_GL();
	static void CloseSDL_GL();

private:

	// Locked functions
	GLManager() {};
	~GLManager() {};
	GLManager(const GLManager& /*app*/) {};
	void operator=(const GLManager& /*app*/) {};

};

NS_JE_END
