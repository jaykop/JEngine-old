#include "Mesh.h"
#include "GraphicSystem.h"

jeBegin

Mesh::Mesh() : m_shape(MESH_NONE), m_drawMode(GL_TRIANGLES),
	m_vao(0), m_vbo(0), m_ebo(0) {}

Mesh::~Mesh()
{
	ClearVertexes();

	if (m_shape == MESH_NONE)
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
	}
}

void Mesh::AddPoint(CR_Vec3 _point) { m_points.push_back(_point); }

void Mesh::AddTextureUV(CR_Vec2 _uv) { m_UVs.push_back(_uv); }

void Mesh::AddNormal(CR_Vec3 _normal) { m_normals.push_back(_normal); }

void Mesh::AddIndice(VertexIndex _indice) { m_indices.push_back(_indice); }

void Mesh::AddPointIndice(unsigned _indice) { m_pointIndices.push_back(_indice); }

vec3 Mesh::GetPoint(unsigned _index) const { return m_points.at(_index); }

vec2 Mesh::GetUV(unsigned _index) const { return m_UVs.at(_index); }

vec3 Mesh::GetNormal(unsigned _index) const { return m_normals.at(_index); }

Mesh::VertexIndex Mesh::GetIndice(unsigned _index) const { return m_indices.at(_index); }

std::size_t Mesh::GetPointCount() const { return m_points.size(); }

std::size_t Mesh::GetIndiceCount() const { return m_indices.size(); }

const std::vector<vec3>& Mesh::GetNormals() const { return m_normals; }

const std::vector<Mesh::VertexIndex>& Mesh::GetIndices() const { return m_indices; }

const std::vector<unsigned>& Mesh::GetPointIndices() const { return m_pointIndices;  }

void Mesh::ClearPoints() { m_points.clear(); }

void Mesh::ClearNormals() { m_normals.clear(); }

void Mesh::ClearUVs() { m_UVs.clear(); }

void Mesh::ClearIndices() { m_indices.clear(); }

void Mesh::ClearVertexes()
{
	ClearPoints();
	ClearUVs();
	ClearNormals();
	ClearIndices();
}

Mesh* Mesh::CreatePoint()
{
	Mesh *pPoint = new Mesh;

	pPoint->AddPoint(vec3(0, 0));
	pPoint->AddTextureUV(vec2(0, 0));
	pPoint->AddNormal(vec3(0, 0, 1.f));
	pPoint->AddIndice({ 0,0,0 });

	return pPoint;
}

Mesh* Mesh::CreateRect()
{
	Mesh *pRect = new Mesh;

	pRect->AddPoint(vec3(-.5f, .5f, 0.f));
	pRect->AddPoint(vec3(.5f, .5f, 0.f));
	pRect->AddPoint(vec3(.5f, -.5f, 0.f));
	pRect->AddPoint(vec3(-.5f, -.5f, 0.f));

	pRect->AddTextureUV(vec2(0.f, 0.f));
	pRect->AddTextureUV(vec2(1.f, 0.f));
	pRect->AddTextureUV(vec2(1.f, 1.f));
	pRect->AddTextureUV(vec2(0.f, 1.f));

	pRect->AddNormal(vec3(0, 0, 1.f));
	pRect->AddNormal(vec3(0, 0, 1.f));
	pRect->AddNormal(vec3(0, 0, 1.f));
	pRect->AddNormal(vec3(0, 0, 1.f));

	pRect->AddIndice({ 0, 0, 0 });
	pRect->AddIndice({ 2, 2, 2 });
	pRect->AddIndice({ 3, 3, 3 });
	pRect->AddIndice({ 2, 2, 2 });
	pRect->AddIndice({ 0, 0, 0 });
	pRect->AddIndice({ 1, 1, 1 });

	return pRect;
}

