#include <string>
#include "GraphicSystem.h"
#include "GLManager.h"
#include "Shader.hpp"
#include "GraphicComponents.h"
#include "MathUtils.h"
#include "Transform.h"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// Light box pipeline
//////////////////////////////////////////////////////////////////////////
void GraphicSystem::LightSourcePipeline()
{
	// Update sprites and lights
	m_IsLight = m_lights.empty() ? false : true;
	
	// Inform that there are lights
	GLM::m_shader[GLM::SHADER_NORMAL]->Use();

	GLM::m_shader[GLM::SHADER_NORMAL]->SetBool(
		GLM::UNIFORM_IS_LIGHT, m_IsLight);

	static int s_lightSize;

	// TODO
	// Multiple lights?
	s_lightSize = int(m_lights.size());
	GLM::m_shader[GLM::SHADER_NORMAL]->SetInt(
		GLM::UNIFORM_LIGHT_SIZE, s_lightSize);

	if (m_IsLight) {

		static vec3 s_lightScale(10.f, 10.f, 10.f), s_lightUp(0, 1, 0);
		static float s_lightDeg = 0.f;

		for (auto light : m_lights) {
			GLM::m_shader[GLM::SHADER_LIGHTING]->Use();

			GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
				GLM::UNIFORM_LIGHT_TRANSLATE,
				mat4::Translate(light->m_position));

			GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
				GLM::UNIFORM_LIGHT_SCALE,
				mat4::Scale(s_lightScale));

			GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
				GLM::UNIFORM_LIGHT_ROTATE,
				mat4::Rotate(s_lightDeg, s_lightUp));

			m_viewport = mat4::Camera(
				m_pMainCamera->m_position, m_pMainCamera->m_target, m_pMainCamera->m_up);
			GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
				GLM::UNIFORM_LIGHT_CAMERA,
				m_viewport);

			GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
				GLM::UNIFORM_LIGHT_PROJECTION,
				m_perspective);

			GLM::m_shader[GLM::SHADER_LIGHTING]->SetVector4(
				GLM::UNIFORM_LIGHT_COLOR,
				light->m_color);

			//glBindVertexArray(GLM::m_lightVao);
			Render(GLM::m_vao[GLM::SHAPE_CUBE], GLM::m_elementSize[GLM::SHAPE_CUBE]);

		} // for (auto light : m_lights) {
	} // if (m_isLight) {
}

//////////////////////////////////////////////////////////////////////////
// Sprite(model) pipeline
//////////////////////////////////////////////////////////////////////////
void GraphicSystem::NormalPipeline(const float _dt)
{
	for (auto sprite : m_sprites) {

		// Emitter
		if (sprite->m_isEmitter) 
			ParticlePipeline(static_cast<Emitter*>(sprite), _dt);

		// Normal models
		else 
			SpritePipeline(sprite);
	}
}

void GraphicSystem::SpritePipeline(Sprite *_sprite)
{
	glEnable(GL_BLEND);
	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	static Transform* s_pTransform;
	s_pTransform = _sprite->m_transform;
	
	GLM::m_shader[GLM::SHADER_NORMAL]->Use();

	GLM::m_shader[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_TRANSLATE, mat4::Translate(s_pTransform->m_position));

	GLM::m_shader[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_SCALE, mat4::Scale(s_pTransform->m_scale));

	GLM::m_shader[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_ROTATE, mat4::Rotate(s_pTransform->m_rotation, s_pTransform->m_rotationAxis));

	// Send camera info to shader
	m_viewport = mat4::Camera(
		m_pMainCamera->m_position, m_pMainCamera->m_target, m_pMainCamera->m_up);

	GLM::m_shader[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_CAMERA, m_viewport);

	// Send projection info to shader
	if (_sprite->m_projection == PERSPECTIVE)
		GLM::m_shader[GLM::SHADER_NORMAL]->SetMatrix(
			GLM::UNIFORM_PROJECTION, m_perspective);
	else
		GLM::m_shader[GLM::SHADER_NORMAL]->SetMatrix(
			GLM::UNIFORM_PROJECTION, m_orthogonal);

	// TODO
	// It so, not draw
	//if (!_sprite->m_culled) {

	MappingPipeline(_sprite);
	
	// Send color info to shader
	GLM::m_shader[GLM::SHADER_NORMAL]->SetVector4(
		GLM::UNIFORM_COLOR,
		_sprite->m_color);

	GLM::m_shader[GLM::SHADER_NORMAL]->SetBool(
		GLM::UNIFORM_FLIP,
		_sprite->m_flip);

	GLM::m_shader[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_ANI_SCALE,
		mat4::Scale(m_aniScale));

	GLM::m_shader[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_ANI_TRANSLATE,
		mat4::Translate(m_aniTranslate));

	if (!_sprite->m_effects.empty())
		EffectsPipeline(_sprite);

	if (_sprite->m_hasMaterial && m_IsLight)
		LightingEffectPipeline(_sprite->m_material);

	// TODO
	// Just render cube for now...
	Render(GLM::m_vao[GLM::SHAPE_CUBE], GLM::m_elementSize[GLM::SHAPE_CUBE]);
	glDisable(GL_BLEND);
}

