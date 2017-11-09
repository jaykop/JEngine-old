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

void Transform::Load(CR_RJValue _data)
{
	CR_RJValue position = _data["Position"];
	CR_RJValue scale = _data["Scale"];
	CR_RJValue rotation = _data["Rotation"];
	CR_RJValue rotation3D = _data["Rotation3d"];

	m_position.Set(position[0].GetFloat(), position[1].GetFloat(), position[2].GetFloat());
	m_scale.Set(scale[0].GetFloat(), scale[1].GetFloat(), scale[2].GetFloat());
	m_rotation3D.Set(rotation3D[0].GetFloat(), rotation3D[1].GetFloat(), rotation3D[2].GetFloat());
	m_rotation = rotation.GetFloat();
}

TransformBuilder::TransformBuilder()
	:ComponentBuilder()
{}

Component* TransformBuilder::CreateComponent(Object* _pOwner) const
{
	return new Transform(_pOwner);
}

JE_END
