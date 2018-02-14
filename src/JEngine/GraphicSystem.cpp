#include <algorithm>
#include "GLManager.h"
#include "GraphicSystem.h"
#include "Application.h"
#include "Transform.h"
#include "InputHandler.h"
#include "Sprite.h"
#include "Light.h"
#include "Camera.h"

JE_BEGIN

GraphicSystem::GraphicSystem()
	:System(), m_pMainCamera(nullptr),
	m_fovy(45.f), m_zNear(.1f), m_zFar(1000.f), m_isLight(false),
	m_backgroundColor(vec4::ZERO), m_orthoComesFirst(true), m_screenColor(vec4::ONE),
	m_width(int(GLM::m_width)), m_height(int(GLM::m_height)), m_lightScale(vec3(10, 10, 10)),
	m_aniScale(vec3::ZERO), m_aniTranslate(vec3::ZERO), m_viewport(mat4()),
	m_sobelAmount(0.f), m_blurSize(0.f), m_blurAmount(0.f), m_maxLights(16),
	m_aliasMode(ALIAS_ALIASED), m_screenEffect(EFFECT_NONE), m_mouseZ(0.f)
{
	m_aspect = GLM::m_width / GLM::m_height;
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

	if (_data.HasMember("Screen")) {
		CR_RJValue color = _data["Screen"];
		m_screenColor.Set(
			color[0].GetFloat(),
			color[1].GetFloat(),
			color[2].GetFloat(),
			color[3].GetFloat()
		);
	}

	if (_data.HasMember("Effect")) {
		CR_RJValue effect = _data["Effect"];
		CR_RJValue type = effect["Type"];

		if (!strcmp("None", type.GetString())) 
			m_screenEffect = EFFECT_NONE;

		else if (!strcmp("Inverse", type.GetString())) 
			m_screenEffect = EFFECT_INVERSE;
		
		else if (!strcmp("Sobel", type.GetString())) {
			static float s_recommend = 0.005f;
			m_screenEffect = EFFECT_SOBEL;
			if (effect.HasMember("SobelAmount")) {
				m_sobelAmount = effect["SobelAmount"].GetFloat();
				if (m_sobelAmount > s_recommend)
					JE_DEBUG_PRINT("!GraphicSystem - Recommend to set sobel amount less than %f.\n", s_recommend);
			}
		}
		else if (!strcmp("Blur", type.GetString())) {
			m_screenEffect = EFFECT_BLUR;
			if (effect.HasMember("BlurAmount"))
				m_blurAmount = effect["BlurAmount"].GetFloat();
			if (effect.HasMember("BlurSize"))
				m_blurSize= effect["BlurSize"].GetFloat();
		}
		else
			JE_DEBUG_PRINT("!GraphicSystem - Wrong type of screen effect.\n");
	}
}

void GraphicSystem::Init()
{
	// If there is no preset camera by user,
	// set the first camera as a main camera.
	if (!m_pMainCamera) 
		m_pMainCamera = m_cameras[0];

	m_mouseZ = m_pMainCamera->m_position.z;

	for (auto light : m_lights)
		light->m_direction.Normalize();
}

void GraphicSystem::Update(const float _dt)
{
	RenderToFramebuffer();

	// Sort orthogonal objects and perspective objects
	SortSprites();
	UpdatePipelines(_dt);

	RenderToScreen();

	// TODO
	GLMousePosition();
	Ray();

	// Deferred rendering tutorial test
	/*render1();
	glFlush();
	render2();*/
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

void GraphicSystem::SortSprites()
{
	//TODO
	// For now let's just put only orthogonal sprites to come first
	// Sort sprites by sprite's z position
	if (m_orthoComesFirst) {
		std::sort(m_sprites.begin(), m_sprites.end(),
			[&](Sprite* _leftSpt, Sprite* _rightSpt) -> bool {

			//Transform* left = _leftSpt->m_transform;
			//Transform* right = _rightSpt->m_transform;

			//if (m_orthoComesFirst) {

			if (_leftSpt->m_projection == PROJECTION_PERSPECTIVE
				&& _rightSpt->m_projection == PROJECTION_ORTHOGONAL)
				return true;

			else if (_leftSpt->m_projection == PROJECTION_ORTHOGONAL
				&& _rightSpt->m_projection == PROJECTION_PERSPECTIVE)
				return false;

			// TODO
			// If two sprites are same projection, no change
			// THIS IS TEMP CODES
			else
				return false;

			//	else
			//		return left->m_position.z > right->m_position.z;
			//}

			//else
			//	return left->m_position.z > right->m_position.z;
		}
		);
	}
}

void GraphicSystem::AddSprite(Sprite* _sprite)
{
	m_sprites.push_back(_sprite);
}

void GraphicSystem::RemoveSprite(Sprite* _sprite)
{
	for (Sprites::iterator it = m_sprites.begin();
		it != m_sprites.end(); ++it) {
		if ((*it)->m_pOwnerId == _sprite->m_pOwnerId) {
			m_sprites.erase(it);
			break;
		}
	}
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
		if ((*it)->m_pOwnerId == _camera->m_pOwnerId) {
			m_cameras.erase(it);
			break;
		}
	}
}

