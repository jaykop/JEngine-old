#pragma once
#include "Vector3.h"
#include "Component.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

jeBegin

class Camera : public Component
{
    jeBaseFriends(Camera);
    friend class GraphicSystem;

public:

    vec3 position, target;
	float fovy, near, far;
	
	void SetCamera(const vec3& _eye, const vec3& _look, const vec3& _up, float _fov, float _aspect,float _distance);
	const vec3& GetViewGeometry() const;

	float GetAspect() const;
	float GetDistance() const;

	const vec3& GetUp() const;
	const vec3& GetRight() const;
	const vec3& GetBack() const;

	void Yaw(float _degree);
	void Pitch(float _degree);
	void Roll(float _degree);
	void Zoom(float _zoom);

    void Register() override;

private:

	vec3 m_up, m_right, m_back, m_viewGeometry;
	float m_distance, m_aspect, width_, height_;

    Camera(Object* pOwner);
	virtual ~Camera() {};
    void operator=(const Camera& copy);

    Camera() = delete;
    Camera(const Camera& /*copy*/) = delete;

    void Load(CR_RJValue _data) override;

    void EditorUpdate(float dt) override;
};

jeDeclareComponentBuilder(Camera);
jeEnd
