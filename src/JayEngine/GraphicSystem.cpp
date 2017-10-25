#include <algorithm>
#include "Sprite.h"
#include "Shader.hpp"
#include "Camera.h"
#include "GLManager.h"
#include "GraphicSystem.h"
#include "Transform.h"
#include "Application.h"
#include "InputHandler.h"
#include "Light.h"

#ifdef JE_SUPPORT_3D
#include "Model.h"
#endif

JE_BEGIN

GraphicSystem::GraphicSystem()
	:System(), m_pTransformStorage(nullptr), m_pMainCamera(nullptr),
	m_fovy(45.f), m_zNear(.1f), m_zFar(1000.f), m_isLight(false),
	m_backgroundColor(vec4::ZERO), m_orthoFirst(false),
	m_width(Application::GetData().m_width),
	m_height(Application::GetData().m_height)
{
	m_aspect = float(m_width) / float(m_height);
	m_right = m_width * .5f;
	m_left = -m_right;
	m_top = m_height * .5f;
	m_bottom = -m_top;

	m_perspective = mat4::Perspective(m_fovy, m_aspect, m_zNear, m_zFar);
	m_orthogonal = mat4::Orthogonal(m_left, m_right, m_bottom, m_top, m_zNear, m_zFar);

#ifndef JE_SUPPORT_3D
	m_target2D.Set(0.f, 0.f, m_zNear);
#endif
}

void GraphicSystem::Load()
{
}

void GraphicSystem::Init()
{
}

void GraphicSystem::Update(float /*dt*/)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w);

	// Sort sprites by sprite's z position
	std::sort(m_sprites.begin(), m_sprites.end(), compareOrder(m_orthoFirst));

	// Update sprites and lights
	m_isLight = m_lights.empty() ? false : true ;

	GLManager::m_shader[GLManager::SHADER_NORMAL].SetBool(
		GLManager::m_uniform[GLManager::UNIFORM_IS_LIGHT], m_isLight);

	if (m_isLight) 
		for (auto light : m_lights)
			Pipeline(light);

	for (auto sprite : m_sprites)
		Pipeline(sprite);

	GLMousePosition();
}

void GraphicSystem::Close()
{
	m_sprites.clear();
	m_cameras.clear();
}

void GraphicSystem::Unload()
{
	
}

void GraphicSystem::AddSprite(Sprite* _sprite)
{
	m_sprites.push_back(_sprite);
}

void GraphicSystem::RemoveSprite(Sprite* _sprite)
{
	for (Sprites::iterator it = m_sprites.begin();
		it != m_sprites.end(); ++it) {
		if ((*it)->m_ownerId == _sprite->m_ownerId) {
			m_sprites.erase(it);
			break;
		}
	}
}

void GraphicSystem::Pipeline(Light* _light)
{
	GLManager::m_shader[GLManager::SHADER_NORMAL].SetFloat(
		GLManager::m_uniform[GLManager::UNIFORM_LIGHT_AMBIENT],
		_light->m_ambientStrength);

	GLManager::m_shader[GLManager::SHADER_NORMAL].SetFloat(
		GLManager::m_uniform[GLManager::UNIFORM_LIGHT_SPECULAR],
		_light->m_specularStrength);

	GLManager::m_shader[GLManager::SHADER_NORMAL].SetVector4(
		GLManager::m_uniform[GLManager::UNIFORM_LIGHT_COLOR],
		_light->m_color);

	GLManager::m_shader[GLManager::SHADER_NORMAL].SetVector3(
		GLManager::m_uniform[GLManager::UNIFORM_LIGHT_POSITION],
		_light->m_transform->m_position);

	GLManager::m_shader[GLManager::SHADER_NORMAL].SetVector3(
		GLManager::m_uniform[GLManager::UNIFORM_CAMERA_POSITION],
		m_pMainCamera->m_position);
}

