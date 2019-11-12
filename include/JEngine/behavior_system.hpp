/******************************************************************************/
/*!
\file   behavior_system.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the definition of BehaviorSystem class
*/
/******************************************************************************/
#pragma once
#include <macros.hpp>

jeBegin

class BehaviorSystem {

	friend class Scene;

public:



private:

	static void initialize();
	static void update();
	static void close();

	BehaviorSystem() = delete;
	~BehaviorSystem() = delete;
	BehaviorSystem(BehaviorSystem&&) = delete;
	BehaviorSystem(const BehaviorSystem&) = delete;
	BehaviorSystem& operator=(BehaviorSystem&&) = delete;
	BehaviorSystem& operator=(const BehaviorSystem&) = delete;

};

jeEnd