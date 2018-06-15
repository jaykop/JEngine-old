#pragma once
#include "Macro.h"
#include <vector>
#include "Vector2.h"
#include "Vector3.h"

jeBegin

class Mesh
{
	friend class Model;
	friend class GrahpicSystem;
	
public:

	void AddPoint(CR_Vec3 _point);
	void AddTextureUV(CR_Vec2 _uv);
	void AddNormal(CR_Vec3 _normal);

	std::vector<vec3>& GetPoints();
	std::vector<vec2>& GetUVs();
	std::vector<vec3>& GetNormals();

private:

	std::vector<vec3>	m_points;
	std::vector<vec2>	m_UVs;
	std::vector<vec3>	m_normals;

	Mesh() {};
	~Mesh()
	{
		m_points.clear();
		m_UVs.clear();
		m_normals.clear();
	};
};

jeEnd