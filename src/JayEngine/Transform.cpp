#include "Transform.h"
#include "PhysicsSystem.h"
#include "SystemManager.h"

JE_BEGIN

Transform::Transform(Object* _owner)
	:Component(_owner), 
	m_position(vec3::ZERO), m_scale(vec3::ZERO), 
	m_rotation(0.f), m_rotation3D(vec3::UNIT_Z)
{
	//SystemManager::GetPhysicsSystem()->AddTransform();
}

JE_END