void GraphicSystem::Pipeline(Sprite* _sprite)
{
	// Use normal shader
	GLManager::m_shader[GLManager::SHADER_NORMAL].Use();

	// Here check if the sprite is
	// either outside the screen or not
	TransformPipeline(_sprite);

	// It so, not draw
	if (!_sprite->m_culled) {
	
		MappingPipeline(_sprite);
		AnimationPipeline(_sprite);
#ifdef JE_SUPPORT_3D
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
#else
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#endif
	}
}

void GraphicSystem::TransformPipeline(Sprite * _sprite)
{
	// Send transform info to shader
	m_pTransformStorage = _sprite->m_transform;

	// Send transform info to shader
	GLManager::m_shader[GLManager::SHADER_NORMAL].SetMatrix(
		GLManager::m_uniform[GLManager::UNIFORM_TRANSLATE], 
		mat4::Translate(m_pTransformStorage->m_position));

	GLManager::m_shader[GLManager::SHADER_NORMAL].SetMatrix(
		GLManager::m_uniform[GLManager::UNIFORM_SCALE],
		mat4::Scale(m_pTransformStorage->m_scale));

	GLManager::m_shader[GLManager::SHADER_NORMAL].SetMatrix(
		GLManager::m_uniform[GLManager::UNIFORM_ROTATE],
		mat4::Rotate(m_pTransformStorage->m_rotation, 
			m_pTransformStorage->m_rotation3D));

	// Send camera info to shader
#ifdef JE_SUPPORT_3D
	m_viewport = mat4::Camera(
		m_pMainCamera->m_position, m_pMainCamera->m_target, m_pMainCamera->m_up);
	GLManager::m_shader[GLManager::SHADER_NORMAL].SetMatrix(
		GLManager::m_uniform[GLManager::UNIFORM_CAMERA],
		m_viewport);
#else
	m_target2D.x = m_pMainCamera->m_position.x;
	m_target2D.y = m_pMainCamera->m_position.y;

	m_viewport = mat4::Camera(
		m_pMainCamera->m_position, m_target2D, m_pMainCamera->m_up);
	GLManager::m_shader[GLManager::SHADER_NORMAL].SetMatrix(
		GLManager::m_uniform[GLManager::UNIFORM_CAMERA],
		m_viewport);
#endif

	// Send projection info to shader
	if (_sprite->m_projection == Sprite::PERSPECTIVE) {
		GLManager::m_shader[GLManager::SHADER_NORMAL].SetMatrix(
			GLManager::m_uniform[GLManager::UNIFORM_PROJECTION],
			m_perspective);

		//m_inside = ;
	}

	else {
		GLManager::m_shader[GLManager::SHADER_NORMAL].SetMatrix(
			GLManager::m_uniform[GLManager::UNIFORM_PROJECTION],
			m_orthogonal);

		//m_inside = ;
	}

	if (m_inside) 
		_sprite->m_culled = true;

	else
		_sprite->m_culled = false;
}

void GraphicSystem::MappingPipeline(Sprite * _sprite)
{
	glBindTexture(GL_TEXTURE_2D, _sprite->GetCurrentTexutre());
	
	// Send color info to shader
	GLManager::m_shader[GLManager::SHADER_NORMAL].SetVector4(
		GLManager::m_uniform[GLManager::UNIFORM_COLOR],
		_sprite->m_color);

	GLManager::m_shader[GLManager::SHADER_NORMAL].SetBool(
		GLManager::m_uniform[GLManager::UNIFORM_FLIP],
		_sprite->m_flip);
}

void GraphicSystem::AnimationPipeline(Sprite * _sprite)
{
	if (_sprite->m_activeAnimation) {

		float realSpeed = _sprite->m_realSpeed;

		if (realSpeed <= _sprite->m_timer.GetTime()) {

			float nextFrame;
			if (_sprite->m_flip)
				nextFrame = _sprite->m_curretFrame - _sprite->m_realFrame;
			else
				nextFrame = _sprite->m_curretFrame + _sprite->m_realFrame;
			
			if (nextFrame >= 1.f)
				_sprite->m_curretFrame = 0.f;
			else
				_sprite->m_curretFrame = nextFrame;

			_sprite->m_timer.Start();
		}
	}

	GLManager::m_shader[GLManager::SHADER_NORMAL].SetMatrix(
		GLManager::m_uniform[GLManager::UNIFORM_ANI_SCALE],
		mat4::Scale(vec3(_sprite->m_realFrame, 1.f)));

	GLManager::m_shader[GLManager::SHADER_NORMAL].SetMatrix(
		GLManager::m_uniform[GLManager::UNIFORM_ANI_TRANSLATE],
		mat4::Translate(vec3(_sprite->m_curretFrame)));
}

