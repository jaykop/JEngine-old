/******************************************************************************/
/*!
\file   physics_system.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the definition of PhysicsSystem class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>

jeBegin

class PhysicsSystem {

	friend class Scene;

public:



private:

	static void initialize();
	static void update();
	static void close();

	PhysicsSystem() = delete;
	~PhysicsSystem() = delete;
	PhysicsSystem(PhysicsSystem&&) = delete;
	PhysicsSystem(const PhysicsSystem&) = delete;
	PhysicsSystem& operator=(PhysicsSystem&&) = delete;
	PhysicsSystem& operator=(const PhysicsSystem&) = delete;

};

jeEnd