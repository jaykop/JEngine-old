#include <algorithm>
#include "Sprite.h"
#include "Texture.h"
#include "Camera.h"
#include "GLManager.h"
#include "GraphicSystem.h"
#include "Transform.h"
#include "Application.h"
#include "InputHandler.h"

NS_JE_BEGIN

GraphicSystem::GraphicSystem()
	:System(), m_pTransformStorage(nullptr), m_pMainCamera(nullptr),
	m_backgroundColor(vec4::ZERO), m_colorStorage(vec4::ZERO),
	m_fovy(45.f), m_zNear(.1f), m_zFar(1000.f), m_mode(MODE_2D),
	m_orthoFirst(false), m_width(Application::GetData().m_width),
	m_height(Application::GetData().m_height)
{
	m_aspect = float(m_width) / float(m_height);
	m_right = m_width * .5f;
	m_left = -m_right;
	m_top = m_height * .5f;
	m_bottom = -m_top;

	m_perspective = mat4::Perspective(m_fovy, m_aspect, m_zNear, m_zFar);
	m_orthogonal = mat4::Orthogonal(m_left, m_right, m_bottom, m_top, m_zNear, m_zFar);
	
}

void GraphicSystem::Load()
{
}

void GraphicSystem::Init()
{
	InitCamera();
}

void GraphicSystem::Update(float /*dt*/)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w);

	// Sort sprites by sprite's z position
	std::sort(m_sprites.begin(), m_sprites.end(), compareOrder(m_orthoFirst));

	// Update sprites
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

void GraphicSystem::Pipeline(Sprite* _sprite)
{
	
	TransformPipeline(_sprite);
	MappingPipeline(_sprite);
	AnimationPipeline(_sprite);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GraphicSystem::TransformPipeline(Sprite * _sprite)
{
	// Send transform info to shader
	m_pTransformStorage = _sprite->m_transform;

	// Send transform info to shader
	glUniformMatrix4fv(GLManager::GetUniform(GLManager::UNIFORM_TRANSLATE), 1, GL_FALSE,
		&mat4::Translate(m_pTransformStorage->m_position).m_member[0][0]);
	glUniformMatrix4fv(GLManager::GetUniform(GLManager::UNIFORM_SCALE), 1, GL_FALSE,
		&mat4::Scale(m_pTransformStorage->m_scale).m_member[0][0]);

	if (m_mode == MODE_2D)
		glUniformMatrix4fv(GLManager::GetUniform(GLManager::UNIFORM_ROTATE), 1, GL_FALSE,
			&mat4::Rotate(m_pTransformStorage->m_rotation, vec3::UNIT_Z).m_member[0][0]);

	// TODO
	// else 3d rotation

	// Send camera info to shader
	m_viewport = mat4::Camera(m_pMainCamera->m_position, m_pMainCamera->m_target, m_pMainCamera->m_up);
	glUniformMatrix4fv(GLManager::GetUniform(GLManager::UNIFORM_CAMERA), 1, GL_FALSE,
		&m_viewport.m_member[0][0]);

	// Send projection info to shader
	if (_sprite->m_projection == Sprite::PERSPECTIVE)
		glUniformMatrix4fv(GLManager::GetUniform(GLManager::UNIFORM_PROJECTION), 1, GL_FALSE,
			&m_perspective.m_member[0][0]);

	else 
		glUniformMatrix4fv(GLManager::GetUniform(GLManager::UNIFORM_PROJECTION), 1, GL_FALSE,
			&m_orthogonal.m_member[0][0]);
}

void GraphicSystem::MappingPipeline(Sprite * _sprite)
{
	// Send color info to shader
	m_colorStorage = _sprite->m_color;

	glUniform4f(GLManager::GetUniform(GLManager::UNIFORM_COLOR),
		m_colorStorage.x, m_colorStorage.y, m_colorStorage.z, m_colorStorage.w);

	glBindTexture(GL_TEXTURE_2D, _sprite->GetCurrentTexutre()->GetId());

	glUniform1i(GLManager::GetUniform(GLManager::UNIFORM_FLIP), true);
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

	glUniformMatrix4fv(GLManager::GetUniform(GLManager::UNIFORM_ANI_SCALE), 1, GL_FALSE,
		&(mat4::Scale(vec3(_sprite->m_realFrame, 1.f))).m_member[0][0]);
	glUniformMatrix4fv(GLManager::GetUniform(GLManager::UNIFORM_ANI_TRANSLATE), 1, GL_FALSE,
		&(mat4::Translate(vec3(_sprite->m_curretFrame))).m_member[0][0]);
}

void GraphicSystem::SetBackgroundColor(const vec4& _color)
{
	m_backgroundColor = _color;
}

const vec4& GraphicSystem::GetBackgroundColor() const
{
	return m_backgroundColor;
}

void GraphicSystem::InitCamera()
{
	
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
			return left->m_position.z > right->m_position.z;
	}

	else
		return left->m_position.z > right->m_position.z;
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

NS_JE_END