void GraphicSystem::SetBackgroundColor(float _r, float _g, float _b, float _a)
{
	m_backgroundColor.Set(_r, _g, _b, _a);
}

void GraphicSystem::SetBackgroundColor(const vec4& _color)
{
	m_backgroundColor = _color;
}

const vec4& GraphicSystem::GetBackgroundColor() const
{
	return m_backgroundColor;
}

void GraphicSystem::SetMainCamera(Camera* _camera)
{
	m_pMainCamera = _camera;
}

Camera* GraphicSystem::GetMainCamera()
{
	return m_pMainCamera;
}

void GraphicSystem::AddCamera(Camera* _camera)
{
	m_cameras.push_back(_camera);
}

void GraphicSystem::RemoveCamera(Camera* _camera)
{	
	for (Cameras::iterator it = m_cameras.begin();
		it != m_cameras.end(); ++it) {
		if ((*it)->m_ownerId == _camera->m_ownerId) {
			m_cameras.erase(it);
			break;
		}
	}
}

void GraphicSystem::AddLight(Light * _light)
{
	m_lights.push_back(_light);
}

void GraphicSystem::RemoveLight(Light * _light)
{
	for (Lights::iterator it = m_lights.begin();
		it != m_lights.end(); ++it) {
		if ((*it)->m_ownerId == _light->m_ownerId) {
			m_lights.erase(it);
			break;
		}
	}
}

bool GraphicSystem::compareOrder::operator()(Sprite * _leftSpt, Sprite * _rightSpt)
{
	Transform* left = _leftSpt->m_transform;
	Transform* right = _rightSpt->m_transform;

	if (m_orthoFirst) {

		if (_leftSpt->m_projection == Sprite::PERSPECTIVE
			&& _rightSpt->m_projection == Sprite::ORTHOGONAL)
			return false;

		else if (_leftSpt->m_projection == Sprite::ORTHOGONAL
			&& _rightSpt->m_projection == Sprite::PERSPECTIVE)
			return true;

		else
			return left->m_position.z < right->m_position.z;
	}

	else
		return left->m_position.z < right->m_position.z;
}

void GraphicSystem::GLMousePosition() {

	// Do unprojection by viewport and proejction matrix
	vec4 in;
	in.x = (2.f * (InputHandler::m_rawPosition.x / m_width)) - 1.f;
	in.y = 1.f - (2.f* (InputHandler::m_rawPosition.y / m_height));
	in.w = in.z = 1.f;
	
	mat4 ortho = m_orthogonal * m_viewport;
	ortho.Inverse();
	vec4 orthoPos = ortho * in;
	float new_w1 = 1.f / orthoPos.w;
	InputHandler::m_orthoPosition.Set(orthoPos.x, orthoPos.y, orthoPos.z);
	InputHandler::m_orthoPosition *= new_w1;

	mat4 perspective = m_perspective * m_viewport;
	perspective.Inverse();
	vec4 perspPos = perspective * in;
	float new_w2 = 1.f / perspPos.w;
	InputHandler::m_perspPosition.Set(perspPos.x, perspPos.y, perspPos.z);
	InputHandler::m_perspPosition *= new_w2;

}

#ifdef JE_SUPPORT_3D

void GraphicSystem::AddModel(Model * _model)
{
	m_models.push_back(_model);

}

void GraphicSystem::RemoveModel(Model * _model)
{
	for (Models::iterator it = m_models.begin();
		it != m_models.end(); ++it) {
		if ((*it)->m_ownerId == _model->m_ownerId) {
			m_models.erase(it);
			break;
		}
	}
}

#endif

JE_END