Mesh* Mesh::CreateCrossRect()
{
	Mesh *pCrossRect = new Mesh;

	//pCrossRect->AddPoint(vec3(-.5f, .5f, 0.f));
	//pCrossRect->AddPoint(vec3(.5f, .5f, 0.f));
	//pCrossRect->AddPoint(vec3(.5f, -.5f, 0.f));
	//pCrossRect->AddPoint(vec3(-.5f, -.5f, 0.f));
	//pCrossRect->AddPoint(vec3(0.f, .5f, .5f));
	//pCrossRect->AddPoint(vec3(0.f, .5f, -.5f));
	//pCrossRect->AddPoint(vec3(0.f, -.5f, -.5f));
	//pCrossRect->AddPoint(vec3(0.f, -.5f, .5f));
	//pCrossRect->AddPoint(vec3(-.5f, 0.f, -.5f));
	//pCrossRect->AddPoint(vec3(.5f, 0.f, -.5f));
	//pCrossRect->AddPoint(vec3(.5f, 0.f, .5f));
	//pCrossRect->AddPoint(vec3(-.5f, 0.f, .5f));

	//pCrossRect->AddTextureUV(vec2(1.f, 0.f));
	//pCrossRect->AddTextureUV(vec2(1.f, 1.f));
	//pCrossRect->AddTextureUV(vec2(0.f, 1.f));
	//pCrossRect->AddTextureUV(vec2(0.f, 0.f));
	//pCrossRect->AddTextureUV(vec2(1.f, 0.f));
	//pCrossRect->AddTextureUV(vec2(1.f, 1.f));
	//pCrossRect->AddTextureUV(vec2(0.f, 1.f));
	//pCrossRect->AddTextureUV(vec2(0.f, 0.f));
	//pCrossRect->AddTextureUV(vec2(1.f, 0.f));
	//pCrossRect->AddTextureUV(vec2(1.f, 1.f));
	//pCrossRect->AddTextureUV(vec2(0.f, 1.f));
	//pCrossRect->AddTextureUV(vec2(0.f, 0.f));

	//pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	//pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	//pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	//pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	//pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	//pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	//pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	//pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	//pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	//pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	//pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	//pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));

	//pCrossRect->AddIndice(0);
	//pCrossRect->AddIndice(2);
	//pCrossRect->AddIndice(3);
	//pCrossRect->AddIndice(2);
	//pCrossRect->AddIndice(0);
	//pCrossRect->AddIndice(1);

	//pCrossRect->AddIndice(5);
	//pCrossRect->AddIndice(7);
	//pCrossRect->AddIndice(6);
	//pCrossRect->AddIndice(7);
	//pCrossRect->AddIndice(5);
	//pCrossRect->AddIndice(4);

	//pCrossRect->AddIndice(8);
	//pCrossRect->AddIndice(10);
	//pCrossRect->AddIndice(11);
	//pCrossRect->AddIndice(10);
	//pCrossRect->AddIndice(8);
	//pCrossRect->AddIndice(9);

	return pCrossRect;
}

