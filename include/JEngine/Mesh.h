#pragma once
#include "Macro.h"
#include <vector>
#include "glew.h"
#include "Vector2.h"
#include "Vector3.h"
#include <unordered_map>

jeBegin

class Mesh
{
	friend class Text;
	friend class Light;
	friend class Model;
	friend class Emitter;
	friend class GLManager;
	friend class AssetManager;
	friend class GraphicSystem;

	using TextureMap = std::unordered_map<std::string, unsigned>;

	struct jeIndex { unsigned a, b, c; };
	struct jeVertex {
		vec3 position;
		vec2 uv;
		vec3 normal;
	};

public:

	enum shape { MESH_CUSTOM, MESH_POINT, MESH_RECT, MESH_CROSSRECT, MESH_CUBE, MESH_TETRAHEDRON, };

	shape m_shape;

	void AddPoint(CR_Vec3 _point);
	void AddTextureUV(CR_Vec2 _uv);
	void AddNormal(CR_Vec3 _normal);
	void AddIndice(jeIndex _indice);

	vec3		GetPoint(unsigned _index) const;
	vec2		GetUV(unsigned _index) const;
	vec3		GetNormal(unsigned _index) const;
	jeIndex		GetIndice(unsigned _index) const;

	std::size_t GetPointCount() const;
	std::size_t GetIndiceCount() const;

	const std::vector<vec3>&	GetNormals() const;
	const std::vector<jeIndex>& GetIndices() const;

	void ClearPoints();
	void ClearNormals();
	void ClearUVs();
	void ClearIndices();

	void ClearVertexes();

private:

	std::vector<jeIndex>	m_indices;
	std::vector<vec3>		m_points;
	std::vector<vec2>		m_UVs;
	std::vector<vec3>		m_normals;

	GLuint		m_vao, m_vbo, m_ebo;

	bool builtIn_;

	Mesh();
	~Mesh();
};

jeEnd