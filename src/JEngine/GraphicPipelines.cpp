#include <string>
#include "GraphicSystem.h"
#include "GLManager.h"
#include "Shader.hpp"
#include "GraphicComponents.h"
#include "MathUtils.h"
#include "Transform.h"

JE_BEGIN

void GraphicSystem::UpdatePipelines(const float _dt)
{
	// Update main camera attributes
	m_viewport = mat4::LookAt(
		m_pMainCamera->m_position, m_pMainCamera->m_target, m_pMainCamera->m_up);

	// Update sprites and lights
	m_isLight = m_lights.empty() ? false : true;

	// Inform that there are lights
	GLM::m_shader[GLM::SHADER_NORMAL]->Use();

	GLM::m_shader[GLM::SHADER_NORMAL]->SetBool(
		GLM::UNIFORM_IS_LIGHT, m_isLight);

	LightSourcePipeline();

	GLM::m_shader[GLM::SHADER_NORMAL]->Use();

	// Send camera info to shader
	GLM::m_shader[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_CAMERA, m_viewport);

	GLM::m_shader[GLM::SHADER_PARTICLE]->Use();

	// Send camera info to shader
	GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
		GLM::UNIFORM_PARTICLE_CAMERA, m_viewport);

	for (auto sprite : m_sprites) {

		// Emitter
		if (sprite->m_isEmitter)
			ParticlePipeline(static_cast<Emitter*>(sprite), _dt);

		// Normal models
		else
			SpritePipeline(sprite);
	}
}

void GraphicSystem::RenderToFramebuffer()
{	
	// Render to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, GLM::m_fbo);
	GLM::m_shader[GLM::SHADER_NORMAL]->Use();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glViewport(0, 0, GLint(m_width), GLint(m_height));

}

void GraphicSystem::RenderToScreen()
{
	// Bind default framebuffer and render to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w);

	glBindVertexArray(GLM::m_vao[GLM::SHAPE_PLANE]);
	glDisable(GL_DEPTH_TEST);
	GLM::m_shader[GLM::SHADER_SCREEN]->Use();
	GLM::m_shader[GLM::SHADER_SCREEN]->SetVector4(GLM::UNIFORM_SCREEN_COLOR, m_screenColor);

	// Impose screen effect 
	GLM::m_shader[GLM::SHADER_SCREEN]->SetEnum(GLM::UNIFORM_SCREEN_EFFECT, m_screenEffect);
	
	if (m_screenEffect == EFFECT_BLUR) {
		GLM::m_shader[GLM::SHADER_SCREEN]->SetFloat(GLM::UNIFORM_SCREEN_BLUR_SIZE, m_blurSize);
		GLM::m_shader[GLM::SHADER_SCREEN]->SetFloat(GLM::UNIFORM_SCREEN_BLUR_AMOUNT, m_blurAmount);
	}
	else if (m_screenEffect == EFFECT_SOBEL)
		GLM::m_shader[GLM::SHADER_SCREEN]->SetFloat(GLM::UNIFORM_SCREEN_SOBEL, m_sobelAmount);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GLM::m_renderTarget);

	glDrawElements(GL_TRIANGLES, GLM::m_elementSize[GLM::SHAPE_PLANE], GL_UNSIGNED_INT, 0);
	glEnable(GL_DEPTH_TEST);

}

