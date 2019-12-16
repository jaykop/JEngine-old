#include "Model.h"
#include "Object.h"
#include "Material.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Material);

Material::Material(Object* pOwner)
	:Component(pOwner), diffuse_(0), 
	specular_(0), shininess_(1.f)
{
	// Connect to model's pointer
	if (pOwner->HasComponent<Model>()) 
		pOwner->GetComponent<Model>()->pMaterial_ = this;

	else
		jeDebugPrint("!Material - This object has no model componnet: %s\n", pOwner->GetName().c_str());
}

Material::~Material()
{
	// Turn off the toggle
	if (GetOwner()->HasComponent<Model>()) 
		GetOwner()->GetComponent<Model>()->pMaterial_ = nullptr;
}

void Material::operator=(const Material & copy)
{
	diffuse_ = copy.diffuse_;
	specular_ = copy.specular_;
	shininess_ = copy.shininess_;
	
	if (GetOwner()->HasComponent<Model>()) 
		GetOwner()->GetComponent<Model>()->pMaterial_ = this;

}

void Material::Load(CR_RJValue data)
{
	if (data.HasMember("Diffuse")) {
		CR_RJValue loadedDiffuse = data["Diffuse"];
		diffuse_ = loadedDiffuse.GetInt();
	}

	if (data.HasMember("Specular")) {
		CR_RJValue loadedSpecular = data["Specular"];
		specular_ = loadedSpecular.GetInt();
	}
	
	if (data.HasMember("Shininess")) {
		CR_RJValue loadedShininess = data["Shininess"];
		shininess_ = loadedShininess.GetFloat();
	}
}

void Material::EditorUpdate(const float /*dt*/)
{
	// TODO
}

jeEnd
