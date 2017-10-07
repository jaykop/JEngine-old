#pragma once
#include "Macro.h"
#include "Vector3.h"

NS_JE_BEGIN

class Camera {

public:

	friend class GraphicSystem;

	//void SetRotation2D(float _rotation);
	//void SetTarget(const vec3& _target);
	//void SetUpVector(const vec3& _upVector);
	//void SetPosition(const vec3& _position);
	//
	//float		GetRotation2D();
	//const vec3& GetTarget() const;
	//const vec3& GetPosition() const;
	//const vec3& GetUpVector() const;

	vec3 m_position, m_up, m_target;
	float m_rotation;

private:

	Camera();
	~Camera() {};

	Camera(const Camera& /*_copy*/) {};
	void operator=(const Camera& /*_copy*/) {};

};

NS_JE_END