//////////////////////////////////////////////////////////////////////////
// Light box pipeline
//////////////////////////////////////////////////////////////////////////
void GraphicSystem::LightSourcePipeline()
{
	static int s_lightSize;

	// TODO
	// Multiple lights?
	s_lightSize = int(m_lights.size());
	GLM::m_shader[GLM::SHADER_NORMAL]->SetInt(
		GLM::UNIFORM_LIGHT_SIZE, s_lightSize);

	if (m_isLight) {

		glEnable(GL_DEPTH_TEST);

		static vec3 s_lightScale(10.f, 10.f, 10.f), s_lightUp(0, 1, 0);
		static float s_lightDeg = 0.f;

		GLM::m_shader[GLM::SHADER_LIGHTING]->Use();

		GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
			GLM::UNIFORM_LIGHT_SCALE,
			mat4::Scale(s_lightScale));

		GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
			GLM::UNIFORM_LIGHT_ROTATE,
			mat4::Rotate(s_lightDeg, s_lightUp));

		GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
			GLM::UNIFORM_LIGHT_CAMERA,
			m_viewport);

		for (auto light : m_lights) {

			GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
				GLM::UNIFORM_LIGHT_TRANSLATE,
				mat4::Translate(light->m_position));

			if (light->m_projection == PROJECTION_PERSPECTIVE)
				GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
					GLM::UNIFORM_LIGHT_PROJECTION, m_perspective);
			else
				GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
					GLM::UNIFORM_LIGHT_PROJECTION, m_orthogonal);

			GLM::m_shader[GLM::SHADER_LIGHTING]->SetVector4(
				GLM::UNIFORM_LIGHT_COLOR,
				light->m_color);

			// TODO
			// Change vertices
			Render(GLM::m_vao[GLM::SHAPE_CUBE], GLM::m_elementSize[GLM::SHAPE_CUBE]);

		} // for (auto light : m_lights) {
	} // if (m_isLight) {

	glDisable(GL_DEPTH_TEST);
}

