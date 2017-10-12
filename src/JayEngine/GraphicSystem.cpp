#include <functional>
#include <algorithm>
#include "Sprite.h"
#include "Texture.h"
#include "Camera.h"
#include "GLManager.h"
#include "GraphicSystem.h"
#include "Transform.h"
#include "Application.h"

NS_JE_BEGIN

GraphicSystem::GraphicSystem()
	:System(), m_pTransformStorage(nullptr), m_pMainCamera(nullptr),
	m_backgroundColor(vec4::ZERO), m_colorStorage(vec4::ZERO),
	m_fovy(45.f), m_zNear(.1f), m_zFar(100.f), m_mode(MODE_2D),
	m_orthoFirst(true)
{
	m_aspect = float(Application::GetData().m_width) / float(Application::GetData().m_height);
	m_right = Application::GetData().m_width * .5f;
	m_left = -m_right;
	m_top = Application::GetData().m_height * .5f;
	m_bottom = -m_top;
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
	std::sort(m_sprites.begin(), m_sprites.end(), SortZorder);

	// Update sprites
	for (auto sprite : m_sprites)
		Pipeline(sprite);
}

void GraphicSystem::Close()
{
	ClearCameraMap();
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
	glUniformMatrix4fv(GLManager::GetUniform(GLManager::UNIFORM_CAMERA), 1, GL_FALSE,
		&mat4::Camera(m_pMainCamera->m_position, m_pMainCamera->m_target, m_pMainCamera->m_up).m_member[0][0]);

	// Send projection info to shader
	if (_sprite->m_projection == Sprite::PERSPECTIVE)
		glUniformMatrix4fv(GLManager::GetUniform(GLManager::UNIFORM_PROJECTION), 1, GL_FALSE,
			&mat4::Perspective(m_fovy, m_aspect, m_zNear, m_zFar).m_member[0][0]);

	else
		glUniformMatrix4fv(GLManager::GetUniform(GLManager::UNIFORM_PROJECTION), 1, GL_FALSE,
			&mat4::Orthogonal(m_left, m_right, m_bottom, m_top, m_zNear, m_zFar).m_member[0][0]);

}

void GraphicSystem::MappingPipeline(Sprite * _sprite)
{
	// Send color info to shader
	m_colorStorage = _sprite->m_color;

	glUniform4f(GLManager::GetUniform(GLManager::UNIFORM_COLOR),
		m_colorStorage.x, m_colorStorage.y, m_colorStorage.z, m_colorStorage.w);

	glBindTexture(GL_TEXTURE_2D, _sprite->GetCurrentTexutre()->GetId());
}

void GraphicSystem::AnimationPipeline(Sprite * _sprite)
{
	if (_sprite->m_activeAnimation) {

		float realSpeed = _sprite->m_realSpeed;

		if (realSpeed <= _sprite->m_timer.GetTime()) {

			float nextFrame = _sprite->m_curretFrame + _sprite->m_realFrame;
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

bool GraphicSystem::SortZorder(const Sprite * _leftSpt, const Sprite * _rightSpt)
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
	AddCamera("Main");
	m_pMainCamera = GetCamera("Main");

	m_pMainCamera->m_up.SetUnitY();
	m_pMainCamera->m_position.Set(0.f, 0.f, 80.f);
}

Camera* GraphicSystem::GetMainCamera()
{
	return m_pMainCamera;
}

void GraphicSystem::AddCamera(const char* _camaraName)
{
	auto found = m_cameraMap.find(_camaraName);

	// Found nothing exsting camera
	// Insert new camera to the list
	if (found == m_cameraMap.end())
		m_cameraMap.insert(
			CameraMap::value_type(
				_camaraName, new Camera));

	else
		JE_DEBUG_PRINT("Cannot add identical component again!\n");
}

void GraphicSystem::RemoveCamera(const char* _camaraName)
{	
	// Find if there is the once
	auto found = m_cameraMap.find(_camaraName);

	// If there is return it
	if (found != m_cameraMap.end())
		delete found->second;

	else 
		JE_DEBUG_PRINT("No such name of camera\n");
}

void GraphicSystem::ClearCameraMap()
{
	for (auto camera : m_cameraMap) {
		delete camera.second;
		camera.second = nullptr;
	}
}

Camera* GraphicSystem::GetCamera(const char* _camaraName)
{
	// Find if there is the once
	auto found = m_cameraMap.find(_camaraName);

	// If there is return it
	if (found != m_cameraMap.end())
		return found->second;

	// Unless...
	else {
		JE_DEBUG_PRINT("No such name of camera\n");
		return nullptr;
	}
}

NS_JE_END

