#include "Random.h"
#include <chrono>
JE_BEGIN

std::mt19937 Random::m_randomObject;

void Random::PlantSeed()
{
	static bool planted = false;

	if (!planted) {
		auto currentTime = std::chrono::system_clock::now();
		auto duration = currentTime.time_since_epoch();;
		unsigned milliseconds = static_cast<unsigned>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
		m_randomObject = std::mt19937(milliseconds);
		planted = true;
		JE_DEBUG_PRINT("Planted random seed.\n");
	}

	else
		JE_DEBUG_PRINT("Already planted.\n");
}

int Random::GetRandomInt(int _min, int _max) 
{
	std::uniform_int_distribution<int> 	intRand(_min, _max);
	return intRand(m_randomObject);
}

bool Random::GetRandBoolean(float _probabilityOfTrue)
{
	std::bernoulli_distribution boolRand(_probabilityOfTrue);
	return boolRand(m_randomObject);
}

vec3 Random::GetRandVec3(float _min, float _max)
{
	return vec3(GetRandomFloat(_min, _max), 
		GetRandomFloat(_min, _max),
		GetRandomFloat(_min, _max));
}

vec4 Random::GetRandVec4(float _min, float _max)
{
	return vec4(GetRandomFloat(_min, _max),
		GetRandomFloat(_min, _max),
		GetRandomFloat(_min, _max),
		GetRandomFloat(_min, _max));
}

float Random::GetRandomFloat(float _min, float _max) 
{
	std::uniform_real_distribution<float>	floatRand(_min, _max);
	return floatRand(m_randomObject);
}

JE_END