#include "Light.h"
#include "SystemManager.h"
#include "Mesh.h"
#include "AssetManager.h"

#ifdef  jeUseBuiltInAllocator
#include "MemoryAllocator.h"
#endif

jeBegin
jeDefineComponentBuilder(Light);

Light::Light(Object * pOwner)
	:Model(pOwner), ambient_(vec4::ONE), diffuse_(vec4::ONE),
	specular_(vec4::ONE), direction_(vec3::ZERO), constant_(0.f), 
	linear_(0.f), quadratic_(0.f), cutOff_(0.f), outerCutOff_(0.f),
	sfactor_(GL_SRC_ALPHA), dfactor_(GL_ONE_MINUS_SRC_ALPHA)
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

void Light::operator=(const Light & /*copy*/)
{
	//color_.Set(copy.color_);
	//ambient_.Set(copy.ambient); 
	//diffuse_.Set(copy.diffuse);
	//specular_.Set(copy.specular); 
	//direction_.Set(copy.direction); 
	//constant_ = copy.constant; 
	//linear_ = copy.linear;
	//quadratic_ = copy.quadratic;
	//cutOff_ = copy.cutOff;
	//outerCutOff_ = copy.outerCutOff;
	//projection_ = copy.projection_;
}

void Light::Register()
{
	SYSTEM::pGraphic_->AddLight(this);
}

