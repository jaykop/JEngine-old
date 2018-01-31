#include "Sprite.h"
#include "GLManager.h"
#include "Transform.h"
#include "AssetManager.h"
#include "SystemManager.h"

JE_BEGIN

Sprite::Sprite(Object* _pOwner)
	:Component(_pOwner), m_color(vec4::ONE), m_projection(PROJECTION_PERSPECTIVE),
	m_mainTex(0), m_transform(nullptr), m_flip(false), m_culled(false), m_bilboard(false),
	m_material(nullptr), m_hasMaterial(false), m_isEmitter(false), 
	m_vao(&(GLM::m_vao[GLM::SHAPE_PLANE])), m_elementSize(GLM::m_elementSize[GLM::SHAPE_PLANE])
	/*, m_isModel(false)*/
{}

void Sprite::Register()
{
	SystemManager::GetGraphicSystem()->AddSprite(this);
	if (m_pOwner->HasComponent<Transform>())
		m_transform = m_pOwner->GetComponent<Transform>();
}

void Sprite::AddTexture(const char *_key)
{
	auto found = m_textureMap.find(_key);
	if (found != m_textureMap.end())
		JE_DEBUG_PRINT("*Sprite: Existing texture - %s.\n", _key);

	else {
		unsigned newTexture = AssetManager::GetTexture(_key);

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

	JE_DEBUG_PRINT("*Sprite: No such name of enrolled texture - %s.\n", _key);
	return 0;
}

Sprite::~Sprite()
{
	// Remove textures
	m_textureMap.clear();
	SystemManager::GetGraphicSystem()->RemoveSprite(this);
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
			JE_DEBUG_PRINT("*Sprite: Wrong projection type - %s\n", projection.GetString());
	}

	if (_data.HasMember("Texture")) {
		CR_RJValue texture = _data["Texture"];
		AddTexture(texture.GetString());
	}

	if (_data.HasMember("Bilboard"))
		m_bilboard = _data["Bilboard"].GetBool();
}

SpriteBuilder::SpriteBuilder()
	:ComponentBuilder()
{}

Component* SpriteBuilder::CreateComponent(Object* _pOwner) const
{
	return new Sprite(_pOwner);
}

JE_END

