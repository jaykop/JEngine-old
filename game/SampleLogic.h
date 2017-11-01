#pragma once
#include "Component.h"

JE_BEGIN

class SampleLogic : public Component
{
	friend class ComponentManager;

public:

	void Load(const rapidjson::Value& _data) override;
	void Init() override;
	void Update(float _dt) override;
	void Close() override;
	void Unload() override;

private:

	SampleLogic() {};
	~SampleLogic() {};
	SampleLogic(const SampleLogic& /*_copy*/) {};
	void operator=(const SampleLogic& /*_copy*/) {};

};

JE_END