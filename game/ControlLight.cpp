#include "ControlLight.h"
#include "GameLogicHeader.h"

#include "Light.h"

JE_BEGIN

ControlLight::ControlLight(Object* _pObject)
	:UserComponent(_pObject)
{}

void ControlLight::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void ControlLight::Load(CR_RJValue /*_data*/)
{}

void ControlLight::Init()
{
	if 	(m_pOwner->HasComponent<Light>())
		m_light = m_pOwner->GetComponent<Light>();
}

void ControlLight::Update(float _dt)
{
	float speed = 1.f * _dt;
	if (INPUT::KeyPressed(JE_A))
		m_light->m_position.x-= speed;
	
	if (INPUT::KeyPressed(JE_D))
		m_light->m_position.x+= speed;

	if (INPUT::KeyPressed(JE_W))
		m_light->m_position.y+= speed;
	
	if (INPUT::KeyPressed(JE_S))
		m_light->m_position.y-= speed;

	if (INPUT::KeyPressed(JE_Z))
		m_light->m_position.z+= speed;
	
	if (INPUT::KeyPressed(JE_C))
		m_light->m_position.z-= speed;
}

void ControlLight::Close()
{}

void ControlLight::Unload()
{}

ControlLightBuilder::ControlLightBuilder()
	:ComponentBuilder()
{}

UserComponent* ControlLightBuilder::CreateComponent(Object* _pOwner) const
{
	return new ControlLight(_pOwner);
}

JE_END