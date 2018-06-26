#pragma once
#include "Macro.h"
#include <unordered_map>
#include "GraphicSystem.h"

jeBegin

class Mesh;
class Font;
class Audio;
class State;
class Texture;
class Archetype;

class AssetManager {

	// Keyword Definitions
	friend class Light;
	friend class Model;
	friend class Emitter;
	friend class Application;
	friend class StateManager;

	using FontMap =			std::unordered_map<std::string, Font*>;
	using AudioMap =		std::unordered_map<std::string, Audio*>;
	using StateMap =		std::unordered_map<std::string, State*>;
	using TextureMap =		std::unordered_map<std::string, unsigned>;
	using ArchetypeMap =	std::unordered_map<std::string, Archetype*>;

	// Locked constuctor, destructor, assign operator
	jeStaticClassDeclaration(AssetManager)

public:

	static void			TakeAScreenshot(const char* _directory = nullptr);

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
	
	static void ShowLoadingPercentage(unsigned _loadedPercentage, unsigned _size);

	static void LoadFont(const char* _path, const char* _audioKey, unsigned _size,
		unsigned long start, unsigned long end);
	static void LoadCharacters(Font* _pFont, float& _newLineLevel, unsigned long _start, unsigned long _end);
	static void LoadAudio(const char* _path, const char* _audioKey);
	static void LoadImage(const char* _path, const char* _textureKey);
	static void LoadArchetype(const char* _path, const char* _archetypeKey);
	static Mesh* LoadObjFile(const char* _path);

	// Private member functions
	static bool SetBuiltInComponents();
	static void LoadAssets();
	static void UnloadAssets();

	// Private member variables
	static unsigned char* m_pPixelChunk;

	static FontMap		m_fontMap;
	static AudioMap		m_audioMap;
	static StateMap		m_stateMap;
	static TextureMap	m_textureMap;
	static ArchetypeMap	m_archetypeMap; 

};

using ASSET = AssetManager;

jeEnd
