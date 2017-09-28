#pragma once
#include <vector>
#include "System.h"
#include "Vector4.h"

NS_JE_BEGIN

class Sprite;
class GraphicSystem : public System
{

public:

	typedef std::vector<Sprite*> Sprites;

	friend class Sprite;
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

	void AddSprite(Sprite* _sprite);
	void RemoveSprite(Sprite* _sprite);

	Sprites m_sprites;
	vec4 m_renderColor;
};

NS_JE_END