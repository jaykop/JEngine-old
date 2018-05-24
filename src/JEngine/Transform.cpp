#include "imgui.h"
#include "Transform.h"
#include "MemoryAllocator.h"

jeBegin
jeDefineComponentBuilder(Transform);

Transform::Transform(Object* _pOwner)
	:Component(_pOwner), 
	m_position(vec3::ZERO), m_scale(vec3::ZERO), 
	m_rotation(0.f), m_rotationAxis(vec3::UNIT_Z)
{}

void Transform::operator= (const Transform& _copy)
{
	m_position.Set(_copy.m_position); 
	m_scale.Set(_copy.m_scale);
	m_rotation = _copy.m_rotation; 
	m_rotationAxis.Set(_copy.m_rotationAxis);
}

void Transform::Load(CR_RJValue _data)
{
	if (_data.HasMember("Position")) {
		CR_RJValue position = _data["Position"];
		m_position.Set(position[0].GetFloat(), position[1].GetFloat(), position[2].GetFloat());
	}

	if (_data.HasMember("Scale")) {
		CR_RJValue scale = _data["Scale"];
		m_scale.Set(scale[0].GetFloat(), scale[1].GetFloat(), scale[2].GetFloat());
	}

	if (_data.HasMember("Rotation")) {
		CR_RJValue rotation = _data["Rotation"];
		m_rotation = rotation.GetFloat();
	}

	if (_data.HasMember("Axis")) {
		CR_RJValue rotation3D = _data["Axis"];
		m_rotationAxis.Set(rotation3D[0].GetFloat(), rotation3D[1].GetFloat(), rotation3D[2].GetFloat());
	}
}

void Transform::EditorUpdate(const float /*_dt*/)
{
	if (m_showEditor) {
		static float s_position[3] = { m_position.x, m_position.y, m_position.z }, 
			s_scale[3] = { m_scale.x, m_scale.y, m_scale.z },
			s_axis[3] = { m_rotationAxis.x, m_rotationAxis.y, m_rotationAxis.z },
			s_rotation = m_rotation;
		
		ImGui::Begin("Transform");
		ImGui::InputFloat3("Position", s_position);
		ImGui::InputFloat3("Scale", s_scale);
		ImGui::InputFloat3("Axis", s_axis);
		ImGui::InputFloat("Rotation", &s_rotation);
		
		if (ImGui::Button("Close"))
			m_showEditor = false;

		ImGui::End();

		m_position.Set(s_position[0], s_position[1], s_position[2]);
		m_scale.Set(s_scale[0], s_scale[1], s_scale[2]);
		m_rotationAxis.Set(s_axis[0], s_axis[1], s_axis[2]);
		m_rotation = s_rotation;

	}
}

jeEnd
