#include <string>
#include <fstream>
#include "Shader.hpp"

NS_JE_BEGIN

GLuint Shader::m_programId;

void Shader::LoadShader(
	const char* _vertex_file_path, 
	const char* _fragment_file_path) {

	bool operate = true;

	// Create the shader
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the vertex shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(_vertex_file_path, std::ios::in);
	
	// If the vertex shader file is valid,
	if (vertexShaderStream.is_open()) {
		std::string line = "";
		while (std::getline(vertexShaderStream, line)) 
			vertexShaderCode += "\n" + line;
		vertexShaderStream.close();
	}

	else {
		JE_DEBUG_PRINT("Impossible to open %s\n", _vertex_file_path);
		operate = false;
	}

	// If vertex file is valid
	if (operate) {
		
		// Read the fragment shader code from the file
		std::string fragmentShaderCode;
		std::ifstream fragmentShaderStream(_fragment_file_path, std::ios::in);

		// If the fragment shader file is valid,
		if (fragmentShaderStream.is_open()) {
			std::string line = "";
			while (std::getline(fragmentShaderStream, line))
				fragmentShaderCode += "\n" + line;
			fragmentShaderStream.close();
		}

		else 
			JE_DEBUG_PRINT("Impossible to open %s\n", _fragment_file_path);

	}
}

NS_JE_END