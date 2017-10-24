#include "Light.h"
#include "Transform.h"
#include "SystemManager.h"
#include "GraphicSystem.h"

JE_BEGIN

Light::Light(Object * _owner)
	:Component(_owner), m_ambientStrength(.1f),
	m_specularStrength(.5f), m_color(vec4::ONE)
{
	SystemManager::GetGraphicSystem()->AddLight(this);
	if (m_pOwner->GetComponent<Transform>())
		m_transform = m_pOwner->GetComponent<Transform>();
}

JE_END
