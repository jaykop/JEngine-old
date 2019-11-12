#include <application.hpp>
#include <asset_manager.hpp>

using namespace JE;

int main()
{
	// set json file directories
	AssetManager::set_initdata_directory("resource/register/initData.json");
	AssetManager::set_scene_directory("resource/register/scene.json");
	AssetManager::set_asset_directory("resource/register/asset.json");
	AssetManager::set_archetype_directory("resource/register/archetype.json");

	Application::run();

	return 0;
}