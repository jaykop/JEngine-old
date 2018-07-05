#include <string>
#include "GraphicSystem.h"
#include "GLManager.h"
#include "Shader.h"
#include "GraphicComponents.h"
#include "Transform.h"
#include "MathUtils.h"

jeBegin

using namespace Math;

void GraphicSystem::UpdatePipelines(float dt)
{
	// Update the perpsective matrix by camera's zoom
	perspective_ = Perspective(
		pMainCamera_->fovy_, pMainCamera_->aspect_,
		pMainCamera_->near_, pMainCamera_->far_);

	// Update the projection size by window screen size
	static vec3 s_windowSize, s_resolutionStandard(1.f / 800.f, 1.f / 600.f, 1.f);
	s_windowSize.Set(float(width_), float(height_), 1.f);

	resolutionScaler_ = s_windowSize * s_resolutionStandard;

	// Update models and lights
	isLight_ = !lights_.empty();

	LightSourcePipeline();

	Shader::Use(GLM::JE_SHADER_MODEL);

	// Inform that there are lights
	Shader::pCurrentShader_->SetBool("boolean_light", isLight_);
	
	// Sort orthogonal objects and perspective objects
	SortModels();

	for (auto model : models_) {

		// Emitter
		if ((model->is_ & Model::IS_EMITTER) == Model::IS_EMITTER)
			ParticlePipeline(static_cast<Emitter*>(model), dt);

		// Text
		else if ((model->is_ & Model::IS_TEXT) == Model::IS_TEXT)
			TextPipeline(static_cast<Text*>(model));

		// Normal models
		else
			ModelPipeline(model);
	}
}

