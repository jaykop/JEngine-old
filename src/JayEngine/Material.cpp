#include "Model.h"
#include "Object.h"
#include "Sprite.h"
#include "Material.h"

JE_BEGIN

Material::Material(Object* _owner)
	:Component(_owner), m_diffuse(0), 
	m_specular(0), m_shininess(1.f)
{
	// Connect to sprite's pointer
	if (_owner->HasComponent<Sprite>()
		&& !_owner->GetComponent<Sprite>()->m_hasMaterial) {
		_owner->GetComponent<Sprite>()->m_material = this;
		_owner->GetComponent<Sprite>()->m_hasMaterial = true;
	}

	else if (_owner->HasComponent<Model>()
		&& !_owner->GetComponent<Model>()->m_hasMaterial) {
		_owner->GetComponent<Model>()->m_material = this;
		_owner->GetComponent<Model>()->m_hasMaterial = true;
	}

	else
		JE_DEBUG_PRINT("This object has no sprite componnet!\n");
}

void Material::Load(CR_RJValue _data)
{
	if (_data.HasMember("Diffuse")) {
		CR_RJValue diffuse = _data["Diffuse"];
		m_diffuse = diffuse.GetInt();
	}

	if (_data.HasMember("Specular")) {
		CR_RJValue specular = _data["Specular"];
		m_specular = specular.GetInt();
	}
	
	if (_data.HasMember("Shininess")) {
		CR_RJValue shininess = _data["Shininess"];
		m_shininess = shininess.GetFloat();
	}
}

MaterialBuilder::MaterialBuilder()
	:ComponentBuilder()
{}

Component* MaterialBuilder::CreateComponent(Object* _pOwner) const
{
	return new Material(_pOwner);
}

JE_END