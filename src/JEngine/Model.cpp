#include "Model.h"
#include "GLManager.h"
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

Model::Model(Object* _pOwner)
	:Component(_pOwner), color(vec4::ONE), projection(PROJECTION_PERSPECTIVE), m_mainTex(0),
	m_pTransform(nullptr), m_culled(false), m_pMaterial(nullptr), sfactor(GL_SRC_ALPHA),
	dfactor(GL_ONE_MINUS_SRC_ALPHA), m_pAnimation(nullptr), m_hiddenStatus(0x0000), m_pInherited(nullptr)
{}

void Model::Register()
{
	SYSTEM::pGraphic_->AddModel(this);
	if (GetOwner()->HasComponent<Transform>())
		m_pTransform = GetOwner()->GetComponent<Transform>();
}

void Model::SetParentToFollow(Object* _pObj)
{
	if (_pObj->HasComponent<Transform>()) {
		m_pInherited = _pObj->GetComponent<Transform>();
		status |= Model::IS_INHERITED;
	}
	else
		jeDebugPrint("!Model - Object to be parent does not habe transform component!: %s\n", _pObj->GetName().c_str());
}

void Model::AddTexture(const char *_key)
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

void Model::RemoveTexture(const char *_key)
{
	m_textureMap.erase(_key);
}

void Model::SetCurrentTexutre(const char *_key)
{
	m_mainTex = GetTexutre(_key);
}

unsigned Model::GetCurrentTexutre() const
{
	return m_mainTex;
}

unsigned Model::GetTexutre(const char *_key)
{
	auto found = m_textureMap.find(_key);
	if (found != m_textureMap.end())
		return found->second;

	jeDebugPrint("!Model - No such name of enrolled texture: %s.\n", _key);
	return 0;
}

Model::~Model()
{
	// Remove textures
	m_textureMap.clear();
	
	if (m_pMeshes) {
		if (!m_pMeshes->builtIn_) {
			delete m_pMeshes;
			m_pMeshes = nullptr;
		}
	}

	SYSTEM::pGraphic_->RemoveModel(this);
}

void Model::operator=(const Model & _copy)
{
	color.Set(_copy.color);
	projection = _copy.projection,
	m_mainTex = _copy.m_mainTex;
	m_pTransform = GetOwner()->GetComponent<Transform>();
	m_culled = _copy.m_culled;
	m_pMaterial = GetOwner()->GetComponent<Material>();
	m_hiddenStatus = _copy.m_hiddenStatus;
}

void Model::Load(CR_RJValue _data)
{
	if (_data.HasMember("Mesh")
		&& _data["Mesh"].GetString())
	{
		std::string meshType = _data["Mesh"].GetString();
		if (!strcmp(meshType.c_str(), "Point")) {
			m_pMeshes = GLM::pMesh_[GLM::SHAPE_POINT];
			m_pMeshes->m_shape = Mesh::MESH_POINT;
		}
		else if (!strcmp(meshType.c_str(), "Rect")) {
			m_pMeshes = GLM::pMesh_[GLM::SHAPE_RECT];
			m_pMeshes->m_shape = Mesh::MESH_RECT;
		}
		else if (!strcmp(meshType.c_str(), "CrossRect")) {
			m_pMeshes = GLM::pMesh_[GLM::SHAPE_CROSSRECT];
			m_pMeshes->m_shape = Mesh::MESH_CROSSRECT;
		}
		else if (!strcmp(meshType.c_str(), "Cube")) {
			m_pMeshes = GLM::pMesh_[GLM::SHAPE_CUBE];
			m_pMeshes->m_shape = Mesh::MESH_CUBE;
		}
		else if (!strcmp(meshType.c_str(), "Tetrahedron")) {
			m_pMeshes = GLM::pMesh_[GLM::SHAPE_TETRAHEDRON];
			m_pMeshes->m_shape = Mesh::MESH_TETRAHEDRON;
		}
		else /*if (!strcmp(meshType.c_str(), "Custom"))*/ {
			m_pMeshes = ASSET::LoadObjFile(meshType.c_str());
			GraphicSystem::DescribeVertex(m_pMeshes);
			m_pMeshes->m_shape = Mesh::MESH_CUSTOM;
			m_pMeshes->builtIn_ = false;
		}
	}
	else {
		m_pMeshes = GraphicSystem::CreateRect();
		m_pMeshes->m_shape = Mesh::MESH_RECT;
	}

	if (_data.HasMember("DrawMode")
		&& _data["DrawMode"].IsString()) {
		
		std::string drawType = _data["DrawMode"].GetString();
		if (!strcmp(drawType.c_str(), "Triangles"))
			m_pMeshes->m_drawMode = GL_TRIANGLES;

		else if (!strcmp(drawType.c_str(), "Triangle_Strip"))
			m_pMeshes->m_drawMode = GL_TRIANGLE_STRIP;

		else if (!strcmp(drawType.c_str(), "Triangle_Fan")) 
			m_pMeshes->m_drawMode = GL_TRIANGLE_FAN;

		else if (!strcmp(drawType.c_str(), "Lines")) 
			m_pMeshes->m_drawMode = GL_LINES;

		else if (!strcmp(drawType.c_str(), "Line_Strip")) 
			m_pMeshes->m_drawMode = GL_LINE_STRIP;
	}

	if (_data.HasMember("Flip")
		&& _data["Flip"].GetBool())
		status |= IS_FLIPPED;

	if (_data.HasMember("Color")) {
		CR_RJValue loadedColor = _data["Color"];
		color.Set(loadedColor[0].GetFloat(), loadedColor[1].GetFloat(),
			loadedColor[2].GetFloat(), loadedColor[3].GetFloat());
	}

	if (_data.HasMember("Projection")) {
		CR_RJValue loadedProjection = _data["Projection"];

		if (!strcmp("Perspective", loadedProjection.GetString()))
			projection = PROJECTION_PERSPECTIVE;

		else if (!strcmp("Orthogonal", loadedProjection.GetString()))
			projection = PROJECTION_ORTHOGONAL;

		else
			jeDebugPrint("!Model - Wrong projection type: %s\n", loadedProjection.GetString());
	}

	if (_data.HasMember("Texture")) {
		CR_RJValue loadedTexture = _data["Texture"];
		AddTexture(loadedTexture.GetString());
	}

	if (_data.HasMember("Bilboard")
		&& _data["Bilboard"].GetBool())
		status |= IS_BILBOARD;
}

void Model::EditorUpdate(const float /*dt*/)
{
	// TODO
}

jeEnd

