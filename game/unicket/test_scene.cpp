// necessary headers
#include "test_scene.h"
#include "game_logics.h"
#include "pch.h"

// components
#include "camera.hpp"

jeBegin

void TestScene::initialize()
{
	// add camera
	Object* mainCamera = ObjectManager::create_object("main_camera");
	mainCamera->add_component<Camera>();
	register_object(mainCamera);

	// scene controller
	Object* controller = ObjectManager::create_object("scene_controller");
	controller->add_component<SceneController>();
	register_object(controller);

	// base init
	Scene::initialize();
}

void TestScene::update(float dt)
{
	// base update
	Scene::update(dt);
}

void TestScene::close()
{
	// base close
	Scene::close();
}

jeEnd