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
	LoadImage("../../src/Resource/Texture/images.png", "testTexture");
	LoadImage("../../src/Resource/Texture/testAnimation.png", "testAnimation");
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

	// Texture size must be power of two for the primitive OpenGL version this is written for. 
	// Find next power of two.
	size_t u2 = 1; while (u2 < width) u2 *= 2;
	size_t v2 = 1; while (v2 < height) v2 *= 2;

	// Ratio for power of two version compared to actual version, 
	// to render the non power of two image with proper size.
	double u3 = (double)width / u2;
	double v3 = (double)height / v2;

	// Make power of two version of the image.
	Image image2(u2 * v2 * 4);
	for (size_t y = 0; y < height; y++)
		for (size_t x = 0; x < width; x++)
			for (size_t c = 0; c < 4; c++)
				image2[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];

	// Enable the texture for OpenGL.
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &newImage->m_texId);
	glBindTexture(GL_TEXTURE_2D, newImage->m_texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image2[0]);

	m_textureMap.insert(TextureMap::value_type(
		_textureKey, newImage));
	
}

void AssetManager::LoadArchetype(const char *_path, const char *_archetypeKey)
{
	// TODO
	// load archetpye assets
}

NS_JE_END
