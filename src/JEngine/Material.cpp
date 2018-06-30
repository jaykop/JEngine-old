#include "Model.h"
#include "Object.h"
#include "Material.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Material);

Material::Material(Object* pOwner)
	:Component(pOwner), diffuse(0), 
	specular(0), shininess(1.f)
{
	// Connect to model's pointer
	if (pOwner->HasComponent<Model>()) 
		pOwner->GetComponent<Model>()->m_pMaterial = this;

	else
		jeDebugPrint("!Material - This object has no model componnet: %s\n", pOwner->GetName().c_str());
}

Material::~Material()
{
	// Turn off the toggle
	if (GetOwner()->HasComponent<Model>()) 
		GetOwner()->GetComponent<Model>()->m_pMaterial = nullptr;
}

void Material::operator=(const Material & copy)
{
	diffuse = copy.diffuse;
	specular = copy.specular;
	shininess = copy.shininess;
	
	if (GetOwner()->HasComponent<Model>()) 
		GetOwner()->GetComponent<Model>()->m_pMaterial = this;

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

void Material::EditorUpdate(const float /*dt*/)
{
	// TODO
}

jeEnd
