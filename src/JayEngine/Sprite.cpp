#include "Sprite.h"
#include "Transform.h"
#include "AssetManager.h"
#include "GraphicSystem.h"
#include "SystemManager.h"

JE_BEGIN

Sprite::Sprite(Object* _owner)
	:Component(_owner), m_color(vec4::ONE),m_projection(PERSPECTIVE), 
	m_mainTex(0), m_curretFrame(0.f), m_animationSpeed(0.f), 
	m_animationFrames(1), m_animationFixFrame(1), m_realSpeed(0.f), 
	m_realFrame(1.f), m_activeAnimation(false), m_transform(nullptr),
	m_flip(false), m_culled(false), m_material(nullptr), m_hasMaterial(false)
{}

bool Sprite::GetActiveAnimationToggle()
{
	return m_activeAnimation;
}

void Sprite::ActiveAnimation(bool _toggle)
{
	m_activeAnimation = _toggle;
	if (m_activeAnimation)
		m_timer.Start();
}

void Sprite::FixAnimationFrame(int _thFrame)
{
	m_animationSpeed = 0.f;
	m_activeAnimation = false;
	m_curretFrame = float(_thFrame) * m_realFrame;
}

void Sprite::SetAnimationFrame(int _numOfFrame)
{
	m_animationFrames = _numOfFrame;
	m_realFrame = 1.f / float(m_animationFrames);
}

void Sprite::SetAnimationSpeed(float _speed)
{
	m_animationSpeed = _speed;
	m_realSpeed = 1.f / _speed;
}

void Sprite::Register()
{
	SystemManager::GetGraphicSystem()->AddSprite(this);
	if (m_pOwner->HasComponent<Transform>())
		m_transform = m_pOwner->GetComponent<Transform>();
}

int Sprite::GetAnimationFrame()
{
	return m_animationFrames;
}

float Sprite::GetAnimationSpeed()
{
	return m_animationSpeed;
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
	m_textureMap.clear();
	SystemManager::GetGraphicSystem()->RemoveSprite(this);
}

void Sprite::Load(CR_RJValue _data)
{
	CR_RJValue flip = _data["Flip"];
	m_flip = flip.GetBool();

	CR_RJValue color = _data["Color"];
	m_color.Set(color[0].GetFloat(), color[1].GetFloat(), 
		color[2].GetFloat(), color[3].GetFloat());

	CR_RJValue projection = _data["Projection"];

	if (!strcmp("Perspective", projection.GetString())) {
		m_projection = PERSPECTIVE;
	}
	
	else if (!strcmp("Orhtogonal", projection.GetString())) {
		m_projection = ORTHOGONAL;
	}
	
	CR_RJValue texture = _data["Texture"];
		AddTexture(texture.GetString());

	if (_data.HasMember("FixAt"))
	{
		CR_RJValue fix = _data["FixAt"];
		FixAnimationFrame(fix.GetInt());
	}
	
	else {
		CR_RJValue active = _data["Active"];
		m_activeAnimation = active.GetBool();
	}

	CR_RJValue frame = _data["Frame"];
	SetAnimationFrame(frame.GetInt());
	CR_RJValue speed = _data["Speed"];
	SetAnimationSpeed(speed.GetFloat());

}

SpriteBuilder::SpriteBuilder()
	:ComponentBuilder()
{}

Component* SpriteBuilder::CreateComponent(Object* _pOwner) const
{
	return new Sprite(_pOwner);
}

JE_END

