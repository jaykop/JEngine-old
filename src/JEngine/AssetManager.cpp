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

// Built-In Component Headers
#include "GraphicComponents.h"
#include "PhysicsComponents.h"
#include "SoundComponents.h"

jeBegin

// Declare static member variables
ASSET::FontMap		ASSET::m_fontMap;
ASSET::AudioMap		ASSET::m_audioMap;
ASSET::StateMap		ASSET::m_stateMap;
ASSET::TextureMap	ASSET::m_textureMap;
ASSET::ArchetypeMap	ASSET::m_archetypeMap;
std::string			ASSET::m_initDirectory, ASSET::m_assetDirectory,
ASSET::m_stateDirectory, ASSET::m_archeDirectory;

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
	jeCheckComponentRegistration(jeRegisterComponent(Transform))

	// Graphic components
	jeCheckComponentRegistration(jeRegisterComponent(Text))
	jeCheckComponentRegistration(jeRegisterComponent(Model))
	jeCheckComponentRegistration(jeRegisterComponent(Camera))
	jeCheckComponentRegistration(jeRegisterComponent(Sprite))
	jeCheckComponentRegistration(jeRegisterComponent(Emitter))
	jeCheckComponentRegistration(jeRegisterComponent(Light))
	jeCheckComponentRegistration(jeRegisterComponent(Material))
	jeCheckComponentRegistration(jeRegisterComponent(Animation))
		
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
        else
            jeDebugPrint("!AssetManager - Failed to load font: %s\n", _path);

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
            continue;
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
        if (_newLineLevel < character.m_size.y)
            _newLineLevel = character.m_size.y;
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
    Sprite::Image	image;
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
