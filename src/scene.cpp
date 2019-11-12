/******************************************************************************/
/*!
\file   application.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/02(yy/mm/dd)

\description
Contains the methods of scene class
*/
/******************************************************************************/

#include <scene.hpp>
#include <object_manager.hpp>
#include <sound_system.hpp>
#include <physics_system.hpp>
#include <graphic_system.hpp>
#include <behavior_system.hpp>

// TODO
#include <iostream>
#include <application.hpp>
#include <input_handler.hpp>

jeBegin

void Scene::bind_system()
{
	GraphicSystem::initialize();
	PhysicsSystem::initialize();
	SoundSystem::initialize();
	BehaviorSystem::initialize();
}

void Scene::unbind_system()
{
	BehaviorSystem::close();
	SoundSystem::close();
	PhysicsSystem::close();
	GraphicSystem::close();
}

Scene::Scene(const char* name)
{
	// assign the scene name
	name_.assign(name);
}

Scene::~Scene()
{
}

void Scene::load()
{
}

void Scene::initialize()
{
	// bind the objects to the manager
	ObjectManager::objects_ = &objects_;
}

void Scene::update(float dt)
{
	// update all systems
	BehaviorSystem::update();
	SoundSystem::update();
	PhysicsSystem::update();
	GraphicSystem::update();

	//TODO
	//INPUT TEST CODE
	if (InputHandler::key_pressed(KEY::A))
		std::cout << "A\n";

	if (InputHandler::key_triggered(KEY::B))
		std::cout << "B\n";

	if (InputHandler::key_pressed(KEY::MOUSE_LEFT))
		std::cout << "MOUSE_LEFT\n";

	if (InputHandler::key_triggered(KEY::MOUSE_RIGHT))
		std::cout << "MOUSE_RIGHT\n";

	if (InputHandler::get_mouse_wheel_status() == InputHandler::MouseWheel::UP)
		std::cout << "Mouse Wheel Up\n";

	else if (InputHandler::get_mouse_wheel_status() == InputHandler::MouseWheel::DOWN)
		std::cout << "Mouse Wheel Down\n";

	if (InputHandler::key_pressed(KEY::ESC))
		Application::quit();
}

void Scene::close()
{
	// make sure current object map belongs to the current scene
	ObjectManager::objects_ = &objects_;

	// clear current object map
	ObjectManager::clear_objects();

	// initialize
	ObjectManager::objects_ = nullptr;
}

void Scene::unload()
{
}

const char* Scene::get_name() const
{
	return name_.c_str();
}

jeEnd