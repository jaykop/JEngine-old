#include <algorithm>
#include "GLManager.h"
#include "GraphicSystem.h"
#include "Application.h"
#include "Transform.h"
#include "Sprite.h"
#include "Light.h"
#include "Camera.h"
#include "Object.h"
#include "MathUtils.h"

jeBegin

using namespace Math;

GraphicSystem::GraphicSystem()
	:System(), m_pMainCamera(nullptr),
	zNear(.1f), zFar(1000.f), m_isLight(false), backgroundColor(vec4::ZERO),
	orthoComesFirst(true), screenColor(vec4::ONE), m_width(int(GLM::m_width)), m_mouseZ(0.f),
	m_height(int(GLM::m_height)), lightScale(vec3(10, 10, 10)), m_aniScale(vec3::ZERO),
	m_aniTranslate(vec3::ZERO), m_viewport(mat4()), sobelAmount(0.f), blurSize(0.f),
	blurAmount(0.f), m_maxLights(16), aliasMode(ALIAS_ALIASED), screenEffect(EFFECT_NONE),
	m_resolutionScaler(GLM::m_width, GLM::m_height, 1.f)
{
	aspect = float(GLM::m_width / GLM::m_height);
	m_right = m_width * .5f;
	m_left = -m_right;
	m_top = m_height * .5f;
	m_bottom = -m_top;

	// Fix the orthogonal matrix
	// because users are not allow to change app size while it is running 
	m_orthogonal = Orthogonal(m_left, m_right, m_bottom, m_top, zNear, zFar);
}

