#include "Camera.h"
#include "MathUtils.h"

NS_JE_BEGIN

Camera::Camera(Object* _owner)
	: Component(_owner),
	m_position(vec3::ZERO), m_up(vec3::UNIT_Y),
	m_target(vec3::ZERO), m_rotation(0.f)
{}

NS_JE_END

