#include <algorithm>
#include "GLManager.h"
#include "GraphicSystem.h"
#include "Application.h"
#include "Transform.h"
#include "Model.h"
#include "Light.h"
#include "Camera.h"
#include "Object.h"
#include "MathUtils.h"

jeBegin

using namespace Math;

GraphicSystem::GraphicSystem()
	:System(), pMainCamera_(nullptr), isLight_(false), backgroundColor_(vec4::ZERO),
	orthoComesFirst_(true), screenColor_(vec4::ONE), width_(int(GLM::width_)),
	height_(int(GLM::height_)), aniScale_(vec3::ZERO), aniTranslate_(vec3::ZERO),
	viewport_(mat4()), sobelAmount_(0.f), blurSize_(0.f), blurAmount_(0.f), maxLights_(16), 
	aliasMode_(JE_ALIAS_ALIASED), screenEffect_(JE_EFFECT_NONE), resolutionScaler_(GLM::width_, GLM::height_, 1.f)
{}

void GraphicSystem::Load(CR_RJDoc data)
{
	if (data.HasMember("Background")) {
		CR_RJValue color = data["Background"];
		backgroundColor_.Set(
			color[0].GetFloat(),
			color[1].GetFloat(),
			color[2].GetFloat(),
			color[3].GetFloat()
		);
	}

	if (data.HasMember("Screen")) {
		CR_RJValue color = data["Screen"];
		screenColor_.Set(
			color[0].GetFloat(),
			color[1].GetFloat(),
			color[2].GetFloat(),
			color[3].GetFloat()
		);
	}

	if (data.HasMember("Effect")) {
		CR_RJValue effect = data["Effect"];
		CR_RJValue type = effect["Type"];

		if (!strcmp("None", type.GetString()))
			screenEffect_ = JE_EFFECT_NONE;

		else if (!strcmp("Inverse", type.GetString()))
			screenEffect_ = JE_EFFECT_INVERSE;

		else if (!strcmp("Sobel", type.GetString())) {
			static float s_recommend = 0.005f;
			screenEffect_ = JE_EFFECT_SOBEL;
			if (effect.HasMember("SobelAmount")) {
				sobelAmount_ = effect["SobelAmount"].GetFloat();
				if (sobelAmount_ > s_recommend)
					jeDebugPrint("!GraphicSystem - Recommend to set sobel amount less than %f.\n", s_recommend);
			}
		}
		else if (!strcmp("Blur", type.GetString())) {
			screenEffect_ = JE_EFFECT_BLUR;
			if (effect.HasMember("BlurAmount"))
				blurAmount_ = effect["BlurAmount"].GetFloat();
			if (effect.HasMember("BlurSize"))
				blurSize_ = effect["BlurSize"].GetFloat();
		}
		else
			jeDebugPrint("!GraphicSystem - Wrong type of screen effect.\n");
	}
}

void GraphicSystem::Init()
{
	// If there is no preset camera by user,
	// set the first camera as a main camera.
	if (!pMainCamera_)
		pMainCamera_ = cameras_[0];

	for (auto light : lights_)
		Normalize(light->direction_);
}

void GraphicSystem::Update(float dt)
{
	RenderToFramebuffer();
	UpdatePipelines(dt);
	RenderToScreen();
}

void GraphicSystem::Close()
{

}

void GraphicSystem::Unload()
{
	lights_.clear();
	models_.clear();
	cameras_.clear();
}

void GraphicSystem::SortModels()
{
	if (orthoComesFirst_) {
		std::sort(models_.begin(), models_.end(),
			[&](Model* leftSpt, Model* rightSpt) -> bool {

			if (leftSpt->projection_ == PROJECTION_PERSPECTIVE
				&& rightSpt->projection_ == PROJECTION_ORTHOGONAL)
				return true;

			else if (leftSpt->projection_ == PROJECTION_ORTHOGONAL
				&& rightSpt->projection_ == PROJECTION_PERSPECTIVE)
				return false;

			else
				return false;
		}
		);
	}
}