void GraphicSystem::Load(CR_RJDoc _data)
{
	if (_data.HasMember("Background")) {
		CR_RJValue color = _data["Background"];
		backgroundColor.Set(
			color[0].GetFloat(),
			color[1].GetFloat(),
			color[2].GetFloat(),
			color[3].GetFloat()
		);
	}

	if (_data.HasMember("Screen")) {
		CR_RJValue color = _data["Screen"];
		screenColor.Set(
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
			screenEffect = EFFECT_NONE;

		else if (!strcmp("Inverse", type.GetString()))
			screenEffect = EFFECT_INVERSE;

		else if (!strcmp("Sobel", type.GetString())) {
			static float s_recommend = 0.005f;
			screenEffect = EFFECT_SOBEL;
			if (effect.HasMember("SobelAmount")) {
				sobelAmount = effect["SobelAmount"].GetFloat();
				if (sobelAmount > s_recommend)
					jeDebugPrint("!GraphicSystem - Recommend to set sobel amount less than %f.\n", s_recommend);
			}
		}
		else if (!strcmp("Blur", type.GetString())) {
			screenEffect = EFFECT_BLUR;
			if (effect.HasMember("BlurAmount"))
				blurAmount = effect["BlurAmount"].GetFloat();
			if (effect.HasMember("BlurSize"))
				blurSize = effect["BlurSize"].GetFloat();
		}
		else
			jeDebugPrint("!GraphicSystem - Wrong type of screen effect.\n");
	}
}

void GraphicSystem::Init()
{
	// If there is no preset camera by user,
	// set the first camera as a main camera.
	if (!m_pMainCamera)
		m_pMainCamera = m_cameras[0];

	m_mouseZ = m_pMainCamera->position.z;

	for (auto light : m_lights)
		Normalize(light->direction);
}

void GraphicSystem::Update(const float _dt)
{
	RenderToFramebuffer();
	glPolygonMode(GL_FRONT_AND_BACK, GLM::m_drawMode);

	// Sort orthogonal objects and perspective objects
	SortSprites();
	UpdatePipelines(_dt);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	RenderToScreen();
	//UpdateMousePosition();
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
	if (orthoComesFirst) {
		std::sort(m_sprites.begin(), m_sprites.end(),
			[&](Sprite* _leftSpt, Sprite* _rightSpt) -> bool {

			if (_leftSpt->projection == PROJECTION_PERSPECTIVE
				&& _rightSpt->projection == PROJECTION_ORTHOGONAL)
				return true;

			else if (_leftSpt->projection == PROJECTION_ORTHOGONAL
				&& _rightSpt->projection == PROJECTION_PERSPECTIVE)
				return false;

			else
				return false;
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
		if ((*it)->GetOwner()->GetId() == _sprite->GetOwner()->GetId()) {
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

Camera* GraphicSystem::GetMainCamera() const
{
	return m_pMainCamera;
}

void GraphicSystem::AddCamera(Camera* _camera)
{
	m_cameras.push_back(_camera);
	if (m_cameras.size())
		m_pMainCamera = m_cameras[0];
}

void GraphicSystem::RemoveCamera(Camera* _camera)
{
	for (Cameras::iterator it = m_cameras.begin();
		it != m_cameras.end(); ++it) {
		if ((*it)->GetOwner()->GetId() == _camera->GetOwner()->GetId()) {
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
		jeDebugPrint("!GraphicSystem: JEngine cannot support the number of lights more than %d.", m_maxLights);
}

void GraphicSystem::RemoveLight(Light * _light)
{
	for (Lights::iterator it = m_lights.begin();
		it != m_lights.end(); ++it) {
		if ((*it)->GetOwner()->GetId() == _light->GetOwner()->GetId()) {
			m_lights.erase(it);
			break;
		}
	}
}

void GraphicSystem::StartAntialiasing()
{
	//Start alias mode
	switch (aliasMode)
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
	switch (aliasMode)
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

//void GraphicSystem::UpdateMousePosition() {
//
//	if (INPUT::KeyPressed(JE_MOUSE_WHEEL_UP)) {
//		m_mouseZ++;
//		//jeDebugPrint("*GraphicSystem: Mouse screen position - [ %f, %f, %f ]\n", INPUT::m_screenPosition.x, INPUT::m_screenPosition.y, m_mouseZ);
//	}
//	else if (INPUT::KeyPressed(JE_MOUSE_WHEEL_DOWN)) {
//		m_mouseZ--;
//		//jeDebugPrint("*GraphicSystem: Mouse screen position - [ %f, %f, %f ]\n", INPUT::m_screenPosition.x, INPUT::m_screenPosition.y, m_mouseZ);
//	}
//
//	// Set mouse;s screen position 
//
//
//}


void GraphicSystem::Ray(Sprite* /*_sprite*/, Transform* /*_transform*/)
{
	//if (_sprite->GetOwnerId() != _transform->GetOwnerId())
	//	jeDebugPrint("!The owners of sprite and transform are not identical.\n");

	//static mat4 s_translate, s_scale, s_rotation,
	//	s_viewport, s_projection;
	//static vec4 s_final, s_position4;
	//static vec3 s_position3;

	//s_position3 = _transform->position;
	//s_position4.Set(s_position3.x, s_position3.y, s_position3.z, 1.f);
	//s_translate = mat4::Translate(s_position3);
	//s_scale = mat4::Scale(_transform->scale);
	//s_rotation = mat4::Rotate(_transform->rotation, _transform->rotationAxis);
	//
	//if (_sprite->projection == PROJECTION_PERSPECTIVE) {
	//	s_projection = m_perspective;
	//	s_viewport = mat4::LookAt(
	//		m_pMainCamera->position, m_pMainCamera->m_target, m_pMainCamera->m_up);
	//}

	//else {	// PROJECTION_ORTHOGONAL
	//	s_projection = m_orthogonal;
	//	s_viewport.SetIdentity();
	//	s_viewport = mat4::Scale(m_resolutionScaler);
	//}

	//s_final = s_projection * s_viewport * (s_scale * s_rotation * s_translate) * s_position4;

	//// TODO...
	//// http://goguri.tistory.com/entry/3D-%ED%94%BC%ED%82%B9
}

jeEnd
