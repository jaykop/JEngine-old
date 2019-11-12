#pragma once
#include <vec3.hpp>
#include <component.hpp>

jeBegin

class Object; 
class Camera : public Component
{
	friend class GraphicSystem;

public:

	vec3 position_, target_;
	float fovy_, near_, far_;

	void set_camera(const vec3& eye, const vec3& look, const vec3& up, float fov, float aspect, float distance);
	const vec3& get_viewGeometry() const;

	float get_aspect() const;
	float get_distance() const;

	const vec3& get_up() const;
	const vec3& get_right() const;
	const vec3& get_back() const;

	void yaw(float degree);
	void pitch(float degree);
	void roll(float degree);
	void zoom(float zoom);

private:

	vec3 up_, right_, back_, viewGeometry_;
	float distance_, aspect_, width_, height_;

	Camera(Object* pOwner);
	virtual ~Camera() {};
	Camera& operator=(const Camera& rhs);
};

jeEnd
