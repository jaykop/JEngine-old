#include "Light.h"
#include "SystemManager.h"
#include "GLManager.h"
#include "Mesh.h"
#include "AssetManager.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Light);

Light::Light(Object * _pOwner)
	:Component(_pOwner), color(vec4::ONE),
	ambient(vec4::ONE), diffuse(vec4::ONE),
	specular(vec4::ONE), position(vec3(0.f, 0.f, 1.f)),
	direction(vec3::ZERO), constant(0.f), linear(0.f),
	quadratic(0.f), cutOff(0.f), outerCutOff(0.f),
	projection(PROJECTION_PERSPECTIVE), scale(vec3::ONE),
	sfactor(GL_SRC_ALPHA), dfactor(GL_ONE_MINUS_SRC_ALPHA)
{}

Light::~Light()
{
	if (m_pMeshes) {
		delete m_pMeshes;
		m_pMeshes = nullptr;
	}

	SYSTEM::GetGraphicSystem()->RemoveLight(this);
}

void Light::operator=(const Light & _copy)
{
	color.Set(_copy.color);
	ambient.Set(_copy.ambient); 
	diffuse.Set(_copy.diffuse);
	specular.Set(_copy.specular); 
	position.Set(_copy.position);
	direction.Set(_copy.direction); 
	constant = _copy.constant; 
	linear = _copy.linear;
	quadratic = _copy.quadratic;
	cutOff = _copy.cutOff;
	outerCutOff = _copy.outerCutOff;
	projection = _copy.projection;
}

void Light::Register()
{
	SYSTEM::GetGraphicSystem()->AddLight(this);
}

void Light::Load(CR_RJValue _data)
{
	if (_data.HasMember("Type")) {
		CR_RJValue loadedType = _data["Type"];
		if (!strcmp(loadedType.GetString(), "Normal"))
			m_type = NORMALLIGHT;
		else if (!strcmp(loadedType.GetString(), "Directional"))
			m_type = DIRECTIONALLIGHT;
		else if (!strcmp(loadedType.GetString(), "Spot"))
			m_type = SPOTLIGHT;
		else if (!strcmp(loadedType.GetString(), "Point"))
			m_type = POINTLIGHT;
	}

	if (_data.HasMember("Mesh")
		&& _data["Mesh"].GetString())
	{
		std::string meshType = _data["Mesh"].GetString();
		if (!strcmp(meshType.c_str(), "Point")) {
			m_pMeshes = Mesh::CreatePoint();
			m_pMeshes->m_shape = Mesh::MESH_POINT;
		}
		else if (!strcmp(meshType.c_str(), "Rect")) {
			m_pMeshes = Mesh::CreateRect();
			m_pMeshes->m_shape = Mesh::MESH_RECT;
		}
		else if (!strcmp(meshType.c_str(), "CrossRect")) {
			m_pMeshes = Mesh::CreateCrossRect();
			m_pMeshes->m_shape = Mesh::MESH_CROSSRECT;
		}
		else if (!strcmp(meshType.c_str(), "Cube")) {
			m_pMeshes = Mesh::CreateCube();
			m_pMeshes->m_shape = Mesh::MESH_CUBE;
		}
		else if (!strcmp(meshType.c_str(), "Tetrahedron")) {
			m_pMeshes = Mesh::CreateTetrahedron();
			m_pMeshes->m_shape = Mesh::MESH_TETRAHEDRON;
		}
		else /*if (!strcmp(meshType.c_str(), "Custom"))*/ {
			m_pMeshes = ASSET::LoadObjFile(meshType.c_str());
			m_pMeshes->m_shape = Mesh::MESH_NONE;
			GLM::DescribeVertex(m_pMeshes->m_vao, m_pMeshes->m_vbo, m_pMeshes->m_ebo, m_pMeshes);
			m_pMeshes->builtIn_ = false;
		}
	}
	else {
		m_pMeshes = Mesh::CreateCube();
		m_pMeshes->m_shape = Mesh::MESH_CUBE;
	}

	if (_data.HasMember("CutOff")) {
		CR_RJValue loadedCutOff = _data["CutOff"];
		cutOff = loadedCutOff.GetFloat();
	}

	if (_data.HasMember("OuterCutOff")) {
		CR_RJValue loadedOutCutOff = _data["OuterCutOff"];
		outerCutOff = loadedOutCutOff.GetFloat();
	}

	if (_data.HasMember("Constant")) {
		CR_RJValue loadedConstant = _data["Constant"];
		constant = loadedConstant.GetFloat();
	}

	if (_data.HasMember("Linear")) {
		CR_RJValue loadedlinear = _data["Linear"];
		linear = loadedlinear.GetFloat();
	}

	if (_data.HasMember("Quadratic")) {
		CR_RJValue loadedQuadratic = _data["Quadratic"];
		quadratic = loadedQuadratic.GetFloat();
	}

	if (_data.HasMember("Direction")) {
		CR_RJValue loadedDirection = _data["Direction"];
		direction.Set(loadedDirection[0].GetFloat(), loadedDirection[1].GetFloat(), loadedDirection[2].GetFloat());
	}

	if (_data.HasMember("Color")) {
		CR_RJValue loadedColor = _data["Color"];
		color.Set(loadedColor[0].GetFloat(), loadedColor[1].GetFloat(), loadedColor[2].GetFloat(), loadedColor[3].GetFloat());
	}

	if (_data.HasMember("Ambient")) {
		CR_RJValue loadedAmbient = _data["Ambient"];
		ambient.Set(loadedAmbient[0].GetFloat(), loadedAmbient[1].GetFloat(), loadedAmbient[2].GetFloat(), loadedAmbient[3].GetFloat());
	}
	
	if (_data.HasMember("Diffuse")) {
		CR_RJValue loadedDiffuse = _data["Diffuse"];
		diffuse.Set(loadedDiffuse[0].GetFloat(), loadedDiffuse[1].GetFloat(), loadedDiffuse[2].GetFloat(), loadedDiffuse[3].GetFloat());
	}
	
	if (_data.HasMember("Specular")) {
		CR_RJValue loadedSpecular = _data["Specular"];
		specular.Set(loadedSpecular[0].GetFloat(), loadedSpecular[1].GetFloat(), loadedSpecular[2].GetFloat(), loadedSpecular[3].GetFloat());
	}
	
	if (_data.HasMember("Position")) {
		CR_RJValue loadedPosition = _data["Position"];
		position.Set(loadedPosition[0].GetFloat(), loadedPosition[1].GetFloat(), loadedPosition[2].GetFloat());
	}

	if (_data.HasMember("Scale")) {
		CR_RJValue loadedScale = _data["Position"];
		scale.Set(loadedScale[0].GetFloat(), loadedScale[1].GetFloat(), loadedScale[2].GetFloat());
	}
}

void Light::EditorUpdate(const float /*dt*/)
{
	// TODO
}

jeEnd