void GraphicSystem::AddLight(Light * _light)
{
	if (m_lights.size() < m_maxLights)
		m_lights.push_back(_light);

	else
		JE_DEBUG_PRINT("!GraphicSystem: JEngine cannot support the number of lights more than %d.", m_maxLights);
}

void GraphicSystem::RemoveLight(Light * _light)
{
	for (Lights::iterator it = m_lights.begin();
		it != m_lights.end(); ++it) {
		if ((*it)->m_pOwnerId == _light->m_pOwnerId) {
			m_lights.erase(it);
			break;
		}
	}
}

void GraphicSystem::StartAntialiasing()
{	
	//Start alias mode
	switch (m_aliasMode)
	{
	case ALIAS_ALIASED:
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
		glDisable(GL_MULTISAMPLE);
		break;

	case ALIAS_ANTIALIASED:
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glDisable(GL_MULTISAMPLE);
		break;

	case ALIAS_MULTISAMPLE:
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
		glEnable(GL_MULTISAMPLE);
		break;
	}
}

void GraphicSystem::EndAntialiasing()
{	
	//End alias mode
	switch (m_aliasMode)
	{
	case ALIAS_ANTIALIASED:
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
		break;

	case ALIAS_MULTISAMPLE:
		glDisable(GL_MULTISAMPLE);
		break;
	}
}

void GraphicSystem::GLMousePosition() {

	if (INPUT::KeyPressed(JE_MOUSE_WHEEL_UP)) {
		m_mouseZ++;
		JE_DEBUG_PRINT("*GraphicSystem: Mouse screen position - [ %f, %f, %f ]\n", INPUT::m_screenPosition.x, INPUT::m_screenPosition.y, m_mouseZ);
	}
	else if (INPUT::KeyPressed(JE_MOUSE_WHEEL_DOWN)) {
		m_mouseZ--;
		JE_DEBUG_PRINT("*GraphicSystem: Mouse screen position - [ %f, %f, %f ]\n", INPUT::m_screenPosition.x, INPUT::m_screenPosition.y, m_mouseZ);
	}
	
	// Set mouse;s screen position 
	static float width = float(m_width)* .5f, height = float(m_height)* .5f;
	INPUT::m_screenPosition.Set(INPUT::m_rawPosition.x - width, height - INPUT::m_rawPosition.y, m_mouseZ);

}


void GraphicSystem::Ray()
{
	vec3 a = INPUT::m_screenPosition;
	//std::cout << a << std::endl;
	float x = (2.0f * INPUT::m_screenPosition.x) / m_width - 1.0f;
	float y = 1.0f - (2.0f * INPUT::m_screenPosition.y) / m_height;
	float z = 1.0f;
	vec3 ray_nds = vec3(x, y, z);
	vec4 ray_clip = vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	vec4 ray_eye = m_perspective.GetInverse().Transpose() * ray_clip;
	ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	vec4 ray4_wor = m_viewport.GetInverse().Transpose() * ray_eye;
	vec3 ray_wor(ray4_wor.x, ray4_wor.y, ray4_wor.z);
	// don't forget to normalise the vector at some point
	ray_wor = ray_wor.GetNormalize();
}

JE_END
