#include "Light.h"
#include "Transform.h"
#include "SystemManager.h"
#include "GraphicSystem.h"

JE_BEGIN

Light::Light(Object * _owner)
	:Component(_owner), m_color(vec4::ONE),
	m_ambient(vec4::ONE), m_diffuse(vec4::ONE),
	m_specular(vec4::ONE), m_position(vec3(0.f, 0.f, 1.f))
{}

void Light::Register()
{
	SystemManager::GetGraphicSystem()->AddLight(this);
}

void Light::Load(CR_RJValue _data)
{
	CR_RJValue color = _data["Color"];
	CR_RJValue ambient = _data["Ambient"];
	CR_RJValue diffuse = _data["Diffuse"];
	CR_RJValue specular = _data["Specular"];
	CR_RJValue position = _data["Position"];

	m_position.Set(position[0].GetFloat(), position[1].GetFloat(), position[2].GetFloat());
	m_color.Set(color[0].GetFloat(), color[1].GetFloat(), color[2].GetFloat(), color[3].GetFloat());
	m_ambient.Set(ambient[0].GetFloat(), ambient[1].GetFloat(), ambient[2].GetFloat(), ambient[3].GetFloat());
	m_diffuse.Set(diffuse[0].GetFloat(), diffuse[1].GetFloat(), diffuse[2].GetFloat(), diffuse[3].GetFloat());
	m_specular.Set(specular[0].GetFloat(), specular[1].GetFloat(), specular[2].GetFloat(), specular[3].GetFloat());
}


LightBuilder::LightBuilder()
	:ComponentBuilder()
{}

Component* LightBuilder::CreateComponent(Object* _pOwner) const
{
	return new Light(_pOwner);
}

JE_END