void GraphicSystem::RenderToFramebuffer() const
{
	// Render to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, GLM::fbo_);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glViewport(0, 0, GLint(width_), GLint(height_));

	// Backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void GraphicSystem::RenderToScreen() const
{
	const static GLsizei sizeOfPlaneIndices
		= static_cast<GLsizei>(GLM::targetMesh_[GLM::JE_TARGET_SCREEN]->GetIndiceCount());

	// Bind default framebuffer and render to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(backgroundColor_.x, backgroundColor_.y, backgroundColor_.z, backgroundColor_.w);

	glDisable(GL_CULL_FACE);	//Disable face culling
	glDisable(GL_DEPTH_TEST);	//Disable depth test

	// Render to plane 2d
	glBindVertexArray(GLM::targetMesh_[GLM::JE_TARGET_SCREEN]->vao_);
	Shader::Use(GLM::JE_SHADER_SCREEN);
	Shader::pCurrentShader_->SetVector4("v4_screenColor_", screenColor_);

	// Impose screen effect 
	Shader::pCurrentShader_->SetEnum("enum_effectType", screenEffect_);

	if (screenEffect_ == JE_EFFECT_BLUR) {
		Shader::pCurrentShader_->SetFloat("float_blurSize_", blurSize_);
		Shader::pCurrentShader_->SetFloat("float_blurAmount_", blurAmount_);
	}
	else if (screenEffect_ == JE_EFFECT_SOBEL)
		Shader::pCurrentShader_->SetFloat("float_sobelAmount_", sobelAmount_);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GLM::texColorBuf_);
	glDrawElements(GL_TRIANGLES, sizeOfPlaneIndices, GL_UNSIGNED_INT, nullptr);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////
// Light box pipeline
//////////////////////////////////////////////////////////////////////////
void GraphicSystem::LightSourcePipeline()
{
	if (isLight_) {

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		Shader::Use(GLM::JE_SHADER_LIGHTING);
		
		for (auto light : lights_) {

			Transform* transform = light->pTransform_;

			Shader::pCurrentShader_->SetMatrix(
				"m4_translate", Translate(transform->position));

			Shader::pCurrentShader_->SetMatrix(
				"m4_rotate", Rotate(Math::DegToRad(transform->rotation), transform->rotationAxis));

			Shader::pCurrentShader_->SetMatrix("m4_scale", Scale(transform->scale));

			Shader::pCurrentShader_->SetMatrix(
				"m4_rotateZ", RotateZ(atan2(light->direction_.y, light->direction_.x)));

			Shader::pCurrentShader_->SetMatrix(
				"m4_rotateY", RotateY(-atan2(light->direction_.z, light->direction_.x)));

			if (light->projection_ == PROJECTION_PERSPECTIVE) {
				Shader::pCurrentShader_->SetMatrix("m4_projection", perspective_);

				viewport_ = LookAt(pMainCamera_->position_, pMainCamera_->target_, pMainCamera_->up_);
			}

			else {
				right_ = width_ * .5f;
				left_ = -right_;
				top_ = height_ * .5f;
				bottom_ = -top_;

				orthogonal_ = Orthogonal(left_, right_, bottom_, top_, pMainCamera_->near_, pMainCamera_->far_);

				Shader::pCurrentShader_->SetMatrix("m4_projection", orthogonal_);

				SetIdentity(viewport_);
				viewport_ = Scale(resolutionScaler_);
			}
			
			Shader::pCurrentShader_->SetMatrix("m4_viewport", viewport_);
			Shader::pCurrentShader_->SetVector4("v4_color", light->color_);

			glBlendFunc(light->sfactor_, light->dfactor_);

			// Update every mesh
			unsigned mode = light->drawMode_;
			for (auto mesh : light->meshes_)
				Render(mesh, mode);
			
		} // for (auto light : lights_) {
	} // if (isLight_) {

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

//////////////////////////////////////////////////////////////////////////
// Model(model) pipeline
//////////////////////////////////////////////////////////////////////////
void GraphicSystem::ModelPipeline(Model *pModel)
{
	static Transform* spTransform;
	spTransform = pModel->pTransform_;

	Shader::Use(GLM::JE_SHADER_MODEL);

	Shader::pCurrentShader_->SetMatrix("m4_translate", Translate(spTransform->position));
	Shader::pCurrentShader_->SetMatrix("m4_scale", Scale(spTransform->scale));
	Shader::pCurrentShader_->SetMatrix("m4_rotate", Rotate(Math::DegToRad(spTransform->rotation), spTransform->rotationAxis));

	Shader::pCurrentShader_->SetVector3("v3_cameraPosition", pMainCamera_->position_);
	Shader::pCurrentShader_->SetBool("boolean_bilboard", (pModel->status_ & Model::IS_BILBOARD) == Model::IS_BILBOARD);

	// Send projection info to shader
	if (pModel->projection_ == PROJECTION_PERSPECTIVE) {

		Shader::pCurrentShader_->SetMatrix("m4_projection", perspective_);
		viewport_ = LookAt(pMainCamera_->position_, pMainCamera_->target_, pMainCamera_->up_);
	}

	else {
		right_ = width_ * .5f;
		left_ = -right_;
		top_ = height_ * .5f;
		bottom_ = -top_;

		orthogonal_ = Orthogonal(left_, right_, bottom_, top_, pMainCamera_->near_, pMainCamera_->far_);

		Shader::pCurrentShader_->SetMatrix("m4_projection", orthogonal_);

		SetIdentity(viewport_);
		viewport_ = Scale(resolutionScaler_);
	}

	// Send camera info to shader
	Shader::pCurrentShader_->SetMatrix("m4_viewport", viewport_);

	// TODO
	// It so, not draw
	//if (!pModel->culled_) {

	MappingPipeline(pModel);

	bool hasParent = (pModel->status_ & Model::IS_INHERITED ) == Model::IS_INHERITED;
	glUniform1i(glGetUniformLocation(Shader::pCurrentShader_->programId_, "hasParent"), hasParent);
	if (hasParent)
	    ParentPipeline(pModel->pInherited_);

	if (pModel->pMaterial_ && isLight_)
	    LightingEffectPipeline(pModel->pMaterial_);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(pModel->sfactor_, pModel->dfactor_);

	// Update every mesh
	unsigned mode = pModel->drawMode_;
	for (auto mesh : pModel->meshes_)
		Render(mesh, mode);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void GraphicSystem::ParentPipeline(Transform* pTransform) const
{
	glUniformMatrix4fv(glGetUniformLocation(Shader::pCurrentShader_->programId_, "m4_parentTranslate"),
		1, GL_FALSE, &Translate(pTransform->position).m[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(Shader::pCurrentShader_->programId_, "m4_parentScale"),
		1, GL_FALSE, &Scale(pTransform->scale).m[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(Shader::pCurrentShader_->programId_, "m4_parentRotate"),
		1, GL_FALSE, &Rotate(DegToRad(pTransform->rotation), pTransform->rotationAxis).m[0][0]);
}

void GraphicSystem::MappingPipeline(Model* pModel)
{
	for (auto mesh : pModel->meshes_) {

		glBindTexture(GL_TEXTURE_2D, mesh->mainTexture_);

		if (pModel->pAnimation_) {

			static Animation* animation;
			animation = pModel->pAnimation_;

			if (animation->active_) {

				static float realSpeed;
				realSpeed = animation->realSpeed_;

				if (realSpeed <= animation->timer_.GetTime()) {

					static float nextFrame;
					if ((pModel->status_ & Model::IS_FLIPPED) == Model::IS_FLIPPED)
						nextFrame = animation->currentFrame_ - animation->realFrame_;
					else
						nextFrame = animation->currentFrame_ + animation->realFrame_;

					if (nextFrame >= 1.f)
						animation->currentFrame_ = 0.f;
					else
						animation->currentFrame_ = nextFrame;

					animation->timer_.Start();
				} // if (realSpeed <= animation->timer_.GetTime()) {
			} // if (animation->activeAnimation) {

			aniScale_.Set(animation->realFrame_, 1.f, 0.f);
			aniTranslate_.Set(animation->currentFrame_, 0.f, 0.f);

		} // if (pModel->m_hasAnimation) {

		else {
			aniScale_.Set(1, 1, 0);
			aniTranslate_.Set(0, 0, 0);
		}

		// Send color info to shader
		Shader::pCurrentShader_->SetVector4("v4_color", pModel->color_);
		Shader::pCurrentShader_->SetBool(
			"boolean_flip", (pModel->status_ & Model::IS_FLIPPED) == Model::IS_FLIPPED);
		Shader::pCurrentShader_->SetMatrix("m4_aniScale", Scale(aniScale_));
		Shader::pCurrentShader_->SetMatrix("m4_aniTranslate", Translate(aniTranslate_));
	}
}

void GraphicSystem::LightingEffectPipeline(Material *pMaterial)
{
	Shader::pCurrentShader_->SetInt("int_lightSize", int(lights_.size()));

	// Send material info to shader
	Shader::pCurrentShader_->SetInt("material.m_specular", pMaterial->specular_);
	Shader::pCurrentShader_->SetInt("material.m_diffuse", pMaterial->diffuse_);
	Shader::pCurrentShader_->SetFloat("material.m_shininess", pMaterial->shininess_);

	static int s_lightIndex;
	static std::string s_index, s_light,
		amb("m_ambient"), spec("m_specular"), diff("m_diffuse"),
		type("type_"), constant("mconstant"), linear_("m_linear_"), dir("mdirectoryection"), pos("mposition"),
		cut("m_cutOff"), outcut("m_outerCutOff"), quad("m_quadratic");
	s_lightIndex = 0;

	for (auto _light : lights_) {

		s_index = std::to_string(s_lightIndex);

		Shader::pCurrentShader_->SetVector4(
			("v4_lightColor[" + s_index + "]").c_str(), _light->color_);

		s_light = "light[" + s_index + "].";

		Shader::pCurrentShader_->SetVector4(
			(s_light + spec).c_str(), _light->specular_);

		Shader::pCurrentShader_->SetVector4(
			(s_light + diff).c_str(), _light->diffuse_);

		Shader::pCurrentShader_->SetEnum(
			(s_light + type).c_str(), _light->type_);

		Shader::pCurrentShader_->SetVector3(
			(s_light + dir).c_str(), _light->direction_);

		Shader::pCurrentShader_->SetFloat(
			(s_light + constant).c_str(), _light->constant_);

		Shader::pCurrentShader_->SetFloat(
			(s_light + linear_).c_str(), _light->linear_);

		Shader::pCurrentShader_->SetFloat(
			(s_light + quad).c_str(), _light->quadratic_);

		Shader::pCurrentShader_->SetVector3(
			(s_light + pos).c_str(), _light->pTransform_->position);

		Shader::pCurrentShader_->SetFloat(
			(s_light + cut).c_str(), cosf(Math::DegToRad(_light->cutOff_)));

		Shader::pCurrentShader_->SetFloat(
			(s_light + outcut).c_str(), cosf(Math::DegToRad(_light->outerCutOff_)));

		s_lightIndex++;
	}
}

void GraphicSystem::TextPipeline(Text * pText)
{
	static Transform* spTransform;
	spTransform = pText->pTransform_;

	Shader::Use(GLM::JE_SHADER_TEXT);

	Shader::pCurrentShader_->SetMatrix("m4_scale", Scale(spTransform->scale));
	Shader::pCurrentShader_->SetMatrix("m4_rotate", Rotate(Math::DegToRad(spTransform->rotation), spTransform->rotationAxis));
	Shader::pCurrentShader_->SetBool("boolean_bilboard", (pText->status_ & Model::IS_BILBOARD) == Model::IS_BILBOARD);
	Shader::pCurrentShader_->SetVector4("v4_color", pText->color_);

	// Send projection info to shader
	if (pText->projection_ == PROJECTION_PERSPECTIVE) {
		Shader::pCurrentShader_->SetMatrix("m4_projection", perspective_);

		viewport_ = LookAt(
			pMainCamera_->position_, pMainCamera_->target_, pMainCamera_->up_);
	}

	else {
		right_ = width_ * .5f;
		left_ = -right_;
		top_ = height_ * .5f;
		bottom_ = -top_;

		orthogonal_ = Orthogonal(left_, right_, bottom_, top_, pMainCamera_->near_, pMainCamera_->far_);

		Shader::pCurrentShader_->SetMatrix("m4_projection", orthogonal_);

		SetIdentity(viewport_);
		viewport_ = Scale(resolutionScaler_);
	}

	// Send camera info to shader
	Shader::pCurrentShader_->SetMatrix("m4_viewport", viewport_);

	// TODO
	// It so, not draw
	//if (!pModel->culled_) {

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(pText->sfactor_, pText->dfactor_);

	// Text does not render its own mesh list
	Render(pText);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void GraphicSystem::ParticlePipeline(Emitter* pEmitter, float dt)
{
	// Check emitter's active toggle
	if (pEmitter->active_) {

		// Particle render attributes setting
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(pEmitter->sfactor_, pEmitter->dfactor_);

		static vec3			s_velocity, s_colorDiff_;
		static bool			s_changeColor, s_rotation;
		static vec4			s_color;
		static unsigned	    s_mode;
		static Transform*   spTransform;

		s_rotation = pEmitter->rotationSpeed_ != 0.f;
		s_changeColor = pEmitter->changeColor_;
		spTransform = pEmitter->pTransform_;
		s_velocity = dt * pEmitter->velocity_;
		s_colorDiff_ = dt * pEmitter->colorDiff_;
		s_mode = pEmitter->drawMode_;

		Shader::Use(GLM::JE_SHADER_PARTICLE);

		Shader::pCurrentShader_->SetMatrix("m4_scale", Scale(spTransform->scale));
		Shader::pCurrentShader_->SetBool("boolean_bilboard", (pEmitter->status_ & Model::IS_BILBOARD) == Model::IS_BILBOARD);

		// Send projection info to shader
		if (pEmitter->projection_ == PROJECTION_PERSPECTIVE) {
			Shader::pCurrentShader_->SetMatrix("m4_projection", perspective_);

			viewport_ = LookAt(
				pMainCamera_->position_, pMainCamera_->target_, pMainCamera_->up_);
		}

		else {
			right_ = width_ * .5f;
			left_ = -right_;
			top_ = height_ * .5f;
			bottom_ = -top_;

			orthogonal_ = Orthogonal(left_, right_, bottom_, top_, pMainCamera_->near_, pMainCamera_->far_);

			Shader::pCurrentShader_->SetMatrix("m4_projection", orthogonal_);

			SetIdentity(viewport_);
			viewport_ = Scale(resolutionScaler_);
		}

		// Send camera info to shader
		Shader::pCurrentShader_->SetMatrix("m4_viewport", viewport_);

		for (auto particle : pEmitter->particles_) {

			if (particle->life < 0.f)
				pEmitter->RefreshParticle(particle);

			else {

				particle->life -= dt;
				particle->position += particle->direction * s_velocity;

				if (s_rotation)
					particle->rotation += particle->rotationSpeed * dt;

				if (s_changeColor)
					particle->color += s_colorDiff_;

				s_color.Set(particle->color.x, particle->color.y, particle->color.z,
					particle->life);

				// Send transform info to shader
				Shader::pCurrentShader_->SetMatrix("m4_translate", Translate(particle->position));
				Shader::pCurrentShader_->SetMatrix("m4_rotate", Rotate(Math::DegToRad(particle->rotation), spTransform->rotationAxis));

				// Send color info to shader
				Shader::pCurrentShader_->SetVector4("v4_color", s_color);
				Shader::pCurrentShader_->SetBool("boolean_hide", particle->hidden);

				// Update every mesh
				for (auto mesh : pEmitter->meshes_) {

					static unsigned	 spTexture;
					spTexture = mesh->mainTexture_;
					glBindTexture(GL_TEXTURE_2D, spTexture);

					Render(mesh, s_mode);
				}
			}
		}

		glDepthMask(GL_TRUE);	// Enable depth buffer writing
		glDisable(GL_BLEND);	// Disable blend
	}

	glBindVertexArray(0);
}

void GraphicSystem::RenderCharacter(Character& character, const vec3& position,
	const vec3& scale, float& newX, float intervalY)
{
	const static int sc_shift = 6;
	static vec3 s_realPosition;

	s_realPosition.x = newX + character.bearing.x * scale.x;
	s_realPosition.y = position.y - (character.size.y - character.bearing.y) * scale.y - intervalY;
	s_realPosition.z = position.z;

	GLfloat width = character.size.x;
	GLfloat height = character.size.y;

	Shader::pCurrentShader_->SetMatrix("m4_translate", Translate(s_realPosition));

	//Update vbo
	float vertices[4][8] = {
		{ 0.f, height, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f },
		{ width, height, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f },
		{ 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f },
		{ width, 0.f, 0.f, 1.f, 1.f ,0.f, 0.f, 1.f }
	};

	glBindTexture(GL_TEXTURE_2D, character.texture);

	// Text component does not use member mesh vector,
	// but pre defined mesh from GLManager
	glBindVertexArray(GLM::targetMesh_[GLM::JE_TARGET_TEXT]->vao_);
	glBindBuffer(GL_ARRAY_BUFFER, GLM::targetMesh_[GLM::JE_TARGET_TEXT]->vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coordinate position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// normals of vertices
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLM::targetMesh_[GLM::JE_TARGET_TEXT]->ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * Text::m_pointIndices.size(),
		static_cast<const void*>(&Text::m_pointIndices[0]), GL_DYNAMIC_DRAW);
	glDrawElements(GL_TRIANGLE_STRIP, GLsizei(Text::m_pointIndices.size()), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);

	newX += (character.advance >> sc_shift) * scale.x;
}

void GraphicSystem::Render(const Text* pText)
{
	Font* pFont = pText->pFont;
	Transform* pTransform = pText->pTransform_;

	const std::wstring c_wcontent = pText->GetWText();
	const std::string c_content = pText->GetText();

	// Check there are contents to print out
	if (!c_wcontent.empty() || !c_content.empty()) {

		static vec3 sposition, sscale;
		sscale = pTransform->scale;
		sposition = pTransform->position;
		const GLfloat nextLineInverval = pFont->newline_ * pFont->fontSize_ * sscale.y / 50.f;

		GLfloat initX = GLfloat(sposition.x), newX = initX, intervalY = 0.f;
		int num_newline = 1;

		// Iterate all character
		if (pText->printWide_) {
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
					Character character = pFont->data_[*letter];
					RenderCharacter(character, sposition, sscale, newX, intervalY);
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
					Character character = pFont->data_[*letter];
					RenderCharacter(character, sposition, sscale, newX, intervalY);
				}
			}
		}
	}
}


void GraphicSystem::Render(const Mesh* pMesh, unsigned drawMode)
{
	glBindVertexArray(pMesh->vao_);
	glDrawElements(drawMode, unsigned(pMesh->GetIndiceCount()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

jeEnd
