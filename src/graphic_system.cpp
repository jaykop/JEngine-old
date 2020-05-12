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
#include <scene_manager.hpp>
#include <scene.hpp>
#include <camera.hpp>
#include <model.hpp>

jeBegin

Camera* GraphicSystem::mainCamera_ = nullptr;
GraphicSystem::Models GraphicSystem::models_;
GraphicSystem::Cameras GraphicSystem::cameras_;
vec4 GraphicSystem::backgroundColor_ = vec4::zero, GraphicSystem::screenColor_ = vec4::zero;

void GraphicSystem::initialize() {

	if (!mainCamera_ && !(cameras_.empty()))
		mainCamera_ = *cameras_.begin();

	//for (auto& model : models_)
	//	model->initialize();
}

void GraphicSystem::update(float /*dt*/) {

	// get current scene color
	backgroundColor_ = SceneManager::get_current_scene()->background;

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(
		backgroundColor_.r, 
		backgroundColor_.g, 
		backgroundColor_.b, 
		backgroundColor_.a);

	//for (auto& model : models_)
	//	models_->update();

}

void GraphicSystem::close() {

	//for (auto& model : models_)
	//	models_->close();

	mainCamera_ = nullptr;
}

void GraphicSystem::render()
{
}

void GraphicSystem::add_model(Model* model) { 
	models_.emplace_back(model); 
}

void GraphicSystem::add_camera(Camera* camera) { 
	cameras_.emplace_back(camera);
}

void GraphicSystem::remove_model(Model* model) { 
	models_.erase(std::remove(models_.begin(), models_.end(), model), models_.end()); 
}

void GraphicSystem::remove_camera(Camera* camera) {
	cameras_.erase(std::remove(cameras_.begin(), cameras_.end(), camera), cameras_.end());
}

jeEnd
