#include <string>
#include "GraphicSystem.h"
#include "GLManager.h"
#include "Shader.h"
#include "GraphicComponents.h"
#include "Transform.h"
#include "MathUtils.h"
#include "Mesh.h"

jeBegin

using namespace Math;

void GraphicSystem::UpdatePipelines(const float _dt)
{
	// Update the perpsective matrix by camera's zoom
	m_perspective = Perspective(m_pMainCamera->zoom, aspect, zNear, zFar);

	// Update the projection size by window screen size
	static vec3 s_windowSize, s_resolutionStandard(1.f / 800.f, 1.f / 600.f, 1.f);
	s_windowSize.Set(float(m_width), float(m_height), 1.f);

	m_resolutionScaler = s_windowSize * s_resolutionStandard;

	// Update sprites and lights
	m_isLight = m_lights.empty() ? false : true;

	LightSourcePipeline();

	Shader::Use(GLM::SHADER_MODEL);

	// Inform that there are lights
	Shader::m_pCurrentShader->SetBool("boolean_light", m_isLight);

	for (auto sprite : m_sprites) {

		// Emitter
		if ((sprite->m_hiddenStatus & Sprite::IS_EMITTER) == Sprite::IS_EMITTER)
			ParticlePipeline(static_cast<Emitter*>(sprite), _dt);

		else if ((sprite->m_hiddenStatus & Sprite::IS_TEXT) == Sprite::IS_TEXT)
			TextPipeline(static_cast<Text*>(sprite));

		// Normal models
		else
			SpritePipeline(sprite);
	}
}

void GraphicSystem::RenderToFramebuffer()
{
	// Render to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, GLM::m_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glViewport(0, 0, GLint(m_width), GLint(m_height));

	// Backface culling
	if (GLM::m_drawMode == GL_TRIANGLES) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
	}

	// Default point size
	if (GLM::m_drawMode == GL_POINTS)
		glPointSize(5);
}

void GraphicSystem::RenderToScreen()
{
	// Bind default framebuffer and render to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);

	glDisable(GL_CULL_FACE);	//Disable face culling
	glDisable(GL_DEPTH_TEST);	//Disable depth test

	// Render to plane 2d
	glBindVertexArray(GLM::m_vao[GLM::SHAPE_PLANE]);
	Shader::m_pCurrentShader->Use(GLM::SHADER_SCREEN);
	Shader::m_pCurrentShader->SetVector4("v4_screenColor", screenColor);

	// Impose screen effect 
	Shader::m_pCurrentShader->SetEnum("enum_effectType", screenEffect);

	if (screenEffect == EFFECT_BLUR) {
		Shader::m_pCurrentShader->SetFloat("float_blurSize", blurSize);
		Shader::m_pCurrentShader->SetFloat("float_blurAmount", blurAmount);
	}
	else if (screenEffect == EFFECT_SOBEL)
		Shader::m_pCurrentShader->SetFloat("float_sobelAmount", sobelAmount);

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
	if (m_isLight) {

		if (GLM::m_mode == GLM::DRAW_FILL)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);

		glEnable(GL_DEPTH_TEST);

		Shader::m_pCurrentShader->Use(GLM::SHADER_LIGHTING);

		Shader::m_pCurrentShader->SetMatrix("m4_scale", Scale(lightScale));

		for (auto light : m_lights) {

			Shader::m_pCurrentShader->SetMatrix(
				"m4_translate", Translate(light->position));

			Shader::m_pCurrentShader->SetMatrix(
				"m4_rotateZ", RotateZ(atan2(light->direction.y, light->direction.x)));

			Shader::m_pCurrentShader->SetMatrix(
				"m4_rotateY", RotateY(-atan2(light->direction.z, light->direction.x)));

			if (light->projection == PROJECTION_PERSPECTIVE) {
				Shader::m_pCurrentShader->SetMatrix("m4_projection", m_perspective);

				m_viewport = LookAt(m_pMainCamera->position, m_pMainCamera->target, m_pMainCamera->up);
			}

			else {
				Shader::m_pCurrentShader->SetMatrix("m4_projection", m_orthogonal);

				SetIdentity(m_viewport);
				m_viewport = Scale(m_resolutionScaler);
			}
			
			Shader::m_pCurrentShader->SetMatrix("m4_viewport", m_viewport);
			Shader::m_pCurrentShader->SetVector4("v4_color", light->color);

			glBlendFunc(light->sfactor, light->dfactor);
			Render(GLM::m_vao[GLM::SHAPE_CONE], GLM::m_elementSize[GLM::SHAPE_CONE]);

		} // for (auto light : m_lights) {
	} // if (m_isLight) {

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

