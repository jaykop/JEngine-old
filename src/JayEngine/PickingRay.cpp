#include "PickingRay.h"

NS_JE_BEGIN

void PickingRay::IntersectionWithPlane(vec3& _position)
{
	float s = -m_clikedPosition.z / m_direction.z;
	_position = m_clikedPosition + m_direction*s;
	_position.z = 0;
}

NS_JE_END

