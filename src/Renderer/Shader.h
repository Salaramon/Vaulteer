#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <string>
#include <iostream>
#include <vector>

#include "GLSLCPPBinder.h"



class Shader {
public:
	// Parameter utility

	template<class... Args>
	struct Parameter_Reader {};

	template<class TPath, class TType, class... Args>
	struct Parameter_Reader<TPath, TType, Args...> : Parameter_Reader<Args...> {
		Parameter_Reader(Shader* shader, TPath path, TType type, Args... args)
			: Parameter_Reader<Args...>(shader, args...) {
			shader->loadShader(path, type);
		}
	};

	template<class TPath, class TType>
	struct Parameter_Reader<TPath, TType> {
	public:
		Parameter_Reader(Shader* shader, TPath path, TType type) {
			shader->loadShader(path, type);
		}
	};

	struct Shader_Info {
		Shader_Info(std::string path, GLenum type) : path(std::move(path)), type(type) {}
		std::string path;
		GLenum type;
	};


	Shader() : shaderProgramID(-1), setUniform(this) {}


	// Arguments are read in alternating std::string path, GLenum type
	template<class ...Infos>
	Shader(Infos ...infos) : setUniform(this) {

		shaderProgramID = glCreateProgram();
		
		std::cout << std::format("Shader program created with id: {}", shaderProgramID) << std::endl;

		Parameter_Reader<Infos...> reader(this, infos...);

		bool ok = linkProgram();
		if (ok)
			populateUniformCache();
		else __debugbreak();
	}

	Shader(const Shader& other) = delete;

	Shader(Shader&& other) noexcept :
			shaderProgramID(other.shaderProgramID),
			shaderIDs(std::move(other.shaderIDs)),
			uniformLocationCache(std::move(other.uniformLocationCache)),
			setUniform(this) {

		other.shaderProgramID = -1;
		other.shaderIDs.clear(); 
	}

	~Shader() {
		for (GLuint id : shaderIDs) {
			glDeleteShader(id);
		}
		std::cout << std::format("Shader program destroyed with id: {}", shaderProgramID) << std::endl;
		glDeleteProgram(shaderProgramID);
	}

	// API

	void loadShader(std::string path, GLenum type);
	void populateUniformCache();

	//Runs the shader program.
	void use() const;

	GLuint getProgramID();

	//Returns all shader files delimited with ';'
	std::string getShaderDesc();
	std::vector<std::string>& getShaderFileNames();

private:
	GLuint shaderProgramID;

	std::vector<GLuint> shaderIDs;
	std::vector<std::string> shaderFileNames;


	//Assembles the shader program.
	//Returns true on successful link.
	bool linkProgram();

	//Compiles shader
	//Returns true on successful compile.
	bool compileShader(GLuint id, const std::string& filename, const char** code);

	//Checks and logs eventual program link errors.
	bool catchProgramLinkError();

	//Checks and logs eventual shader compile errors.
	bool catchCompileError(GLuint id, const std::string& filename);



	//Takes file contents and converts it to string.
	//Returns contents of a file in string format.
	std::string readFile(std::string& path);

	//Wraps OpenGL int vector(iv) functions. !!(Warning: May be incomplete!)
	//Returns an integer representing parameters pertaining to OpenGL's object status.
	template<class T>
	int getDebugInfo(T openGLFunctioniv, unsigned int id, GLenum pname);

	//Wraps OpenGL info log functions. !!(Warning: May be incomplete!)
	template<class T>
	std::string getErrorMessage(T openGLFunctionInfoLog, unsigned int id, int logSize);


	// uniform data API related


	// struct is required for setting default values on lookup where uniforms get optimized away
	struct ShaderIndex {
		GLint loc = -1;
		operator GLint() const { return loc; }
	};

	std::unordered_map<std::string, ShaderIndex> uniformLocationCache;

public:
	struct UniformFunctor {
		UniformFunctor(Shader* shader) :
			shader(shader) {}

