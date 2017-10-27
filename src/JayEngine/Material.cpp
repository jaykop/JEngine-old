#include "Object.h"
#include "Sprite.h"
#include "Material.h"

JE_BEGIN

Material::Material(Object* _owner)
	:Component(_owner), m_diffuse(0), 
	m_specular(0), m_shininess(1.f)
	/*, m_ambient(vec4::ONE), m_diffuse(vec4::ONE), 
	m_specular(vec4::ONE)*/
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

JE_END