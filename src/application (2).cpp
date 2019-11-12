/******************************************************************************/
/*!
\file   application.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/02(yy/mm/dd)

\description
Contains the methods of application class
*/
/******************************************************************************/

#include <gl_manager.hpp>
#include <application.hpp>
#include <random.hpp>
#include <json_parser.hpp>
#include <debug_tools.hpp>
#include <scene_manager.hpp>
#include <asset_manager.hpp>
#include <input_handler.hpp>

jeBegin

// initialize the static variables
bool Application::run_ = true;
sf::Event Application::event_;
Application::AppData Application::data_;
sf::Window Application::window_;

void Application::quit() 
{
	run_ = false;
	SceneManager::status_ = SceneManager::SceneStatus::JE_STATE_QUIT;
}

void Application::run()
{
	// pop a console window
	DEBUG_LEAK_CHECKS(-1);
	DEBUG_CREATE_CONSOLE();

	// if initialization succeeded, run the app
	if (initialize()) {
		
		update();
		close();
	}

	// destroy the console window
	DEBUG_DESTROY_CONSOLE();
}

bool Application::initialize()
{

	// load app init data
	JsonParser::read_file(AssetManager::initDirectory_.c_str());

	const rapidjson::Value& title = JsonParser::get_document()["Title"];
	const rapidjson::Value& fullscreen = JsonParser::get_document()["Fullscreen"];
	const rapidjson::Value& width = JsonParser::get_document()["Width"];
	const rapidjson::Value& height = JsonParser::get_document()["Height"];
	const rapidjson::Value& icon = JsonParser::get_document()["Icon"];

	if (title.IsString() && fullscreen.IsBool()
		&& width.IsInt() && height.IsInt()
		&& icon.IsString()) {

		data_.title.assign(title.GetString());
		data_.icon.assign(icon.GetString());
		data_.isFullscreen = fullscreen.GetBool();
		data_.width = width.GetInt();
		data_.height = height.GetInt();
	}

	Random::seed();

	// instantiate a context to load Opengl resources and load shaders
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	// todo
	// settings.antialiasingLevel = 2;
	settings.majorVersion = 4;
	settings.minorVersion = 5;
	settings.attributeFlags = sf::ContextSettings::Core;

	// create a window
	window_.create(
		sf::VideoMode(data_.width, data_.height),
		data_.title.c_str(),
		sf::Style::Titlebar,
		settings);

	window_.setFramerateLimit(60);	// limit 60 frames
	window_.setVerticalSyncEnabled(true); // Activate vSync

	// activate the OpenGL context
	window_.setActive(true);

	// TODO: REDO THE FULLSCREEN PART
	// Create window
	/*if (data_.isFullscreen)
		window_.create(sf::VideoMode(data_.width, data_.height), data_.title, sf::Style::Fullscreen);
	else */

	AssetManager::load_shaders();
	GLManager::initialize(float(data_.width), float(data_.height));

	// initialize components and assets
	AssetManager::set_bulit_in_components();
	AssetManager::load_assets();

	// initialize key map
	InputHandler::initialize();

	return true;
}

// the process handling scene manager is handled here
void Application::update()
{
	// If the intiailization does not have done properly,
	// stop updating the scene manager
	if (!SceneManager::initialize(&window_))
		return;

	// launch the rednering thread
	// sf::Thread thread(&rendering_thread);
	// thread.launch();

	// update the window 
	while (run_) 
		SceneManager::update(&event_); // update the scene

	SceneManager::close(); // close the scene manager
}

void Application::close()
{
	// deactivate the OpenGL context
	window_.setActive(false);

	InputHandler::close(); // close the input handler
	AssetManager::unload_assets();
	GLManager::close();
	window_.close();
}

//todo
//void Application::rendering_thread()
//{
//	// activate the window's context
//	window_.setActive(true);
//
//	while (run_) {
//		
//		//TODO
//		// Draw...
//
//		// end the current framee --- this is a rendering function
//		// this requires the context activated
//		// window_.display();
//	}
//}

jeEnd