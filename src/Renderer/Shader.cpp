#include "vpch.h"
#include "Renderer/Shader.h"

void Shader::use() {
	glUseProgram(shaderProgramID);
	LOG::CLAS::debug<&Shader::use>(this, &shaderProgramID, std::format("Shader program with id {} is currently active", shaderProgramID));
}

size_t Shader::getShaderID() {
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
	LOG::CLAS::debug<&Shader::loadShader>(this, std::format("Loading shader \"{}\", of type {}", path, type));
	std::string shaderCode = file_to_string(path);
	const char* rawCode = shaderCode.c_str();

	GLuint id = glCreateShader(type);
	shaderIDs.push_back(id);

	std::string filename = path.substr(path.find_last_of('/') + 1);
	shaderFileNames.push_back(filename);

	LOG::CLAS::debug<&Shader::loadShader>(this, std::format("Shader created with id {} from {}", id, filename));

	shader_compile(id, filename, &rawCode);
	shaderProgram_addShader(id);
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

			uniformLocationCache.emplace(uniformName.get(), static_cast<ShaderLocation>(uniformLocation));
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


bool Shader::shader_compile(GLuint id, std::string& filename, const char** code) {
	glShaderSource(id, 1, code, NULL);
	glCompileShader(id);

	LOG::CLAS::debug<&Shader::shader_compile>(this, std::format("shader {} is compiled", filename));

	return shader_catchError(id, filename);
}


bool Shader::shaderProgram_catchError() {
	//Scope is needed for bind for some reason depite also giving it pointer to the class.
	auto getProgramInfo = std::bind(&Shader::getDebugInfo<decltype(glGetProgramiv)>, this, glGetProgramiv, shaderProgramID, std::placeholders::_1);

	if (!getProgramInfo(GL_LINK_STATUS)) {
		getErrorMessage(glGetProgramInfoLog, shaderProgramID, getProgramInfo(GL_INFO_LOG_LENGTH), std::format("Error: Could not link shader program {}", getShaderDesc()));
		return true;
	}

	return false;
}

bool Shader::shader_catchError(GLuint id, std::string& filename) {
	//glGetShaderiv wapped in an lambda to make it parameters less verbose.
	auto getShaderInfo = std::bind(&Shader::getDebugInfo<decltype(glGetShaderiv)>, this, glGetShaderiv, id, std::placeholders::_1);

	if (!getShaderInfo(GL_COMPILE_STATUS)) {
		getErrorMessage(glGetShaderInfoLog, id, getShaderInfo(GL_INFO_LOG_LENGTH), std::format("Error: Could not compile shader {}:", filename));
		return true;
	}

	return false;
}

std::string Shader::file_to_string(std::string& path) {
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	//Converting file to string format.
	try {
		std::stringstream stream;

		file.open(path);
		stream << file.rdbuf();
		file.close();
		return stream.str();
	}
	catch (std::ifstream::failure& e) {
#ifdef DEBUG_ENABLED
		LOG::CLAS::debug<&Shader::file_to_string>(this, std::format("Error reading file:\n\t{}", path));
#else
		std::cout << std::format("Error reading file:\n\t{}", path) << std::endl;
#endif
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
void Shader::getErrorMessage(T openGLFunctionInfoLog, unsigned int id, int logSize, std::string errorMessagePrepend) {
	std::vector<char> log(logSize);
	openGLFunctionInfoLog(id, logSize, NULL, log.data());

#ifdef DEBUG_ENABLED
	LOG::CLAS::debug<&Shader::getErrorMessage<T>>(this, std::vector<DY::SQLTypes::TEXT>({Tag::Debug::DirectOutput}), std::format("{}\n\t{}\n", errorMessageAppend, log.data()));
#else
	std::cout << std::format("{}\n\t{}\n", errorMessagePrepend, log.data()) << std::endl;
#endif
}
