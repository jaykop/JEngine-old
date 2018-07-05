#include "Mesh.h"
#include "AssetManager.h"
#include "GLManager.h"

jeBegin

Mesh::Mesh() : vao_(0), vbo_(0), ebo_(0), 
	builtIn_(false), mainTexture_(0) {}

Mesh::~Mesh() {

	ClearVertexes();

	// Remove textures
	textureMap_.clear();

	// Delete buffers
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &vbo_);
	glDeleteBuffers(1, &ebo_);
}

void Mesh::AddPoint(CR_Vec3 point) { points_.push_back(point); }

void Mesh::AddTextureUV(CR_Vec2 uv) { textureUVs_.push_back(uv); }

void Mesh::AddNormal(CR_Vec3 normal) { normals_.push_back(normal); }

void Mesh::AddIndice(jeIndex indice) { indices_.push_back(indice); }

vec3 Mesh::GetPoint(unsigned index) const { return points_.at(index); }

vec2 Mesh::GetUV(unsigned index) const { return textureUVs_.at(index); }

vec3 Mesh::GetNormal(unsigned index) const { return normals_.at(index); }

Mesh::jeIndex Mesh::GetIndice(unsigned index) const { return indices_.at(index); }

std::size_t Mesh::GetPointCount() const { return points_.size(); }

std::size_t Mesh::GetIndiceCount() const { return indices_.size(); }

const std::vector<vec3>& Mesh::GetNormals() const { return normals_; }

const std::vector<Mesh::jeIndex>& Mesh::GetIndices() const { return indices_; }

void Mesh::ClearPoints() { points_.clear(); }

void Mesh::ClearNormals() { normals_.clear(); }

void Mesh::ClearUVs() { textureUVs_.clear(); }

void Mesh::ClearIndices() { indices_.clear(); }

void Mesh::ClearVertexes() {

	ClearPoints();
	ClearUVs();
	ClearNormals();
	ClearIndices();
}

void Mesh::AddTexture(const char *key)
{
	auto found = textureMap_.find(key);
	if (found != textureMap_.end())
		jeDebugPrint("!Model - Existing texture: %s.\n", key);

	else {
		unsigned newTexture = ASSET::GetTexture(key);

		if (textureMap_.empty())
			mainTexture_ = newTexture;

		textureMap_.insert(
			TextureMap::value_type(
				key, newTexture));
	}
}

void Mesh::RemoveTexture(const char *key)
{
	textureMap_.erase(key);
}

void Mesh::SetCurrentTexutre(const char *key)
{
	mainTexture_ = GetTexutre(key);
}

unsigned Mesh::GetCurrentTexutre() const
{
	return mainTexture_;
}

unsigned Mesh::GetTexutre(const char *key)
{
	auto found = textureMap_.find(key);
	if (found != textureMap_.end())
		return found->second;

	jeDebugPrint("!Model - No such name of enrolled texture: %s.\n", key);
	return 0;
}


Mesh* Mesh::CreatePoint()
{
	Mesh *pPoint = new Mesh;

	pPoint->AddPoint(vec3(0, 0));
	pPoint->AddTextureUV(vec2(0, 0));
	pPoint->AddNormal(vec3(0, 0, 0.f));
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

Mesh* Mesh::CreateWireframeBox()
{
	Mesh*  pLineBox = new Mesh;

	pLineBox->AddPoint(vec3(-.5f, .5f, -.5f));
	pLineBox->AddPoint(vec3(-.5f, .5f, .5f));
	pLineBox->AddPoint(vec3(.5f, .5f, .5f));
	pLineBox->AddPoint(vec3(.5f, .5f, -.5f));

	pLineBox->AddPoint(vec3(-.5f, -.5f, -.5f));
	pLineBox->AddPoint(vec3(-.5f, -.5f, .5f));
	pLineBox->AddPoint(vec3(.5f, -.5f, .5f));
	pLineBox->AddPoint(vec3(.5f, -.5f, -.5f));

	pLineBox->AddTextureUV(vec2::ZERO);
	pLineBox->AddNormal(vec3::ZERO);

	pLineBox->AddIndice({ 0, 0, 0 });
	pLineBox->AddIndice({ 1, 0, 0 });
	pLineBox->AddIndice({ 2, 0, 0 });
	pLineBox->AddIndice({ 3, 0, 0 });

	pLineBox->AddIndice({ 4, 0, 0 });
	pLineBox->AddIndice({ 5, 0, 0 });
	pLineBox->AddIndice({ 6, 0, 0 });
	pLineBox->AddIndice({ 7, 0, 0 });

	pLineBox->builtIn_ = true;
	
	GLM::DescribeVertex(pLineBox);

	return pLineBox;
}


jeEnd
