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

jeBegin

class Camera;
class Model;
class GraphicSystem {

	friend class Scene;

public:

	static void set_camera(Camera* mainCameras);
	static Camera get_camera();

	vec4 backgroundColor_, screenColor_;

private:

	static void initialize();
	static void update();
	static void close();

	void add_model(Model* m);
	void remove_model(Model* m);

	void add_camera(Camera* c);
	void remove_camera(Camera* c);

	//void add_light(Light* pLight);
	//void remove_light(Light* pLight);

	void render();

	GraphicSystem() = delete;
	~GraphicSystem() = delete;
	GraphicSystem(GraphicSystem&&) = delete;
	GraphicSystem(const GraphicSystem&) = delete;
	GraphicSystem& operator=(GraphicSystem&&) = delete;
	GraphicSystem& operator=(const GraphicSystem&) = delete;

};

jeEnd