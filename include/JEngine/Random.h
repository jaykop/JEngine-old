#pragma once
#include <random>
#include "Vector3.h"
#include "Vector4.h"

JE_BEGIN

class Random {

	friend class Application;

	// Locked constuctor, destructor, assign operator
	JE_THIS_IS_STATIC_CLASS(Random)

public:

	static float	GetRandomFloat(float _min, float _max);
	static int		GetRandomInt(int _min, int _max);
	static bool		GetRandBoolean(float _probabilityOfTrue);
	static vec3		GetRandVec3(float _minX, float _minY, float _minZ, float _maxX, float _maxY, float _maxZ);
	static vec4		GetRandVec4(float _minX, float _minY, float _minZ, float _minW, float _maxX, float _maxY, float _maxZ, float _maxW);
	static vec3		GetRandVec3(const vec3& _minVec3, const vec3& _maxVec3);
	static vec4		GetRandVec4(const vec4& _minVec4, const vec4& _maxVec4);

private:
	
	static void	PlantSeed(); 
	
	static std::mt19937	m_randomObject;

};

JE_END