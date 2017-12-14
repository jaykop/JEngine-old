#include <algorithm>
#include "Shader.hpp"
#include "GLManager.h"
#include "GraphicSystem.h"
#include "Transform.h"
#include "Application.h"
#include "InputHandler.h"
#include "GraphicComponents.h"
#include "MathUtils.h"

JE_BEGIN

GraphicSystem::GraphicSystem()
	:System(), m_pTransformStorage(nullptr), m_pMainCamera(nullptr),
	m_fovy(45.f), m_zNear(.1f), m_zFar(1000.f), m_isLight(false),
	m_backgroundColor(vec4::ZERO), m_orthoFirst(false),
	m_width(Application::GetData().m_width), m_hasAnimation(false),
	m_height(Application::GetData().m_height),
	m_aniScale(vec3::ZERO), m_aniTranslate(vec3::ZERO)
{
	m_aspect = float(m_width) / float(m_height);
	m_right = m_width * .5f;
	m_left = -m_right;
	m_top = m_height * .5f;
	m_bottom = -m_top;

	m_perspective = mat4::Perspective(m_fovy, m_aspect, m_zNear, m_zFar);
	m_orthogonal = mat4::Orthogonal(m_left, m_right, m_bottom, m_top, m_zNear, m_zFar);

}

void GraphicSystem::Load(CR_RJDoc _data)
{
	if (_data.HasMember("Background")) {
		CR_RJValue color = _data["Background"];
		m_backgroundColor.Set(
			color[0].GetFloat(),
			color[1].GetFloat(),
			color[2].GetFloat(),
			color[3].GetFloat()
		);
	}
}

void GraphicSystem::Init()
{
	// If there is no preset camera by user,
	// set the first camera as a main camera.
	if (!m_pMainCamera)
		m_pMainCamera = m_cameras[0];
}

void GraphicSystem::Update(float _dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w);

	// Sort sprites by sprite's z position
	std::sort(m_sprites.begin(), m_sprites.end(), compareOrder(m_orthoFirst));

	// Update sprites and lights
	m_isLight = m_lights.empty() ? false : true ;

	if (m_isLight) 
		LightPipeline();
	
	SpritePipeline(_dt);
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

void GraphicSystem::LightPipeline()
{
	// Inform that there are lights
	GLM::m_shaders[GLM::SHADER_NORMAL]->SetBool(
		GLM::UNIFORM_IS_LIGHT, m_isLight);

	for (auto light : m_lights) {
		GLM::m_shaders[GLM::SHADER_LIGHTING]->Use();

		GLM::m_shaders[GLM::SHADER_LIGHTING]->SetMatrix(
			GLM::UNIFORM_LIGHT_TRANSLATE,
			mat4::Translate(light->m_position));

		GLM::m_shaders[GLM::SHADER_LIGHTING]->SetMatrix(
			GLM::UNIFORM_LIGHT_SCALE,
			mat4::Scale(vec3(10.f, 10.f, 10.f)));

		GLM::m_shaders[GLM::SHADER_LIGHTING]->SetMatrix(
			GLM::UNIFORM_LIGHT_ROTATE,
			mat4::Rotate(0.f, vec3(0.f, 1.f, 0.f)));

		m_viewport = mat4::Camera(
			m_pMainCamera->m_position, m_pMainCamera->m_target, m_pMainCamera->m_up);
		GLM::m_shaders[GLM::SHADER_LIGHTING]->SetMatrix(
			GLM::UNIFORM_LIGHT_CAMERA,
			m_viewport);

		GLM::m_shaders[GLM::SHADER_LIGHTING]->SetMatrix(
			GLM::UNIFORM_LIGHT_PROJECTION,
			m_perspective);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
}

void GraphicSystem::SpritePipeline(float _dt)
{
	for (auto sprite : m_sprites) {
		// Use normal shader
		GLM::m_shaders[GLM::SHADER_NORMAL]->Use();

		// Emitter
		if (sprite->m_isEmitter) {
			ParticlePipeline(
				static_cast<Emitter*>(sprite), _dt);
		}

		else {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Here check if the sprite is
			// either outside the screen or not
			TransformPipeline(sprite);

			// TODO
			// It so, not draw
			//if (!_sprite->m_culled) {

			MappingPipeline(sprite, sprite->m_material);
			AnimationPipeline(sprite);

			if (m_isLight)
				LightPipeline();

			if (!sprite->m_effects.empty())
				EffectsPipeline(sprite);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
	}
}

void GraphicSystem::ParticlePipeline(Emitter* _emitter, float _dt)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	switch (_emitter->m_type) {

	case Emitter::ParticleType::PT_NORMAL:
	default:
		NormalUpdate(_emitter, _dt);
		break;
	}
}

void GraphicSystem::NormalUpdate(Emitter* _emitter, float _dt)
{
	static vec3		direction, velocity;
	static float	offset;
	static vec4		color;

	offset				= _dt * _emitter->m_life;
	direction			= _emitter->m_direction;
	velocity			= _emitter->m_velocity;
	m_pTransformStorage = _emitter->m_transform;

	for (auto particle : _emitter->m_particles) {

		particle->m_position += _dt * particle->m_direction;
		particle->m_position.z = 0.f;
		//particle->m_color -= ;
		particle->m_life -= _dt * offset;

		if (particle->m_life < 0.f)
			particle->Refresh();

		// Send transform info to shader
		GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
			GLM::UNIFORM_TRANSLATE,
			mat4::Translate(particle->m_position));

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
			GLM::UNIFORM_SCALE,
			mat4::Scale(m_pTransformStorage->m_scale));

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
			GLM::UNIFORM_ROTATE,
			mat4::Rotate(m_pTransformStorage->m_rotation,
				m_pTransformStorage->m_rotationAxis));

		// Send camera info to shader
		m_viewport = mat4::Camera(
			m_pMainCamera->m_position, m_pMainCamera->m_target, m_pMainCamera->m_up);
		GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
			GLM::UNIFORM_CAMERA,
			m_viewport);

		// Send projection info to shader
		if (_emitter->m_projection == Sprite::PERSPECTIVE) {
			GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
				GLM::UNIFORM_PROJECTION,
				m_perspective);
		}

		else {
			GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
				GLM::UNIFORM_PROJECTION,
				m_orthogonal);
		}

		glBindTexture(GL_TEXTURE_2D, _emitter->GetCurrentTexutre());

		// Send color info to shader
		color.Set(particle->m_color.x, particle->m_color.y, particle->m_color.z,
			particle->m_life);
		GLM::m_shaders[GLM::SHADER_NORMAL]->SetVector4(
			GLM::UNIFORM_COLOR,
			color);

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
			GLM::UNIFORM_ANI_SCALE,
			mat4::Scale(vec3::ONE));

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
			GLM::UNIFORM_ANI_TRANSLATE,
			mat4::Translate(vec3::ZERO));

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
}

