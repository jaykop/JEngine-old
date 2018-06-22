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
	float zoom, near, far;
	
	void SetCamera(const vec3& _eye, const vec3& _look, const vec3& _up, float _fov, float _aspect,float _distance);
	const vec3& GetViewGeometry() const;

	float GetFovy() const;
	float GetAspect() const;
	float GetDistance() const;
	const vec3& GetBack() const;

	void Yaw(float _degree);
	void Pitch(float _degree);
	void Roll(float _degree);
	void Zoom(float _zoom);

    void Register() override;

private:

	vec3 m_up, m_right, m_back, m_viewGeometry;
	float m_distance, m_fovy, m_aspect, m_width, m_height;

    Camera(Object* _pOwner);
    ~Camera() {};
    void operator=(const Camera& _copy);

    Camera() = delete;
    Camera(const Camera& /*_copy*/) = delete;

    void Load(CR_RJValue _data) override;

    void EditorUpdate(const float _dt) override;
};

jeDeclareComponentBuilder(Camera);
jeEnd
