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

    vec3 position_, target_;
	float fovy_, near_, far_;
	
	void SetCamera(const vec3& eye, const vec3& look, const vec3& up, float fov, float aspect,float distance);
	const vec3& GetViewGeometry() const;

	float GetAspect() const;
	float GetDistance() const;

	const vec3& GetUp() const;
	const vec3& GetRight() const;
	const vec3& GetBack() const;

	void Yaw(float degree);
	void Pitch(float degree);
	void Roll(float degree);
	void Zoom(float zoom);

    void Register() override;

private:

	vec3 up_, right_, back_, viewGeometry_;
	float distance_, aspect_, width_, height_;

    Camera(Object* pOwner);
	virtual ~Camera() {};
    void operator=(const Camera& copy);

    Camera() = delete;
    Camera(const Camera& /*copy*/) = delete;

    void Load(CR_RJValue data) override;

    void EditorUpdate(float dt) override;
};

jeDeclareComponentBuilder(Camera);
jeEnd