void GraphicSystem::MappingPipeline(Sprite* _sprite)
{
	glBindTexture(GL_TEXTURE_2D, _sprite->GetCurrentTexutre());

	if (_sprite->m_hasAnimation) {
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

	} // if (_sprite->m_hasAnimation) {

	else {
		m_aniScale.Set(1, 1, 0);
		m_aniTranslate.Set(0, 0, 0);
	}
}

void GraphicSystem::LightingEffectPipeline(Material *_material)
{
	// Send material info to shader
	GLM::m_shader[GLM::SHADER_NORMAL]->SetInt(
		GLM::UNIFORM_MATERIAL_SPECULAR,
		_material->m_specular);

	GLM::m_shader[GLM::SHADER_NORMAL]->SetInt(
		GLM::UNIFORM_MATERIAL_DIFFUSE,
		_material->m_diffuse);

	GLM::m_shader[GLM::SHADER_NORMAL]->SetFloat(
		GLM::UNIFORM_MATERIAL_SHININESS,
		_material->m_shininess);

	static int s_lightIndex; 
	static std::string s_index, s_color, s_light, s_input,
		amb("m_ambient"), spec("m_specular"), diff("m_diffuse"), 
		type("m_type"), constant("m_constant"), linear("m_linear"), dir("m_direction"), pos("m_position"),
		cut("m_cutOff"), outcut("m_outerCutOff"), quad("m_quadratic");
	s_lightIndex = 0;

	for (auto _light : m_lights) {

		s_index = std::to_string(s_lightIndex);

		s_color = "v4_lightColor[" + s_index + "]";
		GLM::m_shader[GLM::SHADER_NORMAL]->SetVector4(
			s_color.c_str(), _light->m_color);

		s_light = "light[" + s_index + "].";

		s_input = s_light + spec;
		GLM::m_shader[GLM::SHADER_NORMAL]->SetVector4(
			s_input.c_str(), _light->m_specular);

		s_input = s_light + diff;
		GLM::m_shader[GLM::SHADER_NORMAL]->SetVector4(
			s_input.c_str(), _light->m_diffuse);

		s_input = s_light + type;
		GLM::m_shader[GLM::SHADER_NORMAL]->SetEnum(
			s_input.c_str(), _light->m_type);

		s_input = s_light + dir;
		GLM::m_shader[GLM::SHADER_NORMAL]->SetVector3(
			s_input.c_str(), _light->m_direction);

		s_input = s_light + constant;
		GLM::m_shader[GLM::SHADER_NORMAL]->SetFloat(
			s_input.c_str(), _light->m_constant);

		s_input = s_light + linear;
		GLM::m_shader[GLM::SHADER_NORMAL]->SetFloat(
			s_input.c_str(), _light->m_linear);

		s_input = s_light + quad;
		GLM::m_shader[GLM::SHADER_NORMAL]->SetFloat(
			s_input.c_str(), _light->m_quadratic);

		s_input = s_light + pos;
		GLM::m_shader[GLM::SHADER_NORMAL]->SetVector3(
			s_input.c_str(), _light->m_position);

		s_input = s_light + cut;
		GLM::m_shader[GLM::SHADER_NORMAL]->SetFloat(
			s_input.c_str(), cosf(Math::DegToRad(_light->m_cutOff)));

		s_input = s_light + outcut;
		GLM::m_shader[GLM::SHADER_NORMAL]->SetFloat(
			s_input.c_str(), cosf(Math::DegToRad(_light->m_outerCutOff)));

		s_lightIndex++;
	}
}

