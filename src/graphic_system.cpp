/******************************************************************************/
/*!
\file   graphic_system.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the methods of GraphicSystem class
*/
/******************************************************************************/

#include <glew.h>
#include <graphic_system.hpp>

jeBegin

void GraphicSystem::initialize()
{
}

void GraphicSystem::update()
{
	glClearColor(1.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void GraphicSystem::close()
{

}

void GraphicSystem::add_model(Model* m)
{
}

void GraphicSystem::remove_model(Model* m)
{
}

void GraphicSystem::add_camera(Camera* c)
{
}

void GraphicSystem::remove_camera(Camera* c)
{
}

void GraphicSystem::render()
{
}

jeEnd
