#include <algorithm>
#include "GLManager.h"
#include "GraphicSystem.h"
#include "Sprite.h"
#include "Light.h"
#include "Camera.h"
#include "Application.h"
#include "Transform.h"
#include "InputHandler.h"

JE_BEGIN

GraphicSystem::GraphicSystem()
	:System(), m_pMainCamera(nullptr),
	m_fovy(45.f), m_zNear(.1f), m_zFar(1000.f), m_isLight(false),
	m_backgroundColor(vec4::ZERO), m_orthoFirst(false),
	m_width(Application::GetData().m_width), m_height(Application::GetData().m_height),
	m_aniScale(vec3::ZERO), m_aniTranslate(vec3::ZERO), m_viewport(mat4())
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

void GraphicSystem::Update(const float _dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, m_backgroundColor.w);

	// Sort sprites by sprite's z position
	std::sort(m_sprites.begin(), m_sprites.end(), compareOrder(m_orthoFirst));

	LightSourcePipeline();
	NormalPipeline(_dt);
	
	// TODO
	// GLMousePosition();
}

void GraphicSystem::Close()
{

}

void GraphicSystem::Unload()
{
	m_lights.clear();
	m_sprites.clear();
	m_cameras.clear();
}

void GraphicSystem::Render(const unsigned _vao, const unsigned _vbo, const unsigned _ebo,
	const float _vertices[], const unsigned _indices[], 
	const int _verticesSize, const int _indicesSize, const int _elementSize)
{
	// Send transform info to shader
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _verticesSize, _vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// text coordinate position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// normals of vertices
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indicesSize, _indices, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, _elementSize, GL_UNSIGNED_INT, 0);
}

void GraphicSystem::RenderParticle(const int _particleSize, float *_positionData, float *_colorData)
{
	static unsigned s_maxSize = 1000;// _emitter->m_maxSize;
	 
	glEnable(GL_BLEND);					// Enable blend 
	glDepthMask(GL_FALSE);				// Ignore depth buffer writing
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glBindBuffer(GL_ARRAY_BUFFER, GLM::m_particlePosition);
	//glBufferData(GL_ARRAY_BUFFER, s_maxSize * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	//glBufferSubData(GL_ARRAY_BUFFER, 0, _particleSize * sizeof(GLfloat) * 3, _positionData);

	//glBindBuffer(GL_ARRAY_BUFFER, GLM::m_particleColor);
	//glBufferData(GL_ARRAY_BUFFER, s_maxSize * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	//glBufferSubData(GL_ARRAY_BUFFER, 0, _particleSize * sizeof(GLubyte) * 4, _colorData);

	//// 1rst attribute buffer : vertices
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, GLM::m_particleVbo); 
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	//// 2nd attribute buffer : positions of particles' centers
	//glEnableVertexAttribArray(2);
	//glBindBuffer(GL_ARRAY_BUFFER, GLM::m_particlePosition);
	//glVertexAttribPointer(
	//	2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
	//	3,                                // size : x + y + z + size => 4
	//	GL_FLOAT,                         // type
	//	GL_FALSE,                         // normalized?
	//	0,                                // stride
	//	(void*)0                          // array buffer offset
	//);

	//// 3rd attribute buffer : particles' colors
	//glEnableVertexAttribArray(3);
	//glBindBuffer(GL_ARRAY_BUFFER, GLM::m_particleColor);
	//glVertexAttribPointer(
	//	3,                                // attribute. No particular reason for 1, but must match the layout in the shader.
	//	4,                                // size : r + g + b + a => 4
	//	GL_UNSIGNED_BYTE,                 // type
	//	GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
	//	0,                                // stride
	//	(void*)0                          // array buffer offset
	//);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 4, _particleSize);

	glDepthMask(GL_TRUE);	// Enable depth buffer writing
	glDisable(GL_BLEND);	// Disable blend
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

void GraphicSystem::SetBackgroundColor(float _r, float _g, float _b, float _a)
{
	m_backgroundColor.Set(_r, _g, _b, _a);
}

void GraphicSystem::SetBackgroundColor(CR_vec4 _color)
{
	m_backgroundColor = _color;
}

const vec4& GraphicSystem::GetBackgroundColor() const
{
	return m_backgroundColor;
}

int GraphicSystem::GetWidth() const
{
	return m_width;
}

int	GraphicSystem::GetHeight() const
{
	return m_height;
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
	if (m_cameras.size() == 1)
		m_pMainCamera = _camera;
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

		if (_leftSpt->m_projection == PERSPECTIVE
			&& _rightSpt->m_projection == ORTHOGONAL)
			return false;

		else if (_leftSpt->m_projection == ORTHOGONAL
			&& _rightSpt->m_projection == PERSPECTIVE)
			return true;

		else
			return left->m_position.z < right->m_position.z;
	}

	else
		return left->m_position.z < right->m_position.z;
}

void GraphicSystem::GLMousePosition() {

	// Do unprojection by viewport and proejction matrix
	static vec4 in, orthoPos, perspPos;
	static mat4 ortho, perspective;
	static float orthoOffset, perspOffset;

	in.x = (2.f * (InputHandler::m_rawPosition.x / m_width)) - 1.f;
	in.y = 1.f - (2.f* (InputHandler::m_rawPosition.y / m_height));
	in.w = in.z = 1.f;
	
	ortho = m_orthogonal * m_viewport;
	ortho.Inverse();
	orthoPos = ortho * in;
	orthoOffset = 1.f / orthoPos.w;
	InputHandler::m_orthoPosition.Set(orthoPos.x, orthoPos.y, orthoPos.z);
	InputHandler::m_orthoPosition *= orthoOffset;

	perspective = m_perspective * m_viewport;
	perspective.Inverse();
	perspPos = perspective * in;
	perspOffset = 1.f / perspPos.w;
	InputHandler::m_perspPosition.Set(perspPos.x, perspPos.y, perspPos.z);
	InputHandler::m_perspPosition *= perspOffset;

}

JE_END

