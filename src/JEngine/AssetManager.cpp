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

// Built-In Component Headers
#include "GraphicComponents.h"
#include "PhysicsComponents.h"
#include "SoundComponents.h"

jeBegin

using namespace Math;

// Declare static member variables
ASSET::FontMap		ASSET::m_fontMap;
ASSET::AudioMap		ASSET::m_audioMap;
ASSET::StateMap		ASSET::m_stateMap;
ASSET::TextureMap	ASSET::m_textureMap;
ASSET::ArchetypeMap	ASSET::m_archetypeMap;
std::string			ASSET::m_initDirectory, ASSET::m_assetDirectory,
					ASSET::m_stateDirectory, ASSET::m_archeDirectory;
unsigned char*		ASSET::m_pPixelChunk = nullptr;

void AssetManager::ShowLoadingPercentage(unsigned _loadedPercentage, unsigned _size)
{
    std::string title;
    title.assign(APP::m_Data.m_title + " - Loading... " + std::to_string((float)_loadedPercentage / _size * 100.f) + "%");
    SDL_SetWindowTitle(APP::m_pWindow, title.c_str());
}

void AssetManager::LoadAssets()
{
    // Read state info
    JSON::ReadFile(ASSET::m_stateDirectory.c_str());
    CR_RJValue states = JSON::GetDocument()["State"];
    CR_RJValue fristStates = JSON::GetDocument()["FirstState"];

    // Read asset info
    JSON::ReadFile(ASSET::m_assetDirectory.c_str());
    CR_RJValue textures = JSON::GetDocument()["Texture"];

    // Read font info
    CR_RJValue fonts = JSON::GetDocument()["Font"];

    // Get sizes of them
    unsigned stateSize = states.Size(), textureSize = textures.Size(),
        fontSize = fonts.Size(),
        realLoadingPercentage = 0,
        loadingPercentage = stateSize + textureSize + fontSize;

    // Load states
    for (rapidjson::SizeType i = 0; i < stateSize; ++i) {
        STATE::PushState(states[i]["Directory"].GetString(), states[i]["Key"].GetString());
        jeDebugPrint("*AssetManager - Loaded state: %s.\n", states[i]["Directory"].GetString());
        realLoadingPercentage++;
        ShowLoadingPercentage(realLoadingPercentage, loadingPercentage);
    }

    // Set first state
    STATE::SetStartingState(fristStates.GetString());
    jeDebugPrint("*AssetManager - The first state is %s.\n", fristStates.GetString());

    // Load images
    for (rapidjson::SizeType i = 0; i < textureSize; ++i) {
        LoadImage(textures[i]["Directory"].GetString(), textures[i]["Key"].GetString());
        jeDebugPrint("*AssetManager - Loaded texture: %s.\n", textures[i]["Directory"].GetString());
        realLoadingPercentage++;
        ShowLoadingPercentage(realLoadingPercentage, loadingPercentage);
    }

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

    SDL_SetWindowTitle(APP::m_pWindow, APP::m_Data.m_title.c_str());

    // Load engine components
    COMPONENT::m_loadingCustomLogic = false;
}

void AssetManager::UnloadAssets()
{
    // Clear audio map
    //for (auto audio : m_audioMap) {
    //	if (audio.second) {
    //		delete audio.second;
    //		audio.second = nullptr;
    //	}
    //}

    // Clear font map
    for (auto font : m_fontMap) {
        if (font.second) {
            delete font.second;
            font.second = nullptr;
        }
    }
    m_fontMap.clear();

    // Clear texture map
    m_textureMap.clear();

    //// Clear archetype map
    //for (auto archetype : m_archetypeMap) {
    //	if (archetype.second) {
    //		delete archetype.second;
    //		archetype.second = nullptr;
    //	}
    //}

    // Clear state map
    m_stateMap.clear();

    COMPONENT::ClearBuilders();
}

