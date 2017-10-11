#pragma once
#include "Macro.h"
//#include "SDL2\SDL_image.h"

NS_JE_BEGIN

class Texture {

	friend class AssetManager;

public:
	
	unsigned GetId();

private:

	Texture() {};
	~Texture() {};
	Texture(const Texture& /*_copy*/) {};
	void operator= (const Texture& /*_copy*/) {};

	//SDL_Surface *m_surface;
	unsigned	 m_texId;

};

NS_JE_END