#pragma once
#include "Macro.h"
#include "Component.h"
#include "Vector3.h"
#include "Vector4.h"

JE_BEGIN

class Light : public Component
{

	friend class GraphicSystem;
	friend class ComponentManager;

public:

	vec3		m_position;
	vec4		m_ambient, m_specular, m_diffuse, m_color;

private:

	Light(Object* _owner = nullptr);
	~Light() {};
	Light(const Light& /*_copy*/) {};
	void operator=(const Light& /*_copy*/) {};

};

JE_END