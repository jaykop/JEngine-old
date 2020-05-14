#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <renderer.hpp>
#include <shader.hpp>
#include <graphic_system.hpp>

jeBegin

jeDefineComponentBuilder(Renderer);

void Renderer::add_to_system() {
	GraphicSystem::add_model(this);
}

void Renderer::remove_from_system() {
	GraphicSystem::remove_model(this);
}

void Renderer::load(const rapidjson::Value& data) {

}

Renderer::Renderer(Object* owner)
: Component(owner) {
	;
}

void Renderer::draw(Shader* shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].draw(shader);
}

jeEnd