void GraphicSystem::LightingPipeline()
{
	for (auto _light : m_lights) {
		GLM::m_shaders[GLM::SHADER_NORMAL]->SetVector4(
			GLM::UNIFORM_LIGHT_AMBIENT,
			_light->m_ambient);

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetVector4(
			GLM::UNIFORM_LIGHT_SPECULAR,
			_light->m_specular);

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetVector4(
			GLM::UNIFORM_LIGHT_DIFFUSE,
			_light->m_diffuse);

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetVector4(
			GLM::UNIFORM_LIGHT_COLOR,
			_light->m_color);

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetEnum(
			GLM::UNIFORM_LIGHT_TYPE,
			_light->m_type);

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetVector3(
			GLM::UNIFORM_LIGHT_DIRECTION,
			_light->m_direction);

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetFloat(
			GLM::UNIFORM_LIGHT_CONST,
			_light->m_constant);

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetFloat(
			GLM::UNIFORM_LIGHT_LINEAR,
			_light->m_linear);

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetFloat(
			GLM::UNIFORM_LIGHT_QUAD,
			_light->m_quadratic);

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetVector3(
			GLM::UNIFORM_LIGHT_POSITION,
			_light->m_position);

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetFloat(
			GLM::UNIFORM_LIGHT_CUTOFF,
			cosf(Math::DegToRad(_light->m_cutOff)));

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetFloat(
			GLM::UNIFORM_LIGHT_OUTERCUTOFF,
			cosf(Math::DegToRad(_light->m_outerCutOff)));
	}
}

void GraphicSystem::TransformPipeline(Sprite * _sprite)
{
	// Send transform info to shader
	m_pTransformStorage = _sprite->m_transform;

	// Send transform info to shader
	GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_TRANSLATE, 
		mat4::Translate(m_pTransformStorage->m_position));

	GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_SCALE,
		mat4::Scale(m_pTransformStorage->m_scale));

	GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_ROTATE,
		mat4::Rotate(m_pTransformStorage->m_rotation, 
			m_pTransformStorage->m_rotationAxis));

	// Send camera info to shader
	m_viewport = mat4::Camera(
		m_pMainCamera->m_position, m_pMainCamera->m_target, m_pMainCamera->m_up);
	GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_CAMERA,
		m_viewport);

	// Send projection info to shader
	if (_sprite->m_projection == Sprite::PERSPECTIVE) {
		GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
			GLM::UNIFORM_PROJECTION,
			m_perspective);

		//m_inside = ;
	}

	else {
		GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
			GLM::UNIFORM_PROJECTION,
			m_orthogonal);

		//m_inside = ;
	}

	// TODO
	//if (m_inside) 
	//	_sprite->m_culled = true;

	//else
	//	_sprite->m_culled = false;
}

