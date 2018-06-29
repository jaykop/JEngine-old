#include "Mesh.h"
#include "AssetManager.h"
#include "GLManager.h"

jeBegin

Mesh::Mesh() : m_vao(0), m_vbo(0), m_ebo(0), 
	builtIn_(false), m_mainTex(0) {}

Mesh::~Mesh() {

	ClearVertexes();

	// Remove textures
	m_textureMap.clear();

	// Delete buffers
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
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

void Mesh::AddTexture(const char *_key)
{
	auto found = m_textureMap.find(_key);
	if (found != m_textureMap.end())
		jeDebugPrint("!Model - Existing texture: %s.\n", _key);

	else {
		unsigned newTexture = ASSET::GetTexture(_key);

		if (m_textureMap.empty())
			m_mainTex = newTexture;

		m_textureMap.insert(
			TextureMap::value_type(
				_key, newTexture));
	}
}

void Mesh::RemoveTexture(const char *_key)
{
	m_textureMap.erase(_key);
}

void Mesh::SetCurrentTexutre(const char *_key)
{
	m_mainTex = GetTexutre(_key);
}

unsigned Mesh::GetCurrentTexutre() const
{
	return m_mainTex;
}

unsigned Mesh::GetTexutre(const char *_key)
{
	auto found = m_textureMap.find(_key);
	if (found != m_textureMap.end())
		return found->second;

	jeDebugPrint("!Model - No such name of enrolled texture: %s.\n", _key);
	return 0;
}


Mesh* Mesh::CreatePoint()
{
	Mesh *pPoint = new Mesh;

	pPoint->AddPoint(vec3(0, 0));
	pPoint->AddTextureUV(vec2(0, 0));
	pPoint->AddNormal(vec3(0, 0, 1.f));
	pPoint->AddIndice({ 0,0,0 });

	GLM::DescribeVertex(pPoint);
	return pPoint;
}

Mesh* Mesh::CreateRect()
{
	Mesh *pRect = new Mesh;

	pRect->AddPoint(vec3(-.5f, .5f, 0.f));
	pRect->AddPoint(vec3(-.5f, -.5f, 0.f));
	pRect->AddPoint(vec3(.5f, -.5f, 0.f));
	pRect->AddPoint(vec3(.5f, .5f, 0.f));

	pRect->AddTextureUV(vec2(0.f, 0.f));
	pRect->AddTextureUV(vec2(0.f, 1.f));
	pRect->AddTextureUV(vec2(1.f, 1.f));
	pRect->AddTextureUV(vec2(1.f, 0.f));

	pRect->AddNormal(vec3(0, 0, 1.f));

	pRect->AddIndice({ 2, 2, 0 });
	pRect->AddIndice({ 0, 0, 0 });
	pRect->AddIndice({ 1, 1, 0 });
	pRect->AddIndice({ 2, 2, 0 });
	pRect->AddIndice({ 3, 3, 0 });
	pRect->AddIndice({ 0, 0, 0 });

	GLM::DescribeVertex(pRect);
	return pRect;
}

Mesh* Mesh::CreateCrossRect()
{
	Mesh *pCrossRect = new Mesh;

	pCrossRect->AddPoint(vec3(-.5f, .5f, 0.f));
	pCrossRect->AddPoint(vec3(.5f, .5f, 0.f));
	pCrossRect->AddPoint(vec3(.5f, -.5f, 0.f));
	pCrossRect->AddPoint(vec3(-.5f, -.5f, 0.f));
	pCrossRect->AddPoint(vec3(0.f, .5f, .5f));
	pCrossRect->AddPoint(vec3(0.f, .5f, -.5f));
	pCrossRect->AddPoint(vec3(0.f, -.5f, -.5f));
	pCrossRect->AddPoint(vec3(0.f, -.5f, .5f));
	pCrossRect->AddPoint(vec3(-.5f, 0.f, -.5f));
	pCrossRect->AddPoint(vec3(.5f, 0.f, -.5f));
	pCrossRect->AddPoint(vec3(.5f, 0.f, .5f));
	pCrossRect->AddPoint(vec3(-.5f, 0.f, .5f));

	pCrossRect->AddTextureUV(vec2(1.f, 0.f));
	pCrossRect->AddTextureUV(vec2(1.f, 1.f));
	pCrossRect->AddTextureUV(vec2(0.f, 1.f));
	pCrossRect->AddTextureUV(vec2(0.f, 0.f));
	pCrossRect->AddTextureUV(vec2(1.f, 0.f));
	pCrossRect->AddTextureUV(vec2(1.f, 1.f));
	pCrossRect->AddTextureUV(vec2(0.f, 1.f));
	pCrossRect->AddTextureUV(vec2(0.f, 0.f));
	pCrossRect->AddTextureUV(vec2(1.f, 0.f));
	pCrossRect->AddTextureUV(vec2(1.f, 1.f));
	pCrossRect->AddTextureUV(vec2(0.f, 1.f));
	pCrossRect->AddTextureUV(vec2(0.f, 0.f));

	pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	pCrossRect->AddNormal(vec3(0.0f, 0.0f, 1.0f));

	pCrossRect->AddIndice({ 0, 0, 0 });
	pCrossRect->AddIndice({ 2, 2, 2 });
	pCrossRect->AddIndice({ 3, 3, 3 });
	pCrossRect->AddIndice({ 2, 2, 2 });
	pCrossRect->AddIndice({ 0, 0, 0 });
	pCrossRect->AddIndice({ 1, 1, 1 });

	pCrossRect->AddIndice({ 5, 5, 5 });
	pCrossRect->AddIndice({ 7, 7, 7 });
	pCrossRect->AddIndice({ 6, 6, 6 });
	pCrossRect->AddIndice({ 7, 7, 7 });
	pCrossRect->AddIndice({ 5, 5, 5 });
	pCrossRect->AddIndice({ 4, 4, 4 });

	pCrossRect->AddIndice({ 8, 8, 8 });
	pCrossRect->AddIndice({ 10, 10, 10 });
	pCrossRect->AddIndice({ 11, 11, 11 });
	pCrossRect->AddIndice({ 10, 10, 10 });
	pCrossRect->AddIndice({ 8, 8, 8 });
	pCrossRect->AddIndice({ 9, 9, 9 });

	GLM::DescribeVertex(pCrossRect);
	return pCrossRect;
}

Mesh* Mesh::CreateCube()
{
	Mesh *pCube = new Mesh;

	pCube->AddPoint(vec3(-.5f, .5f, .5f));
	pCube->AddPoint(vec3(.5f, .5f, .5f));
	pCube->AddPoint(vec3(.5f, -.5f, .5f));
	pCube->AddPoint(vec3(-.5f, -.5f, .5f));
	pCube->AddPoint(vec3(.5f, .5f, -.5f));
	pCube->AddPoint(vec3(-.5f, .5f, -.5f));
	pCube->AddPoint(vec3(-.5f, -.5f, -.5f));
	pCube->AddPoint(vec3(.5f, -.5f, -.5f));
	pCube->AddPoint(vec3(-.5f, .5f, -.5f));
	pCube->AddPoint(vec3(-.5f, .5f, .5f));
	pCube->AddPoint(vec3(-.5f, -.5f, .5f));
	pCube->AddPoint(vec3(-.5f, -.5f, -.5f));
	pCube->AddPoint(vec3(.5f, .5f, .5f));
	pCube->AddPoint(vec3(.5f, .5f, -.5f));
	pCube->AddPoint(vec3(.5f, -.5f, -.5f));
	pCube->AddPoint(vec3(.5f, -.5f, .5f));
	pCube->AddPoint(vec3(-.5f, -.5f, .5f));
	pCube->AddPoint(vec3(.5f, -.5f, .5f));
	pCube->AddPoint(vec3(.5f, -.5f, -.5f));
	pCube->AddPoint(vec3(-.5f, -.5f, -.5f));
	pCube->AddPoint(vec3(-.5f, .5f, -.5f));
	pCube->AddPoint(vec3(.5f, .5f, -.5f));
	pCube->AddPoint(vec3(.5f, .5f, .5f));
	pCube->AddPoint(vec3(-.5f, .5f, .5f));

	pCube->AddTextureUV(vec2(.25f, .25f));
	pCube->AddTextureUV(vec2(.5f, .25f));
	pCube->AddTextureUV(vec2(.5f, .5f));
	pCube->AddTextureUV(vec2(.25f, .5f));
	pCube->AddTextureUV(vec2(.75f, .25f));
	pCube->AddTextureUV(vec2(1.f, .25f));
	pCube->AddTextureUV(vec2(1.f, .5f));
	pCube->AddTextureUV(vec2(.75f, .5f));
	pCube->AddTextureUV(vec2(0.f, .25f));
	pCube->AddTextureUV(vec2(0.f, .5f));
	pCube->AddTextureUV(vec2(.5f, .75f));
	pCube->AddTextureUV(vec2(.25f, .75f));
	pCube->AddTextureUV(vec2(.25f, 0.f));
	pCube->AddTextureUV(vec2(.5f, 0.f));

	pCube->AddNormal(vec3(0.0f, 0.0f, 1.0f));
	pCube->AddNormal(vec3(0.0f, 0.0f, -1.0f));
	pCube->AddNormal(vec3(-1.0f, 0.0f, 0.0f));
	pCube->AddNormal(vec3(1.0f, 0.0f, 0.0f));
	pCube->AddNormal(vec3(0.0f, -1.0f, 0.0f));
	pCube->AddNormal(vec3(0.0f, 1.0f, 0.0f));

	pCube->AddIndice({ 1 ,1, 0 });
	pCube->AddIndice({ 0, 0, 0 });
	pCube->AddIndice({ 2 ,2, 0 });
	pCube->AddIndice({ 3, 3, 0 });
	pCube->AddIndice({ 2 ,2, 0 });
	pCube->AddIndice({ 0, 0, 0 });

	pCube->AddIndice({ 5 ,5, 1 });
	pCube->AddIndice({ 7 ,7, 1 });
	pCube->AddIndice({ 6, 6, 1 });
	pCube->AddIndice({ 7 ,7, 1 });
	pCube->AddIndice({ 5 ,5, 1 });
	pCube->AddIndice({ 4 ,4, 1 });

	pCube->AddIndice({ 11, 9, 2 });
	pCube->AddIndice({ 10 ,3, 2 });
	pCube->AddIndice({ 8 ,8, 2 });
	pCube->AddIndice({ 9, 0, 2 });
	pCube->AddIndice({ 8 ,8, 2 });
	pCube->AddIndice({ 10 ,3, 2 });

	pCube->AddIndice({ 13 ,4, 3 });
	pCube->AddIndice({ 15, 2, 3 });
	pCube->AddIndice({ 14, 7, 3 });
	pCube->AddIndice({ 15 ,2, 3 });
	pCube->AddIndice({ 13, 4, 3 });
	pCube->AddIndice({ 12 ,1, 3 });

	pCube->AddIndice({ 19, 11, 4 });
	pCube->AddIndice({ 18, 10, 4 });
	pCube->AddIndice({ 16, 3, 4 });
	pCube->AddIndice({ 17, 2, 4 });
	pCube->AddIndice({ 16, 3, 4 });
	pCube->AddIndice({ 18, 10, 4 });

	pCube->AddIndice({ 23, 0, 5 });
	pCube->AddIndice({ 22, 1, 5 });
	pCube->AddIndice({ 20, 12, 5 });
	pCube->AddIndice({ 21, 13, 5 });
	pCube->AddIndice({ 20, 12, 5 });
	pCube->AddIndice({ 22, 1, 5 });

	GLM::DescribeVertex(pCube);
	return pCube;
}

Mesh* Mesh::CreateTetrahedron()
{
	Mesh *pTetrahedron = new Mesh;

	const static float height = sqrt(3.f) / 2.f;

	pTetrahedron->AddPoint(vec3(0.f, 1.f, 0.f));
	pTetrahedron->AddPoint(vec3(0.f, 0.f, -height));
	pTetrahedron->AddPoint(vec3(-.5f, 0.f, .5f));
	pTetrahedron->AddPoint(vec3(.5f, 0.f, .5f));

	// TODO
	pTetrahedron->AddTextureUV(vec2(0, 1));
	pTetrahedron->AddTextureUV(vec2(0.5f, 1));
	pTetrahedron->AddTextureUV(vec2(1, 1));
	pTetrahedron->AddTextureUV(vec2(0.25f, 1 - height * .5f));
	pTetrahedron->AddTextureUV(vec2(0.75f, 1 - height * .5f));
	pTetrahedron->AddTextureUV(vec2(0.5f, 1 - height));

	// TODO Get proper normals
	pTetrahedron->AddNormal(vec3(0, 0, 0));

	pTetrahedron->AddIndice({ 2, 3, 0 });
	pTetrahedron->AddIndice({ 3, 4, 0 });
	pTetrahedron->AddIndice({ 0, 5, 0 });

	pTetrahedron->AddIndice({ 3, 4, 0 });
	pTetrahedron->AddIndice({ 1, 1, 0 });
	pTetrahedron->AddIndice({ 0, 2, 0 });

	pTetrahedron->AddIndice({ 1, 1, 0 });
	pTetrahedron->AddIndice({ 2, 3, 0 });
	pTetrahedron->AddIndice({ 0, 0, 0 });

	pTetrahedron->AddIndice({ 1, 1, 0 });
	pTetrahedron->AddIndice({ 3, 4, 0 });
	pTetrahedron->AddIndice({ 2, 3, 0 });

	GLM::DescribeVertex(pTetrahedron);
	return pTetrahedron;
}


jeEnd
