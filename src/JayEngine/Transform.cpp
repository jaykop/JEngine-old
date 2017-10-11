#include "Transform.h"

NS_JE_BEGIN

Transform::Transform(Object* _owner)
	:Component(_owner), 
	m_position(vec3::ZERO), m_scale(vec3::ZERO), m_rotation(0.f)
{}

NS_JE_END