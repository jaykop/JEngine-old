#pragma once
#include <random>
#include "Vector3.h"
#include "Vector4.h"

JE_BEGIN

class Random {

	friend class Application;

public:

	static float	GetRandomFloat(float _min, float _max);
	static int		GetRandomInt(int _min, int _max);
	static bool		GetRandBoolean(float _probabilityOfTrue);
	static vec3		GetRandVec3(float  _min, float _max);
	static vec4		GetRandVec4(float  _min, float  _max);
	static vec3		GetRandVec3(float _x, float _y, float _z);
	static vec4		GetRandVec4(float _x, float _y, float _z, float _w);

private:
	
	static void	PlantSeed(); 
	
	static std::mt19937	m_randomObject;

	Random() = delete;
	Random(const Random& /*_copy*/) = delete;
	void operator=(const Random& /*_copy*/) = delete;

};

JE_END