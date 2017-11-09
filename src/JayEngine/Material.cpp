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

	else
		JE_DEBUG_PRINT("This object has no sprite componnet!\n");
}

void Material::Load(CR_RJValue _data)
{
	CR_RJValue diffuse = _data["Diffuse"];
	CR_RJValue specular = _data["Specular"];
	CR_RJValue shininess = _data["Position"];

	m_diffuse = diffuse.GetInt();
	m_specular = specular.GetInt();
	m_shininess = shininess.GetFloat();
}

MaterialBuilder::MaterialBuilder()
	:ComponentBuilder()
{}

Component* MaterialBuilder::CreateComponent(Object* _pOwner) const
{
	return new Material(_pOwner);
}

JE_END