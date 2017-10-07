#include "Transform.h"

NS_JE_BEGIN

Transform::Transform(Object* _owner)
	:Component(_owner), 
	m_position(vec3::ZERO), m_scale(vec3::ZERO), m_rotation(0.f)
{}

//void Transform::SetPosition(const vec3& _position)
//{
//	m_position = _position;
//}
//
//void Transform::SetScale(const vec3& _scale)
//{
//	m_scale = _scale;
//}
//
//void Transform::SetRotation(float _rotation)
//{
//	m_rotation = _rotation;
//}
//
//float Transform::GetRotation() const
//{
//	return m_rotation;
//}
//
//const vec3& Transform::GetScale() const
//{
//	return m_scale;
//}
//
//const vec3& Transform::GetPosition() const
//{
//	return m_position;
//}

NS_JE_END