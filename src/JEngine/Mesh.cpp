#include "Mesh.h"

jeBegin

Mesh::Mesh() : m_shape(MESH_CUSTOM), m_vao(0), m_vbo(0), m_ebo(0), builtIn_(false) {}

Mesh::~Mesh() {

	if (m_shape == MESH_CUSTOM)
	{
		ClearVertexes();
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
	}
}

void Mesh::AddPoint(CR_Vec3 _point) { m_points.push_back(_point); }

void Mesh::AddTextureUV(CR_Vec2 _uv) { m_UVs.push_back(_uv); }

void Mesh::AddNormal(CR_Vec3 _normal) { m_normals.push_back(_normal); }

void Mesh::AddIndice(jeIndex _indice) { m_indices.push_back(_indice); }

vec3 Mesh::GetPoint(unsigned _index) const { return m_points.at(_index); }

vec2 Mesh::GetUV(unsigned _index) const { return m_UVs.at(_index); }

vec3 Mesh::GetNormal(unsigned _index) const { return m_normals.at(_index); }

Mesh::jeIndex Mesh::GetIndice(unsigned _index) const { return m_indices.at(_index); }

std::size_t Mesh::GetPointCount() const { return m_points.size(); }

std::size_t Mesh::GetIndiceCount() const { return m_indices.size(); }

const std::vector<vec3>& Mesh::GetNormals() const { return m_normals; }

const std::vector<Mesh::jeIndex>& Mesh::GetIndices() const { return m_indices; }

void Mesh::ClearPoints() { m_points.clear(); }

void Mesh::ClearNormals() { m_normals.clear(); }

void Mesh::ClearUVs() { m_UVs.clear(); }

void Mesh::ClearIndices() { m_indices.clear(); }

void Mesh::ClearVertexes() {

	ClearPoints();
	ClearUVs();
	ClearNormals();
	ClearIndices();
}

jeEnd
