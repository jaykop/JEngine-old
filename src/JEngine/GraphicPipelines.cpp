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
	m_isLight = m_lights.empty() ? false : true;
	
	// Inform that there are lights
	GLM::m_shader[GLM::SHADER_NORMAL]->Use();

	GLM::m_shader[GLM::SHADER_NORMAL]->SetBool(
		GLM::UNIFORM_IS_LIGHT, m_isLight);

	static int s_lightSize;

	// TODO
	// Multiple lights?
	s_lightSize = int(m_lights.size());
	GLM::m_shader[GLM::SHADER_NORMAL]->SetInt(
		GLM::UNIFORM_LIGHT_SIZE, s_lightSize);

	if (m_isLight) {

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
			Render(GLM::m_lightVao, GLM::m_vbo, GLM::m_ebo,
				GLM::m_vertices, GLM::m_indices,
				sizeof(GLM::m_vertices), sizeof(GLM::m_indices), GLM::m_cube);
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
	// Here check if the sprite is
	// either outside the screen or not

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

	if (_sprite->m_hasMaterial && m_isLight)
		LightingEffectPipeline(_sprite->m_material);

	// TODO
	// Just render cube for now...
	Render(GLM::m_vao, GLM::m_vbo, GLM::m_ebo,
		GLM::m_vertices, GLM::m_indices,
		sizeof(GLM::m_vertices), sizeof(GLM::m_indices), GLM::m_cube);
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

		GLM::m_shader[GLM::SHADER_PARTICLE]->Use();

		static vec3		s_velocity, s_colorDiff, s_position;
		static float	s_doubleDt;
		static bool		s_changeColor;
		static vec4		s_color;
		static unsigned s_texture, s_count;
		static Transform* s_pTransform;
		static Emitter::ParticleType s_type;

		s_count = 0;
		s_changeColor = _emitter->m_changeColor;
		s_type = _emitter->m_type;
		s_pTransform = _emitter->m_transform;
		s_texture = _emitter->m_mainTex;
		s_doubleDt = _dt * _dt;
		s_velocity = _dt * _emitter->m_velocity;
		s_colorDiff = s_doubleDt * _emitter->m_colorDiff;
		s_color.Set(_emitter->m_startColor.x, _emitter->m_startColor.y,
			_emitter->m_startColor.z, 1.f);

		// Send transform info to shader
		GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
			GLM::UNIFORM_PARTICLE_TRANSLATE, mat4::Translate(s_pTransform->m_position));

		GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
			GLM::UNIFORM_PARTICLE_SCALE, mat4::Scale(s_pTransform->m_scale));

		GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
			GLM::UNIFORM_PARTICLE_ROTATE, mat4::Rotate(s_pTransform->m_rotation, s_pTransform->m_rotationAxis));

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

		for (auto particle : _emitter->m_particles) {

			if (particle->m_life < 0.f) {
				if (s_type == Emitter::PT_NORMAL)
					particle->Refresh();
				else if(s_type == Emitter::PT_RAIN)
					particle->RainRefresh();
				//TODO
				// Smog Type
			}

			//else {

				particle->m_life -= s_doubleDt;
				particle->m_position += particle->m_direction * s_velocity;
				s_position = particle->m_position;

				static int s_posStride, s_colorStride;
				
				s_posStride = 3 * s_count;
				s_colorStride = 4 * s_count;

				_emitter->m_positionData[s_posStride] = s_position.x;
				_emitter->m_positionData[s_posStride + 1] = s_position.y;
				_emitter->m_positionData[s_posStride + 2] = s_position.z;

				//if (s_changeColor) {
				//	particle->m_color += s_colorDiff;
					s_color.Set(particle->m_color.x, particle->m_color.y, particle->m_color.z,
						particle->m_life);

				//}
				//else
				//	s_color.w = particle->m_life;

				//_emitter->m_colorData[s_colorStride] = s_color.x;
				_emitter->m_colorData[s_colorStride + 1] = s_color.y;
				_emitter->m_colorData[s_colorStride + 2] = s_color.z;
				_emitter->m_colorData[s_colorStride + 3] = s_color.w;


				/*if (s_type != Emitter::PT_EXPLODE)
					GLM::m_shader[GLM::SHADER_PARTICLE]->SetBool(
						GLM::UNIFORM_PARTICLE_HIDE, particle->m_standBy);

				else {
					GLM::m_shader[GLM::SHADER_PARTICLE]->SetBool(
						GLM::UNIFORM_PARTICLE_HIDE, false);

					if (particle->m_life <= 0.f) {
						_emitter->m_count++;
						if (_emitter->m_size == _emitter->m_count)
							_emitter->m_active = false;
					}
				}*/

				s_count++;

				//glBindVertexArray(GLM::m_vao);
				/*Render(GLM::m_vbo, GLM::m_ebo,
					GLM::m_verticesParticle, GLM::m_indicesParticle,
					sizeof(GLM::m_verticesParticle), sizeof(GLM::m_indicesParticle), GLM::m_particle);*/
			//}
		}

		glEnable(GL_BLEND);					// Enable blend 
		glDepthMask(GL_FALSE);				// Ignore depth buffer writing
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		glBindVertexArray(_emitter->m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, _emitter->m_position);
		glBufferData(GL_ARRAY_BUFFER, _emitter->m_size * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, _emitter->m_size * sizeof(GLfloat) * 3, _emitter->m_positionData);

		glBindBuffer(GL_ARRAY_BUFFER, _emitter->m_color);
		glBufferData(GL_ARRAY_BUFFER, _emitter->m_size * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
		glBufferSubData(GL_ARRAY_BUFFER, 0, _emitter->m_size * sizeof(GLubyte) * 4, _emitter->m_colorData);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, GLM::m_particleVbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// text coordinate position
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// 2nd attribute buffer : positions of particles' centers
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, _emitter->m_position);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 3rd attribute buffer : particles' colors
		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, _emitter->m_color);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glVertexAttribDivisor(0, 0);
		glVertexAttribDivisor(1, 0); 
		glVertexAttribDivisor(2, 0); 
		glVertexAttribDivisor(3, 1); 
		glVertexAttribDivisor(4, 1); 

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLM::m_particleEbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLM::m_indicesParticle), GLM::m_indicesParticle, GL_STATIC_DRAW);
		glDrawElementsInstanced(GL_TRIANGLES, GLM::m_particle, GL_UNSIGNED_INT, 0, _emitter->m_size);

		//glDisableVertexAttribArray(0);
		//glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(2);
		//glDisableVertexAttribArray(3);
		//glDisableVertexAttribArray(4);
		
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
}

JE_END