		void operator()(Binder::UniformInfo uniform, GLfloat value1) const {
			glUniform1f(shader->uniformLocationCache[uniform.name], value1);
			}
		void operator()(Binder::UniformInfo uniform, GLfloat value1, GLfloat value2) const {
			glUniform2f(shader->uniformLocationCache[uniform.name], value1, value2);
		}
		void operator()(Binder::UniformInfo uniform, GLfloat value1, GLfloat value2, GLfloat value3) const {
			glUniform3f(shader->uniformLocationCache[uniform.name], value1, value2, value3);
		}
		void operator()(Binder::UniformInfo uniform, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4) const {
			glUniform4f(shader->uniformLocationCache[uniform.name], value1, value2, value3, value4);
		}
		void operator()(Binder::UniformInfo uniform, GLint value1) const {
			glUniform1i(shader->uniformLocationCache[uniform.name], value1);
		}
		void operator()(Binder::UniformInfo uniform, GLint value1, GLint value2) const {
			glUniform2i(shader->uniformLocationCache[uniform.name], value1, value2);
		}
		void operator()(Binder::UniformInfo uniform, GLint value1, GLint value2, GLint value3) const {
			glUniform3i(shader->uniformLocationCache[uniform.name], value1, value2, value3);
		}
		void operator()(Binder::UniformInfo uniform, GLint value1, GLint value2, GLint value3, GLint value4) const {
			glUniform4i(shader->uniformLocationCache[uniform.name], value1, value2, value3, value4);
		}
		void operator()(Binder::UniformInfo uniform, GLuint value1) const {
			glUniform1ui(shader->uniformLocationCache[uniform.name], value1);
		}
		void operator()(Binder::UniformInfo uniform, GLuint value1, GLuint value2) const {
			glUniform2ui(shader->uniformLocationCache[uniform.name], value1, value2);
		}
		void operator()(Binder::UniformInfo uniform, GLuint value1, GLuint value2, GLuint value3) const {
			glUniform3ui(shader->uniformLocationCache[uniform.name], value1, value2, value3);
		}
		void operator()(Binder::UniformInfo uniform, GLuint value1, GLuint value2, GLuint value3, GLuint value4) const {
			glUniform4ui(shader->uniformLocationCache[uniform.name], value1, value2, value3, value4);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::vec1& value) const {
			glUniform1fv(shader->uniformLocationCache[uniform.name], count, &value[0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::vec2& value) const {
			glUniform2fv(shader->uniformLocationCache[uniform.name], count, &value[0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::vec3& value) const {
			glUniform3fv(shader->uniformLocationCache[uniform.name], count, &value[0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::vec4& value) const {
			glUniform4fv(shader->uniformLocationCache[uniform.name], count, &value[0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::ivec1& value) const {
			glUniform1iv(shader->uniformLocationCache[uniform.name], count, &value[0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::ivec2& value) const {
			glUniform2iv(shader->uniformLocationCache[uniform.name], count, &value[0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::ivec3& value) const {
			glUniform3iv(shader->uniformLocationCache[uniform.name], count, &value[0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::ivec4& value) const {
			glUniform4iv(shader->uniformLocationCache[uniform.name], count, &value[0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::uvec1& value) const {
			glUniform1uiv(shader->uniformLocationCache[uniform.name], count, &value[0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::uvec2& value) const {
			glUniform2uiv(shader->uniformLocationCache[uniform.name], count, &value[0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::uvec3& value) const {
			glUniform3uiv(shader->uniformLocationCache[uniform.name], count, &value[0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::uvec4& value) const {
			glUniform4uiv(shader->uniformLocationCache[uniform.name], count, &value[0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat2& value) const {
			glUniformMatrix2fv(shader->uniformLocationCache[uniform.name], count, transpose, &value[0][0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat3& value) const {
			glUniformMatrix3fv(shader->uniformLocationCache[uniform.name], count, transpose, &value[0][0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat4& value) const {
			glUniformMatrix4fv(shader->uniformLocationCache[uniform.name], count, transpose, &value[0][0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat2x3& value) const {
			glUniformMatrix2x3fv(shader->uniformLocationCache[uniform.name], count, transpose, &value[0][0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat3x2& value) const {
			glUniformMatrix3x2fv(shader->uniformLocationCache[uniform.name], count, transpose, &value[0][0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat2x4& value) const {
			glUniformMatrix2x4fv(shader->uniformLocationCache[uniform.name], count, transpose, &value[0][0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat4x2& value) const {
			glUniformMatrix4x2fv(shader->uniformLocationCache[uniform.name], count, transpose, &value[0][0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat3x4& value) const {
			glUniformMatrix3x4fv(shader->uniformLocationCache[uniform.name], count, transpose, &value[0][0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat4x3& value) const {
			glUniformMatrix4x3fv(shader->uniformLocationCache[uniform.name], count, transpose, &value[0][0]);
		}
	private:
		Shader* shader;
	};

	UniformFunctor setUniform;
};
