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
#include <object.hpp>
#include <transform.hpp>
#include <model.hpp>

jeBegin

Scene::Scene(const char* name)
{
	// assign the scene name
	name_ = name;
}

Scene::~Scene()
{
}

void Scene::load()
{
	// bind the objects to the manager
	ObjectManager::objects_ = &objects_;

	// Here load from json file...

	// TODO
	Object* test = ObjectManager::create_object("3d model");
	test->add_component<Model>();
	test->add_component<Transform>();
	register_object(test);
}

void Scene::initialize()
{
	BehaviorSystem::initialize();
	GraphicSystem::initialize();
	PhysicsSystem::initialize();
	SoundSystem::initialize();
}

void Scene::update(float dt)
{
	// update all systems
	BehaviorSystem::update(dt);
	SoundSystem::update(dt);
	PhysicsSystem::update(dt);
	GraphicSystem::update(dt);

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
	BehaviorSystem::close();
	SoundSystem::close();
	PhysicsSystem::close();
	GraphicSystem::close();
}

void Scene::unload()
{	
	// make sure current object map belongs to the current scene
	ObjectManager::objects_ = &objects_;

	// clear current object map
	ObjectManager::clear_objects();

	// initialize
	ObjectManager::objects_ = nullptr;
}

const char* Scene::get_name() const
{
	return name_;
}

void Scene::register_object(Object* obj) {

	// check if the object is pointing null
	if (!ObjectManager::objects_) {
		ObjectManager::objects_->insert({ obj->get_name(), obj });
		obj->register_components();
	}
}

jeEnd