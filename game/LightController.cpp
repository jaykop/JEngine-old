#include "LightController.h"
#include "GameLogicHeader.h"

#include "Light.h"

JE_BEGIN

LightController::LightController(Object* _pObject)
	:CustomComponent(_pObject)
{}

void LightController::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void LightController::Load(CR_RJValue /*_data*/)
{}

void LightController::Init()
{
	if 	(m_pOwner->HasComponent<Light>())
		m_light = m_pOwner->GetComponent<Light>();
}

void LightController::Update(const float _dt)
{
	float speed = 60.f * _dt;
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

void LightController::Close()
{}

void LightController::Unload()
{}

LightControllerBuilder::LightControllerBuilder()
	:ComponentBuilder()
{}

CustomComponent* LightControllerBuilder::CreateComponent(Object* _pOwner) const
{
	return new LightController(_pOwner);
}

JE_END