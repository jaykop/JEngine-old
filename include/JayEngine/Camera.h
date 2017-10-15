#pragma once
#include "Vector3.h"
#include "Component.h"

NS_JE_BEGIN

class Camera : public Component
{

	friend class GraphicSystem;
	friend class ComponentManager;

public:

	vec3 m_position, m_up, m_target;
	float m_rotation;

private:

	Camera(Object* _owner = nullptr);
	~Camera() {};

	Camera(const Camera& /*_copy*/) {};
	void operator=(const Camera& /*_copy*/) {};

};

NS_JE_END