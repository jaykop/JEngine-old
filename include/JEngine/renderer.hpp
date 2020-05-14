#pragma once
#include <component_builder.hpp>
#include <component.hpp>
#include <string>

jeBegin

class Mesh;
class Shader;
class Object;

class Renderer : public Component {

	jeBaseFriends(Renderer);
	friend class GraphicSystem;

public:

	void load_renderer(const std::string& path);

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& data);

private:

	std::string directory;
	std::vector<Mesh*> meshes_;

	/*  Functions   */
	Renderer(Object* owner);
	virtual ~Renderer() {}

	void draw(Shader* shader);

	Renderer() = delete;

};

jeDeclareComponentBuilder(Renderer);

jeEnd