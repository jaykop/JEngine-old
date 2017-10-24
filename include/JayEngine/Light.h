#pragma once
#include "Macro.h"
#include "Component.h"
#include "Vector4.h"

JE_BEGIN

class Transform;
class Light : public Component
{

	friend class GraphicSystem;
	friend class ComponentManager;

public:

	float		m_ambientStrength;
	float		m_specularStrength;
	vec4		m_color;
	Transform*	m_transform;

private:

	Light(Object* _owner = nullptr);
	~Light() {};
	Light(const Light& /*_copy*/) {};
	void operator=(const Light& /*_copy*/) {};

};

JE_END