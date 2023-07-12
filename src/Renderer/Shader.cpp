#include "vpch.h"
#include "Renderer/Shader.h"

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

std::string Shader::glShaderTypeToString(GLenum type) {
	switch(type) {
	case GL_VERTEX_SHADER: return "vertex";
	case GL_FRAGMENT_SHADER: return "fragment";
	case GL_GEOMETRY_SHADER: return "geometry";
	case GL_COMPUTE_SHADER: return "compute";
	default: return "other";
	}
}

void Shader::loadShader(std::string path, GLenum type) {
	std::string shaderCode = FileSystem::readFileToString(path);
	const char* rawCode = shaderCode.c_str();

	GLuint id = glCreateShader(type);
	shaderIDs.push_back(id);

	std::string filename = path.substr(path.find_last_of('/') + 1);
	shaderFileNames.push_back(filename);
	
	Log::trace("Loaded {} shader ID {} from \"{}\"", glShaderTypeToString(type), id, path);

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
		GLint binding = 0;

		glGetProgramiv(shaderProgramID, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &max_name_len);
		
		auto uniformName = std::make_unique<char[]>(max_name_len);
		for (GLint i = 0; i < uniformBufferCount; i++) {
			glGetActiveUniformBlockName(shaderProgramID, i, max_name_len, &nameLength, uniformName.get());
			glGetActiveUniformBlockiv(shaderProgramID, i, GL_UNIFORM_BLOCK_DATA_SIZE, &dataLength);
			glGetActiveUniformBlockiv(shaderProgramID, i, GL_UNIFORM_BLOCK_BINDING, &binding);

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
		Log::error("Error linking program <{}>:\n\t{}", getShaderDesc(), error);
		return true;
	}
	return false;
}

bool Shader::catchCompileError(GLuint id, const std::string& filename) {
	//glGetShaderiv wapped in an lambda to make it parameters less verbose.
	auto getShaderInfo = std::bind(&Shader::getDebugInfo<decltype(glGetShaderiv)>, this, glGetShaderiv, id, std::placeholders::_1);

	if (!getShaderInfo(GL_COMPILE_STATUS)) {
		auto error = getErrorMessage(glGetShaderInfoLog, id, getShaderInfo(GL_INFO_LOG_LENGTH));
		Log::error("Error compiling shader <{}>:\n\t{}", filename, error);
		return true;
	}

	return false;
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