void GraphicSystem::MappingPipeline(Sprite* _sprite, Material* _material)
{
	glBindTexture(GL_TEXTURE_2D, _sprite->GetCurrentTexutre());
	
	// Send color info to shader
	GLM::m_shaders[GLM::SHADER_NORMAL]->SetVector4(
		GLM::UNIFORM_COLOR,
		_sprite->m_color);

	GLM::m_shaders[GLM::SHADER_NORMAL]->SetBool(
		GLM::UNIFORM_FLIP,
		_sprite->m_flip);

	// Send material info to shader
	if (_sprite->m_hasMaterial) {

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetInt(
			GLM::UNIFORM_MATERIAL_SPECULAR,
			_material->m_specular);

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetInt(
			GLM::UNIFORM_MATERIAL_DIFFUSE,
			_material->m_diffuse);

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetFloat(
			GLM::UNIFORM_MATERIAL_SHININESS,
			_material->m_shininess);
	}
}

void GraphicSystem::EffectsPipeline(Sprite *_sprite)
{
	// Send visual effect info to shader
	float effectsSize = float(_sprite->m_effects.size());
	for (auto effect : _sprite->m_effects) {

		GLM::m_shaders[GLM::SHADER_NORMAL]->SetFloat(
			GLM::UNIFORM_EFFECT_BLUR_SIZE, effectsSize);

		if (effect.second->m_active) {

			VisualEffect::VEType type = effect.second->m_type;

			switch (type) {

			case VisualEffect::VEType::VE_BLUR: {
				GLM::m_shaders[GLM::SHADER_NORMAL]->SetEnum(
					GLM::UNIFORM_EFFECT_TYPE, type);

				Blur* blur = static_cast<Blur*>(effect.second);

				GLM::m_shaders[GLM::SHADER_NORMAL]->SetFloat(
					GLM::UNIFORM_EFFECT_BLUR_SIZE, blur->m_size);

				GLM::m_shaders[GLM::SHADER_NORMAL]->SetFloat(
					GLM::UNIFORM_EFFECT_BLUR_AMOUNT, blur->m_amount);

				break;
			}
			case VisualEffect::VEType::VE_SOBEL: {
				GLM::m_shaders[GLM::SHADER_NORMAL]->SetEnum(
					GLM::UNIFORM_EFFECT_TYPE, type);

				Sobel* sobel = static_cast<Sobel*>(effect.second);

				GLM::m_shaders[GLM::SHADER_NORMAL]->SetFloat(
					GLM::UNIFORM_EFFECT_SOBEL, sobel->m_amount);

				break;
			}
			case VisualEffect::VEType::VE_INVERSE: {
				GLM::m_shaders[GLM::SHADER_NORMAL]->SetEnum(
					GLM::UNIFORM_EFFECT_TYPE, type);

				// Everything will be operated on shader.

				break;
			}
			} // switch (type) {
		} // if (effect.second->m_active) {
	} // for (auto effect : _sprite->m_effects) {
}

void GraphicSystem::AnimationPipeline(Sprite* _sprite)
{
	m_hasAnimation = _sprite->m_hasAnimation;
	if (m_hasAnimation) {
		Animation* animation = _sprite->m_animation;
		if (animation->m_activeAnimation) {

			float realSpeed = animation->m_realSpeed;

			if (realSpeed <= animation->m_timer.GetTime()) {

				float nextFrame;
				if (_sprite->m_flip)
					nextFrame = animation->m_curretFrame - animation->m_realFrame;
				else
					nextFrame = animation->m_curretFrame + animation->m_realFrame;

				if (nextFrame >= 1.f)
					animation->m_curretFrame = 0.f;
				else
					animation->m_curretFrame = nextFrame;

				animation->m_timer.Start();
			} // if (realSpeed <= animation->m_timer.GetTime()) {
		} // if (animation->m_activeAnimation) {

		m_aniScale.Set(animation->m_realFrame, 1.f, 0.f);
		m_aniTranslate.Set(animation->m_curretFrame, 0.f, 0.f);

	} // if (m_hasAnimation) {

	else {
		m_aniScale.Set(1,1,0);
		m_aniTranslate.Set(0,0,0);
	}

	GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_ANI_SCALE,
		mat4::Scale(m_aniScale));

	GLM::m_shaders[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_ANI_TRANSLATE,
		mat4::Translate(m_aniTranslate));
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

JE_END

