/******************************************************************************/
/*!
\file   sound_system.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the definition of SoundSystem class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>

jeBegin

class SoundSystem {

	friend class Scene;

public:



private:

	static void initialize();
	static void update();
	static void close();

	SoundSystem() = delete;
	~SoundSystem() = delete;
	SoundSystem(SoundSystem&&) = delete;
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem& operator=(SoundSystem&&) = delete;
	SoundSystem& operator=(const SoundSystem&) = delete;

};

jeEnd