void GraphicSystem::EffectsPipeline(Sprite *_sprite)
{
	// Send visual effect info to shader
	for (auto effect : _sprite->m_effects) {

		if (effect.second->m_active) {

			VisualEffect::VEType type = effect.second->m_type;

			switch (type) {

			case VisualEffect::VEType::VE_BLUR: {
				GLM::m_shader[GLM::SHADER_NORMAL]->SetEnum(
					GLM::UNIFORM_EFFECT_TYPE, type);

				Blur* blur = static_cast<Blur*>(effect.second);

				GLM::m_shader[GLM::SHADER_NORMAL]->SetFloat(
					GLM::UNIFORM_EFFECT_BLUR_SIZE, blur->m_size);

				GLM::m_shader[GLM::SHADER_NORMAL]->SetFloat(
					GLM::UNIFORM_EFFECT_BLUR_AMOUNT, blur->m_amount);

				break;
			}
			case VisualEffect::VEType::VE_SOBEL: {
				GLM::m_shader[GLM::SHADER_NORMAL]->SetEnum(
					GLM::UNIFORM_EFFECT_TYPE, type);

				Sobel* sobel = static_cast<Sobel*>(effect.second);

				GLM::m_shader[GLM::SHADER_NORMAL]->SetFloat(
					GLM::UNIFORM_EFFECT_SOBEL, sobel->m_amount);

				break;
			}
			case VisualEffect::VEType::VE_INVERSE: {
				GLM::m_shader[GLM::SHADER_NORMAL]->SetEnum(
					GLM::UNIFORM_EFFECT_TYPE, type);

				// Everything will be operated on shader.

				break;
			}
			} // switch (type) {
		} // if (effect.second->m_active) {
	} // for (auto effect : _sprite->m_effects) {
}

void GraphicSystem::ParticlePipeline(Emitter* _emitter, const float _dt)
{
	// Check emitter's active toggle
	if (_emitter->m_active) {

		//glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);					// Enable blend 
		glDepthMask(GL_FALSE);				// Ignore depth buffer writing
		//glDisable(GL_DEPTH_TEST);
		//glBlendFunc(GL_ONE, GL_ONE);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		//glBlendFunc(GL_ONE, GL_SRC_ALPHA);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		static vec3						s_velocity, s_colorDiff;
		static float					s_doubleDt;
		static bool						s_changeColor;
		static vec4						s_color;
		static unsigned					s_texture;
		static Transform*				s_pTransform;
		static Emitter::ParticleType	s_type;

		s_changeColor = _emitter->m_changeColor;
		s_type = _emitter->m_type;
		s_pTransform = _emitter->m_transform;
		s_texture = _emitter->m_mainTex;
		s_doubleDt = _dt * _dt;
		s_velocity = _dt * _emitter->m_velocity;
		s_colorDiff = s_doubleDt * _emitter->m_colorDiff;

		GLM::m_shader[GLM::SHADER_PARTICLE]->Use();

		for (auto particle : _emitter->m_particles) {

			if (particle->m_life < 0.f)
				particle->Refresh();

			else {

				particle->m_life -= s_doubleDt;
				particle->m_position += particle->m_direction * s_velocity;
				particle->m_rotation += particle->m_rotateDiff;

				if (s_changeColor)
					particle->m_color += s_colorDiff;

				s_color.Set(particle->m_color.x, particle->m_color.y, particle->m_color.z,
					particle->m_life);

				// Send transform info to shader
				GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
					GLM::UNIFORM_PARTICLE_TRANSLATE, mat4::Translate(particle->m_position));

				GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
					GLM::UNIFORM_PARTICLE_SCALE, mat4::Scale(s_pTransform->m_scale));

				GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
					GLM::UNIFORM_PARTICLE_ROTATE, mat4::Rotate(particle->m_rotation, s_pTransform->m_rotationAxis));

				// Send camera info to shader
				m_viewport = mat4::Camera(
					m_pMainCamera->m_position, m_pMainCamera->m_target, m_pMainCamera->m_up);

				GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
					GLM::UNIFORM_PARTICLE_CAMERA, m_viewport);

				// Send projection info to shader
				if (_emitter->m_projection == PERSPECTIVE)
					GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
						GLM::UNIFORM_PARTICLE_PROJECTION, m_perspective);
				else
					GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
						GLM::UNIFORM_PARTICLE_PROJECTION, m_orthogonal);

				glBindTexture(GL_TEXTURE_2D, s_texture);

				// Send color info to shader
				GLM::m_shader[GLM::SHADER_PARTICLE]->SetVector4(
					GLM::UNIFORM_PARTICLE_COLOR, s_color);
			}

			Render(GLM::m_vao[GLM::SHAPE_POINT], GLM::m_elementSize[GLM::SHAPE_POINT]);
			//Render(GLM::m_vao[GLM::SHAPE_PARTICLE], GLM::m_elementSize[GLM::SHAPE_PARTICLE]);
			//Render(GLM::m_vao[GLM::SHAPE_CUBE], GLM::m_elementSize[GLM::SHAPE_CUBE]);
		}

		//glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);	// Enable depth buffer writing
		glDisable(GL_BLEND);	// Disable blend
	}
}

JE_END
