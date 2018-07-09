#include "lodepng.h"
#include "GLManager.h"
#include "State.h"
#include "AssetManager.h"
#include "JsonParser.h"
#include "StateManager.h"
#include "ComponentManager.h"
#include "imgui.h"
#include "Application.h"
#include "SDL.h"
#include "Mesh.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include "MathUtils.h"
#include "Shader.h"
#include <thread>
#include "Importer.hpp"
#include "postprocess.h"
#include "scene.h"

// Built-In Component Headers
#include "GraphicComponents.h"
#include "PhysicsComponents.h"
#include "SoundComponents.h"

jeBegin

using namespace Math;

// Declare static member variables
ASSET::FontMap		ASSET::fontMap_;
ASSET::AudioMap		ASSET::audioMap_;
ASSET::StateMap		ASSET::stateMap_;
ASSET::TextureMap	ASSET::textureMap_;
ASSET::ArchetypeMap	ASSET::archetypeMap_;
std::string			ASSET::initDirectory_, ASSET::assetDirectory_,
					ASSET::stateDirectory_, ASSET::archeDirectory_;
unsigned char*		ASSET::pPixelChunk_ = nullptr;
std::unordered_map<std::string, ASSET::Image> ASSET::images_;

void AssetManager::ShowLoadingPercentage(unsigned loadedPercentage, unsigned size)
{
    std::string title;
    title.assign(APP::data_.title + " - Loading... " + std::to_string((float)loadedPercentage / size * 100.f) + "%");
    SDL_SetWindowTitle(APP::pWindow_, title.c_str());
}

void AssetManager::LoadAssets()
{
    // Read state info
    JSON::ReadFile(stateDirectory_.c_str());
    CR_RJValue states = JSON::GetDocument()["State"];

    // Read asset info
    JSON::ReadFile(assetDirectory_.c_str());
    CR_RJValue textures = JSON::GetDocument()["Texture"];

    // Read font info
    CR_RJValue fonts = JSON::GetDocument()["Font"];

    // Get sizes of them
    unsigned stateSize = states.Size(), textureSize = textures.Size(),
        fontSize = fonts.Size(),
        realLoadingPercentage = 0,
        loadingPercentage = stateSize + textureSize + fontSize;

    // Load states using thread
    for (rapidjson::SizeType i = 0; i < stateSize; ++i) {
		std::thread stateLoader(&STATE::PushState, states[i]["Directory"].GetString(), states[i]["Key"].GetString());
		stateLoader.join();
        jeDebugPrint("*AssetManager - Loaded state: %s.\n", states[i]["Directory"].GetString());
        realLoadingPercentage++;
        ShowLoadingPercentage(realLoadingPercentage, loadingPercentage);
    }

    // Set first state
	CR_RJValue fristStates = JSON::GetDocument()["FirstState"];
	std::string firstStateName = STATE::firstState_.empty() ? fristStates.GetString() : STATE::firstState_;
    STATE::SetStartingState(firstStateName.c_str());
    jeDebugPrint("*AssetManager - The first state is %s.\n", firstStateName.c_str());

    // Load images using thread
    for (rapidjson::SizeType i = 0; i < textureSize; ++i) {
		std::thread imageLoader(&LoadImage, textures[i]["Directory"].GetString(), textures[i]["Key"].GetString());
		imageLoader.join();
		jeDebugPrint("*AssetManager - Loaded image: %s.\n", textures[i]["Directory"].GetString());
        realLoadingPercentage++;
        ShowLoadingPercentage(realLoadingPercentage, loadingPercentage);
    }

	// Register images to gpu
	for (auto it : images_)
		RegisterImage(it.second, it.first.c_str());

    // Load font
    for (rapidjson::SizeType i = 0; i < fontSize; ++i) {

        // Load default ascii characters (0 - 128)
        LoadFont(fonts[i]["Directory"].GetString(), fonts[i]["Key"].GetString(), fonts[i]["Size"].GetUint(),
            0, 128);

        // Load additional unicode set
        for (unsigned j = 0; j < fonts[i]["Additional"].Size(); ++j) {
            LoadFont(fonts[i]["Directory"].GetString(), fonts[i]["Key"].GetString(), fonts[i]["Size"].GetUint(),
                static_cast<unsigned long>(fonts[i]["Additional"][j][0].GetUint64()),
                static_cast<unsigned long>(fonts[i]["Additional"][j][1].GetUint64()));
        }
        realLoadingPercentage++;
        ShowLoadingPercentage(realLoadingPercentage, loadingPercentage);
    }

    SDL_SetWindowTitle(APP::pWindow_, APP::data_.title.c_str());

}

