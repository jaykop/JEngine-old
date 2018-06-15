#include "Mesh.h"

jeBegin

void Mesh::AddPoint(CR_Vec3 _point)
{
	m_points.push_back(_point);
}

void Mesh::AddTextureUV(CR_Vec2 _uv)
{
	m_UVs.push_back(_uv);
}

void Mesh::AddNormal(CR_Vec3 _normal)
{
	m_normals.push_back(_normal);
}

std::vector<vec3>& Mesh::GetPoints()
{
	return m_points;
}

std::vector<vec2>& Mesh::GetUVs()
{
	return m_UVs;
}

std::vector<vec3>& Mesh::GetNormals()
{
	return m_normals;
}

jeEnd
