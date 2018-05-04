#include "lodepng.h"
#include "GLManager.h"
#include "State.h"
#include "AssetManager.h"
#include "JsonParser.h"
#include "StateManager.h"
#include "ComponentManager.h"
#include "imgui.h"

// Built-In Component Headers
#include "GraphicComponents.h"
#include "PhysicsComponents.h"
#include "SoundComponents.h"

JE_BEGIN

// Declare static member variables
ASSET::FontMap		ASSET::m_fontMap;
ASSET::AudioMap		ASSET::m_audioMap;
ASSET::StateMap		ASSET::m_stateMap;
ASSET::TextureMap	ASSET::m_textureMap;
ASSET::ArchetypeMap	ASSET::m_archetypeMap;
std::string			ASSET::m_initDirectory, ASSET::m_assetDirectory,
					ASSET::m_stateDirectory, ASSET::m_archeDirectory;

void AssetManager::Load()
{
	// Load states
	JSON::ReadFile(ASSET::m_stateDirectory.c_str());
	CR_RJValue states = JSON::GetDocument()["State"];
	for (rapidjson::SizeType i = 0; i < states.Size(); ++i) {
		STATE::PushState(states[i]["Directory"].GetString(), states[i]["Key"].GetString());
		JE_DEBUG_PRINT("*AssetManager - Loaded state: %s.\n", states[i]["Directory"].GetString());
	}

	CR_RJValue fristStates = JSON::GetDocument()["FirstState"];
	STATE::SetStartingState(fristStates.GetString());
	JE_DEBUG_PRINT("*AssetManager - First state is %s.\n", fristStates.GetString());

	// Load images
	JSON::ReadFile(ASSET::m_assetDirectory.c_str());
	CR_RJValue textures = JSON::GetDocument()["Texture"];
	for (rapidjson::SizeType i = 0; i < textures.Size(); ++i) {
		LoadImage(textures[i]["Directory"].GetString(), textures[i]["Key"].GetString());
		JE_DEBUG_PRINT("*AssetManager - Loaded texture: %s.\n", textures[i]["Directory"].GetString());
	}

	// Load font
	CR_RJValue fonts = JSON::GetDocument()["Font"];
	for (rapidjson::SizeType i = 0; i < fonts.Size(); ++i)
		LoadFont(fonts[i]["Directory"].GetString(), fonts[i]["Key"].GetString(), fonts[i]["Size"].GetUint());

	// Load engine components
	COMPONENT::m_loadingCustomLogic = false;

	LoadBuiltInComponents();
}

void AssetManager::Unload()
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

void AssetManager::LoadBuiltInComponents()
{
	// Load built-in components

	// Physics components
	JE_REGISTER_COMPONENT(Transform);

	// Graphic components
	JE_REGISTER_COMPONENT(Text);
	JE_REGISTER_COMPONENT(Model);
	JE_REGISTER_COMPONENT(Camera);
	JE_REGISTER_COMPONENT(Sprite);
	JE_REGISTER_COMPONENT(Emitter);
	JE_REGISTER_COMPONENT(Light);
	JE_REGISTER_COMPONENT(Material);
	JE_REGISTER_COMPONENT(Animation);

	JE_DEBUG_PRINT("*AssetManager - Loaded bulit-in components.\n");
}


void AssetManager::LoadFont(const char * _path, const char* _key, unsigned _size)
{
	Font* newFont = new Font;
	newFont->m_fontSize = _size;

	// Init freetype
	if (FT_Init_FreeType(&newFont->m_lib))
		JE_DEBUG_PRINT("!AssetManager - Could not init freetype library: %s\n", _path);

	// Check freetype face init
	if (bool a = !FT_New_Face(newFont->m_lib, _path, 0, &newFont->m_face))
		JE_DEBUG_PRINT("*AssetManager - Loaded font: %s\n", _path);
	else
		JE_DEBUG_PRINT("!AssetManager - Failed to load font: %s\n", _path);

	FT_Select_Charmap(newFont->m_face, FT_ENCODING_UNICODE);

	FT_Set_Pixel_Sizes(newFont->m_face, 0, _size);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	static float s_newLineLevel = 0;

	// Load first 128 characters of ASCII set
	for (unsigned long c = 0xAC00; c < 0xAD00; c++)
	{

		// Load character glyph 
		if (FT_Load_Char(newFont->m_face, c, FT_LOAD_RENDER))
		{
			JE_DEBUG_PRINT("!AssetManager - Failed to load Glyph.\n");
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
			newFont->m_face->glyph->bitmap.width,
			newFont->m_face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			newFont->m_face->glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for later use
		Character character = {
			texture, GLuint(newFont->m_face->glyph->advance.x),
			vec2(float(newFont->m_face->glyph->bitmap.width), float(newFont->m_face->glyph->bitmap.rows)),
			vec2(float(newFont->m_face->glyph->bitmap_left), float(newFont->m_face->glyph->bitmap_top))
		};
		if (s_newLineLevel < character.m_size.y)
			s_newLineLevel = character.m_size.y;
		newFont->m_data.insert(Font::FontData::value_type(c, character));
	}
	newFont->m_newLineInterval = s_newLineLevel;

	glBindTexture(GL_TEXTURE_2D, 0);

	m_fontMap.insert(FontMap::value_type(_key, newFont));
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
		JE_DEBUG_PRINT("!AssetManager - Decoder error %d / %s.\n", error, lodepng_error_text(error));

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

	JE_DEBUG_PRINT("!AssetManager - Cannot find such name of font resource: %s.\n", _key);
	return nullptr;
}

State* AssetManager::GetState(const char *_key)
{
	auto found = m_stateMap.find(_key);
	if (found != m_stateMap.end())
		return found->second;

	JE_DEBUG_PRINT("!AssetManager - Cannot find such name of state resource: %s.\n", _key);
	return nullptr;
}

Audio* AssetManager::GetAudio(const char *_key)
{
	auto found = m_audioMap.find(_key);
	if (found != m_audioMap.end())
		return found->second;

	JE_DEBUG_PRINT("!AssetManager - Cannot find such name of audio resource: %s.\n", _key);
	return nullptr;
}

unsigned AssetManager::GetTexture(const char *_key)
{
	auto found = m_textureMap.find(_key);
	if (found != m_textureMap.end())
		return found->second;

	JE_DEBUG_PRINT("!AssetManager - Cannot find such name of texture resource: %s.\n", _key);
	return 0;
}

Archetype* AssetManager::GetArchetype(const char *_key)
{
	auto found = m_archetypeMap.find(_key);
	if (found != m_archetypeMap.end())
		return found->second;

	JE_DEBUG_PRINT("!AssetManager: Cannot find such name of archetype resource: %s.\n", _key);
	return nullptr;
}

JE_END
