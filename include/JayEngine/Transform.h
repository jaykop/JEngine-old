#pragma once
#include "Vector3.h"
#include "Component.h"

NS_JE_BEGIN

class Transform : public Component
{

public:

	friend class PhysicsSystem;
	friend class ComponentManager;

	vec3 m_position, m_scale;
	float m_rotation;

private:

	Transform(Object* _owner = nullptr);
	~Transform() {};
	Transform(const Transform& /*_copy*/) {};
	void operator= (const Transform& /*_copy*/) {};

};

NS_JE_END