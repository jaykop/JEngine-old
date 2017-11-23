#include "Light.h"
#include "Transform.h"
#include "SystemManager.h"
#include "GraphicSystem.h"

JE_BEGIN

Light::Light(Object * _owner)
	:Component(_owner), m_color(vec4::ONE),
	m_ambient(vec4::ONE), m_diffuse(vec4::ONE),
	m_specular(vec4::ONE), m_position(vec3(0.f, 0.f, 1.f)),
	m_direction(vec3::ZERO), m_constant(0.f), m_linear(0.f),
	m_quadratic(0.f), m_cutOff(0.f), m_outerCutOff(0.f)
{}

void Light::Register()
{
	SystemManager::GetGraphicSystem()->AddLight(this);
}

void Light::Load(CR_RJValue _data)
{
	if (_data.HasMember("Type")) {
		CR_RJValue type = _data["Type"];
		if (!strcmp(type.GetString(), "Normal"))
			m_type = NORMALLIGHT;
		else if (!strcmp(type.GetString(), "Directional"))
			m_type = DIRECTIONALLIGHT;
		else if (!strcmp(type.GetString(), "Spot"))
			m_type = SPOTLIGHT;
		else if (!strcmp(type.GetString(), "Point"))
			m_type = POINTLIGHT;
	}

	if (_data.HasMember("CutOff")) {
		CR_RJValue cutOff = _data["CutOff"];
		m_cutOff = cutOff.GetFloat();
	}

	if (_data.HasMember("OuterCutOff")) {
		CR_RJValue outCutOff = _data["OuterCutOff"];
		m_outerCutOff = outCutOff.GetFloat();
	}

	if (_data.HasMember("Constant")) {
		CR_RJValue constant = _data["Constant"];
		m_constant = constant.GetFloat();
	}

	if (_data.HasMember("Linear")) {
		CR_RJValue linear = _data["Linear"];
		m_linear = linear.GetFloat();
	}

	if (_data.HasMember("Quadratic")) {
		CR_RJValue quadratic = _data["Quadratic"];
		m_quadratic = quadratic.GetFloat();
	}

	if (_data.HasMember("Direction")) {
		CR_RJValue direction = _data["Direction"];
		m_direction.Set(direction[0].GetFloat(), direction[1].GetFloat(), direction[2].GetFloat());
	}

	if (_data.HasMember("Color")) {
		CR_RJValue color = _data["Color"];
		m_color.Set(color[0].GetFloat(), color[1].GetFloat(), color[2].GetFloat(), color[3].GetFloat());
	}

	if (_data.HasMember("Ambient")) {
		CR_RJValue ambient = _data["Ambient"];
		m_ambient.Set(ambient[0].GetFloat(), ambient[1].GetFloat(), ambient[2].GetFloat(), ambient[3].GetFloat());
	}
	
	if (_data.HasMember("Diffuse")) {
		CR_RJValue diffuse = _data["Diffuse"];
		m_diffuse.Set(diffuse[0].GetFloat(), diffuse[1].GetFloat(), diffuse[2].GetFloat(), diffuse[3].GetFloat());
	}
	
	if (_data.HasMember("Specular")) {
		CR_RJValue specular = _data["Specular"];
		m_specular.Set(specular[0].GetFloat(), specular[1].GetFloat(), specular[2].GetFloat(), specular[3].GetFloat());
	}
	
	if (_data.HasMember("Position")) {
		CR_RJValue position = _data["Position"];
		m_position.Set(position[0].GetFloat(), position[1].GetFloat(), position[2].GetFloat());
	}
}


LightBuilder::LightBuilder()
	:ComponentBuilder()
{}

Component* LightBuilder::CreateComponent(Object* _pOwner) const
{
	return new Light(_pOwner);
}

JE_END
