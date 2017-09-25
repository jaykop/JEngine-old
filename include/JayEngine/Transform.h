#pragma once
#include "Macro.h"
#include "Vector3.h"
#include "Component.h"

NS_JE_BEGIN

class Transform : public Component
{

public:

	Transform(Object* _owner);
	~Transform() {};

	void SetPosition(const vec3& _position);
	void SetScale(const vec3& _scale);
	void SetRotation(float _rotation);

	float GetRotation() const;
	const vec3& GetScale() const;
	const vec3& GetPosition() const;

private:

	Transform(const Transform& /*_copy*/) {};
	void operator= (const Transform& /*_copy*/) {};

	vec3 m_position, m_scale;
	float m_rotation;

};

NS_JE_END