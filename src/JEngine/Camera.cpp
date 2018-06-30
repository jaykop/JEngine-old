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
	position(vec3::ZERO), near(.1f), far(1000.f),
	m_up(vec3::UNIT_Y), target(vec3::ZERO), m_right(vec3::ZERO), m_back(vec3::ZERO),
	m_viewGeometry(vec3::ZERO), m_distance(1.f), fovy(0.f), m_aspect(0.f),
	width_(0.f), height_(0.f)
{
	SetCamera(position, vec3::UNIT_Z, m_up,	45.f, GLM::width_ / GLM::height_, 1.f);
}

void Camera::SetCamera(const vec3& _eye, const vec3& _look, const vec3& _up, 
	float _fov, float _aspect, float _distance)
{
	position = _eye;
	m_right = GetNormalize(CrossProduct(_look, _up));
	m_up = GetNormalize(CrossProduct(m_right, _look));
	m_back = GetNormalize(-_look);

	fovy = _fov;
	m_aspect = _aspect;
	m_distance = _distance;
	width_ = 2 * tanf(.5f*fovy);
	height_ = width_ / m_aspect;

	m_viewGeometry.Set(width_, height_, m_distance);
}

const vec3& Camera::GetViewGeometry() const
{
	return m_viewGeometry;
}

float Camera::GetAspect() const
{
	return m_aspect;
}

float Camera::GetDistance() const
{
	return m_distance;
}

const vec3& Camera::GetUp() const
{
	return m_up;
}

const vec3& Camera::GetRight() const
{
	return m_right;
}

const vec3& Camera::GetBack() const
{
	return m_back;
}

void Camera::Yaw(float _degree)
{
	mat4 rotate = Rotate(DegToRad(_degree), m_up);

	vec4 right(m_right.x, m_right.y, m_right.z, 1.f);
	right = rotate * right;
	m_right.Set(right.x, right.y, right.z);

	vec4 back(m_back.x, m_back.y, m_back.z, 1.f);
	back = rotate * back;
	m_back.Set(back.x, back.y, back.z);
}

void Camera::Pitch(float _degree)
{
	mat4 rotate = Rotate(DegToRad(_degree), m_right);

	vec4 up(m_up.x, m_up.y, m_up.z, 1.f);
	up = rotate * up;
	m_up.Set(up.x, up.y, up.z);

	vec4 back(m_back.x, m_back.y, m_back.z, 1.f);
	back = rotate * back;
	m_back.Set(back.x, back.y, back.z);
}

void Camera::Roll(float _degree)
{
	mat4 rotate = Rotate(DegToRad(_degree), m_back);

	vec4 right(m_right.x, m_right.y, m_right.z, 1.f);
	right = rotate * right;
	m_right.Set(right.x, right.y, right.z);

	vec4 up(m_up.x, m_up.y, m_up.z, 1.f);
	up = rotate * up;
	m_up.Set(up.x, up.y, up.z);
}

void Camera::Zoom(float _zoom)
{
	width_ *= _zoom;
	height_ += _zoom;
}

void Camera::Register()
{
	SYSTEM::pGraphic_->AddCamera(this);
}

void Camera::operator=(const Camera & copy)
{
	position.Set(copy.position);
	m_up.Set(copy.m_up);
	target.Set(copy.target);
}

void Camera::Load(CR_RJValue _data)
{

	if (_data.HasMember("Up")) {
		CR_RJValue loadedUp = _data["Up"];
		m_up.Set(loadedUp[0].GetFloat(), loadedUp[1].GetFloat(), loadedUp[2].GetFloat());
	}
	
	if (_data.HasMember("Position")) {
		CR_RJValue loadedPosition = _data["Position"];
		position.Set(loadedPosition[0].GetFloat(), loadedPosition[1].GetFloat(), loadedPosition[2].GetFloat());
	}

	if (_data.HasMember("Target")) {
		CR_RJValue loadedTarget = _data["Target"];
		target.Set(loadedTarget[0].GetFloat(), loadedTarget[1].GetFloat(), loadedTarget[2].GetFloat());
	}

	if (_data.HasMember("Far"))
		far = _data["Far"].GetFloat();

	if (_data.HasMember("Near"))
		near = _data["Near"].GetFloat();

	if (_data.HasMember("Look")
		&& _data.HasMember("Fovy")
		&& _data.HasMember("Distance")) {

		CR_RJValue loadedLook = _data["Look"];
		vec3 look(loadedLook[0].GetFloat(), loadedLook[1].GetFloat(), loadedLook[2].GetFloat());

		SetCamera(position, look, m_up, 
			_data["Fovy"].GetFloat(), GLM::width_ / GLM::height_, _data["Distance"].GetFloat());
	}

}

void Camera::EditorUpdate(const float /*dt*/)
{
	// TODO
}

jeEnd
