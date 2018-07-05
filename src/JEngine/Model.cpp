#include "Model.h"
#include "Material.h"
#include "Transform.h"
#include "AssetManager.h"
#include "SystemManager.h"
#include "Object.h"
#include "Mesh.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Model);

Model::Model(Object* pOwner)
	:Component(pOwner), drawMode_(GL_TRIANGLES), color_(vec4::ONE), projection_(PROJECTION_PERSPECTIVE), 
	pTransform_(nullptr), culled_(false), pMaterial_(nullptr), sfactor_(GL_SRC_ALPHA),
	dfactor_(GL_ONE_MINUS_SRC_ALPHA), pAnimation_(nullptr), is_(0x0000), pInherited_(nullptr)
{}

void Model::Register()
{
	SYSTEM::pGraphic_->AddModel(this);
	if (GetOwner()->HasComponent<Transform>())
		pTransform_ = GetOwner()->GetComponent<Transform>();
}

void Model::AddMesh(Mesh* pMesh)
{
	return meshes_.push_back(pMesh);
}

void Model::RemoveMesh(unsigned index)
{
	unsigned count = 0;
	for (auto it = meshes_.begin();
		it != meshes_.end(); ++it, ++count) {
		if (count == index)
			meshes_.erase(it);
	}
}

Mesh* Model::GetMesh(unsigned index) const
{
	return meshes_.at(index);
}

unsigned Model::GetMeshCount() const
{
	return unsigned(meshes_.size());
}

void Model::SetParentToFollow(Object* pObject)
{
	if (pObject->HasComponent<Transform>()) {
		pInherited_ = pObject->GetComponent<Transform>();
		status_ |= Model::IS_INHERITED;
	}
	else
		jeDebugPrint("!Model - Object to be parent does not habe transform component!: %s\n", pObject->GetName().c_str());
}

Model::~Model()
{
	// Clear mesh container
	for (auto mesh : meshes_) {
		if (!mesh->builtIn_) {
			delete mesh;
			mesh = nullptr;
		}
	}
	meshes_.clear();

	if (IS_LIGHT != is_)
		SYSTEM::pGraphic_->RemoveModel(this);
}

void Model::operator=(const Model & copy)
{
	color_.Set(copy.color_);
	projection_ = copy.projection_,
	pTransform_ = GetOwner()->GetComponent<Transform>();
	culled_ = copy.culled_;
	pMaterial_ = GetOwner()->GetComponent<Material>();
	is_ = copy.is_;
}

void Model::Load(CR_RJValue data)
{
	if (data.HasMember("Mesh")
		&& data["Mesh"].IsArray())
	{
		CR_RJValue loadedMeshes = data["Mesh"];

		for (unsigned meshIndex = 0; meshIndex < loadedMeshes.Size(); ++meshIndex) {

			CR_RJValue currentMesh = loadedMeshes[meshIndex];
			Mesh* newMesh = nullptr;

			// Check either if there is shape
			if (currentMesh.HasMember("Shape")
				&& currentMesh["Shape"].IsString()) {
				std::string meshType = currentMesh["Shape"].GetString();

				if (!strcmp(meshType.c_str(), "Point"))
					newMesh = Mesh::CreatePoint();

				else if (!strcmp(meshType.c_str(), "CrossRect"))
					newMesh = Mesh::CreateCrossRect();

				else if (!strcmp(meshType.c_str(), "Cube"))
					newMesh = Mesh::CreateCube();

				else if (!strcmp(meshType.c_str(), "Tetrahedron"))
					newMesh = Mesh::CreateTetrahedron();

				else if (!strcmp(meshType.c_str(), "Rect"))
					newMesh = Mesh::CreateRect();

				else 
					newMesh = ASSET::LoadObjFile(meshType.c_str());
			}
			// If not, set default mesh type
			else
				newMesh = Mesh::CreateRect();

			AddMesh(newMesh);

			// Load texture
			if (currentMesh.HasMember("Texture")
				&& currentMesh["Texture"].IsArray()) {
				for (unsigned textureIndex = 0; textureIndex < currentMesh["Texture"].Size(); ++textureIndex)
					newMesh->AddTexture(currentMesh["Texture"][textureIndex].GetString());
			}
		}
	}

	if (data.HasMember("DrawMode")
		&& data["DrawMode"].IsString()) {
		
		std::string drawType = data["DrawMode"].GetString();
		if (!strcmp(drawType.c_str(), "Triangles"))
			drawMode_ = GL_TRIANGLES;

		else if (!strcmp(drawType.c_str(), "Triangle_Strip"))
			drawMode_ = GL_TRIANGLE_STRIP;

		else if (!strcmp(drawType.c_str(), "Triangle_Fan"))
			drawMode_ = GL_TRIANGLE_FAN;

		else if (!strcmp(drawType.c_str(), "Lines"))
			drawMode_ = GL_LINES;

		else if (!strcmp(drawType.c_str(), "Line_Strip"))
			drawMode_ = GL_LINE_STRIP;

		else if (!strcmp("Quad", drawType.c_str()))
			drawMode_ = GL_QUADS;

		else if (!strcmp("Quad_Strip", drawType.c_str()))
			drawMode_ = GL_QUAD_STRIP;

		else if (!strcmp("Points", drawType.c_str()))
			drawMode_ = GL_POINTS;
	}

	if (data.HasMember("Flip")
		&& data["Flip"].GetBool())
		status_ |= IS_FLIPPED;

	if (data.HasMember("Color")) {
		CR_RJValue loadedColor = data["Color"];
		color_.Set(loadedColor[0].GetFloat(), loadedColor[1].GetFloat(),
			loadedColor[2].GetFloat(), loadedColor[3].GetFloat());
	}

	if (data.HasMember("Projection")) {
		CR_RJValue loadedProjection = data["Projection"];

		if (!strcmp("Perspective", loadedProjection.GetString()))
			projection_ = PROJECTION_PERSPECTIVE;

		else if (!strcmp("Orthogonal", loadedProjection.GetString()))
			projection_ = PROJECTION_ORTHOGONAL;

		else
			jeDebugPrint("!Model - Wrong projection type: %s\n", loadedProjection.GetString());
	}

	if (data.HasMember("Bilboard")
		&& data["Bilboard"].GetBool())
		status_ |= IS_BILBOARD;
}

void Model::EditorUpdate(const float /*dt*/)
{
	// TODO
}

jeEnd

