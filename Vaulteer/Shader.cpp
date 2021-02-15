#include "Shader.h"

Shader::Shader(std::string pathGSGLVertexCode, std::string pathGSGLFragmentCode)
{
	setup(pathGSGLVertexCode, pathGSGLFragmentCode);
}

Shader::Shader(shr::ShaderCode& shaderVertexCode, std::string pathGSGLFragmentCode)
{
	setup(shaderVertexCode.getCode(), pathGSGLFragmentCode);
}

Shader::Shader(shr::ShaderCode& shaderVertexCode, shr::ShaderCode& shaderFragmentCode)
{
	setup(shaderVertexCode.getCode(), shaderFragmentCode.getCode());
}

void Shader::use()
{
	glUseProgram(shaderProgram.id);
}

void Shader::setUniform(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(shaderProgram.id, name.c_str()), value);
}

void Shader::setMatrix(const std::string &name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

size_t Shader::getShaderID()
{
	return shaderProgram.id;
}

void Shader::setup(std::string pathGSGLVertexCode, std::string pathGSGLFragmentCode)
{

	std::string stringVertex = file_to_string(pathGSGLVertexCode);
	std::string stringFragment = file_to_string(pathGSGLFragmentCode);

	const char
		* stringGSGLVertexCode = stringVertex.c_str(),
		* stringGSGLFragmentCode = stringFragment.c_str();

	Shader_Vertex shaderVertex;
	Shader_Fragment shaderFragment;

	Shader_Shader 
		* vertexShader = &shaderVertex,
		* fragmentShader = &shaderFragment;

	shader_compile(vertexShader, &stringGSGLVertexCode);
	shader_compile(fragmentShader, &stringGSGLFragmentCode);

	shaderProgram_addShader(vertexShader);
	shaderProgram_addShader(fragmentShader);

	shaderProgram_link();

}


void Shader::shaderProgram_addShader(Shader_Shader* shader)
{
	glAttachShader(shaderProgram.id, shader->id);
}


bool Shader::shaderProgram_link()
{
	glLinkProgram(shaderProgram.id);

	return shaderProgram_catchError();
}


bool Shader::shader_compile(Shader_Shader* shader, const char** code)
{
	glShaderSource(shader->id, 1, code, NULL);
	glCompileShader(shader->id);

	return shader_catchError(shader);
}


bool Shader::shaderProgram_catchError()
{
	//Scope is needed for bind for some reason depite also giving it pointer to the class.
	auto getProgramInfo = std::bind(&Shader::getDebugInfo<decltype(glGetProgramiv)>, this, glGetProgramiv, shaderProgram.id, std::placeholders::_1);

	if (!getProgramInfo(GL_LINK_STATUS))
	{
		getErrorMessage(glGetProgramInfoLog, shaderProgram.id, getProgramInfo(GL_INFO_LOG_LENGTH), "Error: Could not link shader:");
		return true;
	}

	return false;
}

bool Shader::shader_catchError(Shader_Shader* shader)
{
	//glGetShaderiv wapped in an lambda to make it parameters less verbose.
	auto getShaderInfo = std::bind(&Shader::getDebugInfo<decltype(glGetShaderiv)>, this, glGetShaderiv, shader->id, std::placeholders::_1);

	if (!getShaderInfo(GL_COMPILE_STATUS))
	{
		getErrorMessage(glGetShaderInfoLog, shader->id, getShaderInfo(GL_INFO_LOG_LENGTH), "Error: Could not compile shader:");

		return true;
	}

	return false;
}

std::string Shader::file_to_string(std::string path)
{

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
	catch (std::ifstream::failure& e)
	{
		//Return empty string upon error.
		std::cout << "Error: Could not read file:\n" << path << std::endl;
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
	std::cout << errorMessageAppend << "\n" << log.data() << std::endl;
}