#pragma once
#include "Macro.h"
#include "GL\glew.h"

NS_JE_BEGIN

class Shader {

private:

	static void LoadShader(
		const char* _vertex_file_path,
		const char* _fragment_file_path);

public:
	
	Shader() {};
	~Shader() {};
	Shader(const Shader& /*_copy*/) {};
	void operator=(const Shader& /*_copy*/) {};

	static GLuint m_programId;
};

NS_JE_END