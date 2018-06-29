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
	:Model(_pOwner), ambient(vec4::ONE), diffuse(vec4::ONE),
	specular(vec4::ONE), direction(vec3::ZERO), constant(0.f), 
	linear(0.f), quadratic(0.f), cutOff(0.f), outerCutOff(0.f),
	sfactor(GL_SRC_ALPHA), dfactor(GL_ONE_MINUS_SRC_ALPHA)
{}

Light::~Light()
{
	// Clear mesh container
	for (auto mesh : meshes_) {
		if (!mesh->builtIn_) {
			delete mesh;
			mesh = nullptr;
		}
	}
	meshes_.clear();

	SYSTEM::pGraphic_->RemoveLight(this);
}

void Light::operator=(const Light & _copy)
{
	color.Set(_copy.color);
	ambient.Set(_copy.ambient); 
	diffuse.Set(_copy.diffuse);
	specular.Set(_copy.specular); 
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
	SYSTEM::pGraphic_->AddLight(this);
}

void Light::Load(CR_RJValue _data)
{
	if (_data.HasMember("Mesh")
		&& _data["Mesh"].IsArray())
	{
		CR_RJValue loadedMeshes = _data["Mesh"];

		for (unsigned meshIndex = 0; meshIndex < loadedMeshes.Size(); ++meshIndex) {

			CR_RJValue currentMesh = loadedMeshes[meshIndex];
			if (currentMesh.HasMember("Shape")
				&& currentMesh["Shape"].IsString()) {

				std::string meshType = currentMesh["Shape"].GetString();
				Mesh* newMesh = nullptr;

				if (!strcmp(meshType.c_str(), "Point"))
					newMesh = Mesh::CreatePoint();

				else if (!strcmp(meshType.c_str(), "Rect"))
					newMesh = Mesh::CreateRect();

				else if (!strcmp(meshType.c_str(), "CrossRect"))
					newMesh = Mesh::CreateCrossRect();

				/*else if (!strcmp(meshType.c_str(), "Cube"))
					newMesh = Mesh::CreateCube();
*/
				else if (!strcmp(meshType.c_str(), "Tetrahedron"))
					newMesh = Mesh::CreateTetrahedron();

				else if (!strcmp(meshType.c_str(), "Custom"))
					newMesh = ASSET::LoadObjFile(meshType.c_str());

				else
					AddMesh(Mesh::CreateCube());

				AddMesh(newMesh);

				if (currentMesh.HasMember("Texture")
					&& currentMesh["Texture"].IsArray()) {

					for (unsigned textureIndex = 0; textureIndex < currentMesh["Texture"].Size(); ++textureIndex)
						newMesh->AddTexture(currentMesh["Texture"][textureIndex].GetString());
				}
			}
		}
	}

	if (_data.HasMember("DrawMode")
		&& _data["DrawMode"].IsString()) {

		std::string drawType = _data["DrawMode"].GetString();
		if (!strcmp(drawType.c_str(), "Triangles"))
			m_drawMode = GL_TRIANGLES;

		else if (!strcmp(drawType.c_str(), "Triangle_Strip"))
			m_drawMode = GL_TRIANGLE_STRIP;

		else if (!strcmp(drawType.c_str(), "Triangle_Fan"))
			m_drawMode = GL_TRIANGLE_FAN;

		else if (!strcmp(drawType.c_str(), "Lines"))
			m_drawMode = GL_LINES;

		else if (!strcmp(drawType.c_str(), "Line_Strip"))
			m_drawMode = GL_LINE_STRIP;

		else if (!strcmp("Quad", drawType.c_str()))
			m_drawMode = GL_QUADS;

		else if (!strcmp("Quad_Strip", drawType.c_str()))
			m_drawMode = GL_QUAD_STRIP;
	}

	if (_data.HasMember("Flip")
		&& _data["Flip"].GetBool())
		status |= IS_FLIPPED;

	if (_data.HasMember("Color")) {
		CR_RJValue loadedColor = _data["Color"];
		color.Set(loadedColor[0].GetFloat(), loadedColor[1].GetFloat(),
			loadedColor[2].GetFloat(), loadedColor[3].GetFloat());
	}

	if (_data.HasMember("Projection")) {
		CR_RJValue loadedProjection = _data["Projection"];

		if (!strcmp("Perspective", loadedProjection.GetString()))
			projection = PROJECTION_PERSPECTIVE;

		else if (!strcmp("Orthogonal", loadedProjection.GetString()))
			projection = PROJECTION_ORTHOGONAL;

		else
			jeDebugPrint("!Model - Wrong projection type: %s\n", loadedProjection.GetString());
	}

	if (_data.HasMember("Bilboard")
		&& _data["Bilboard"].GetBool())
		status |= IS_BILBOARD;

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

}

void Light::EditorUpdate(const float /*dt*/)
{
	// TODO
}

jeEnd
