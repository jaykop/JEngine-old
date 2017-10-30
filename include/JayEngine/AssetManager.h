#pragma once
#include "Macro.h"
#include <vector>
#include <unordered_map>

JE_BEGIN

class Audio;
class State;
class Texture;
class Archetype;

class AssetManager {

	// Keyword Definitions
	friend class Application;
	friend class StateManager;
	typedef std::vector<unsigned char>					Image;
	typedef std::unordered_map<std::string, Audio*>		AudioMap;
	typedef std::unordered_map<std::string, State*>		StateMap;
	typedef std::unordered_map<std::string, unsigned>	TextureMap;
	typedef std::unordered_map<std::string, Archetype*>	ArchetypeMap;

public:

	//static void LoadState(const char* _path, const char* _stateKey);
	static void LoadAudio(const char* _path, const char* _audioKey);
	static void LoadImage(const char* _path, const char* _textureKey);
	static void LoadArchetype(const char* _path, const char* _archetypeKey);

	static State*		GetState(const char* _key);
	static Audio*		GetAudio(const char* _key);
	static unsigned		GetTexture(const char* _key);
	static Archetype*	GetArchetype(const char* _key);

private:
	
	// Locked constructors and destructor
	AssetManager() {};
	~AssetManager() {};
	AssetManager(const AssetManager& /*_copy*/) {};
	void operator=(const AssetManager& /*_copy*/) {};

	// Private member functions
	static void Load();
	static void Unload();

	// Private member variables
	static AudioMap		m_audioMap;
	static StateMap		m_stateMap;
	static TextureMap	m_textureMap;
	static ArchetypeMap	m_archetypeMap;

};

typedef AssetManager ASSET;

JE_END
