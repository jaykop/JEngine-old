/******************************************************************************/
/*!
\file   asset_manager.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/03(yy/mm/dd)

\description
Contains the definition of asset_manager class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>
#include <unordered_map>
// #include <graphic_system.h>

jeBegin

class Mesh;
class Audio;
class Scene;
class Texture;
class Archetype;
struct Font;

// generic asset manager class
class AssetManager {

	friend class Application;

	using FontMap =	std::unordered_map<std::string, Font*>;
	using AudioMap = std::unordered_map<std::string, Audio*>;
	using SceneMap = std::unordered_map<std::string, Scene*>;
	using TextureMap = std::unordered_map<std::string, unsigned>;
	using ArchetypeMap = std::unordered_map<std::string, Archetype*>;

	struct Image {
		std::vector<unsigned char> pixels;
		unsigned handle, width, height;
	};

public:

	static void generate_screenshot(const char* directory);

	static void	set_initdata_directory(const char* dir);
	static void	set_asset_directory(const char* dir);
	static void	set_scene_directory(const char* dir);
	static void	set_archetype_directory(const char* dir);

	static Font* get_font(const char* key);
	static Scene* get_scene(const char* key);
	static Audio* get_audio(const char* key);
	static unsigned	get_texture(const char* key);
	static Archetype* get_archetype(const char* key);

private:

	static void load_shaders();
	static void load_font(const char* path, const char* audioKey, unsigned size,
		unsigned long start, unsigned long end);
	static void load_characters(Font* font, float& newLineLevel, unsigned long start, unsigned long end);
	static void load_audio(const char* path, const char* audioKey);
	static void load_image(const char* path, const char* textureKey);
	static void register_image(Image& image, const char* textureKey);
	static void load_archetype(const char* path, const char* archetypeKey);
	static Mesh* load_object(const char* path);
	
	static bool set_bulit_in_components();
	static void load_assets();
	static void unload_assets();

	static unsigned char* pixel_chunk;
	static std::unordered_map<std::string, Image> images_;

	static FontMap		fontMap_;
	static AudioMap		audioMap_;
	static SceneMap		sceneMap_;
	static TextureMap	textureMap_;
	static ArchetypeMap	archetypeMap_;

	static std::string initDirectory_, assetDirectory_,
		stateDirectory_, archeDirectory_;

};

jeEnd