#pragma once
#include <component_builder.hpp>
#include <component.hpp>
#include <string>
#include <vector>

jeBegin

class Mesh;
class Light;
class Shader;
class Transform;
class DebugDrawer;

class Renderer : public Component {

	jeBaseFriends(Renderer);
	friend class GraphicSystem;

public:

	using Meshes = std::vector<Mesh*>;
	void load_renderer(const std::string& path);

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& data);

public:

	enum RenderType { R_NONE, R_POSITION, R_NORMAL };

	Renderer(Object* owner);
	virtual ~Renderer() {}

	// public methods
	void set_mesh(const std::string& name);
	const Meshes& get_meshes(void) const;

	void draw(Shader* pShader);
	void draw_normals(Shader* pShader);

	static void draw_lighting_effect(Light* pLight, Shader* pShader);
	static void draw_quad(unsigned& vao);

	bool renderBoundary_;
	bool renderFaceNormals_, renderVertexNormals_;
	static bool renderObj_;

	void draw_debug_info(Shader* pShader);
	// void on_gui(void) override;

	Transform* pTrans_ = nullptr;
	DebugDrawer* pDDrawer_ = nullptr;

	static RenderType renderType_;

private:

	// private members
	Meshes meshes_;
	//AABB aabb_;
	//OBB obb_;
	//BoundingEllipsoid ellipse_;
	//BoundingSphere sphere_;
	bool h_;

	// private methods
	// void GenerateBV(void);
};

jeDeclareComponentBuilder(Renderer);

jeEnd