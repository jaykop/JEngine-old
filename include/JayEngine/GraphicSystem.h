#pragma once
#include <vector>
#include "System.h"

NS_JE_BEGIN

class Sprite;
class GraphicSystem : public System
{

public:

	friend class SystemManager;

	void Load() override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;

private:

	GraphicSystem();
	~GraphicSystem() {};
	GraphicSystem(const GraphicSystem& /*_copy*/) {};
	void operator=(const GraphicSystem& /*_copy*/) {};

	std::vector<Sprite*> m_sprites;
};

NS_JE_END