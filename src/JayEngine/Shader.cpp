#include <vector>
#include <string>
#include <fstream>
#include "Shader.hpp"

NS_JE_BEGIN

GLuint Shader::m_programId = 0;

void Shader::LoadShader(
	const char* _vertex_file_path, 
	const char* _fragment_file_path) {

	// Both strings are should be valid
	if (_vertex_file_path && _fragment_file_path) {

		bool vsToggle = true, fsToggle = true;

		// Create the shader
		GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the vertex shader code from the file
		std::string vertexShaderCode;
		std::ifstream vertexShaderStream(_vertex_file_path, std::ios::in);

		// Read the fragment shader code from the file
		std::string fragmentShaderCode;
		std::ifstream fragmentShaderStream(_fragment_file_path, std::ios::in);

		GLint result = GL_FALSE;
		int infoLogLength;

		// If the vertex shader file is valid,
		if (vertexShaderStream.is_open()) {
			std::string line = "";
			while (std::getline(vertexShaderStream, line))
				vertexShaderCode += "\n" + line;
			vertexShaderStream.close();

		}	// if (vertexShaderStream.is_open()) {

		else {
			JE_DEBUG_PRINT("Impossible to open %s\n", _vertex_file_path);
			vsToggle = false;

		}	// else {

		// If vertex file is valid
		// load fragement shader
		if (vsToggle) {

			// If the fragment shader file is valid,
			if (fragmentShaderStream.is_open()) {
				std::string line = "";
				while (std::getline(fragmentShaderStream, line))
					fragmentShaderCode += "\n" + line;
				fragmentShaderStream.close();

			}	// if (fragmentShaderStream.is_open()) {

			else {
				JE_DEBUG_PRINT("Impossible to open %s\n", _fragment_file_path);
				fsToggle = false;

			}	// else {

		}	// if (vsToggle) {

		// So both shaders are valid and compatible,
		if (vsToggle && fsToggle) {

			// Complie vertex shader
			JE_DEBUG_PRINT("Compiling shader: %s\n", _vertex_file_path);
			char const* vertexSourcePointer = vertexShaderCode.c_str();
			glShaderSource(vertexShaderId, 1, &vertexSourcePointer, NULL);
			glCompileShader(vertexShaderId);

			// Check vertex shader
			glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
			glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

			if (infoLogLength > 0) {
				std::vector<char> VertexShaderErrorMessage(infoLogLength + 1);
				glGetShaderInfoLog(vertexShaderId, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
				JE_DEBUG_PRINT("%4s\n", &VertexShaderErrorMessage[0]);

			}	// if (infoLogLength > 0) {

			// Complie fragment shader
			JE_DEBUG_PRINT("Compiling shader: %s\n", _fragment_file_path);

			// Get shader string
			char const* fragmentSourcePointer = fragmentShaderCode.c_str();
			glShaderSource(fragmentShaderId, 1, &fragmentSourcePointer, NULL);
			glCompileShader(fragmentShaderId);

			// Check fragment shader
			glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
			glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Check if copiled properly
			if (infoLogLength > 0) {
				std::vector<char> FragShaderErrorMessage(infoLogLength + 1);
				glGetShaderInfoLog(fragmentShaderId, infoLogLength, NULL, &FragShaderErrorMessage[0]);
				JE_DEBUG_PRINT("%4s\n", &FragShaderErrorMessage[0]);

			}	// if (infoLogLength > 0) {

			// Link the program
			JE_DEBUG_PRINT("Linking program...\n");
			m_programId = glCreateProgram();

			// Combine two shaders into the program
			glAttachShader(m_programId, vertexShaderId);
			glAttachShader(m_programId, fragmentShaderId);
			glLinkProgram(m_programId);

			// Check the program
			glGetProgramiv(m_programId, GL_LINK_STATUS, &result);
			glGetShaderiv(m_programId, GL_INFO_LOG_LENGTH, &infoLogLength);

			// Check if linked properly
			if (infoLogLength > 0) {
				std::vector<char> ProgramErrorMessage(infoLogLength + 1);
				glGetShaderInfoLog(m_programId, infoLogLength, NULL, &ProgramErrorMessage[0]);
				JE_DEBUG_PRINT("%4s\n", &ProgramErrorMessage[0]);

			}	// if (infoLogLength > 0) {

			glUseProgram(m_programId);	// Start using shade  program

			glDetachShader(m_programId, vertexShaderId);
			glDetachShader(m_programId, fragmentShaderId);

			glDeleteShader(vertexShaderId);
			glDeleteShader(fragmentShaderId);

		}	// if (vsToggle && fsToggle) {

	}	// if (_vertex_file_path && _fragment_file_path) {

	else 
		JE_DEBUG_PRINT("Shader directory is not valid...\n");
}

NS_JE_END