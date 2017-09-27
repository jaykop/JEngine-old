#include "Sprite.h"
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

		// Get color from component and send to shader
		m_renderColor = (*sprite).GetColor();
		glUniform4f(GLManager::GetUniform(GLManager::UNIFORM_COLOR),
			m_renderColor.x, m_renderColor.y, m_renderColor.z, m_renderColor.w);

		// Render image
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void GraphicSystem::Close()
{
}

void GraphicSystem::Unload()
{
}

NS_JE_END

