#include "GL\glew.h"
#include "lodepng.h"
#include "State.h"
#include "Texture.h"
#include "Application.h"
#include "AssetManager.h"

NS_JE_BEGIN

// Declare static member variables
AssetManager::AudioMap		AssetManager::m_audioMap;
AssetManager::StateMap		AssetManager::m_stateMap;
AssetManager::TextureMap	AssetManager::m_textureMap;
AssetManager::ArchetypeMap	AssetManager::m_archetypeMap;

void AssetManager::Load()
{
	LoadImage("../src/resource/texture/rect.png", "rect");
	LoadImage("../src/resource/texture/circle.png", "circle");
	LoadImage("../src/resource/texture/images.png", "testTexture");
	LoadImage("../src/resource/texture/testAnimation.png", "testAnimation");
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
	for (auto texture : m_textureMap) {
		if (texture.second) {
			delete texture.second;
			texture.second = nullptr;
		}
	}

	//// Clear archetype map
	//for (auto archetype : m_archetypeMap) {
	//	if (archetype.second) {
	//		delete archetype.second;
	//		archetype.second = nullptr;
	//	}
	//}
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

Texture* AssetManager::GetTexture(const char *_key)
{
	auto found = m_textureMap.find(_key);
	if (found != m_textureMap.end())
		return found->second;

	JE_DEBUG_PRINT("Cannot find such name of texture resource: %s.\n", _key);
	return nullptr;
}

Archetype* AssetManager::GetArchetype(const char *_key)
{
	auto found = m_archetypeMap.find(_key);
	if (found != m_archetypeMap.end())
		return found->second;

	JE_DEBUG_PRINT("Cannot find such name of archetype resource: %s.\n", _key);
	return nullptr;
}

void AssetManager::LoadState(const char *_path, const char *_stateKey)
{
	// TODO
	// load state assets
}

void AssetManager::LoadAudio(const char *_path, const char *_audioKey)
{
	// TODO
	// load audio assets
}

void AssetManager::LoadImage(const char *_path, const char *_textureKey)
{
	Texture		*newImage = new Texture;
	Image		image;
	unsigned	width, height;
	unsigned	error = lodepng::decode(image, width, height, _path);

	if (error)
		JE_DEBUG_PRINT("decoder error %d: %s.\n", error, lodepng_error_text(error));

	// Enable the texture for OpenGL.
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &newImage->m_texId);
	glBindTexture(GL_TEXTURE_2D, newImage->m_texId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	m_textureMap.insert(TextureMap::value_type(
		_textureKey, newImage));
	
}

void AssetManager::LoadArchetype(const char *_path, const char *_archetypeKey)
{
	// TODO
	// load archetpye assets
}

NS_JE_END
