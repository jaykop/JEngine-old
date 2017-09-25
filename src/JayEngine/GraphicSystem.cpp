#include "GLManager.h"
#include "GraphicSystem.h"

NS_JE_BEGIN

GraphicSystem::GraphicSystem()
	:System()
{
}

void GraphicSystem::Load()
{
}

void GraphicSystem::Init()
{
}

void GraphicSystem::Update(float /*dt*/)
{
	// Update sprites
	for (auto sprite : m_sprites) {
		;
	}

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
}

void GraphicSystem::Close()
{
}

void GraphicSystem::Unload()
{
}

NS_JE_END

