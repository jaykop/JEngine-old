#pragma once
#include "Vector3.h"
#include "Component.h"

NS_JE_BEGIN

class Transform : public Component
{

	struct Edge {
		Vector3 m_leftTop, m_rightTop, m_leftBot, m_rightBot;
	};

	friend class PhysicsSystem;
	friend class ComponentManager;

public:

	vec3 m_position, m_scale, m_rotation3D;
	float m_rotation;
	Edge m_edges;

private:

	Transform(Object* _owner = nullptr);
	~Transform() {};
	Transform(const Transform& /*_copy*/) {};
	void operator= (const Transform& /*_copy*/) {};

};

NS_JE_END