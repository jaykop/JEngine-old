#pragma once
#include "Macro.h"
#include "Vector3.h"

NS_JE_BEGIN

class PickingRay {

public:

	friend class InputManager;

	PickingRay();
	~PickingRay();

private:

	PickingRay(const PickingRay& /*_cpoy*/);
	void operator=(const PickingRay& /*_cpoy*/);

	void IntersectionWithPlane(vec3& _position);

	vec3 m_clikedPosition;
	vec3 m_direction;
};

NS_JE_END