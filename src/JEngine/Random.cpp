#include <chrono>
#include "Random.h"

jeBegin

std::mt19937 Random::randObj_;

void Random::PlantSeed()
{
	static bool planted = false;

	if (!planted) {
		auto currentTime = std::chrono::system_clock::now();
		auto duration = currentTime.time_since_epoch();
		unsigned milliseconds = static_cast<unsigned>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
		randObj_ = std::mt19937(milliseconds);
		planted = true;
		jeDebugPrint("*Random - Planted random seed object.\n");
	}

	else
		jeDebugPrint("!Random - Already planted.\n");
}

int Random::GetRandomInt(int min, int max) 
{
	if (min > max) {
		jeDebugPrint("!Random - Set minimum lower than maximum / returning 0.\n");
		return 0;
	}

	std::uniform_int_distribution<int> 	intRand(min, max);
	return intRand(randObj_);
}

bool Random::GetRandBoolean(float probabilityOfTrue)
{
	std::bernoulli_distribution boolRand(probabilityOfTrue);
	return boolRand(randObj_);
}

vec3 Random::GetRandVec3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	return vec3(GetRandomFloat(minX, maxX),
		GetRandomFloat(minY, maxY),
		GetRandomFloat(minZ, maxZ));
}

vec4 Random::GetRandVec4(float minX, float minY, float minZ, float minW, float maxX, float maxY, float maxZ, float maxW)
{
	return vec4(GetRandomFloat(minX, maxX),
		GetRandomFloat(minY, maxY),
		GetRandomFloat(minZ, maxZ),
		GetRandomFloat(minW, maxW));
}

vec3 Random::GetRandVec3(const vec3& minVec3, const vec3& maxVec3)
{
	return vec3(GetRandomFloat(minVec3.x, maxVec3.x),
		GetRandomFloat(minVec3.y, maxVec3.y),
		GetRandomFloat(minVec3.z, maxVec3.z));
}

vec4 Random::GetRandVec4(const vec4& minVec4, const vec4& maxVec4)
{
	return vec4(GetRandomFloat(minVec4.x, maxVec4.x),
		GetRandomFloat(minVec4.y, maxVec4.y),
		GetRandomFloat(minVec4.z, maxVec4.z),
		GetRandomFloat(minVec4.w, maxVec4.w));
}

float Random::GetRandomFloat(float min, float max) 
{
	if (min > max) {
		std::uniform_real_distribution<float>	floatRand(max, min);
		return floatRand(randObj_);
	}

	std::uniform_real_distribution<float>	floatRand(min, max);
	return floatRand(randObj_);
}

jeEnd