bool AssetManager::SetBuiltInComponents()
{
    // Load built-in components

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


void AssetManager::LoadFont(const char * _path, const char* _key, unsigned _size,
    unsigned long _start, unsigned long _end)
{
    // Set pointer to new font
    Font* newFont = nullptr;
    static bool s_existing = false;
    static float s_newLineLevel = 0;
    auto found = m_fontMap.find(_key);

    if (found != m_fontMap.end()) {
        // There is existing font map
        s_existing = true;
        // Then get that one
        newFont = found->second;
        // Load the size of that font
        s_newLineLevel = newFont->m_newLineInterval;
    }

    else {

        // No existing font
        s_existing = false;
        // Then get a new font 
        newFont = new Font;

        // Init freetype
        if (FT_Init_FreeType(&newFont->m_lib))
            jeDebugPrint("!AssetManager - Could not init freetype library: %s\n", _path);

        // Check freetype face init
        if (bool a = !FT_New_Face(newFont->m_lib, _path, 0, &newFont->m_face))
            jeDebugPrint("*AssetManager - Loaded font: %s\n", _path);
		else {
			jeDebugPrint("!AssetManager - Failed to load font: %s\n", _path);
			return;
		}
        // Select unicode range
        FT_Select_Charmap(newFont->m_face, FT_ENCODING_UNICODE);

        // Set pixel size
        FT_Set_Pixel_Sizes(newFont->m_face, 0, _size);
        // Set size of the font
        newFont->m_fontSize = _size;

        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    }

    LoadCharacters(newFont, s_newLineLevel, _start, _end);

    // If there is not existing font in the list,
    // add new one
    if (!s_existing) {
        newFont->m_newLineInterval = s_newLineLevel;
        m_fontMap.insert(FontMap::value_type(_key, newFont));
    }
}

void AssetManager::LoadCharacters(Font* _pFont, float& _newLineLevel,
    unsigned long _start, unsigned long _end)
{
    // Load first 128 characters of ASCII set
    for (unsigned long c = _start; c < _end; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(_pFont->m_face, c, FT_LOAD_RENDER))
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
            _pFont->m_face->glyph->bitmap.width,
            _pFont->m_face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            _pFont->m_face->glyph->bitmap.buffer
        );

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Now store character for later use
        Character character = {
                texture, GLuint(_pFont->m_face->glyph->advance.x),
                vec2(float(_pFont->m_face->glyph->bitmap.width), float(_pFont->m_face->glyph->bitmap.rows)),
                vec2(float(_pFont->m_face->glyph->bitmap_left), float(_pFont->m_face->glyph->bitmap_top))
        };
        if (_newLineLevel < character.size.y)
            _newLineLevel = character.size.y;
        _pFont->m_data.insert(Font::FontData::value_type(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void AssetManager::LoadAudio(const char* /*_path*/, const char* /*_audioKey*/)
{
    // TODO
    // load audio assets
}

void AssetManager::LoadImage(const char *_path, const char *_textureKey)
{
    unsigned		newImage;
    Model::Image	image;
    unsigned		width, height;
    unsigned		error = lodepng::decode(image, width, height, _path);
	
	if (error)
        jeDebugPrint("!AssetManager - Decoder error %d / %s.\n", error, lodepng_error_text(error));

    // Enable the texture for OpenGL.
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &newImage);
    glBindTexture(GL_TEXTURE_2D, newImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    m_textureMap.insert(TextureMap::value_type(
        _textureKey, newImage));
}

void AssetManager::LoadArchetype(const char* /*_path*/, const char* /*_archetypeKey*/)
{
    // TODO
    // load archetpye assets
}

Mesh* AssetManager::LoadObj(const char* _path)
{
	Mesh *pNewMesh = new Mesh;
	bool hasNormal = false;

	std::ifstream obj(_path, std::ios::in);

	if (!obj) {
		jeDebugPrint("!AssetManager - Cannot load the object file: %s", _path);
		return nullptr;
	}

	std::string line;
	std::vector<unsigned> elements;
	std::vector<vec3> temp_points, temp_normals;
	std::vector<vec2> temp_uvs;

	static unsigned index = 0;

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
			temp_uvs.push_back(uv);
			pNewMesh->AddTextureUV(uv);
		}
		else if (line.substr(0, 3) == "vn ") {
			hasNormal = true;
			std::istringstream s(line.substr(2));
			vec3 normal; s >> normal.x; s >> normal.y; s >> normal.z;
			temp_normals.push_back(normal);
			pNewMesh->AddNormal(normal);
		}
		else if (line.substr(0, 2) == "f ") {

			// Calculate normal here - instread of getting normal from obj file
			if (!hasNormal) {
				temp_normals.resize(temp_points.size(), vec3::ZERO);
				for (unsigned i = 0; i < elements.size(); i += 3) {
					unsigned ia = elements[i];
					unsigned ib = elements[i + 1];
					unsigned ic = elements[i + 2];

					vec3 normal = GetNormalize(
						CrossProduct((temp_points[ib] - temp_points[ia]),
						(temp_points[ic] - temp_points[ia])));
					temp_normals[ia] = temp_normals[ib] = temp_normals[ic] = normal;
				}
				hasNormal = true;
			}

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
					else
						s >> c;
				}

				if (a) 	--a;
				if (b)	--b;
				if (c)	--c;

				pNewMesh->AddIndice({a, b, c });
				pNewMesh->AddPointIndice(index++);
			}
		}
		
		if (pNewMesh->GetNormals().empty()) {
			for (unsigned i = 0; i < temp_normals.size(); ++i)
				pNewMesh->AddNormal(temp_normals.at(i));
		}
		// TODO
		//else if (line.substr(0, 2) == "l ") {
		//	
		//}

	}
	index = 0;
	return pNewMesh;
}

