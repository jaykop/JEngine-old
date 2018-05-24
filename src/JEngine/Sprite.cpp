#include "Sprite.h"
#include "GLManager.h"
#include "Material.h"
#include "Transform.h"
#include "AssetManager.h"
#include "SystemManager.h"
#include "MemoryAllocator.h"

jeBegin

jeDefineStaticAllocator(Sprite);

Sprite::Sprite(Object* _pOwner)
	:Component(_pOwner), m_color(vec4::ONE), m_projection(PROJECTION_PERSPECTIVE),
	m_mainTex(0), m_transform(nullptr), m_flip(false), m_culled(false), m_bilboard(false),
	m_material(nullptr), m_hasMaterial(false), m_isEmitter(false), m_isText(false),
	m_vao(&(GLM::m_vao[GLM::SHAPE_PLANE])), m_elementSize(GLM::m_elementSize[GLM::SHAPE_PLANE]),
	m_sfactor(GL_SRC_ALPHA), m_dfactor(GL_ONE_MINUS_SRC_ALPHA)
{}

void Sprite::Register()
{
	SYSTEM::GetGraphicSystem()->AddSprite(this);
	if (m_pOwner->HasComponent<Transform>())
		m_transform = m_pOwner->GetComponent<Transform>();
}

void Sprite::AddTexture(const char *_key)
{
	auto found = m_textureMap.find(_key);
	if (found != m_textureMap.end())
		jeDebugPrint("!Sprite - Existing texture: %s.\n", _key);

	else {
		unsigned newTexture = ASSET::GetTexture(_key);

		if (!m_textureMap.size())
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
	m_color.Set(_copy.m_color); 
	m_projection = _copy.m_projection,
	m_mainTex = _copy.m_mainTex; 
	m_transform = m_pOwner->GetComponent<Transform>(); 
	m_flip = _copy.m_flip; 
	m_culled = _copy.m_culled; 
	m_bilboard = _copy.m_bilboard;
	m_material = m_pOwner->GetComponent<Material>();
	m_hasMaterial = _copy.m_hasMaterial;
	m_isEmitter = _copy.m_isEmitter;
	m_isText = _copy.m_isText;
	m_vao = _copy.m_vao; 
	m_elementSize = _copy.m_elementSize;
}

void Sprite::Load(CR_RJValue _data)
{
	if (_data.HasMember("Flip")) {
		CR_RJValue flip = _data["Flip"];
		m_flip = flip.GetBool();
	}

	if (_data.HasMember("Color")) {
		CR_RJValue color = _data["Color"];
		m_color.Set(color[0].GetFloat(), color[1].GetFloat(),
			color[2].GetFloat(), color[3].GetFloat());
	}

	if (_data.HasMember("Projection")) {
		CR_RJValue projection = _data["Projection"];

		if (!strcmp("Perspective", projection.GetString())) {
			m_projection = PROJECTION_PERSPECTIVE;
		}

		else if (!strcmp("Orthogonal", projection.GetString())) {
			m_projection = PROJECTION_ORTHOGONAL;
		}
		else
			jeDebugPrint("!Sprite - Wrong projection type: %s\n", projection.GetString());
	}

	if (_data.HasMember("Texture")) {
		CR_RJValue texture = _data["Texture"];
		AddTexture(texture.GetString());
	}

	if (_data.HasMember("Bilboard"))
		m_bilboard = _data["Bilboard"].GetBool();
}

void Sprite::EditorUpdate(const float /*_dt*/)
{
	// TODO
}

SpriteBuilder::SpriteBuilder()
	:ComponentBuilder()
{}

Component* SpriteBuilder::CreateComponent(Object* _pOwner) const
{
	return new Sprite(_pOwner);
}

jeEnd

