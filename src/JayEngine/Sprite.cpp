#include "Sprite.h"
#include "AssetManager.h"
#include "GraphicSystem.h"
#include "SystemManager.h"

NS_JE_BEGIN

Sprite::Sprite(Object* _owner)
	:Component(_owner), m_color(vec4::UNIT_W),
	m_projection(PERSPECTIVE), m_mainTex(nullptr)
{
	SystemManager::m_grpSystem->AddSprite(this);
}

void Sprite::FixAnimationFrame(int _thFrame)
{
	m_animationSpeed = 0.f;
	m_activeAnimation = false;
	m_realFrame = _thFrame * m_realFrame;
}

void Sprite::SetAnimationFrame(int _numOfFrame)
{
	m_animationFrames = _numOfFrame;
	m_realFrame = 1.f / float(_numOfFrame);
}

void Sprite::SetAnimationSpeed(float _speed)
{
	m_animationSpeed = _speed;
	m_realSpeed = 1.f / _speed;
}

void Sprite::AddTexture(const char *_key)
{
	auto found = m_textureMap.find(_key);
	if (found != m_textureMap.end())
		JE_DEBUG_PRINT("Found exsiting name of texture: %s.\n", _key);

	else {
		Texture* newTexture = AssetManager::GetTexture(_key);

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

Texture* Sprite::GetCurrentTexutre()
{
	return m_mainTex;
}

Texture* Sprite::GetTexutre(const char *_key)
{
	auto found = m_textureMap.find(_key);
	if (found != m_textureMap.end())
		return found->second;

	JE_DEBUG_PRINT("Cannot find such name of texture: %s.\n", _key);
	return nullptr;
}

Sprite::~Sprite()
{
	m_textureMap.clear();
	SystemManager::m_grpSystem->RemoveSprite(this);
}

NS_JE_END