void AssetManager::TakeAScreenshot(const char* _directory)
{
	// Get the total size of image
	unsigned width = APP::m_Data.m_width, height = APP::m_Data.m_height, size = width * height * 4;

	// Send the pixel info to the image vector
	std::vector<unsigned char> image;
	m_pPixelChunk = new unsigned char[size];
	//image.resize(size);

	// Read pixel from window screen
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &m_pPixelChunk[0]);

	// Invert the image vertucally
	for (unsigned y = 0; y < height / 2; y++)
		for (unsigned x = 0; x < width; x++)
		{
			unsigned index = 4 * (width * y + x);
			unsigned invertedInder = 4 * (width * (height - y - 1) + x);

			std::swap(m_pPixelChunk[index + 0], m_pPixelChunk[invertedInder + 0]);
			std::swap(m_pPixelChunk[index + 1], m_pPixelChunk[invertedInder + 1]);
			std::swap(m_pPixelChunk[index + 2], m_pPixelChunk[invertedInder + 2]);
			std::swap(m_pPixelChunk[index + 3], m_pPixelChunk[invertedInder + 3]);

		}

	// Check error
	unsigned error = lodepng::encode(image, m_pPixelChunk, width, height);
	if (!error) {

		std::string fileName;
		if (_directory)
			fileName.assign(_directory);
		
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


	delete[] m_pPixelChunk;
	m_pPixelChunk = nullptr;
}

void AssetManager::SetInitDirectory(const char * _dir)
{
    m_initDirectory.assign(_dir);
}

void AssetManager::SetAssetDirectory(const char * _dir)
{
    m_assetDirectory.assign(_dir);
}

void AssetManager::SetStateDirectory(const char * _dir)
{
    m_stateDirectory.assign(_dir);
}

void AssetManager::SetArchetypeDirectory(const char * _dir)
{
    m_archeDirectory.assign(_dir);
}

Font* AssetManager::GetFont(const char *_key)
{
    auto found = m_fontMap.find(_key);
    if (found != m_fontMap.end())
        return found->second;

    jeDebugPrint("!AssetManager - Cannot find such name of font resource: %s.\n", _key);
    return nullptr;
}

State* AssetManager::GetState(const char *_key)
{
    auto found = m_stateMap.find(_key);
    if (found != m_stateMap.end())
        return found->second;

    jeDebugPrint("!AssetManager - Cannot find such name of state resource: %s.\n", _key);
    return nullptr;
}

Audio* AssetManager::GetAudio(const char *_key)
{
    auto found = m_audioMap.find(_key);
    if (found != m_audioMap.end())
        return found->second;

    jeDebugPrint("!AssetManager - Cannot find such name of audio resource: %s.\n", _key);
    return nullptr;
}

unsigned AssetManager::GetTexture(const char *_key)
{
    auto found = m_textureMap.find(_key);
    if (found != m_textureMap.end())
        return found->second;

    jeDebugPrint("!AssetManager - Cannot find such name of texture resource: %s.\n", _key);
    return 0;
}

Archetype* AssetManager::GetArchetype(const char *_key)
{
    auto found = m_archetypeMap.find(_key);
    if (found != m_archetypeMap.end())
        return found->second;

    jeDebugPrint("!AssetManager: Cannot find such name of archetype resource: %s.\n", _key);
    return nullptr;
}

jeEnd
