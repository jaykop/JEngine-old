#include "Sprite.h"
#include "GraphicSystem.h"
#include "SystemManager.h"

NS_JE_BEGIN

Sprite::Sprite(Object* _owner)
	:Component(_owner), m_color(vec4::ZERO)
{
	SystemManager::m_grpSystem->AddSprite(this);
}

Sprite::~Sprite()
{
	SystemManager::m_grpSystem->RemoveSprite(this);
}

void Sprite::SetColor(const vec4& _color)
{
	m_color = _color;
}

const vec4& Sprite::GetColor() const
{
	return m_color;
}


NS_JE_END

