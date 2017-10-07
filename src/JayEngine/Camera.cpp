#include "Camera.h"
#include "MathUtils.h"

NS_JE_BEGIN

Camera::Camera()
	:m_position(vec3::ZERO), m_up(vec3::ZERO), 
	m_target(vec3::ZERO), m_rotation(0.f)
{}

//void Camera::SetRotation2D(float _rotation)
//{
//	m_rotation = _rotation;
//	float degree = m_rotation + 90.f;
//	m_up.x = cosf(Math::DegToRad(degree));
//	m_up.y = sinf(Math::DegToRad(degree));
//}
//
//void Camera::SetTarget(const vec3& _target)
//{
//	m_target = _target;
//}
//
//void Camera::SetUpVector(const vec3& _upVector)
//{
//	m_up = _upVector;
//}
//
//void Camera::SetPosition(const vec3& _position)
//{
//	m_position = _position;
//}
//
//float Camera::GetRotation2D()
//{
//	return m_rotation;
//}
//
//const vec3& Camera::GetTarget() const
//{
//	return m_target;
//}
//
//const vec3& Camera::GetPosition() const
//{
//	return m_position;
//}
//
//const vec3& Camera::GetUpVector() const
//{
//	return m_up;
//}

NS_JE_END