//////////////////////////////////////////////////////////////////////////
// Sprite(model) pipeline
//////////////////////////////////////////////////////////////////////////
void GraphicSystem::SpritePipeline(Sprite *_sprite)
{
	static Transform* s_pTransform;
	s_pTransform = _sprite->m_pTransform;

	Shader::m_pCurrentShader->Use(GLM::SHADER_MODEL);

	Shader::m_pCurrentShader->SetMatrix("m4_translate", Translate(s_pTransform->position));
	Shader::m_pCurrentShader->SetMatrix("m4_scale", Scale(s_pTransform->scale));
	Shader::m_pCurrentShader->SetMatrix("m4_rotate", Rotate(Math::DegToRad(s_pTransform->rotation), s_pTransform->rotationAxis));

	Shader::m_pCurrentShader->SetVector3("v3_cameraPosition", m_pMainCamera->position);
	Shader::m_pCurrentShader->SetBool("boolean_bilboard", (_sprite->status & Sprite::IS_BILBOARD) == Sprite::IS_BILBOARD);

	// Send projection info to shader
	if (_sprite->projection == PROJECTION_PERSPECTIVE) {

		Shader::m_pCurrentShader->SetMatrix("m4_projection", m_perspective);
		m_viewport = LookAt(m_pMainCamera->position, m_pMainCamera->target, m_pMainCamera->up);
	}

	else {
		Shader::m_pCurrentShader->SetMatrix("m4_projection", m_orthogonal);

		SetIdentity(m_viewport);
		m_viewport = Scale(m_resolutionScaler);
	}

	// Send camera info to shader
	Shader::m_pCurrentShader->SetMatrix("m4_viewport", m_viewport);

	// TODO
	// It so, not draw
	//if (!_sprite->m_culled) {

	MappingPipeline(_sprite);

	bool hasParent = (_sprite->status & Sprite::IS_INHERITED ) == Sprite::IS_INHERITED;
	glUniform1i(glGetUniformLocation(Shader::m_pCurrentShader->m_programId, "hasParent"), hasParent);
	if (hasParent)
	    ParentPipeline(_sprite->m_pInherited);

	if (_sprite->m_pMaterial && m_isLight)
	    LightingEffectPipeline(_sprite->m_pMaterial);

	if (GLM::m_mode == GLM::DRAW_FILL)
	    glEnable(GL_BLEND);
	else
	    glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(_sprite->sfactor, _sprite->dfactor);

	Render(_sprite->m_pMeshes);
	//Render(*(_sprite->pVao), _sprite->elementSize);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void GraphicSystem::ParentPipeline(Transform* _pTransform) const
{
	glUniformMatrix4fv(glGetUniformLocation(Shader::m_pCurrentShader->m_programId, "m4_parentTranslate"),
		1, GL_FALSE, &Translate(_pTransform->position).m[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(Shader::m_pCurrentShader->m_programId, "m4_parentScale"),
		1, GL_FALSE, &Scale(_pTransform->scale).m[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(Shader::m_pCurrentShader->m_programId, "m4_parentRotate"),
		1, GL_FALSE, &Rotate(DegToRad(_pTransform->rotation), _pTransform->rotationAxis).m[0][0]);
}

void GraphicSystem::MappingPipeline(Sprite* _sprite)
{
	glBindTexture(GL_TEXTURE_2D, _sprite->GetCurrentTexutre());

	if (_sprite->m_pAnimation) {

		static Animation* animation;
		animation = _sprite->m_pAnimation;

		if (animation->m_activeAnimation) {

			static float realSpeed;
			realSpeed = animation->m_realSpeed;

			if (realSpeed <= animation->m_timer.GetTime()) {

				static float nextFrame;
				if ((_sprite->status & Sprite::IS_FLIPPED) == Sprite::IS_FLIPPED)
					nextFrame = animation->m_currentFrame - animation->m_realFrame;
				else
					nextFrame = animation->m_currentFrame + animation->m_realFrame;

				if (nextFrame >= 1.f)
					animation->m_currentFrame = 0.f;
				else
					animation->m_currentFrame = nextFrame;

				animation->m_timer.Start();
			} // if (realSpeed <= animation->m_timer.GetTime()) {
		} // if (animation->activeAnimation) {

		m_aniScale.Set(animation->m_realFrame, 1.f, 0.f);
		m_aniTranslate.Set(animation->m_currentFrame, 0.f, 0.f);

	} // if (_sprite->m_hasAnimation) {

	else {
		m_aniScale.Set(1, 1, 0);
		m_aniTranslate.Set(0, 0, 0);
	}

	// Send color info to shader
	Shader::m_pCurrentShader->SetVector4("v4_color", _sprite->color);
	Shader::m_pCurrentShader->SetBool(
		"boolean_flip", (_sprite->status & Sprite::IS_FLIPPED) == Sprite::IS_FLIPPED);
	Shader::m_pCurrentShader->SetMatrix("m4_aniScale", Scale(m_aniScale));
	Shader::m_pCurrentShader->SetMatrix("m4_aniTranslate", Translate(m_aniTranslate));
}

void GraphicSystem::LightingEffectPipeline(Material *_material)
{
	Shader::m_pCurrentShader->SetInt("int_lightSize", int(m_lights.size()));

	// Send material info to shader
	Shader::m_pCurrentShader->SetInt("material.m_specular", _material->specular);
	Shader::m_pCurrentShader->SetInt("material.m_diffuse", _material->diffuse);
	Shader::m_pCurrentShader->SetFloat("material.m_shininess", _material->shininess);

	static int s_lightIndex;
	static std::string s_index, s_light,
		amb("m_ambient"), spec("m_specular"), diff("m_diffuse"),
		type("m_type"), constant("m_constant"), linear("m_linear"), dir("m_direction"), pos("m_position"),
		cut("m_cutOff"), outcut("m_outerCutOff"), quad("m_quadratic");
	s_lightIndex = 0;

	for (auto _light : m_lights) {

		s_index = std::to_string(s_lightIndex);

		Shader::m_pCurrentShader->SetVector4(
			("v4_lightColor[" + s_index + "]").c_str(), _light->color);

		s_light = "light[" + s_index + "].";

		Shader::m_pCurrentShader->SetVector4(
			(s_light + spec).c_str(), _light->specular);

		Shader::m_pCurrentShader->SetVector4(
			(s_light + diff).c_str(), _light->diffuse);

		Shader::m_pCurrentShader->SetEnum(
			(s_light + type).c_str(), _light->m_type);

		Shader::m_pCurrentShader->SetVector3(
			(s_light + dir).c_str(), _light->direction);

		Shader::m_pCurrentShader->SetFloat(
			(s_light + constant).c_str(), _light->constant);

		Shader::m_pCurrentShader->SetFloat(
			(s_light + linear).c_str(), _light->linear);

		Shader::m_pCurrentShader->SetFloat(
			(s_light + quad).c_str(), _light->quadratic);

		Shader::m_pCurrentShader->SetVector3(
			(s_light + pos).c_str(), _light->position);

		Shader::m_pCurrentShader->SetFloat(
			(s_light + cut).c_str(), cosf(Math::DegToRad(_light->cutOff)));

		Shader::m_pCurrentShader->SetFloat(
			(s_light + outcut).c_str(), cosf(Math::DegToRad(_light->outerCutOff)));

		s_lightIndex++;
	}
}

void GraphicSystem::TextPipeline(Text * _text)
{
	static Transform* s_pTransform;
	s_pTransform = _text->m_pTransform;

	Shader::Use(GLM::SHADER_TEXT);

	Shader::m_pCurrentShader->SetMatrix("m4_scale", Scale(s_pTransform->scale));
	Shader::m_pCurrentShader->SetMatrix("m4_rotate", Rotate(Math::DegToRad(s_pTransform->rotation), s_pTransform->rotationAxis));
	Shader::m_pCurrentShader->SetBool("boolean_bilboard", (_text->status & Sprite::IS_BILBOARD) == Sprite::IS_BILBOARD);
	Shader::m_pCurrentShader->SetVector4("v4_color", _text->color);

	// Send projection info to shader
	if (_text->projection == PROJECTION_PERSPECTIVE) {
		Shader::m_pCurrentShader->SetMatrix("m4_projection", m_perspective);

		m_viewport = LookAt(
			m_pMainCamera->position, m_pMainCamera->target, m_pMainCamera->up);
	}

	else {
		Shader::m_pCurrentShader->SetMatrix("m4_projection", m_orthogonal);

		SetIdentity(m_viewport);
		m_viewport = Scale(m_resolutionScaler);
	}

	// Send camera info to shader
	Shader::m_pCurrentShader->SetMatrix("m4_viewport", m_viewport);

	// TODO
	// It so, not draw
	//if (!_sprite->m_culled) {

	if (GLM::m_mode == GLM::DRAW_FILL)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(_text->sfactor, _text->dfactor);

	Render(_text->pFont, _text, s_pTransform, _text->m_printWide);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void GraphicSystem::ParticlePipeline(Emitter* _emitter, const float _dt)
{
	// Check emitter's active toggle
	if (_emitter->active) {

		// Particle render attributes setting
		if (GLM::m_mode == GLM::DRAW_FILL)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);

		glDepthMask(GL_FALSE);
		glBlendFunc(_emitter->sfactor, _emitter->dfactor);

		// Points
		if (_emitter->renderType == Emitter::PARTICLERENDER_POINT) {
			glPointSize(_emitter->pointSize);
			glEnable(GL_POINT_SMOOTH);
		}

		// Plane 2d and 3d form
		else
			glDisable(GL_POINT_SMOOTH);

		static GLuint	    s_vao, s_elementSize;
		static vec3	    s_velocity, s_colorDiff;
		static bool	    s_changeColor, s_rotation;
		static vec4	    s_color;
		static unsigned	    s_texture;
		static Transform*   s_pTransform;

		s_vao = *(_emitter->pVao);
		s_elementSize = _emitter->elementSize;
		s_rotation = _emitter->rotationSpeed != 0.f;
		s_changeColor = _emitter->m_changeColor;
		s_pTransform = _emitter->m_pTransform;
		s_texture = _emitter->m_mainTex;
		s_velocity = _dt * _emitter->velocity;
		s_colorDiff = _dt * _emitter->colorDiff;

		Shader::Use(GLM::SHADER_PARTICLE);

		Shader::m_pCurrentShader->SetMatrix("m4_scale", Scale(s_pTransform->scale));
		Shader::m_pCurrentShader->SetBool("boolean_bilboard", (_emitter->status & Sprite::IS_BILBOARD) == Sprite::IS_BILBOARD);

		// Send projection info to shader
		if (_emitter->projection == PROJECTION_PERSPECTIVE) {
			Shader::m_pCurrentShader->SetMatrix("m4_projection", m_perspective);

			m_viewport = LookAt(
				m_pMainCamera->position, m_pMainCamera->target, m_pMainCamera->up);
		}

		else {
			Shader::m_pCurrentShader->SetMatrix("m4_projection", m_orthogonal);

			SetIdentity(m_viewport);
			m_viewport = Scale(m_resolutionScaler);
		}

		// Send camera info to shader
		Shader::m_pCurrentShader->SetMatrix("m4_viewport", m_viewport);

		glBindTexture(GL_TEXTURE_2D, s_texture);

		for (auto particle : _emitter->m_particles) {

			if (particle->life < 0.f)
				particle->Refresh();

			else {

				particle->life -= _dt;
				particle->position += particle->direction * s_velocity;

				if (s_rotation)
					particle->rotation += particle->rotationSpeed * _dt;

				if (s_changeColor)
					particle->color += s_colorDiff;

				s_color.Set(particle->color.x, particle->color.y, particle->color.z,
					particle->life);

				// Send transform info to shader
				Shader::m_pCurrentShader->SetMatrix("m4_translate", Translate(particle->position));
				Shader::m_pCurrentShader->SetMatrix("m4_rotate", Rotate(Math::DegToRad(particle->rotation), s_pTransform->rotationAxis));

				// Send color info to shader
				Shader::m_pCurrentShader->SetVector4("v4_color", s_color);
				Shader::m_pCurrentShader->SetBool("boolean_hide", particle->hidden);

				Render(s_vao, s_elementSize);
			}
		}

		glDepthMask(GL_TRUE);	// Enable depth buffer writing
		glDisable(GL_BLEND);	// Disable blend
	}

	glBindVertexArray(0);
}

void GraphicSystem::Render(const unsigned &_vao, const int _elementSize)
{
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _elementSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GraphicSystem::RenderCharacter(Character& _character, const vec3& _position,
	const vec3& _scale, float& _newX, float _intervalY)
{
	const static int sc_shift = 6;
	static vec3 s_realPosition;

	s_realPosition.x = _newX + _character.bearing.x * _scale.x;
	s_realPosition.y = _position.y - (_character.size.y - _character.bearing.y) * _scale.y - _intervalY;
	s_realPosition.z = _position.z;

	GLfloat width = _character.size.x;
	GLfloat height = _character.size.y;

	Shader::m_pCurrentShader->SetMatrix("m4_translate", Translate(s_realPosition));

	//Update vbo
	float vertices[4][8] = {
		{ 0.f, height, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f },
		{ width, height, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f },
		{ 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f },
		{ width, 0.f, 0.f, 1.f, 1.f ,0.f, 0.f, 1.f }
	};

	unsigned verticesSize = sizeof(vertices) / sizeof(GLfloat);

	m_vertexArray.clear();
	m_vertexArray.reserve(verticesSize);
	for (unsigned index = 0; index < 4; ++index) 
		m_vertexArray.push_back(jeVertex{ 
			vec3(vertices[index][0], vertices[index][1], vertices[index][2]), 
			vec2(vertices[index][3], vertices[index][4]), 
			vec3(vertices[index][5], vertices[index][6], vertices[index][7])} );

	glBindTexture(GL_TEXTURE_2D, _character.texture);
	Render(GLM::m_vao[GLM::SHAPE_TEXT], GLM::m_vbo[GLM::SHAPE_TEXT], GLM::m_ebo[GLM::SHAPE_TEXT],
		m_vertexArray, Text::m_idices, GL_TRIANGLE_STRIP);
	
	/*glBindBuffer(GL_ARRAY_BUFFER, GLM::m_vbo[GLM::SHAPE_PLANE]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);*/

	_newX += (_character.advance >> sc_shift) * _scale.x;
}

void GraphicSystem::Render(Font* _font, Text*_text, Transform* _transform, bool _printUnicode)
{
	const std::wstring c_wcontent = _text->GetWText();
	const std::string c_content = _text->GetText();

	// Check there are contents to print out
	if (!c_wcontent.empty() || !c_content.empty()) {

		static vec3 s_position, s_scale;
		s_scale = _transform->scale;
		s_position = _transform->position;
		const GLfloat nextLineInverval = _font->m_newLineInterval * _font->m_fontSize * s_scale.y / 50.f;

		GLfloat initX = GLfloat(s_position.x), newX = initX, intervalY = 0.f;
		int num_newline = 1;

		glBindVertexArray(GLM::m_vao[GLM::SHAPE_PLANE]);

		// Iterate all character
		if (_printUnicode) {
			std::wstring::const_iterator letter;
			for (letter = c_wcontent.begin(); letter != c_wcontent.end(); ++letter)
			{
				const wchar_t newline = *letter;
				if (newline == L'\n') {
					newX = initX;
					intervalY = nextLineInverval * num_newline;
					++num_newline;
				}

				else {
					Character character = _font->m_data[*letter];
					RenderCharacter(character, s_position, s_scale, newX, intervalY);
				}
			}
		}

		else
		{
			std::string::const_iterator letter;
			// Iterate all character
			for (letter = c_content.begin(); letter != c_content.end(); ++letter)
			{
				const wchar_t newline = *letter;
				if (newline == '\n') {
					newX = initX;
					intervalY = nextLineInverval * num_newline;
					++num_newline;
				}

				else {
					Character character = _font->m_data[*letter];
					RenderCharacter(character, s_position, s_scale, newX, intervalY);
				}
			}
		}

		glBindVertexArray(0);
	}
}


void GraphicSystem::Render(const Mesh* _pMesh)
{
	m_vertexArray.clear();
	m_vertexArray.reserve(_pMesh->GetPointCount());
	for (unsigned index = 0; index < _pMesh->GetPointCount(); ++index)
		m_vertexArray.push_back(jeVertex{_pMesh->GetPoint(index), _pMesh->GetUV(index), _pMesh->GetNormal(index)});
	
	switch (_pMesh->m_shape)
	{
	case Mesh::MESH_NONE:
		Render(GLM::m_vao[GLM::SHAPE_CONE], GLM::m_vbo[GLM::SHAPE_CONE], GLM::m_ebo[GLM::SHAPE_CONE],
			m_vertexArray, _pMesh->GetIndices(), _pMesh->m_drawMode);
		break;

	case Mesh::MESH_POINT:
		Render(GLM::m_vao[GLM::SHAPE_POINT], GLM::m_vbo[GLM::SHAPE_POINT], GLM::m_ebo[GLM::SHAPE_POINT],
			m_vertexArray, _pMesh->GetIndices(), _pMesh->m_drawMode);
		break;

	case Mesh::MESH_RECT:
		Render(GLM::m_vao[GLM::SHAPE_PLANE], GLM::m_vbo[GLM::SHAPE_PLANE], GLM::m_ebo[GLM::SHAPE_PLANE],
			m_vertexArray, _pMesh->GetIndices(), _pMesh->m_drawMode);
		break;

	case Mesh::MESH_CROSSRECT:
		Render(GLM::m_vao[GLM::SHAPE_PLANE3D], GLM::m_vbo[GLM::SHAPE_PLANE3D], GLM::m_ebo[GLM::SHAPE_PLANE3D],
			m_vertexArray, _pMesh->GetIndices(), _pMesh->m_drawMode);
		break;

	case Mesh::MESH_CUBE:
		Render(GLM::m_vao[GLM::SHAPE_CUBE], GLM::m_vbo[GLM::SHAPE_CUBE], GLM::m_ebo[GLM::SHAPE_CUBE],
			m_vertexArray, _pMesh->GetIndices(), _pMesh->m_drawMode);
		break;

	case Mesh::MESH_TETRAHEDRON:
		Render(GLM::m_vao[GLM::SHAPE_CONE], GLM::m_vbo[GLM::SHAPE_CONE], GLM::m_ebo[GLM::SHAPE_CONE], 
			m_vertexArray, _pMesh->GetIndices(), _pMesh->m_drawMode);
		break;

	default:
		break;
	}
}

void GraphicSystem::Render(unsigned _vao, unsigned _vbo, unsigned _ebo, 
	const Vertexes& _vertexes, const Indices& _indices, unsigned _drawMode)
{
	// This part actual render
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(jeVertex) * _vertexes.size(),
	    static_cast<const void*>(&_vertexes[0]), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(jeVertex), reinterpret_cast<void*>(offsetof(jeVertex, jeVertex::position)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(jeVertex), reinterpret_cast<void*>(offsetof(jeVertex, jeVertex::uv)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(jeVertex), reinterpret_cast<void*>(offsetof(jeVertex, jeVertex::normal)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * _indices.size(),
		static_cast<const void*>(&_indices[0]), GL_DYNAMIC_DRAW);

	glDrawElements(_drawMode, static_cast<GLsizei>(_indices.size()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}


jeEnd
