#include "vpch.h"
#include "Renderer/Shader.h"

void Shader::use() {
	glUseProgram(shaderProgramID);
	LOG::CLAS::debug<&Shader::use>(this, &shaderProgramID, std::format("Shader program with id {} is currently active", shaderProgramID));
}

size_t Shader::getShaderID() {
	return shaderProgramID;
}

void Shader::loadShader(std::string path, GLenum type) {
	LOG::CLAS::debug<&Shader::loadShader>(this, std::format("Loading shader \"{}, of type {}", path, type));
	std::string shaderCode = file_to_string(path);
	const char* rawCode = shaderCode.c_str();

	shaderIDs.push_back(glCreateShader(type));
	LOG::CLAS::debug<&Shader::loadShader>(this, std::format("Shader created with id: {}", shaderIDs.back()));

	shader_compile(shaderIDs.back(), &rawCode);
	shaderProgram_addShader(shaderIDs.back());
}

void Shader::populateUniformCache() {
	uniformLocationCache.clear();

	GLint uniformCount = 0;
	glGetProgramiv(shaderProgramID, GL_ACTIVE_UNIFORMS, &uniformCount);

	if (uniformCount != 0) {
		GLint max_name_len = 0;
		GLsizei length = 0;
		GLsizei count = 0;
		GLenum type = GL_NONE;
		glGetProgramiv(shaderProgramID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

		auto uniformName = std::make_unique<char[]>(max_name_len);
		for (GLint i = 0; i < uniformCount; i++) {
			glGetActiveUniform(shaderProgramID, i, max_name_len, &length, &count, &type, uniformName.get());

			GLint uniformLocation = glGetUniformLocation(shaderProgramID, uniformName.get());

			uniformLocationCache.emplace(uniformName.get(), uniformLocation);
		}
	}

	LOG::CLAS::debug<&Shader::populateUniformCache>(this, &uniformLocationCache, std::format("unifrom cache populated with {} uniforms", uniformLocationCache.size()));
}


void Shader::shaderProgram_addShader(GLuint id) {
	glAttachShader(shaderProgramID, id);
	LOG::CLAS::debug<&Shader::shaderProgram_addShader>(this, &shaderProgramID, std::format("shader with id {} is attached to program with id {}", id, shaderProgramID));
}


bool Shader::shaderProgram_link() {
	glLinkProgram(shaderProgramID);
	LOG::CLAS::debug<&Shader::shaderProgram_link>(this, &shaderProgramID, std::format("shader program with id {} is linked", shaderProgramID));
	return shaderProgram_catchError();
}


bool Shader::shader_compile(GLuint id, const char** code) {
	glShaderSource(id, 1, code, NULL);
	glCompileShader(id);

	LOG::CLAS::debug<&Shader::shader_compile>(this, std::format("shader with id {} is compiled", id));

	return shader_catchError(id);
}


bool Shader::shaderProgram_catchError() {
	//Scope is needed for bind for some reason depite also giving it pointer to the class.
	auto getProgramInfo = std::bind(&Shader::getDebugInfo<decltype(glGetProgramiv)>, this, glGetProgramiv, shaderProgramID, std::placeholders::_1);

	if (!getProgramInfo(GL_LINK_STATUS)) {
		getErrorMessage(glGetProgramInfoLog, shaderProgramID, getProgramInfo(GL_INFO_LOG_LENGTH), "Error: Could not link shader:");
		return true;
	}

	return false;
}

bool Shader::shader_catchError(GLuint id) {
	//glGetShaderiv wapped in an lambda to make it parameters less verbose.
	auto getShaderInfo = std::bind(&Shader::getDebugInfo<decltype(glGetShaderiv)>, this, glGetShaderiv, id, std::placeholders::_1);

	if (!getShaderInfo(GL_COMPILE_STATUS)) {
		getErrorMessage(glGetShaderInfoLog, id, getShaderInfo(GL_INFO_LOG_LENGTH), "Error: Could not compile shader:");
		return true;
	}

	return false;
}

std::string Shader::file_to_string(std::string path) {

	std::ifstream file;
	std::stringstream stream;
	std::string string;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	//Converting file to string format.
	try {

		file.open(path);
		stream << file.rdbuf();
		file.close();
		string = stream.str();

	}
	catch (std::ifstream::failure& e) {
		//Return empty string upon error.
		LOG::CLAS::debug<&Shader::file_to_string>(this, std::format("Could not read file:\n\t{}", path));
		return "";
	}

	return string;
}

template<class T>
int Shader::getDebugInfo(T openGLFunctioniv, unsigned int id, GLenum pname) {
	int info;
	openGLFunctioniv(id, pname, &info);
	return info;
}

template<class T>
void Shader::getErrorMessage(T openGLFunctionInfoLog, unsigned int id, int logSize, std::string errorMessageAppend) {
	std::vector<char> log(logSize);
	openGLFunctionInfoLog(id, logSize, NULL, log.data());
	LOG::CLAS::debug<&Shader::getErrorMessage<T>>(this, std::format("{}\n\t{}\n", errorMessageAppend, log.data()));
}
