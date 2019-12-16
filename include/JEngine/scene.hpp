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

	// Prevent to clone this class
	Scene() = delete;
	jePreventClone(Scene)

	// Only SceneManager can manage this class
	friend class SceneManager;

public:

	const char* get_name() const;
	void register_object(Object* obj);

	// colors
	vec4 background, screen;

private:

	// todo: remove this later!
	void tes_func();

	Scene(const char* name);
	~Scene();

	void load();
	void initialize();
	void update(float dt);
	void close();
	void unload();

	Scene* lastScene_ = nullptr; // pointer to the last scene (before this scene)
	const char* name_, *directory_; 

	// obj container
	ObjectMap objects_;
	
};

jeEnd