void GraphicSystem::AddModel(Model* pModel)
{
	models_.push_back(pModel);
}

void GraphicSystem::RemoveModel(Model* pModel)
{
	for (Models::iterator it = models_.begin();
		it != models_.end(); ++it) {
		if ((*it)->GetOwner()->GetId() == pModel->GetOwner()->GetId()) {
			models_.erase(it);
			break;
		}
	}
}

int GraphicSystem::GetWidth() const
{
	return width_;
}

int	GraphicSystem::GetHeight() const
{
	return height_;
}

void GraphicSystem::SetMainCamera(Camera* pCamera)
{
	pMainCamera_ = pCamera;
}

Camera* GraphicSystem::GetMainCamera() const
{
	return pMainCamera_;
}

void GraphicSystem::AddCamera(Camera* pCamera)
{
	cameras_.push_back(pCamera);
	if (cameras_.size())
		pMainCamera_ = cameras_[0];
}

void GraphicSystem::RemoveCamera(Camera* pCamera)
{
	for (Cameras::iterator it = cameras_.begin();
		it != cameras_.end(); ++it) {
		if ((*it)->GetOwner()->GetId() == pCamera->GetOwner()->GetId()) {
			cameras_.erase(it);
			break;
		}
	}
}

void GraphicSystem::AddLight(Light * pLight)
{
	if (lights_.size() < maxLights_)
		lights_.push_back(pLight);

	else
		jeDebugPrint("!GraphicSystem: JEngine cannot support the number of lights more than %d.", maxLights_);
}

void GraphicSystem::RemoveLight(Light * pLight)
{
	for (Lights::iterator it = lights_.begin();
		it != lights_.end(); ++it) {
		if ((*it)->GetOwner()->GetId() == pLight->GetOwner()->GetId()) {
			lights_.erase(it);
			break;
		}
	}
}

void GraphicSystem::StartAntialiasing()
{
	//Start alias mode
	switch (aliasMode_)
	{
	case JE_ALIAS_ALIASED:
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
		glDisable(GL_MULTISAMPLE);
		break;

	case JE_ALIAS_ANTIALIASED:
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glDisable(GL_MULTISAMPLE);
		break;

	case JE_ALIAS_MULTISAMPLE:
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
	switch (aliasMode_)
	{
	case JE_ALIAS_ANTIALIASED:
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
		break;

	case JE_ALIAS_MULTISAMPLE:
		glDisable(GL_MULTISAMPLE);
		break;
	}
}

void GraphicSystem::Ray(Model* /*pModel*/, Transform* /*pTransform*/)
{
	//if (pModel->GetOwnerId() != pTransform->GetOwnerId())
	//	jeDebugPrint("!The owners of model and transform are not identical.\n");

	//static mat4 s_translate, s_scale, s_rotation,
	//	s_viewport, s_projection_;
	//static vec4 s_final, s_position4;
	//static vec3 s_position3;

	//s_position3 = pTransform->position_;
	//s_position4.Set(s_position3.x, s_position3.y, s_position3.z, 1.f);
	//s_translate = mat4::Translate(s_position3);
	//s_scale = mat4::Scale(pTransform->scale_);
	//s_rotation = mat4::Rotate(pTransform->rotation_, pTransform->rotationAxis_);
	//
	//if (pModel->projection_ == PROJECTION_PERSPECTIVE) {
	//	s_projection_ = perspective_;
	//	s_viewport = mat4::LookAt(
	//		pMainCamera_->position, pMainCamera_->m_target, pMainCamera_->up_);
	//}

	//else {	// PROJECTION_ORTHOGONAL
	//	s_projection_ = orthogonal_;
	//	s_viewport.SetIdentity();
	//	s_viewport = mat4::Scale(resolutionScaler_);
	//}

	//s_final = s_projection_ * s_viewport * (s_scale * s_rotation * s_translate) * s_position4;

	//// TODO...
	//// http://goguri.tistory.com/entry/3D-%ED%94%BC%ED%82%B9
}

jeEnd
