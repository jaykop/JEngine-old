#include "Camera.h"
#include "SystemManager.h"
#include "GraphicSystem.h"
#include "MathUtils.h"
#include "GLManager.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Camera);

using namespace Math;

Camera::Camera(Object* pOwner)
	: Component(pOwner),
	position_(vec3::ZERO), near_(.1f), far_(1000.f),
	up_(vec3::UNIT_Y), target_(vec3::ZERO), right_(vec3::ZERO), back_(vec3::ZERO),
	viewGeometry_(vec3::ZERO), distance_(1.f), fovy_(0.f), aspect_(0.f),
	width_(0.f), height_(0.f)
{
	SetCamera(position_, vec3::UNIT_Z, up_,	45.f, GLM::width_ / GLM::height_, 1.f);
}

void Camera::SetCamera(const vec3& eye, const vec3& look, const vec3& up, 
	float fov, float aspect, float distance)
{
	position_ = eye;
	right_ = GetNormalize(CrossProduct(look, up));
	up_ = GetNormalize(CrossProduct(right_, look));
	back_ = GetNormalize(-look);

	fovy_ = fov;
	aspect_ = aspect;
	distance_ = distance;
	width_ = 2 * tanf(.5f*fovy_);
	height_ = width_ / aspect_;

	viewGeometry_.Set(width_, height_, distance_);
}

const vec3& Camera::GetViewGeometry() const
{
	return viewGeometry_;
}

float Camera::GetAspect() const
{
	return aspect_;
}

float Camera::GetDistance() const
{
	return distance_;
}

const vec3& Camera::GetUp() const
{
	return up_;
}

const vec3& Camera::GetRight() const
{
	return right_;
}

const vec3& Camera::GetBack() const
{
	return back_;
}

void Camera::Yaw(float degree)
{
	mat4 rotate = Rotate(DegToRad(degree), up_);

	vec4 right(right_.x, right_.y, right_.z, 1.f);
	right = rotate * right;
	right_.Set(right.x, right.y, right.z);

	vec4 back(back_.x, back_.y, back_.z, 1.f);
	back = rotate * back;
	back_.Set(back.x, back.y, back.z);
}

void Camera::Pitch(float degree)
{
	mat4 rotate = Rotate(DegToRad(degree), right_);

	vec4 up(up_.x, up_.y, up_.z, 1.f);
	up = rotate * up;
	up_.Set(up.x, up.y, up.z);

	vec4 back(back_.x, back_.y, back_.z, 1.f);
	back = rotate * back;
	back_.Set(back.x, back.y, back.z);
}

void Camera::Roll(float degree)
{
	mat4 rotate = Rotate(DegToRad(degree), back_);

	vec4 right(right_.x, right_.y, right_.z, 1.f);
	right = rotate * right;
	right_.Set(right.x, right.y, right.z);

	vec4 up(up_.x, up_.y, up_.z, 1.f);
	up = rotate * up;
	up_.Set(up.x, up.y, up.z);
}

void Camera::Zoom(float zoom)
{
	width_ *= zoom;
	height_ += zoom;
}

void Camera::Register()
{
	SYSTEM::pGraphic_->AddCamera(this);
}

void Camera::operator=(const Camera & copy)
{
	position_.Set(copy.position_);
	up_.Set(copy.up_);
	target_.Set(copy.target_);
}

void Camera::Load(CR_RJValue data)
{

	if (data.HasMember("Up")) {
		CR_RJValue loadedUp = data["Up"];
		up_.Set(loadedUp[0].GetFloat(), loadedUp[1].GetFloat(), loadedUp[2].GetFloat());
	}
	
	if (data.HasMember("Position")) {
		CR_RJValue loadedPosition = data["Position"];
		position_.Set(loadedPosition[0].GetFloat(), loadedPosition[1].GetFloat(), loadedPosition[2].GetFloat());
	}

	if (data.HasMember("Target")) {
		CR_RJValue loadedTarget = data["Target"];
		target_.Set(loadedTarget[0].GetFloat(), loadedTarget[1].GetFloat(), loadedTarget[2].GetFloat());
	}

	if (data.HasMember("Far"))
		far_ = data["Far"].GetFloat();

	if (data.HasMember("Near"))
		near_ = data["Near"].GetFloat();

	if (data.HasMember("Look")
		&& data.HasMember("Fovy")
		&& data.HasMember("Distance")) {

		CR_RJValue loadedLook = data["Look"];
		vec3 look(loadedLook[0].GetFloat(), loadedLook[1].GetFloat(), loadedLook[2].GetFloat());

		SetCamera(position_, look, up_, 
			data["Fovy"].GetFloat(), GLM::width_ / GLM::height_, data["Distance"].GetFloat());
	}

}

void Camera::EditorUpdate(const float /*dt*/)
{
	// TODO
}

jeEnd
