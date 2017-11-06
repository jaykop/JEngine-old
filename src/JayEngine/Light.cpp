#include "Light.h"
#include "Transform.h"
#include "SystemManager.h"
#include "GraphicSystem.h"

JE_BEGIN

Light::Light(Object * _owner)
	:Component(_owner), m_color(vec4::ONE),
	m_ambient(vec4::ONE), m_diffuse(vec4::ONE),
	m_specular(vec4::ONE), m_position(vec3(0.f, 0.f, 1.f))
{
	SystemManager::GetGraphicSystem()->AddLight(this);
}

void Light::Load(const RJValue& /*_data*/)
{
}


LightBuilder::LightBuilder()
	:ComponentBuilder()
{}

Component* LightBuilder::CreateComponent(Object* _pOwner) const
{
	return new Light(_pOwner);
}

JE_END
