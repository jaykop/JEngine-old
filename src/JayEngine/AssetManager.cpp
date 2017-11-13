#include "GL\glew.h"
#include "lodepng.h"
#include "State.h"
#include "AssetManager.h"
#include "JsonParser.h"
#include "StateManager.h"
#include "ComponentBuilder.h"
#include "ComponentManager.h"

// Built-In Component Headers
#include "GraphicComponents.h"
#include "PhysicsComponents.h"
#include "SoundComponents.h"

JE_BEGIN

// Declare static member variables
ASSET::AudioMap		ASSET::m_audioMap;
ASSET::StateMap		ASSET::m_stateMap;
ASSET::TextureMap	ASSET::m_textureMap;
ASSET::ArchetypeMap	ASSET::m_archetypeMap;
std::string			ASSET::m_initDirectory;
std::string			ASSET::m_assetDirectory;
std::string			ASSET::m_stateDirectory;
std::string			ASSET::m_archeDirectory;

void AssetManager::Load()
{
	// Load built-in components

	// Physics components
	JE_ADD_COMPONENT(Transform);

	// Graphic components
	JE_ADD_COMPONENT(Model);
	JE_ADD_COMPONENT(Camera);
	JE_ADD_COMPONENT(Sprite);
	JE_ADD_COMPONENT(Light);
	JE_ADD_COMPONENT(Material);
	JE_ADD_COMPONENT(Animation);

	// Load states
	JSON::ReadFile(ASSET::m_stateDirectory.c_str());
	CR_RJValue states = JSON::GetDocument()["State"];
	for (rapidjson::SizeType i = 0; i < states.Size(); ++i) 
		STATE::PushState(states[i]["Directory"].GetString(), states[i]["Key"].GetString());

	// Load images
	JSON::ReadFile(ASSET::m_assetDirectory.c_str());
	CR_RJValue textures = JSON::GetDocument()["Texture"];
	for (rapidjson::SizeType i = 0; i < textures.Size(); ++i) 
		LoadImage(textures[i]["Directory"].GetString(), textures[i]["Key"].GetString());
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

State* AssetManager::GetState(const char *_key)
{
	auto found = m_stateMap.find(_key);
	if (found != m_stateMap.end())
		return found->second;

	JE_DEBUG_PRINT("Cannot find such name of state resource: %s.\n", _key);
	return nullptr;
}

Audio* AssetManager::GetAudio(const char *_key)
{
	auto found = m_audioMap.find(_key);
	if (found != m_audioMap.end())
		return found->second;

	JE_DEBUG_PRINT("Cannot find such name of audio resource: %s.\n", _key);
	return nullptr;
}

unsigned AssetManager::GetTexture(const char *_key)
{
	auto found = m_textureMap.find(_key);
	if (found != m_textureMap.end())
		return found->second;

	JE_DEBUG_PRINT("Cannot find such name of texture resource: %s.\n", _key);
	return 0;
}

Archetype* AssetManager::GetArchetype(const char *_key)
{
	auto found = m_archetypeMap.find(_key);
	if (found != m_archetypeMap.end())
		return found->second;

	JE_DEBUG_PRINT("Cannot find such name of archetype resource: %s.\n", _key);
	return nullptr;
}

void AssetManager::LoadAudio(const char* /*_path*/, const char* /*_audioKey*/)
{
	// TODO
	// load audio assets
}

void AssetManager::LoadImage(const char *_path, const char *_textureKey)
{
	unsigned	newImage;
	Image		image;
	unsigned	width, height;
	unsigned	error = lodepng::decode(image, width, height, _path);

	if (error)
		JE_DEBUG_PRINT("decoder error %d: %s.\n", error, lodepng_error_text(error));

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

JE_END
