#include "Sprite.h"
#include "GLManager.h"
#include "Material.h"
#include "Transform.h"
#include "AssetManager.h"
#include "SystemManager.h"
#include "Object.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Sprite);

Sprite::Sprite(Object* _pOwner)
	:Component(_pOwner), color(vec4::ONE), projection(PROJECTION_PERSPECTIVE),
	m_mainTex(0), m_pTransform(nullptr), flip(false), m_culled(false), bilboard(false),
	m_pMaterial(nullptr), sfactor(GL_SRC_ALPHA), dfactor(GL_ONE_MINUS_SRC_ALPHA), m_pAnimation(nullptr),
	pVao(&(GLM::m_vao[GLM::SHAPE_PLANE])), elementSize(GLM::m_elementSize[GLM::SHAPE_PLANE]),
	status(0x0000)
{}

void Sprite::Register()
{
	SYSTEM::GetGraphicSystem()->AddSprite(this);
	if (GetOwner()->HasComponent<Transform>())
		m_pTransform = GetOwner()->GetComponent<Transform>();
}

void Sprite::AddTexture(const char *_key)
{
	auto found = m_textureMap.find(_key);
	if (found != m_textureMap.end())
		jeDebugPrint("!Sprite - Existing texture: %s.\n", _key);

	else {
		unsigned newTexture = ASSET::GetTexture(_key);

		if (m_textureMap.empty())
			m_mainTex = newTexture;

		m_textureMap.insert(
			TextureMap::value_type(
				_key, newTexture));
	}
}

void Sprite::RemoveTexture(const char *_key)
{
	m_textureMap.erase(_key);
}

void Sprite::SetCurrentTexutre(const char *_key)
{
	m_mainTex = GetTexutre(_key);
}

unsigned Sprite::GetCurrentTexutre()
{
	return m_mainTex;
}

unsigned Sprite::GetTexutre(const char *_key)
{
	auto found = m_textureMap.find(_key);
	if (found != m_textureMap.end())
		return found->second;

	jeDebugPrint("!Sprite - No such name of enrolled texture: %s.\n", _key);
	return 0;
}

Sprite::~Sprite()
{
	// Remove textures
	m_textureMap.clear();
	SYSTEM::GetGraphicSystem()->RemoveSprite(this);
}

void Sprite::operator=(const Sprite & _copy)
{
	color.Set(_copy.color); 
	projection = _copy.projection,
	m_mainTex = _copy.m_mainTex; 
	m_pTransform = GetOwner()->GetComponent<Transform>(); 
	flip = _copy.flip; 
	m_culled = _copy.m_culled; 
	bilboard = _copy.bilboard;
	m_pMaterial = GetOwner()->GetComponent<Material>();
	status = _copy.status;
	pVao = _copy.pVao; 
	elementSize = _copy.elementSize;
}

void Sprite::Load(CR_RJValue _data)
{
	if (_data.HasMember("Flip")) {
		CR_RJValue loadedFlip = _data["Flip"];
		flip = loadedFlip.GetBool();
	}

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
			jeDebugPrint("!Sprite - Wrong projection type: %s\n", loadedProjection.GetString());
	}

	if (_data.HasMember("Texture")) {
		CR_RJValue loadedTexture = _data["Texture"];
		AddTexture(loadedTexture.GetString());
	}

	if (_data.HasMember("Bilboard"))
		bilboard = _data["Bilboard"].GetBool();
}

void Sprite::EditorUpdate(const float /*_dt*/)
{
	// TODO
}

jeEnd

