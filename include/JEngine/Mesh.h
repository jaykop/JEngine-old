#pragma once
#include "Macro.h"
#include <vector>
#include "glew.h"
#include "Vector2.h"
#include "Vector3.h"

jeBegin

class Mesh
{
	friend class Light;
	friend class Model;
	friend class Emitter;
	friend class GLManager;
	friend class AssetManager;

public:

	enum shape { MESH_NONE, MESH_POINT, MESH_RECT, MESH_CROSSRECT, MESH_CUBE, MESH_TETRAHEDRON, };

	shape		m_shape;
	unsigned	m_drawMode;
	GLuint		m_vao, m_vbo, m_ebo;

	void AddPoint(CR_Vec3 _point);
	void AddTextureUV(CR_Vec2 _uv);
	void AddNormal(CR_Vec3 _normal);
	void AddIndice(unsigned _indice);

	vec3		GetPoint(unsigned _index) const;
	vec2		GetUV(unsigned _index) const;
	vec3		GetNormal(unsigned _index) const;
	unsigned	GetIndice(unsigned _index) const;

	std::size_t GetPointCount() const;
	const std::vector<unsigned>& GetIndices() const;

	void ClearPoints();
	void ClearNormals();
	void ClearUVs();
	void ClearIndices();

	void ClearVertexes();

private:

	static Mesh* CreatePoint();
	static Mesh* CreateRect();
	static Mesh* CreateCrossRect();
	static Mesh* CreateCube();
	static Mesh* CreateTetrahedron();

	void CreateCustomObject();

	std::vector<unsigned>	m_indices;
	std::vector<vec3>		m_points;
	std::vector<vec2>		m_UVs;
	std::vector<vec3>		m_normals;

	Mesh();
	~Mesh();
};

jeEnd