void Light::Load(CR_RJValue data)
{
	if (data.HasMember("Mesh")
		&& data["Mesh"].IsArray())
	{
		CR_RJValue loadedMeshes = data["Mesh"];

		for (unsigned meshIndex = 0; meshIndex < loadedMeshes.Size(); ++meshIndex) {

			CR_RJValue currentMesh = loadedMeshes[meshIndex];
			Mesh* newMesh = nullptr;

			// Check either if there is shape
			if (currentMesh.HasMember("Shape")
				&& currentMesh["Shape"].IsString()) {
				std::string meshType = currentMesh["Shape"].GetString();

				if (!strcmp(meshType.c_str(), "Point"))
					newMesh = Mesh::CreatePoint();

				else if (!strcmp(meshType.c_str(), "CrossRect"))
					newMesh = Mesh::CreateCrossRect();

				else if (!strcmp(meshType.c_str(), "Cube"))
					newMesh = Mesh::CreateCube();

				else if (!strcmp(meshType.c_str(), "Tetrahedron"))
					newMesh = Mesh::CreateTetrahedron();

				else if (!strcmp(meshType.c_str(), "Rect"))
					newMesh = Mesh::CreateRect();

				else 
					newMesh = ASSET::LoadObjFile(meshType.c_str());
			}
			// If not, set default mesh type
			else
				newMesh = Mesh::CreateRect();

			AddMesh(newMesh);

			// Load texture
			if (currentMesh.HasMember("Texture")
				&& currentMesh["Texture"].IsArray()) {
				for (unsigned textureIndex = 0; textureIndex < currentMesh["Texture"].Size(); ++textureIndex)
					newMesh->AddTexture(currentMesh["Texture"][textureIndex].GetString());
			}
		}
	}

	if (data.HasMember("DrawMode")
		&& data["DrawMode"].IsString()) {

		std::string drawType = data["DrawMode"].GetString();
		if (!strcmp(drawType.c_str(), "Triangles"))
			drawMode_ = GL_TRIANGLES;

		else if (!strcmp(drawType.c_str(), "Triangle_Strip"))
			drawMode_ = GL_TRIANGLE_STRIP;

		else if (!strcmp(drawType.c_str(), "Triangle_Fan"))
			drawMode_ = GL_TRIANGLE_FAN;

		else if (!strcmp(drawType.c_str(), "Lines"))
			drawMode_ = GL_LINES;

		else if (!strcmp(drawType.c_str(), "Line_Strip"))
			drawMode_ = GL_LINE_STRIP;

		else if (!strcmp("Quad", drawType.c_str()))
			drawMode_ = GL_QUADS;

		else if (!strcmp("Quad_Strip", drawType.c_str()))
			drawMode_ = GL_QUAD_STRIP;

		else if (!strcmp("Points", drawType.c_str()))
			drawMode_ = GL_POINTS;
	}

	if (data.HasMember("Flip")
		&& data["Flip"].GetBool())
		status_ |= IS_FLIPPED;

	if (data.HasMember("Color")) {
		CR_RJValue loadedColor = data["Color"];
		color_.Set(loadedColor[0].GetFloat(), loadedColor[1].GetFloat(),
			loadedColor[2].GetFloat(), loadedColor[3].GetFloat());
	}

	if (data.HasMember("Projection")) {
		CR_RJValue loadedProjection = data["Projection"];

		if (!strcmp("Perspective", loadedProjection.GetString()))
			projection_ = PROJECTION_PERSPECTIVE;

		else if (!strcmp("Orthogonal", loadedProjection.GetString()))
			projection_ = PROJECTION_ORTHOGONAL;

		else
			jeDebugPrint("!Model - Wrong projection type: %s\n", loadedProjection.GetString());
	}

	if (data.HasMember("Bilboard")
		&& data["Bilboard"].GetBool())
		status_ |= IS_BILBOARD;

	if (data.HasMember("Type")) {
		CR_RJValue loadedType = data["Type"];
		if (!strcmp(loadedType.GetString(), "Normal"))
			type_ = NORMALLIGHT;
		else if (!strcmp(loadedType.GetString(), "Directional"))
			type_ = DIRECTIONALLIGHT;
		else if (!strcmp(loadedType.GetString(), "Spot"))
			type_ = SPOTLIGHT;
		else if (!strcmp(loadedType.GetString(), "Point"))
			type_ = POINTLIGHT;
	}

	if (data.HasMember("CutOff")) {
		CR_RJValue loadedCutOff = data["CutOff"];
		cutOff_ = loadedCutOff.GetFloat();
	}

	if (data.HasMember("OuterCutOff")) {
		CR_RJValue loadedOutCutOff = data["OuterCutOff"];
		outerCutOff_ = loadedOutCutOff.GetFloat();
	}

	if (data.HasMember("Constant")) {
		CR_RJValue loadedConstant = data["Constant"];
		constant_ = loadedConstant.GetFloat();
	}

	if (data.HasMember("Linear")) {
		CR_RJValue loadedlinear = data["Linear"];
		linear_ = loadedlinear.GetFloat();
	}

	if (data.HasMember("Quadratic")) {
		CR_RJValue loadedQuadratic = data["Quadratic"];
		quadratic_ = loadedQuadratic.GetFloat();
	}

	if (data.HasMember("Direction")) {
		CR_RJValue loadedDirection = data["Direction"];
		direction_.Set(loadedDirection[0].GetFloat(), loadedDirection[1].GetFloat(), loadedDirection[2].GetFloat());
	}

	if (data.HasMember("Ambient")) {
		CR_RJValue loadedAmbient = data["Ambient"];
		ambient_.Set(loadedAmbient[0].GetFloat(), loadedAmbient[1].GetFloat(), loadedAmbient[2].GetFloat(), loadedAmbient[3].GetFloat());
	}
	
	if (data.HasMember("Diffuse")) {
		CR_RJValue loadedDiffuse = data["Diffuse"];
		diffuse_.Set(loadedDiffuse[0].GetFloat(), loadedDiffuse[1].GetFloat(), loadedDiffuse[2].GetFloat(), loadedDiffuse[3].GetFloat());
	}
	
	if (data.HasMember("Specular")) {
		CR_RJValue loadedSpecular = data["Specular"];
		specular_.Set(loadedSpecular[0].GetFloat(), loadedSpecular[1].GetFloat(), loadedSpecular[2].GetFloat(), loadedSpecular[3].GetFloat());
	}

}

void Light::EditorUpdate(const float /*dt*/)
{
	// TODO
}

jeEnd
