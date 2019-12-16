#pragma once
#include <random>
#include "Vector3.h"
#include "Vector4.h"

jeBegin

class Random {

	friend class Application;

	// Locked constuctor, destructor, assign operator
	jeStaticClassDeclaration(Random)

public:

	static float	GetRandomFloat(float min, float max);
	static int		GetRandomInt(int min, int max);
	static bool		GetRandBoolean(float probabilityOfTrue);
	static vec3		GetRandVec3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	static vec4		GetRandVec4(float minX, float minY, float minZ, float minW, float maxX, float maxY, float maxZ, float maxW);
	static vec3		GetRandVec3(const vec3& minVec3, const vec3& maxVec3);
	static vec4		GetRandVec4(const vec4& minVec4, const vec4& maxVec4);

private:
	
	static void	PlantSeed(); 
	
	static std::mt19937	randObj_;

};

using RAND = Random;

jeEnd
