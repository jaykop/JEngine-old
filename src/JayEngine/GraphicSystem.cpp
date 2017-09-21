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
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void GraphicSystem::Close()
{
}

void GraphicSystem::Unload()
{
}

NS_JE_END