Mesh* Mesh::CreateCube()
{
	Mesh *pCube = new Mesh;

	//pCube->AddPoint(vec3(-.5f, .5f, .5f));
	//pCube->AddPoint(vec3(.5f, .5f, .5f));
	//pCube->AddPoint(vec3(.5f, -.5f, .5f));
	//pCube->AddPoint(vec3(-.5f, -.5f, .5f));

	//pCube->AddPoint(vec3(.5f, .5f, -.5f));
	//pCube->AddPoint(vec3(-.5f, .5f, -.5f));
	//pCube->AddPoint(vec3(-.5f, -.5f, -.5f));
	//pCube->AddPoint(vec3(.5f, -.5f, -.5f));

	//pCube->AddPoint(vec3(-.5f, .5f, -.5f));
	//pCube->AddPoint(vec3(-.5f, .5f, .5f));
	//pCube->AddPoint(vec3(-.5f, -.5f, .5f));
	//pCube->AddPoint(vec3(-.5f, -.5f, -.5f));

	//pCube->AddPoint(vec3(.5f, .5f, .5f));
	//pCube->AddPoint(vec3(.5f, .5f, -.5f));
	//pCube->AddPoint(vec3(.5f, -.5f, -.5f));
	//pCube->AddPoint(vec3(.5f, -.5f, .5f));

	//pCube->AddPoint(vec3(-.5f, -.5f, .5f));
	//pCube->AddPoint(vec3(.5f, -.5f, .5f));
	//pCube->AddPoint(vec3(.5f, -.5f, -.5f));
	//pCube->AddPoint(vec3(-.5f, -.5f, -.5f));

	//pCube->AddPoint(vec3(-.5f, .5f, -.5f));
	//pCube->AddPoint(vec3(.5f, .5f, -.5f));
	//pCube->AddPoint(vec3(.5f, .5f, .5f));
	//pCube->AddPoint(vec3(-.5f, .5f, .5f));

	//pCube->AddTextureUV(vec2(.25f, .25f));
	//pCube->AddTextureUV(vec2(.5f, .25f));
	//pCube->AddTextureUV(vec2(.5f, .5f));
	//pCube->AddTextureUV(vec2(.25f, .5f));

	//pCube->AddTextureUV(vec2(.75f, .25f));
	//pCube->AddTextureUV(vec2(1.f, .25f));
	//pCube->AddTextureUV(vec2(1.f, .5f));
	//pCube->AddTextureUV(vec2(.75f, .5f));

	//pCube->AddTextureUV(vec2(0.f, .25f));
	//pCube->AddTextureUV(vec2(.25f, .25f));
	//pCube->AddTextureUV(vec2(.25f, .5f));
	//pCube->AddTextureUV(vec2(0.f, .5f));

	//pCube->AddTextureUV(vec2(.5f, .25f));
	//pCube->AddTextureUV(vec2(.75f, .25f));
	//pCube->AddTextureUV(vec2(.75f, .5f));
	//pCube->AddTextureUV(vec2(.5f, .5f));

	//pCube->AddTextureUV(vec2(.25f, .5f));
	//pCube->AddTextureUV(vec2(.5f, .5f));
	//pCube->AddTextureUV(vec2(.5f, .75f));
	//pCube->AddTextureUV(vec2(.25f, .75f));

	//pCube->AddTextureUV(vec2(.25f, 0.f));
	//pCube->AddTextureUV(vec2(.5f, 0.f));
	//pCube->AddTextureUV(vec2(.5f, .25f));
	//pCube->AddTextureUV(vec2(.25f, .25f));

	//pCube->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	//pCube->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	//pCube->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	//pCube->AddNormal(vec3(0.0f, 0.0f, 1.0f));

	//pCube->AddNormal(vec3(0.0f, 0.0f, -1.0f));
	//pCube->AddNormal(vec3(0.0f, 0.0f, -1.0f));
	//pCube->AddNormal(vec3(0.0f, 0.0f, -1.0f));
	//pCube->AddNormal(vec3(0.0f, 0.0f, -1.0f));

	//pCube->AddNormal(vec3(-1.0f, 0.0f, 0.0f));
	//pCube->AddNormal(vec3(-1.0f, 0.0f, 0.0f));
	//pCube->AddNormal(vec3(-1.0f, 0.0f, 0.0f));
	//pCube->AddNormal(vec3(-1.0f, 0.0f, 0.0f));

	//pCube->AddNormal(vec3(1.0f, 0.0f, 0.0f));
	//pCube->AddNormal(vec3(1.0f, 0.0f, 0.0f));
	//pCube->AddNormal(vec3(1.0f, 0.0f, 0.0f));
	//pCube->AddNormal(vec3(1.0f, 0.0f, 0.0f));

	//pCube->AddNormal(vec3(0.0f, -1.0f, 0.0f));
	//pCube->AddNormal(vec3(0.0f, -1.0f, 0.0f));
	//pCube->AddNormal(vec3(0.0f, -1.0f, 0.0f));
	//pCube->AddNormal(vec3(0.0f, -1.0f, 0.0f));

	//pCube->AddNormal(vec3(0.0f, 1.0f, 0.0f));
	//pCube->AddNormal(vec3(0.0f, 1.0f, 0.0f));
	//pCube->AddNormal(vec3(0.0f, 1.0f, 0.0f));
	//pCube->AddNormal(vec3(0.0f, 1.0f, 0.0f));

	//pCube->AddIndice(3);
	//pCube->AddIndice(0);
	//pCube->AddIndice(2);
	//pCube->AddIndice(1);
	//pCube->AddIndice(2);
	//pCube->AddIndice(0);

	//pCube->AddIndice(6);
	//pCube->AddIndice(7);
	//pCube->AddIndice(5);
	//pCube->AddIndice(4);
	//pCube->AddIndice(5);
	//pCube->AddIndice(7);

	//pCube->AddIndice(8);
	//pCube->AddIndice(10);
	//pCube->AddIndice(11);
	//pCube->AddIndice(10);
	//pCube->AddIndice(8);
	//pCube->AddIndice(9);

	//pCube->AddIndice(14);
	//pCube->AddIndice(15);
	//pCube->AddIndice(13);
	//pCube->AddIndice(12);
	//pCube->AddIndice(13);
	//pCube->AddIndice(15);

	//pCube->AddIndice(16);
	//pCube->AddIndice(18);
	//pCube->AddIndice(19);
	//pCube->AddIndice(18);
	//pCube->AddIndice(16);
	//pCube->AddIndice(17);

	//pCube->AddIndice(20);
	//pCube->AddIndice(22);
	//pCube->AddIndice(23);
	//pCube->AddIndice(22);
	//pCube->AddIndice(20);
	//pCube->AddIndice(21);

	return pCube;
}

Mesh* Mesh::CreateTetrahedron()
{
	// TODO
	Mesh *pTetrahedron = new Mesh;

	//pTetrahedron->AddPoint();
	//pTetrahedron->AddTextureUV();
	//pTetrahedron->AddNormal();

	return pTetrahedron;
}

void Mesh::CreateCustomObject()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	std::vector<jeVertex> vertices;
	vertices.reserve(GetIndiceCount());
	
	for (unsigned index = 0; index < GetIndiceCount(); ++index) {
		VertexIndex vi = GetIndices().at(index);
		vertices.push_back(jeVertex{ GetPoint(vi.a), GetUV(vi.b), GetNormal(vi.c) });
	}
	
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(jeVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * m_pointIndices.size(), &m_pointIndices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(jeVertex),
		reinterpret_cast<void*>(offsetof(jeVertex, jeVertex::position)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(jeVertex),
		reinterpret_cast<void*>(offsetof(jeVertex, jeVertex::uv)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(jeVertex),
		reinterpret_cast<void*>(offsetof(jeVertex, jeVertex::normal)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

jeEnd
