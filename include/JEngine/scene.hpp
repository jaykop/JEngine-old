/******************************************************************************/
/*!
\file   scene.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/02(yy/mm/dd)

\description
Contains the definition of Scene class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>
#include <unordered_map>
#include <vec4.hpp>

jeBegin

class Object;
using ObjectMap = std::unordered_map<const char*, Object*>;

// Generic scene class
class Scene {

	// Only SceneManager can manage this class
	friend class SceneManager;

public:

	const char* get_name() const;

private:

	static void bind_system();
	static void unbind_system();

	Scene(const char* name);
	~Scene();

	void load();
	void initialize();
	void update(float dt);
	void close();
	void unload();

	Scene* lastScene_ = nullptr; // pointer to the last scene (before this scene)
	const char* name_, *directory_; 

	// container
	ObjectMap objects_;

	vec4 background, screen;


	// Prevent to clone this class
	Scene() = delete;
	Scene(Scene&&) = delete;
	Scene(const Scene&) = delete;
	Scene& operator= (Scene&&) = delete;
	Scene& operator= (const Scene&) = delete;
};

jeEnd
