#include "imgui.h"
#include "Transform.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Transform);

Transform::Transform(Object* pOwner)
	:Component(pOwner), 
	position(vec3::ZERO), scale(vec3::ZERO), 
	rotation(0.f), rotationAxis(vec3::UNIT_Z)
{}

void Transform::operator= (const Transform& copy)
{
	position.Set(copy.position); 
	scale.Set(copy.scale);
	rotation = copy.rotation; 
	rotationAxis.Set(copy.rotationAxis);
}

void Transform::Load(CR_RJValue data)
{
	if (data.HasMember("Position")) {
		CR_RJValue loadedPosition = data["Position"];
		position.Set(loadedPosition[0].GetFloat(), loadedPosition[1].GetFloat(), loadedPosition[2].GetFloat());
	}

	if (data.HasMember("Scale")) {
		CR_RJValue loadedScale = data["Scale"];
		scale.Set(loadedScale[0].GetFloat(), loadedScale[1].GetFloat(), loadedScale[2].GetFloat());
	}

	if (data.HasMember("Rotation")) {
		CR_RJValue loadedRotation = data["Rotation"];
		rotation = loadedRotation.GetFloat();
	}

	if (data.HasMember("Axis")) {
		CR_RJValue loadedRotation3D = data["Axis"];
		rotationAxis.Set(loadedRotation3D[0].GetFloat(), loadedRotation3D[1].GetFloat(), loadedRotation3D[2].GetFloat());
	}
}

void Transform::EditorUpdate(const float /*dt*/)
{
	if (showEditor_) {
		static float s_position[3] = { position.x, position.y, position.z }, 
			s_scale[3] = { scale.x, scale.y, scale.z },
			s_axis[3] = { rotationAxis.x, rotationAxis.y, rotationAxis.z },
			s_rotation = rotation;
		
		ImGui::Begin("Transform");
		ImGui::InputFloat3("Position", s_position);
		ImGui::InputFloat3("Scale", s_scale);
		ImGui::InputFloat3("Axis", s_axis);
		ImGui::InputFloat("Rotation", &s_rotation);
		
		if (ImGui::Button("Close"))
			showEditor_ = false;

		ImGui::End();

		position.Set(s_position[0], s_position[1], s_position[2]);
		scale.Set(s_scale[0], s_scale[1], s_scale[2]);
		rotationAxis.Set(s_axis[0], s_axis[1], s_axis[2]);
		rotation = s_rotation;

	}
}

jeEnd
