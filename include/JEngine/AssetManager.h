#pragma once
#include "Macro.h"
#include <unordered_map>
#include "GraphicSystem.h"

struct SDL_Window;

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

	struct Image {
		std::vector<unsigned char> pixels;
		unsigned handle, width, height;
	};

	// Locked constuctor, destructor, assign operator
	jeStaticClassDeclaration(AssetManager)

public:

	static void		DrawLoadingScreen(SDL_Window* pWindow, const char* directory);
	static void		TakeAScreenshot(const char* directory = nullptr);

	static void		SetInitDirectory(const char* dir);
	static void		SetAssetDirectory(const char* dir);
	static void		SetStateDirectory(const char* dir);
	static void		SetArchetypeDirectory(const char* dir);

	static Font*		GetFont(const char* key);
	static State*		GetState(const char* key);
	static Audio*		GetAudio(const char* key);
	static unsigned		GetTexture(const char* key);
	static Archetype*	GetArchetype(const char* key);

private:

	static std::string initDirectory_, assetDirectory_,
		stateDirectory_, archeDirectory_;
	
	static void ShowLoadingPercentage(unsigned loadedPercentage, unsigned size);

	static void LoadFont(const char* path, const char* audioKey, unsigned size,
		unsigned long start, unsigned long end);
	static void LoadCharacters(Font* pFont, float& newLineLevel, unsigned long start, unsigned long end);
	static void LoadAudio(const char* path, const char* audioKey);
	static void LoadImage(const char* path, const char* textureKey);
	static void RegisterImage(Image& image, const char* textureKey);
	static void LoadArchetype(const char* path, const char* archetypeKey);
	static Mesh* LoadObjFile(const char* path);

	// Private member functions
	static bool SetBuiltInComponents();
	static void LoadAssets();
	static void UnloadAssets();

	// Private member variables
	static unsigned char* pPixelChunk_;
	static std::unordered_map<std::string, Image> images_;

	static FontMap		fontMap_;
	static AudioMap		audioMap_;
	static StateMap		stateMap_;
	static TextureMap	textureMap_;
	static ArchetypeMap	archetypeMap_; 

};

using ASSET = AssetManager;

jeEnd
