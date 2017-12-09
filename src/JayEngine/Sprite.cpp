#include "Sprite.h"
#include "Transform.h"
#include "AssetManager.h"
#include "GraphicSystem.h"
#include "SystemManager.h"

JE_BEGIN

Sprite::Sprite(Object* _owner)
	:Component(_owner), m_color(vec4::ONE),m_projection(PERSPECTIVE), 
	m_mainTex(0),m_transform(nullptr), m_flip(false), m_culled(false), 
	m_material(nullptr), m_hasMaterial(false), m_isEmitter(false)
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
		JE_DEBUG_PRINT("Found exsiting name of texture: %s.\n", _key);

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

	JE_DEBUG_PRINT("Cannot find such name of texture: %s.\n", _key);
	return 0;
}

Sprite::~Sprite()
{
	// Remove textures
	m_textureMap.clear();
	SystemManager::GetGraphicSystem()->RemoveSprite(this);

	// Remove effects
	for (auto effect : m_effects) {
		delete effect.second;
		effect.second = nullptr;
	}

	m_effects.clear();
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
			m_projection = PERSPECTIVE;
		}

		else if (!strcmp("Orhtogonal", projection.GetString())) {
			m_projection = ORTHOGONAL;
		}
	}

	if (_data.HasMember("Texture")) {
		CR_RJValue texture = _data["Texture"];
		AddTexture(texture.GetString());
	}

	if (_data.HasMember("Effect")) {
		CR_RJValue effect = _data["Effect"];

		for (unsigned i = 0; i < effect.Size(); ++i) {
			if (!strcmp(effect[i].GetString(), "Blur"))
				AddEffect<Blur>();
			else if (!strcmp(effect[i].GetString(), "Manipulation"))
				AddEffect<Manipulation>();
			else if (!strcmp(effect[i].GetString(), "Inverse"))
				AddEffect<Inverse>();
			else if (!strcmp(effect[i].GetString(), "Sobel"))
				AddEffect<Sobel>();
		}
	}
}

void Sprite::ConvertVEType(const char* _name, VisualEffect::VEType& _veType)
{
	std::string additional("class JEngine::");
	std::string blur = additional + "Blur", inv = additional + "Sobel", 
		sobel = additional + "Sobel", manip = additional + "Sobel";

	if (!strcmp(_name, blur.c_str()))
		_veType = VisualEffect::VEType::VE_BLUR;
	else if (!strcmp(_name, sobel.c_str()))
		_veType = VisualEffect::VEType::VE_SOBEL;
	else if (!strcmp(_name, inv.c_str()))
		_veType = VisualEffect::VEType::VE_INVERSE;
	else if (!strcmp(_name, manip.c_str()))
		_veType = VisualEffect::VEType::VE_MANIPULATION;
}

SpriteBuilder::SpriteBuilder()
	:ComponentBuilder()
{}

Component* SpriteBuilder::CreateComponent(Object* _pOwner) const
{
	return new Sprite(_pOwner);
}

JE_END