//////////////////////////////////////////////////////////////////////////
// Sprite(model) pipeline
//////////////////////////////////////////////////////////////////////////
void GraphicSystem::SpritePipeline(Sprite *_sprite)
{
	static Transform* s_pTransform;
	s_pTransform = _sprite->m_transform;

	GLM::m_shader[GLM::SHADER_NORMAL]->Use();

	GLM::m_shader[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_TRANSLATE, mat4::Translate(s_pTransform->m_position));

	GLM::m_shader[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_SCALE, mat4::Scale(s_pTransform->m_scale));

	GLM::m_shader[GLM::SHADER_NORMAL]->SetMatrix(
		GLM::UNIFORM_ROTATE, mat4::Rotate(s_pTransform->m_rotation, s_pTransform->m_rotationAxis));

	GLM::m_shader[GLM::SHADER_NORMAL]->SetVector3(
		GLM::UNIFORM_CAMERA_POSITION, m_pMainCamera->m_position);

	GLM::m_shader[GLM::SHADER_NORMAL]->SetBool(
		GLM::UNIFORM_BILBOARD, _sprite->m_bilboard);

	// Send projection info to shader
	if (_sprite->m_projection == PROJECTION_PERSPECTIVE)
		GLM::m_shader[GLM::SHADER_NORMAL]->SetMatrix(
			GLM::UNIFORM_PROJECTION, m_perspective);
	else
		GLM::m_shader[GLM::SHADER_NORMAL]->SetMatrix(
			GLM::UNIFORM_PROJECTION, m_orthogonal);

	// TODO
	// It so, not draw
	//if (!_sprite->m_culled) {

	MappingPipeline(_sprite);

	if (_sprite->m_hasMaterial && m_isLight)
		LightingEffectPipeline(_sprite->m_material);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	Render(*(_sprite->m_vao), _sprite->m_elementSize);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void GraphicSystem::MappingPipeline(Sprite* _sprite)
{
	glBindTexture(GL_TEXTURE_2D, _sprite->GetCurrentTexutre());

	if (_sprite->m_hasAnimation) {
		
		static Animation* animation;
		animation = _sprite->m_animation;

		if (animation->m_activeAnimation) {

			static float realSpeed;
			realSpeed = animation->m_realSpeed;

			if (realSpeed <= animation->m_timer.GetTime()) {

				static float nextFrame;
				if (_sprite->m_flip)
					nextFrame = animation->m_currentFrame - animation->m_realFrame;
				else
					nextFrame = animation->m_currentFrame + animation->m_realFrame;

				if (nextFrame >= 1.f)
					animation->m_currentFrame = 0.f;
				else
					animation->m_currentFrame = nextFrame;

				animation->m_timer.Start();
			} // if (realSpeed <= animation->m_timer.GetTime()) {
		} // if (animation->m_activeAnimation) {

		m_aniScale.Set(animation->m_realFrame, 1.f, 0.f);
		m_aniTranslate.Set(animation->m_currentFrame, 0.f, 0.f);

	} // if (_sprite->m_hasAnimation) {

	else {
		m_aniScale.Set(1, 1, 0);
		m_aniTranslate.Set(0, 0, 0);
	}

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

void GraphicSystem::ParticlePipeline(Emitter* _emitter, const float _dt)
{
	// Check emitter's active toggle
	if (_emitter->m_active) {

		static GLenum s_mode;

		// Particle render attributes setting
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);				// Ignore depth buffer writing
		//glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		
		// Points
		if (_emitter->m_renderType == Emitter::PARTICLERENDER_POINT) {	
			s_mode = GL_POINTS;
			glPointSize(_emitter->m_pointSize);
			glEnable(GL_POINT_SMOOTH);
		}
		// Plane
		else if (_emitter->m_renderType == Emitter::PARTICLERENDER_PLANE) {	
			s_mode = GL_TRIANGLES;
			glDisable(GL_POINT_SMOOTH);
		}

		// Cross form
		else {
			s_mode = GL_TRIANGLES;
			glDisable(GL_POINT_SMOOTH);
		}

		static GLuint					s_vao, s_elementSize;
		static vec3						s_velocity, s_colorDiff;
		static float					s_doubleDt;
		static bool						s_changeColor, s_rotation;
		static vec4						s_color;
		static unsigned					s_texture;
		static Transform*				s_pTransform;
		static Emitter::ParticleType	s_type;

		s_vao = *(_emitter->m_vao);
		s_elementSize = _emitter->m_elementSize;
		s_rotation = _emitter->m_rotationSpeed;
		s_changeColor = _emitter->m_changeColor;
		s_type = _emitter->m_type;
		s_pTransform = _emitter->m_transform;
		s_texture = _emitter->m_mainTex;
		s_doubleDt = _dt * _dt;
		s_velocity = _dt * _emitter->m_velocity;
		s_colorDiff = s_doubleDt * _emitter->m_colorDiff;

		GLM::m_shader[GLM::SHADER_PARTICLE]->Use();

		GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
			GLM::UNIFORM_PARTICLE_SCALE, mat4::Scale(s_pTransform->m_scale));

		GLM::m_shader[GLM::SHADER_PARTICLE]->SetBool(
			GLM::UNIFORM_PARTICLE_BILBOARD, _emitter->m_bilboard);

		// Send projection info to shader
		if (_emitter->m_projection == PROJECTION_PERSPECTIVE)
			GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
				GLM::UNIFORM_PARTICLE_PROJECTION, m_perspective);
		else
			GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
				GLM::UNIFORM_PARTICLE_PROJECTION, m_orthogonal);

		glBindTexture(GL_TEXTURE_2D, s_texture);

		for (auto particle : _emitter->m_particles) {

			if (particle->m_life < 0.f)
				particle->Refresh();

			else {

				particle->m_life -= s_doubleDt;
				particle->m_position += particle->m_direction * s_velocity;

				if (s_rotation)
					particle->m_rotation += particle->m_rotationSpeed * _dt;

				if (s_changeColor)
					particle->m_color += s_colorDiff;

				s_color.Set(particle->m_color.x, particle->m_color.y, particle->m_color.z,
					particle->m_life);

				// Send transform info to shader
				GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
					GLM::UNIFORM_PARTICLE_TRANSLATE, mat4::Translate(particle->m_position));

				GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
					GLM::UNIFORM_PARTICLE_ROTATE, mat4::Rotate(particle->m_rotation, s_pTransform->m_rotationAxis));

				// Send color info to shader
				GLM::m_shader[GLM::SHADER_PARTICLE]->SetVector4(
					GLM::UNIFORM_PARTICLE_COLOR, s_color);

				GLM::m_shader[GLM::SHADER_PARTICLE]->SetBool(
					GLM::UNIFORM_PARTICLE_HIDE, particle->m_hidden);

				Render(s_vao, s_elementSize, s_mode);
			}
		}

		//glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);	// Enable depth buffer writing
		glDisable(GL_BLEND);	// Disable blend
	}
}