void AssetManager::UnloadAssets()
{
    // Clear audio map
    //for (auto audio : audioMap_) {
    //	if (audio.second) {
    //		delete audio.second;
    //		audio.second = nullptr;
    //	}
    //}

    // Clear font map
    for (auto font : fontMap_) {
        if (font.second) {
            delete font.second;
            font.second = nullptr;
        }
    }
    fontMap_.clear();

    // Clear texture map
    textureMap_.clear();

    //// Clear archetype map
    //for (auto archetype : archetypeMap_) {
    //	if (archetype.second) {
    //		delete archetype.second;
    //		archetype.second = nullptr;
    //	}
    //}

    // Clear state map
    stateMap_.clear();

    COMPONENT::ClearBuilders();
}

bool AssetManager::SetBuiltInComponents()
{
    // Load built-in components
	COMPONENT::loadingCustomLogic_ = false;

    // Physics components
	jeCheckComponentRegistration(jeRegisterComponent(Transform));

	// Graphic components
	jeCheckComponentRegistration(jeRegisterComponent(Text));
	jeCheckComponentRegistration(jeRegisterComponent(Model));
	jeCheckComponentRegistration(jeRegisterComponent(Camera));
	jeCheckComponentRegistration(jeRegisterComponent(Emitter));
	jeCheckComponentRegistration(jeRegisterComponent(Light));
	jeCheckComponentRegistration(jeRegisterComponent(Material));
	jeCheckComponentRegistration(jeRegisterComponent(Animation));
		
	jeDebugPrint("*AssetManager - Loaded bulit-in components.\n");

    return true;
}

