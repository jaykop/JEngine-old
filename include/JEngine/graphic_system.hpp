/******************************************************************************/
/*!
\file   graphic_system.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the definition of GraphicSystem class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>
#include <vec4.hpp>
#include <vector>

jeBegin

class Model;
class Camera;
class GraphicSystem {

	// Prevent to clone this class
	GraphicSystem() = delete;
	~GraphicSystem() = delete;

	jePreventClone(GraphicSystem)

	friend class Scene;
	friend class Camera;
	friend class Model;

	using Models = std::vector<Model*>;
	using Cameras = std::vector<Camera*>;

public:

	static void set_camera(Camera* mainCameras);
	static Camera* get_camera();

	vec4 backgroundColor_, screenColor_;

private:

	static void initialize();
	static void update(float dt);
	static void close();

	static void add_model(Model* model);
	static void remove_model(Model* model);

	static void add_camera(Camera* camera);
	static void remove_camera(Camera* camera);

	//void add_light(Light* pLight);
	//void remove_light(Light* pLight);

	static void render();

	static Camera* mainCamera_;
	static Models models_;
	static Cameras cameras_;

};

jeEnd