// TODO
void GraphicSystem::render1()
{
	glBindFramebuffer(GL_FRAMEBUFFER, GLM::m_deferredFBO);
	GLM::m_shader[GLM::SHADER_DEFERRED]->Use();

	glViewport(0, 0, GLint(m_width), GLint(m_height));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w);
	glEnable(GL_DEPTH_TEST);
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &(GLM::m_passIndex1));

	m_viewport = mat4::LookAt(
		m_pMainCamera->m_position, m_pMainCamera->m_target, m_pMainCamera->m_up);

	GLM::m_shader[GLM::SHADER_DEFERRED]->SetInt("PositionTex", 0);
	GLM::m_shader[GLM::SHADER_DEFERRED]->SetInt("NormalTex", 2);
	GLM::m_shader[GLM::SHADER_DEFERRED]->SetInt("ColorTex", 1);

	GLM::m_shader[GLM::SHADER_DEFERRED]->SetVector3("Material.Kd", vec3(0.5f, 0.5f, 0.5f));
	GLM::m_shader[GLM::SHADER_DEFERRED]->SetVector4("Light.Position", vec4(10.0f, 10.0f, 10.0f, 1.0f));
	GLM::m_shader[GLM::SHADER_DEFERRED]->SetVector3("Light.Intensity", vec3(1.0f, 1.0f, 1.0f));

	static Transform* s_pTransform;
	s_pTransform = (*m_sprites.begin())->m_transform;

	glBindTexture(GL_TEXTURE_2D, (*m_sprites.begin())->GetCurrentTexutre());

	GLM::m_shader[GLM::SHADER_DEFERRED]->SetMatrix(GLM::UNIFORM_DEFERRED_TRANSLATE, mat4::Translate(s_pTransform->m_position));
	GLM::m_shader[GLM::SHADER_DEFERRED]->SetMatrix(GLM::UNIFORM_DEFERRED_SCALE, mat4::Scale(s_pTransform->m_scale));
	GLM::m_shader[GLM::SHADER_DEFERRED]->SetMatrix(GLM::UNIFORM_DEFERRED_ROTATE, mat4::Rotate(s_pTransform->m_rotation, s_pTransform->m_rotationAxis));
	GLM::m_shader[GLM::SHADER_DEFERRED]->SetMatrix(GLM::UNIFORM_DEFERRED_CAMERA, m_viewport);

	// Send projection info to shader
	if ((*m_sprites.begin())->m_projection == PROJECTION_PERSPECTIVE)
		GLM::m_shader[GLM::SHADER_DEFERRED]->SetMatrix(
			GLM::UNIFORM_DEFERRED_PROJECTION, m_perspective);
	else
		GLM::m_shader[GLM::SHADER_DEFERRED]->SetMatrix(
			GLM::UNIFORM_DEFERRED_PROJECTION, m_orthogonal);

	Render(*(*m_sprites.begin())->m_vao, (*m_sprites.begin())->m_elementSize);

	glFinish();

}

void GraphicSystem::render2()
{
	// Revert to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &(GLM::m_passIndex2));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	static mat4 identity;
	identity.SetIdentity();

	GLM::m_shader[GLM::SHADER_DEFERRED]->SetMatrix(GLM::UNIFORM_DEFERRED_TRANSLATE, identity);
	GLM::m_shader[GLM::SHADER_DEFERRED]->SetMatrix(GLM::UNIFORM_DEFERRED_SCALE, identity);
	GLM::m_shader[GLM::SHADER_DEFERRED]->SetMatrix(GLM::UNIFORM_DEFERRED_ROTATE, identity);
	GLM::m_shader[GLM::SHADER_DEFERRED]->SetMatrix(GLM::UNIFORM_DEFERRED_CAMERA, identity);
	GLM::m_shader[GLM::SHADER_DEFERRED]->SetMatrix(GLM::UNIFORM_DEFERRED_PROJECTION, identity);

	// Render the quad
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GLM::m_positionBuffer);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GLM::m_colorBuffer);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GLM::m_normalBuffer);
	Render(GLM::m_vao[GLM::SHAPE_PLANE], GLM::m_elementSize[GLM::SHAPE_PLANE]);
}

JE_END
