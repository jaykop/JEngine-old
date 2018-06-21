#include <chrono>
#include "Random.h"

jeBegin

std::mt19937 Random::m_randomObject;

void Random::PlantSeed()
{
	static bool planted = false;

	if (!planted) {
		auto currentTime = std::chrono::system_clock::now();
		auto duration = currentTime.time_since_epoch();
		unsigned milliseconds = static_cast<unsigned>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
		m_randomObject = std::mt19937(milliseconds);
		planted = true;
		jeDebugPrint("*Random - Planted random seed object.\n");
	}

	else
		jeDebugPrint("!Random - Already planted.\n");
}

int Random::GetRandomInt(int _min, int _max) 
{
	if (_min > _max) {
		jeDebugPrint("!Random - Set minimum lower than maximum / returning 0.\n");
		return 0;
	}

	std::uniform_int_distribution<int> 	intRand(_min, _max);
	return intRand(m_randomObject);
}

bool Random::GetRandBoolean(float _probabilityOfTrue)
{
	std::bernoulli_distribution boolRand(_probabilityOfTrue);
	return boolRand(m_randomObject);
}

vec3 Random::GetRandVec3(float _minX, float _minY, float _minZ, float _maxX, float _maxY, float _maxZ)
{
	return vec3(GetRandomFloat(_minX, _maxX),
		GetRandomFloat(_minY, _maxY),
		GetRandomFloat(_minZ, _maxZ));
}

vec4 Random::GetRandVec4(float _minX, float _minY, float _minZ, float _minW, float _maxX, float _maxY, float _maxZ, float _maxW)
{
	return vec4(GetRandomFloat(_minX, _maxX),
		GetRandomFloat(_minY, _maxY),
		GetRandomFloat(_minZ, _maxZ),
		GetRandomFloat(_minW, _maxW));
}

vec3 Random::GetRandVec3(const vec3& _minVec3, const vec3& _maxVec3)
{
	return vec3(GetRandomFloat(_minVec3.x, _maxVec3.x),
		GetRandomFloat(_minVec3.y, _maxVec3.y),
		GetRandomFloat(_minVec3.z, _maxVec3.z));
}

vec4 Random::GetRandVec4(const vec4& _minVec4, const vec4& _maxVec4)
{
	return vec4(GetRandomFloat(_minVec4.x, _maxVec4.x),
		GetRandomFloat(_minVec4.y, _maxVec4.y),
		GetRandomFloat(_minVec4.z, _maxVec4.z),
		GetRandomFloat(_minVec4.w, _maxVec4.w));
}

float Random::GetRandomFloat(float _min, float _max) 
{
	if (_min > _max) {
		jeDebugPrint("!Random - Set minimum lower than maximum / returning 0.\n");
		return 0.f;
	}

	std::uniform_real_distribution<float>	floatRand(_min, _max);
	return floatRand(m_randomObject);
}

jeEnd
