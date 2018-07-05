#include "imgui.h"
#include "Transform.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Transform);

Transform::Transform(Object* pOwner)
	:Component(pOwner), 
	position_(vec3::ZERO), scale_(vec3::ZERO),
	rotation_(0.f), rotationAxis_(vec3::UNIT_Z)
{}

void Transform::operator= (const Transform& copy)
{
	position_.Set(copy.position_);
	scale_.Set(copy.scale_);
	rotation_ = copy.rotation_; 
	rotationAxis_.Set(copy.rotationAxis_);
}

void Transform::Load(CR_RJValue data)
{
	if (data.HasMember("Position")) {
		CR_RJValue loadedPosition = data["Position"];
		position_.Set(loadedPosition[0].GetFloat(), loadedPosition[1].GetFloat(), loadedPosition[2].GetFloat());
	}

	if (data.HasMember("Scale")) {
		CR_RJValue loadedScale = data["Scale"];
		scale_.Set(loadedScale[0].GetFloat(), loadedScale[1].GetFloat(), loadedScale[2].GetFloat());
	}

	if (data.HasMember("Rotation")) {
		CR_RJValue loadedRotation = data["Rotation"];
		rotation_ = loadedRotation.GetFloat();
	}

	if (data.HasMember("Axis")) {
		CR_RJValue loadedRotation3D = data["Axis"];
		rotationAxis_.Set(loadedRotation3D[0].GetFloat(), loadedRotation3D[1].GetFloat(), loadedRotation3D[2].GetFloat());
	}
}

void Transform::EditorUpdate(const float /*dt*/)
{
	if (showEditor_) {
		static float s_position[3] = { position_.x, position_.y, position_.z },
			s_scale[3] = { scale_.x, scale_.y, scale_.z },
			s_axis[3] = { rotationAxis_.x, rotationAxis_.y, rotationAxis_.z },
			s_rotation = rotation_;
		
		ImGui::Begin("Transform");
		ImGui::InputFloat3("Position", s_position);
		ImGui::InputFloat3("Scale", s_scale);
		ImGui::InputFloat3("Axis", s_axis);
		ImGui::InputFloat("Rotation", &s_rotation);
		
		if (ImGui::Button("Close"))
			showEditor_ = false;

		ImGui::End();

		position_.Set(s_position[0], s_position[1], s_position[2]);
		scale_.Set(s_scale[0], s_scale[1], s_scale[2]);
		rotationAxis_.Set(s_axis[0], s_axis[1], s_axis[2]);
		rotation_ = s_rotation;

	}
}

jeEnd
