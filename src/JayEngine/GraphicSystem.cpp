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

void GraphicSystem::AddSprite(Sprite* _sprite)
{
	m_sprites.push_back(_sprite);
}

void GraphicSystem::RemoveSprite(Sprite* _sprite)
{
	for (Sprites::iterator it = m_sprites.begin();
		it != m_sprites.end(); ++it) {
		if ((*it)->m_id == _sprite->m_id) {
			m_sprites.erase(it);
			break;
		}
	}
}

NS_JE_END

