#pragma once
#include <component_builder.hpp>
#include <component.hpp>
#include <string>
#include <vector>
#include <vec4.hpp>
#include <mat4.hpp>

jeBegin

class Mesh;
class Light;
class Shader;
class Transform;
class Animation2D;
class DebugDrawer;

class Renderer : public Component {

	jeBaseFriends(Renderer);
	friend class GraphicSystem;

public:

	using Meshes = std::vector<Mesh*>;

protected:

	virtual void add_to_system();
	virtual void remove_from_system();
	virtual void load(const rapidjson::Value& data);

public:

	enum ProjectType { PERSPECTIVE, ORTHOGONAL };
	enum RenderType { R_NONE, R_POSITION, R_NORMAL };

	Renderer(Object* owner);
	virtual ~Renderer() {}

	// public methods
	void set_mesh(const std::string& name);
	const Meshes& get_meshes(void) const;

	void draw(Shader* shader, Camera* camera, const mat4& perspective);
	void draw(Shader* shader);
	void draw_normals(Shader* shader);
	void run_animation(Shader* shader);

	static void draw_quad(Mesh* m);
	static void draw_lighting_effect(Light* light, Shader* shader);

	bool is2d;
	bool renderBoundary_;
	bool renderFaceNormals_, renderVertexNormals_;
	static bool renderObj_;

	void draw_debug_info(Shader* shader);
	// void on_gui(void) override;

	Transform* transform_ = nullptr;
	Animation2D* animation_ = nullptr;
	DebugDrawer* ddrawer_ = nullptr;

	vec4 color;
	ProjectType prjType;
	unsigned sfactor, dfactor;

	static RenderType renderType_;

private:

	// private members
	Meshes meshes_;
	//AABB aabb_;
	//OBB obb_;
	//BoundingEllipsoid ellipse_;
	//BoundingSphere sphere_;
	bool h_;
	int status_;

	// private methods
	// void GenerateBV(void);
};

jeDeclareComponentBuilder(Renderer);

jeEnd