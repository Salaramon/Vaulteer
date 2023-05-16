#include "vpch.h"
#include "Renderer/Shader.h"

#include "Techniques/UniformBufferTechnique.h"

void Shader::use() const {
	glUseProgram(shaderProgramID);
}

GLuint Shader::getProgramID() {
	return shaderProgramID;
}

std::string Shader::getShaderDesc() {
	std::stringstream ss;
	for (auto& str : shaderFileNames)
		ss << str << ';';
	return ss.str();
}

std::vector<std::string>& Shader::getShaderFileNames() {
	return shaderFileNames;
}

void Shader::loadShader(std::string path, GLenum type) {
	std::cout << std::format("Loading shader \"{}\", of type {}", path, type) << std::endl;
	std::string shaderCode = readFile(path);
	const char* rawCode = shaderCode.c_str();

	GLuint id = glCreateShader(type);
	shaderIDs.push_back(id);

	std::string filename = path.substr(path.find_last_of('/') + 1);
	shaderFileNames.push_back(filename);

	std::cout << std::format("Shader created with id {} from {}", id, filename) << std::endl;

	compileShader(id, filename, &rawCode);
	glAttachShader(shaderProgramID, id);
}


void Shader::populateUniformCache() {
	uniformLocationCache.clear();

	GLint uniformCount = 0;
	glGetProgramiv(shaderProgramID, GL_ACTIVE_UNIFORMS, &uniformCount);

	if (uniformCount != 0) {
		GLint max_name_len = 0;
		GLsizei nameLength = 0;
		GLsizei count = 0;
		GLenum type = GL_NONE;
		glGetProgramiv(shaderProgramID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

		auto uniformName = std::make_unique<char[]>(max_name_len);
		for (GLint i = 0; i < uniformCount; i++) {
			glGetActiveUniform(shaderProgramID, i, max_name_len, &nameLength, &count, &type, uniformName.get());

			GLint uniformLocation = glGetUniformLocation(shaderProgramID, uniformName.get());
			uniformLocationCache.emplace(uniformName.get(), static_cast<ShaderIndex>(uniformLocation));
		}
	}


	GLint uniformBufferCount = 0;
	glGetProgramiv(shaderProgramID, GL_ACTIVE_UNIFORM_BLOCKS, &uniformBufferCount);

	if (uniformBufferCount != 0) {
		GLint max_name_len = 0;
		GLsizei nameLength = 0;
		GLsizei dataLength = 0;
		glGetProgramiv(shaderProgramID, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &max_name_len);
		
		auto uniformName = std::make_unique<char[]>(max_name_len);
		for (GLint i = 0; i < uniformBufferCount; i++) {
			glGetActiveUniformBlockName(shaderProgramID, i, max_name_len, &nameLength, uniformName.get());
			glGetActiveUniformBlockiv(shaderProgramID, i, GL_UNIFORM_BLOCK_DATA_SIZE, &dataLength);

			auto binding = glGetUniformBlockIndex(shaderProgramID, uniformName.get());
			UniformBufferTechnique::put(shaderProgramID, uniformName.get(), {uniformName.get(), binding, dataLength});
		}
	}
}


bool Shader::linkProgram() {
	glLinkProgram(shaderProgramID);
	return !catchProgramLinkError();
}


bool Shader::compileShader(GLuint id, const std::string& filename, const char** code) {
	glShaderSource(id, 1, code, nullptr);
	glCompileShader(id);

	return !catchCompileError(id, filename);
}


bool Shader::catchProgramLinkError() {
	//Scope is needed for bind for some reason depite also giving it pointer to the class.
	auto getProgramInfo = std::bind(&Shader::getDebugInfo<decltype(glGetProgramiv)>, this, glGetProgramiv, shaderProgramID, std::placeholders::_1);

	if (!getProgramInfo(GL_LINK_STATUS)) {
		auto error = getErrorMessage(glGetProgramInfoLog, shaderProgramID, getProgramInfo(GL_INFO_LOG_LENGTH));
		std::cout << std::format("Error: Could not link shader program {}:\n\t{}", getShaderDesc(), error) << std::endl;
		return true;
	}
	return false;
}

bool Shader::catchCompileError(GLuint id, const std::string& filename) {
	//glGetShaderiv wapped in an lambda to make it parameters less verbose.
	auto getShaderInfo = std::bind(&Shader::getDebugInfo<decltype(glGetShaderiv)>, this, glGetShaderiv, id, std::placeholders::_1);

	if (!getShaderInfo(GL_COMPILE_STATUS)) {
		auto error = getErrorMessage(glGetShaderInfoLog, id, getShaderInfo(GL_INFO_LOG_LENGTH));
		std::cout << std::format("Error: could not compile shader {}:\n\t{}", filename, error) << std::endl;
		return true;
	}

	return false;
}

std::string Shader::readFile(std::string& path) {
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		std::stringstream stream;

		file.open(path);
		stream << file.rdbuf();
		file.close();
		return stream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << std::format("Error reading file:\n\t{}", path) << std::endl;
		//Return empty string upon error.
		return "";
	}
}

template<class T>
int Shader::getDebugInfo(T openGLFunctioniv, unsigned int id, GLenum pname) {
	int info;
	openGLFunctioniv(id, pname, &info);
	return info;
}

template<class T>
std::string Shader::getErrorMessage(T openGLFunctionInfoLog, unsigned int id, int logSize) {
	std::vector<char> log(logSize);
	openGLFunctionInfoLog(id, logSize, NULL, log.data());

	return { log.data(), log.size() };
}
