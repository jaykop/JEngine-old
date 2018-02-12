#pragma once
#include "Macro.h"
#include <vector>
#include <unordered_map>

JE_BEGIN

class Font;
class Audio;
class State;
class Texture;
class Archetype;

class AssetManager {

	// Keyword Definitions
	friend class Application;
	friend class StateManager;

	using FontMap =			std::unordered_map<std::string, Font*>;
	using AudioMap =		std::unordered_map<std::string, Audio*>;
	using StateMap =		std::unordered_map<std::string, State*>;
	using TextureMap =		std::unordered_map<std::string, unsigned>;
	using ArchetypeMap =	std::unordered_map<std::string, Archetype*>;

public:

	static void			SetInitDirectory(const char* _dir);
	static void			SetAssetDirectory(const char* _dir);
	static void			SetStateDirectory(const char* _dir);
	static void			SetArchetypeDirectory(const char* _dir);

	static Font*		GetFont(const char* _key);
	static State*		GetState(const char* _key);
	static Audio*		GetAudio(const char* _key);
	static unsigned		GetTexture(const char* _key);
	static Archetype*	GetArchetype(const char* _key);

private:

	static std::string m_initDirectory, m_assetDirectory,
		m_stateDirectory, m_archeDirectory;
	
	static void LoadFont(const char* _path, const char* _audioKey, unsigned _size);
	static void LoadAudio(const char* _path, const char* _audioKey);
	static void LoadImage(const char* _path, const char* _textureKey);
	static void LoadArchetype(const char* _path, const char* _archetypeKey);

	// Locked constructors and destructor
	AssetManager() = delete;
	~AssetManager() = delete;
	AssetManager(const AssetManager& /*_copy*/) = delete;
	void operator=(const AssetManager& /*_copy*/) = delete;

	// Private member functions
	static void LoadBuiltInComponents();
	static void Load();
	static void Unload();

	// Private member variables
	static FontMap		m_fontMap;
	static AudioMap		m_audioMap;
	static StateMap		m_stateMap;
	static TextureMap	m_textureMap;
	static ArchetypeMap	m_archetypeMap; 

};

using ASSET = AssetManager;

JE_END
