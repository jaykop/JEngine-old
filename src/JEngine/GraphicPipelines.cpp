#include <string>
#include "GraphicSystem.h"
#include "GLManager.h"
#include "Shader.h"
#include "GraphicComponents.h"
#include "MathUtils.h"
#include "Transform.h"

jeBegin

void GraphicSystem::UpdatePipelines(const float _dt)
{
	// Update the perpsective matrix by camera's zoom
	m_perspective = mat4::Perspective(m_pMainCamera->zoom, aspect, zNear, zFar);

	// Update the projection size by window screen size
    static vec3 s_windowSize, s_resolutionStandard(1.f / 800.f, 1.f / 600.f, 1.f);
    s_windowSize.Set(float(m_width), float(m_height), 1.f);

    m_resolutionScaler = s_windowSize * s_resolutionStandard;

    // Update sprites and lights
    m_isLight = m_lights.empty() ? false : true;

    LightSourcePipeline();

    GLM::m_shader[GLM::SHADER_MODEL]->Use();

    // Inform that there are lights
    GLM::m_shader[GLM::SHADER_MODEL]->SetBool(
        GLM::UNIFORM_IS_LIGHT, m_isLight);

    for (auto sprite : m_sprites) {

        // Emitter
        if ((sprite->status & Sprite::IS_EMITTER) == Sprite::IS_EMITTER)
            ParticlePipeline(static_cast<Emitter*>(sprite), _dt);

        else if ((sprite->status & Sprite::IS_TEXT) == Sprite::IS_TEXT)
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
    GLM::m_shader[GLM::SHADER_SCREEN]->Use();
    GLM::m_shader[GLM::SHADER_SCREEN]->SetVector4(GLM::UNIFORM_SCREEN_COLOR, screenColor);

    // Impose screen effect 
    GLM::m_shader[GLM::SHADER_SCREEN]->SetEnum(GLM::UNIFORM_SCREEN_EFFECT, screenEffect);

    if (screenEffect == EFFECT_BLUR) {
        GLM::m_shader[GLM::SHADER_SCREEN]->SetFloat(GLM::UNIFORM_SCREEN_BLUR_SIZE, blurSize);
        GLM::m_shader[GLM::SHADER_SCREEN]->SetFloat(GLM::UNIFORM_SCREEN_BLUR_AMOUNT, blurAmount);
    }
    else if (screenEffect == EFFECT_SOBEL)
        GLM::m_shader[GLM::SHADER_SCREEN]->SetFloat(GLM::UNIFORM_SCREEN_SOBEL, sobelAmount);

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

        static float s_lightDeg = 0.f;

        GLM::m_shader[GLM::SHADER_LIGHTING]->Use();

        GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
            GLM::UNIFORM_LIGHT_SCALE,
            mat4::Scale(lightScale));

        for (auto light : m_lights) {

            GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
                GLM::UNIFORM_LIGHT_TRANSLATE,
                mat4::Translate(light->position));

            GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
                GLM::UNIFORM_LIGHT_ROTATEZ,
                mat4::RotateZ(atan2(light->direction.y, light->direction.x)));

            GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
                GLM::UNIFORM_LIGHT_ROTATEY,
                mat4::RotateY(-atan2(light->direction.z, light->direction.x)));

            if (light->projection == PROJECTION_PERSPECTIVE) {
                GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
                    GLM::UNIFORM_LIGHT_PROJECTION, m_perspective);

                m_viewport = mat4::LookAt(
                    m_pMainCamera->position, m_pMainCamera->target, m_pMainCamera->up);

            }

            else {
                GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
                    GLM::UNIFORM_LIGHT_PROJECTION, m_orthogonal);

                m_viewport.SetIdentity();
                m_viewport = mat4::Scale(m_resolutionScaler);
            }

            GLM::m_shader[GLM::SHADER_LIGHTING]->SetMatrix(
                GLM::UNIFORM_LIGHT_CAMERA,
                m_viewport);

            GLM::m_shader[GLM::SHADER_LIGHTING]->SetVector4(
                GLM::UNIFORM_LIGHT_COLOR,
                light->color);

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

    GLM::m_shader[GLM::SHADER_MODEL]->Use();

    GLM::m_shader[GLM::SHADER_MODEL]->SetMatrix(
        GLM::UNIFORM_TRANSLATE, mat4::Translate(s_pTransform->position));

    GLM::m_shader[GLM::SHADER_MODEL]->SetMatrix(
        GLM::UNIFORM_SCALE, mat4::Scale(s_pTransform->scale));

    GLM::m_shader[GLM::SHADER_MODEL]->SetMatrix(
        GLM::UNIFORM_ROTATE, mat4::Rotate(Math::DegToRad(s_pTransform->rotation), s_pTransform->rotationAxis));

    GLM::m_shader[GLM::SHADER_MODEL]->SetVector3(
        GLM::UNIFORM_CAMERA_POSITION, m_pMainCamera->position);

    GLM::m_shader[GLM::SHADER_MODEL]->SetBool(
        GLM::UNIFORM_BILBOARD, _sprite->bilboard);

    // Send projection info to shader
    if (_sprite->projection == PROJECTION_PERSPECTIVE) {
        GLM::m_shader[GLM::SHADER_MODEL]->SetMatrix(
            GLM::UNIFORM_PROJECTION, m_perspective);

        m_viewport = mat4::LookAt(
            m_pMainCamera->position, m_pMainCamera->target, m_pMainCamera->up);
    }

    else {
        GLM::m_shader[GLM::SHADER_MODEL]->SetMatrix(
            GLM::UNIFORM_PROJECTION, m_orthogonal);

        m_viewport.SetIdentity();
        m_viewport = mat4::Scale(m_resolutionScaler);
    }

    // Send camera info to shader
    GLM::m_shader[GLM::SHADER_MODEL]->SetMatrix(
        GLM::UNIFORM_CAMERA, m_viewport);

    // TODO
    // It so, not draw
    //if (!_sprite->m_culled) {

    MappingPipeline(_sprite);

    if (((_sprite->status & Sprite::HAS_MATERIAL) == Sprite::HAS_MATERIAL)
		&& m_isLight)
        LightingEffectPipeline(_sprite->m_pMaterial);

    if (GLM::m_mode == GLM::DRAW_FILL)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(_sprite->sfactor, _sprite->dfactor);

    Render(*(_sprite->pVao), _sprite->elementSize);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void GraphicSystem::MappingPipeline(Sprite* _sprite)
{
    glBindTexture(GL_TEXTURE_2D, _sprite->GetCurrentTexutre());

    if ( ( _sprite->status & Sprite::HAS_ANIMATION ) == Sprite::HAS_ANIMATION) {

        static Animation* animation;
        animation = _sprite->m_pAnimation;

        if (animation->m_activeAnimation) {

            static float realSpeed;
            realSpeed = animation->m_realSpeed;

            if (realSpeed <= animation->m_timer.GetTime()) {

                static float nextFrame;
                if (_sprite->flip)
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
    GLM::m_shader[GLM::SHADER_MODEL]->SetVector4(
        GLM::UNIFORM_COLOR,
        _sprite->color);

    GLM::m_shader[GLM::SHADER_MODEL]->SetBool(
        GLM::UNIFORM_FLIP,
        _sprite->flip);

    GLM::m_shader[GLM::SHADER_MODEL]->SetMatrix(
        GLM::UNIFORM_ANI_SCALE,
        mat4::Scale(m_aniScale));

    GLM::m_shader[GLM::SHADER_MODEL]->SetMatrix(
        GLM::UNIFORM_ANI_TRANSLATE,
        mat4::Translate(m_aniTranslate));
}

void GraphicSystem::LightingEffectPipeline(Material *_material)
{
    GLM::m_shader[GLM::SHADER_MODEL]->SetInt(
        GLM::UNIFORM_LIGHT_SIZE, int(m_lights.size()));

    // Send material info to shader
    GLM::m_shader[GLM::SHADER_MODEL]->SetInt(
        GLM::UNIFORM_MATERIAL_SPECULAR,
        _material->specular);

    GLM::m_shader[GLM::SHADER_MODEL]->SetInt(
        GLM::UNIFORM_MATERIAL_DIFFUSE,
        _material->diffuse);

    GLM::m_shader[GLM::SHADER_MODEL]->SetFloat(
        GLM::UNIFORM_MATERIAL_SHININESS,
        _material->shininess);

    static int s_lightIndex;
    static std::string s_index, s_color, s_light, s_input,
        amb("m_ambient"), spec("m_specular"), diff("m_diffuse"),
        type("m_type"), constant("m_constant"), linear("m_linear"), dir("direction"), pos("position"),
        cut("m_cutOff"), outcut("m_outerCutOff"), quad("m_quadratic");
    s_lightIndex = 0;

    for (auto _light : m_lights) {

        s_index = std::to_string(s_lightIndex);

        s_color = "v4_lightColor[" + s_index + "]";
        GLM::m_shader[GLM::SHADER_MODEL]->SetVector4(
            s_color.c_str(), _light->color);

        s_light = "light[" + s_index + "].";

        s_input = s_light + spec;
        GLM::m_shader[GLM::SHADER_MODEL]->SetVector4(
            s_input.c_str(), _light->specular);

        s_input = s_light + diff;
        GLM::m_shader[GLM::SHADER_MODEL]->SetVector4(
            s_input.c_str(), _light->diffuse);

        s_input = s_light + type;
        GLM::m_shader[GLM::SHADER_MODEL]->SetEnum(
            s_input.c_str(), _light->m_type);

        s_input = s_light + dir;
        GLM::m_shader[GLM::SHADER_MODEL]->SetVector3(
            s_input.c_str(), _light->direction);

        s_input = s_light + constant;
        GLM::m_shader[GLM::SHADER_MODEL]->SetFloat(
            s_input.c_str(), _light->constant);

        s_input = s_light + linear;
        GLM::m_shader[GLM::SHADER_MODEL]->SetFloat(
            s_input.c_str(), _light->linear);

        s_input = s_light + quad;
        GLM::m_shader[GLM::SHADER_MODEL]->SetFloat(
            s_input.c_str(), _light->quadratic);

        s_input = s_light + pos;
        GLM::m_shader[GLM::SHADER_MODEL]->SetVector3(
            s_input.c_str(), _light->position);

        s_input = s_light + cut;
        GLM::m_shader[GLM::SHADER_MODEL]->SetFloat(
            s_input.c_str(), cosf(Math::DegToRad(_light->cutOff)));

        s_input = s_light + outcut;
        GLM::m_shader[GLM::SHADER_MODEL]->SetFloat(
            s_input.c_str(), cosf(Math::DegToRad(_light->outerCutOff)));

        s_lightIndex++;
    }
}

void GraphicSystem::TextPipeline(Text * _text)
{
    static Transform* s_pTransform;
    s_pTransform = _text->m_pTransform;

    GLM::m_shader[GLM::SHADER_TEXT]->Use();

    GLM::m_shader[GLM::SHADER_TEXT]->SetMatrix(
        GLM::UNIFORM_TEXT_SCALE, mat4::Scale(s_pTransform->scale));

    GLM::m_shader[GLM::SHADER_TEXT]->SetMatrix(
        GLM::UNIFORM_TEXT_ROTATE, mat4::Rotate(Math::DegToRad(s_pTransform->rotation), s_pTransform->rotationAxis));

    GLM::m_shader[GLM::SHADER_TEXT]->SetBool(
        GLM::UNIFORM_TEXT_BILBOARD, _text->bilboard);

    GLM::m_shader[GLM::SHADER_TEXT]->SetVector4(
        GLM::UNIFORM_TEXT_COLOR, _text->color);

    // Send projection info to shader
    if (_text->projection == PROJECTION_PERSPECTIVE) {
        GLM::m_shader[GLM::SHADER_TEXT]->SetMatrix(
            GLM::UNIFORM_TEXT_PROJECTION, m_perspective);

        m_viewport = mat4::LookAt(
            m_pMainCamera->position, m_pMainCamera->target, m_pMainCamera->up);
    }

    else {
        GLM::m_shader[GLM::SHADER_TEXT]->SetMatrix(
            GLM::UNIFORM_TEXT_PROJECTION, m_orthogonal);

        m_viewport.SetIdentity();
        m_viewport = mat4::Scale(m_resolutionScaler);
    }

    // Send camera info to shader
    GLM::m_shader[GLM::SHADER_TEXT]->SetMatrix(
        GLM::UNIFORM_TEXT_CAMERA, m_viewport);

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
        s_rotation = _emitter->rotationSpeed == 0.f ? false : true;
        s_changeColor = _emitter->m_changeColor;
        s_pTransform = _emitter->m_pTransform;
        s_texture = _emitter->m_mainTex;
        s_velocity = _dt * _emitter->velocity;
        s_colorDiff = _dt * _emitter->colorDiff;

        GLM::m_shader[GLM::SHADER_PARTICLE]->Use();

        GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
            GLM::UNIFORM_PARTICLE_SCALE, mat4::Scale(s_pTransform->scale));

        GLM::m_shader[GLM::SHADER_PARTICLE]->SetBool(
            GLM::UNIFORM_PARTICLE_BILBOARD, _emitter->bilboard);

        // Send projection info to shader
        if (_emitter->projection == PROJECTION_PERSPECTIVE) {
            GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
                GLM::UNIFORM_PARTICLE_PROJECTION, m_perspective);

            m_viewport = mat4::LookAt(
                m_pMainCamera->position, m_pMainCamera->target, m_pMainCamera->up);
        }

        else {
            GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
                GLM::UNIFORM_PARTICLE_PROJECTION, m_orthogonal);

            m_viewport.SetIdentity();
            m_viewport = mat4::Scale(m_resolutionScaler);
        }

        // Send camera info to shader
        GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
            GLM::UNIFORM_PARTICLE_CAMERA, m_viewport);

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
                GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
                    GLM::UNIFORM_PARTICLE_TRANSLATE, mat4::Translate(particle->position));

                GLM::m_shader[GLM::SHADER_PARTICLE]->SetMatrix(
                    GLM::UNIFORM_PARTICLE_ROTATE, mat4::Rotate(Math::DegToRad(particle->rotation), s_pTransform->rotationAxis));

                // Send color info to shader
                GLM::m_shader[GLM::SHADER_PARTICLE]->SetVector4(
                    GLM::UNIFORM_PARTICLE_COLOR, s_color);

                GLM::m_shader[GLM::SHADER_PARTICLE]->SetBool(
                    GLM::UNIFORM_PARTICLE_HIDE, particle->hidden);

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

    GLM::m_shader[GLM::SHADER_TEXT]->SetMatrix(
        GLM::UNIFORM_TEXT_TRANSLATE, mat4::Translate(s_realPosition));

    //Update vbo
    GLfloat vertices[4][8] = {
            { 0.f, height, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f },
            { width, height, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f },
            { 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f },
            { width, 0.f, 0.f, 1.f, 1.f ,0.f, 0.f, 1.f }
    };

    const static auto sizeOfVertices = sizeof(vertices);

    glBindTexture(GL_TEXTURE_2D, _character.texture);
    glBindBuffer(GL_ARRAY_BUFFER, GLM::m_vbo[GLM::SHAPE_TEXT]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfVertices, vertices);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

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

        glBindVertexArray(GLM::m_vao[GLM::SHAPE_TEXT]);

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


jeEnd