void AssetManager::LoadFont(const char * path, const char* key, unsigned size,
    unsigned long start, unsigned long end)
{
    // Set pointer to new font
    Font* newFont = nullptr;
    static bool s_existing = false;
    static float s_newLineLevel = 0;
    auto found = fontMap_.find(key);

    if (found != fontMap_.end()) {
        // There is existing font map
        s_existing = true;
        // Then get that one
        newFont = found->second;
        // Load the size of that font
        s_newLineLevel = newFont->newline_;
    }

    else {

        // No existing font
        s_existing = false;
        // Then get a new font 
        newFont = new Font;

        // Init freetype
        if (FT_Init_FreeType(&newFont->lib_))
            jeDebugPrint("!AssetManager - Could not init freetype library: %s\n", path);

        // Check freetype face init
        if (bool a = !FT_New_Face(newFont->lib_, path, 0, &newFont->face_))
            jeDebugPrint("*AssetManager - Loaded font: %s\n", path);
		else {
			jeDebugPrint("!AssetManager - Failed to load font: %s\n", path);
			return;
		}

        // Select unicode range
        FT_Select_Charmap(newFont->face_, FT_ENCODING_UNICODE);
        // Set pixel size
        FT_Set_Pixel_Sizes(newFont->face_, 0, size);
        // Set size of the font
        newFont->fontSize_ = size;
        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

    LoadCharacters(newFont, s_newLineLevel, start, end);

    // If there is not existing font in the list,
    // add new one
    if (!s_existing) {
        newFont->newline_ = s_newLineLevel;
        fontMap_.insert(FontMap::value_type(key, newFont));
    }
}

void AssetManager::LoadCharacters(Font* pFont, float& newLineLevel,
    unsigned long start, unsigned long end)
{
    // Load first 128 characters of ASCII set
    for (unsigned long c = start; c < end; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(pFont->face_, c, FT_LOAD_RENDER))
        {
            jeDebugPrint("!AssetManager - Failed to load Glyph.\n");
            break;
        }

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
			pFont->face_->glyph->bitmap.width,
			pFont->face_->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
			pFont->face_->glyph->bitmap.buffer
        );

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Now store character for later use
        Character character = {
                texture, GLuint(pFont->face_->glyph->advance.x),
                vec2(float(pFont->face_->glyph->bitmap.width), float(pFont->face_->glyph->bitmap.rows)),
                vec2(float(pFont->face_->glyph->bitmap_left), float(pFont->face_->glyph->bitmap_top))
        };
        if (newLineLevel < character.size.y)
			newLineLevel = character.size.y;
		pFont->data_.insert(Font::FontData::value_type(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void AssetManager::LoadAudio(const char* /*path*/, const char* /*_audioKey*/)
{
    // TODO
    // load audio assets
}

void AssetManager::LoadImage(const char *path, const char *textureKey)
{
    Image	 image;
    unsigned error = lodepng::decode(image.pixels, image.width, image.height, path);
	
	if (error)
        jeDebugPrint("!AssetManager - Decoder error %d / %s.\n", error, lodepng_error_text(error));

	else 
		images_.insert(
			std::unordered_map<std::string, Image>::value_type(textureKey, image));
}

void AssetManager::RegisterImage(Image& image, const char* textureKey)
{    
	// Enable the texture for OpenGL.
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &image.handle);
	glBindTexture(GL_TEXTURE_2D, image.handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, &image.pixels[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	textureMap_.insert(TextureMap::value_type(
		textureKey, image.handle));
}

void AssetManager::LoadArchetype(const char* /*path*/, const char* /*_archetypeKey*/)
{
    // TODO
    // load archetpye assets
}

Mesh* AssetManager::LoadObjFile(const char* path)
{
	Mesh *pNewMesh = new Mesh;

	std::ifstream obj(path, std::ios::in);

	if (!obj) {
		jeDebugPrint("!AssetManager - Cannot load the object file: %s", path);
		return nullptr;
	}

	std::string line;
	std::vector<unsigned> elements;
	std::vector<vec3> temp_points, temp_normals;
	std::vector<vec2> temp_uvs;
	
	while(std::getline(obj, line))	{
		if (line.substr(0, 2) == "v ") {
			std::istringstream s(line.substr(2));
			vec3 point; s >> point.x; s >> point.y; s >> point.z;
			temp_points.push_back(point);
			pNewMesh->AddPoint(point);
		}
		else if (line.substr(0, 3) == "vt ") {
			std::istringstream s(line.substr(2));
			vec2 uv; s >> uv.x; s >> uv.y;;
			uv.y = - uv.y;
			temp_uvs.push_back(uv);
			pNewMesh->AddTextureUV(uv);
		}
		else if (line.substr(0, 3) == "vn ") {
			std::istringstream s(line.substr(2));
			vec3 normal; s >> normal.x; s >> normal.y; s >> normal.z;
			temp_normals.push_back(normal);
			pNewMesh->AddNormal(normal);
		}
		else if (line.substr(0, 2) == "f ") {

			std::istringstream s(line.substr(2));
			unsigned a = 0, b = 0, c = 0;
			
			while (!s.eof()) {
				// Vertex index
				s >> a;

				// Check texture/normal index
				if (s.peek() == '/') {
					s.ignore(1);

					// a/b/c
					if (s.peek() != '/') {
						s >> b;

						if (s.peek() == '/') {
							s.ignore(1);
							s >> c;
						}
					}

					// No texture index
					// a//c
					else {
						s.ignore();
						s >> c;
					}
				}

				if (a) 	--a;
				if (b)	--b;
				if (c)	--c;

				pNewMesh->AddIndice({a, b, c });
			}
		}
		
		if (pNewMesh->GetNormals().empty()) {
			for (unsigned i = 0; i < temp_normals.size(); ++i)
				pNewMesh->AddNormal(temp_normals.at(i));
		}
	}

	GLM::DescribeVertex(pNewMesh);
	return pNewMesh;
}

void AssetManager::ProcessNode(aiNode * pNode, const aiScene * pScene)
{
	// Process all the node;s mesh
	for (unsigned i = 0; i < pNode->mNumMeshes; i++) {
		aiMesh *pMesh = pScene->mMeshes[i];
		// meshes.push_back(ProcessMesh(pMesh, pScene));
	}

	// Then do the same for each of its children
	for (unsigned i = 0; i < pNode->mNumChildren; i++) 
		ProcessNode(pNode->mChildren[i], pScene);
}

Mesh * AssetManager::ProcessMesh(aiMesh * pMesh, const aiScene * pScene)
{
	std::vector<Mesh::jeVertex> vertices;
	std::vector<unsigned> indices, textures;

	for (unsigned i = 0; i < pMesh->mNumVertices; i++) {
		Mesh::jeVertex vertex;
		vec3 vector;

		// Set positions
		vector.x = pMesh->mVertices[i].x;
		vector.y = pMesh->mVertices[i].y;
		vector.z = pMesh->mVertices[i].z;
		vertex.position.Set(vector);

		// Set normlas
		vector.x = pMesh->mNormals[i].x;
		vector.y = pMesh->mNormals[i].y;
		vector.z = pMesh->mNormals[i].z;
		vertex.normal.Set(vector);

		// texture coord
		if (pMesh->mTextureCoords[0]) {
			vec2 vec;
			vec.x = pMesh->mTextureCoords[0][i].x;
			vec.y = pMesh->mTextureCoords[0][i].y;
			vertex.uv.Set(vec2);
		}
		else
			vertex.uv.SetZero();

		// TODO: Locked 
		//// tangent
		//vector.x = mesh->mTangents[i].x;
		//vector.y = mesh->mTangents[i].y;
		//vector.z = mesh->mTangents[i].z;
		//vertex.Tangent = vector;
		//// bitangent
		//vector.x = mesh->mBitangents[i].x;
		//vector.y = mesh->mBitangents[i].y;
		//vector.z = mesh->mBitangents[i].z;
		//vertex.Bitangent = vector;
		//vertices.push_back(vertex);

		// Go through each mehs's faces
		for (unsigned i = 0; i < pMesh->mNumFaces; i++) {
			aiFace face = pMesh->mFaces[i];
			for (unsigned j = 0; j < face.mNumIndices; j++)
				indices.push_back(j);
		}
	}

	// TODO: Locked
	//// process materials
	//aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	//// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	//// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	//// Same applies to other texture as the following list summarizes:
	//// diffuse: texture_diffuseN
	//// specular: texture_specularN
	//// normal: texture_normalN

	//// 1. diffuse maps
	//vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//// 2. specular maps
	//vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//// 3. normal maps
	//std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	//// 4. height maps
	//std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	
	// return new Mesh{ vertices, indices, textures };
	return nullptr;
}

void AssetManager::LoadModel(std::string & path)
{
	Assimp::Importer import;
	const aiScene *pScene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
	{
		jeDebugPrint("!AssetManager - Error from Assimp: %s", import.GetErrorString());
		return;
	}

	// directory = path.substr(0, path.find_last_not_of('/'));

	ProcessNode(pScene->mRootNode, pScene);
}

void AssetManager::TakeAScreenshot(const char* directory)
{
	// Get the total size of image
	unsigned width = APP::data_.width, height = APP::data_.height, size = width * height * 4;

	// Send the pixel info to the image vector
	std::vector<unsigned char> image;
	pPixelChunk_ = new unsigned char[size];

	// Read pixel from window screen
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &pPixelChunk_[0]);

	// Invert the image vertucally
	for (unsigned y = 0; y < height / 2; y++)
		for (unsigned x = 0; x < width; x++)
		{
			unsigned index = 4 * (width * y + x);
			unsigned invertedInder = 4 * (width * (height - y - 1) + x);

			std::swap(pPixelChunk_[index + 0], pPixelChunk_[invertedInder + 0]);
			std::swap(pPixelChunk_[index + 1], pPixelChunk_[invertedInder + 1]);
			std::swap(pPixelChunk_[index + 2], pPixelChunk_[invertedInder + 2]);
			std::swap(pPixelChunk_[index + 3], pPixelChunk_[invertedInder + 3]);
		}

	// Check error
	unsigned error = lodepng::encode(image, pPixelChunk_, width, height);
	if (!error) {

		std::string fileName;
		if (directory)
			fileName.assign(directory);
		
		Time currentTimeInfo = Timer::GetCurrentTimeInfo();

		fileName += std::to_string(currentTimeInfo.year);

		if (currentTimeInfo.month < 10)
			fileName += "0" + std::to_string(currentTimeInfo.month);
		else
			fileName += std::to_string(currentTimeInfo.month);

		if (currentTimeInfo.day < 10)
			fileName += "0" + std::to_string(currentTimeInfo.day);
		else
			fileName += std::to_string(currentTimeInfo.day);

		if (currentTimeInfo.hour < 10)
			fileName += "0" + std::to_string(currentTimeInfo.hour);
		else
			fileName += std::to_string(currentTimeInfo.hour);

		if (currentTimeInfo.minute < 10)
			fileName += "0" + std::to_string(currentTimeInfo.minute);
		else
			fileName += std::to_string(currentTimeInfo.minute);

		if (currentTimeInfo.second < 10)
			fileName += "0" + std::to_string(currentTimeInfo.second);
		else
			fileName += std::to_string(currentTimeInfo.second);

		fileName += ".png";

		lodepng::save_file(image, fileName);
		jeDebugPrint("*AssetManager - Generated screenshot image file : %s\n", fileName.c_str());
	}

	else
		jeDebugPrint("!AssetManager - Cannot export screenshot image : %i\n", error);


	delete[] pPixelChunk_;
	pPixelChunk_ = nullptr;
}

void AssetManager::SetInitDirectory(const char * dir) { initDirectory_.assign(dir); }

void AssetManager::SetAssetDirectory(const char * dir) { assetDirectory_.assign(dir); }

void AssetManager::SetStateDirectory(const char * dir) { stateDirectory_.assign(dir); }

void AssetManager::SetArchetypeDirectory(const char * dir) { archeDirectory_.assign(dir); }

Font* AssetManager::GetFont(const char *key)
{
    auto found = fontMap_.find(key);
    if (found != fontMap_.end())
        return found->second;

    jeDebugPrint("!AssetManager - Cannot find such name of font resource: %s.\n", key);
    return nullptr;
}

State* AssetManager::GetState(const char *key)
{
    auto found = stateMap_.find(key);
    if (found != stateMap_.end())
        return found->second;

    jeDebugPrint("!AssetManager - Cannot find such name of state resource: %s.\n", key);
    return nullptr;
}

Audio* AssetManager::GetAudio(const char *key)
{
    auto found = audioMap_.find(key);
    if (found != audioMap_.end())
        return found->second;

    jeDebugPrint("!AssetManager - Cannot find such name of audio resource: %s.\n", key);
    return nullptr;
}

unsigned AssetManager::GetTexture(const char *key)
{
    auto found = textureMap_.find(key);
    if (found != textureMap_.end())
        return found->second;

    jeDebugPrint("!AssetManager - Cannot find such name of texture resource: %s.\n", key);
    return 0;
}

Archetype* AssetManager::GetArchetype(const char *key)
{
    auto found = archetypeMap_.find(key);
    if (found != archetypeMap_.end())
        return found->second;

    jeDebugPrint("!AssetManager: Cannot find such name of archetype resource: %s.\n", key);
    return nullptr;
}

void AssetManager::DrawLoadingScreen(SDL_Window* pWindow, const char* directory)
{
	// TODO
	// None of this code is not working
	LoadImage(directory, "Splash");

	for (auto image : images_) {
	
		if (!strcmp(image.first.c_str(), "Splash"))
			RegisterImage(image.second, "Splash");
	}

	GLsizei sizeOfPlaneIndices
		= static_cast<GLsizei>(GLM::targetMesh_[GLM::JE_TARGET_SCREEN]->GetIndiceCount());

	Mesh* mesh = Mesh::CreateRect();
	GLM::DescribeVertex(mesh);
	mesh->AddTexture("Splash");

	// Render to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, GLM::fbo_);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glViewport(0, 0, GLint(GLM::width_), GLint(GLM::height_));

	// Backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	Shader::Use(GLM::JE_SHADER_MODEL);

	Shader::pCurrentShader_->SetMatrix("m4_translate", Translate(vec3::ZERO));
	Shader::pCurrentShader_->SetMatrix("m4_scale", Scale(vec3(GLM::width_, GLM::height_, 0.f)));
	Shader::pCurrentShader_->SetMatrix("m4_rotate", Rotate(0.f, vec3::UNIT_Z));

	float right_ = GLM::width_ * .5f;
	float left_ = -right_;
	float top_ = GLM::height_ * .5f;
	float bottom_ = -top_;

	Shader::pCurrentShader_->SetMatrix("m4_projection", Orthogonal(left_, right_, bottom_, top_, 0.1f, 1000.f));

	// Send camera info to shader
	Shader::pCurrentShader_->SetMatrix("m4_viewport", Scale(vec3(GLM::width_ / 800.f, GLM::height_ / 600.f, 1.f)));

	glBindTexture(GL_TEXTURE, mesh->mainTexture_);

	// Send color info to shader
	Shader::pCurrentShader_->SetVector4("v4_color", vec4::ONE);
	Shader::pCurrentShader_->SetBool("boolean_flip", false);
	Shader::pCurrentShader_->SetMatrix("m4_aniScale", Scale(vec3(1,1,0)));
	Shader::pCurrentShader_->SetMatrix("m4_aniTranslate", Translate(vec3::ZERO));

	glBindVertexArray(mesh->vao_);
	glDrawElements(GL_TRIANGLES, sizeOfPlaneIndices, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	// Bind default framebuffer and render to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.f, 0.f, 0.f, 1.f);

	glDisable(GL_CULL_FACE);	//Disable face culling
	glDisable(GL_DEPTH_TEST);	//Disable depth test

	// Render to plane 2d
	glBindVertexArray(GLM::targetMesh_[GLM::JE_TARGET_SCREEN]->vao_);
	Shader::Use(GLM::JE_SHADER_SCREEN);
	Shader::pCurrentShader_->SetVector4("v4_screenColor", vec4::ONE);
	Shader::pCurrentShader_->SetEnum("enum_effectType", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GLM::texColorBuf_);
	glDrawElements(GL_TRIANGLES, sizeOfPlaneIndices, GL_UNSIGNED_INT, nullptr);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(0);

	SDL_GL_SwapWindow(pWindow);

	delete mesh;
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<unsigned> LoadMaterialTextures(aiMaterial */*mat*/, aiTextureType /*type*/, std::string /*typeName*/)
{
	std::vector<unsigned> textures;

	//for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	//{
	//	aiString str;
	//	mat->GetTexture(type, i, &str);
	//	// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
	//	bool skip = false;
	//	for (unsigned int j = 0; j < textures_loaded.size(); j++)
	//	{
	//		if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
	//		{
	//			textures.push_back(textures_loaded[j]);
	//			skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
	//			break;
	//		}
	//	}
	//	if (!skip)
	//	{   // if texture hasn't been loaded already, load it
	//		Texture texture;
	//		texture.id = TextureFromFile(str.C_Str(), this->directory);
	//		texture.type = typeName;
	//		texture.path = str.C_Str();
	//		textures.push_back(texture);
	//		textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
	//	}
	//}
	return textures;
}

jeEnd
