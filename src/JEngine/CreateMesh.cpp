#include "GraphicSystem.h"
#include "Mesh.h"

jeBegin

void GraphicSystem::DescribeVertex(Mesh* pMesh)
{
	// Check either if all the objects are initialized
	// and if not, generate them
	if (!pMesh->m_vao)
		glGenVertexArrays(1, &pMesh->m_vao);
	if (!pMesh->m_vbo)
		glGenBuffers(1, &pMesh->m_vbo);
	if (!pMesh->m_ebo)
		glGenBuffers(1, &pMesh->m_ebo);

	// Set vertices and indices vector container
	std::vector<Mesh::jeVertex> vertices;
	std::vector<unsigned> indicies;
	unsigned indiceCount = unsigned(pMesh->GetIndiceCount());
	vertices.reserve(indiceCount);
	indicies.reserve(indiceCount);

	for (unsigned index = 0; index < indiceCount; ++index) {
		Mesh::jeIndex vi = pMesh->GetIndices().at(index);
		vertices.push_back(Mesh::jeVertex{
			pMesh->GetPoint(vi.a),
			pMesh->GetUV(vi.b),
			pMesh->GetNormal(vi.c) });
		indicies.push_back(index);
	}

	// Decribe the format of vertex and indice
	glBindVertexArray(pMesh->m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, pMesh->m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh::jeVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indicies.size(), &indicies[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::jeVertex),
		reinterpret_cast<void*>(offsetof(Mesh::jeVertex, jeVertex::position)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::jeVertex),
		reinterpret_cast<void*>(offsetof(Mesh::jeVertex, jeVertex::uv)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::jeVertex),
		reinterpret_cast<void*>(offsetof(Mesh::jeVertex, jeVertex::normal)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

Mesh* GraphicSystem::CreatePoint()
{
	Mesh *pPoint = new Mesh;

	pPoint->AddPoint(vec3(0, 0));
	pPoint->AddTextureUV(vec2(0, 0));
	pPoint->AddNormal(vec3(0, 0, 1.f));
	pPoint->AddIndice({ 0,0,0 });

	pPoint->m_shape = Mesh::MESH_POINT;
	pPoint->builtIn_ = true;
	return pPoint;
}

Mesh* GraphicSystem::CreateRect()
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

	pRect->m_shape = Mesh::MESH_RECT;
	pRect->builtIn_ = true;
	return pRect;
}

Mesh* GraphicSystem::CreateCrossRect()
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

	pCrossRect->m_shape = Mesh::MESH_CROSSRECT;
	pCrossRect->builtIn_ = true;
	return pCrossRect;
}

Mesh* GraphicSystem::CreateCube()
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

	pCube->m_shape = Mesh::MESH_CUBE;
	pCube->builtIn_ = true;
	return pCube;
}

Mesh* GraphicSystem::CreateTetrahedron()
{
	Mesh *pTetrahedron = new Mesh;

	pTetrahedron->AddPoint(vec3(0.f, 1.f, .5f));
	pTetrahedron->AddPoint(vec3(-.5f, 0.f, -.5f));
	pTetrahedron->AddPoint(vec3(-.5f, 0.f, .5f));
	pTetrahedron->AddPoint(vec3(.5f, 0.f, .5f));
	pTetrahedron->AddPoint(vec3(.5f, 0.f, -.5f));

	// TODO
	pTetrahedron->AddTextureUV(vec2(0,0));
	pTetrahedron->AddNormal(vec3(0,0,0));

	return pTetrahedron;
}

jeEnd