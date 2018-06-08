#include "Model.h"
#include "Object.h"
#include "Material.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Material);

Material::Material(Object* _pOwner)
	:Component(_pOwner), diffuse(0), 
	specular(0), shininess(1.f)
{
	// Connect to sprite's pointer
	if (_pOwner->HasComponent<Sprite>()
		&& (_pOwner->GetComponent<Sprite>()->status & Sprite::HAS_MATERIAL)
			== Sprite::HAS_MATERIAL) {
		_pOwner->GetComponent<Sprite>()->m_pMaterial = this;
		_pOwner->GetComponent<Sprite>()->status |= Sprite::HAS_MATERIAL;
	}

	else if (_pOwner->HasComponent<Model>()
		&& (_pOwner->GetComponent<Model>()->status & Sprite::HAS_MATERIAL)
		== Sprite::HAS_MATERIAL) {
		_pOwner->GetComponent<Model>()->m_pMaterial = this;
		_pOwner->GetComponent<Sprite>()->status |= Sprite::HAS_MATERIAL;
	}

	else
		jeDebugPrint("!Material - This object has no sprite componnet: %s\n", _pOwner->GetName().c_str());
}

Material::~Material()
{
	// Turn off the toggle
	if (GetOwner()->HasComponent<Sprite>()) {
		GetOwner()->GetComponent<Sprite>()->m_pMaterial = nullptr;
		GetOwner()->GetComponent<Sprite>()->status &= ~Sprite::HAS_MATERIAL;
	}
}

void Material::operator=(const Material & _copy)
{
	diffuse = _copy.diffuse;
	specular = _copy.specular;
	shininess = _copy.shininess;
	
	if (GetOwner()->HasComponent<Sprite>()
		&& (GetOwner()->GetComponent<Sprite>()->status & Sprite::HAS_MATERIAL)
		== Sprite::HAS_MATERIAL) {
		GetOwner()->GetComponent<Sprite>()->m_pMaterial = this;
		GetOwner()->GetComponent<Sprite>()->status |= Sprite::HAS_MATERIAL;
	}
	else if (GetOwner()->HasComponent<Model>()
		&& (GetOwner()->GetComponent<Model>()->status & Sprite::HAS_MATERIAL)
		== Sprite::HAS_MATERIAL) {
		GetOwner()->GetComponent<Model>()->m_pMaterial = this;
		GetOwner()->GetComponent<Sprite>()->status |= Sprite::HAS_MATERIAL;
	}

}

void Material::Load(CR_RJValue _data)
{
	if (_data.HasMember("Diffuse")) {
		CR_RJValue loadedDiffuse = _data["Diffuse"];
		diffuse = loadedDiffuse.GetInt();
	}

	if (_data.HasMember("Specular")) {
		CR_RJValue loadedSpecular = _data["Specular"];
		specular = loadedSpecular.GetInt();
	}
	
	if (_data.HasMember("Shininess")) {
		CR_RJValue loadedShininess = _data["Shininess"];
		shininess = loadedShininess.GetFloat();
	}
}

void Material::EditorUpdate(const float /*_dt*/)
{
	// TODO
}

jeEnd
