#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>
#include <vector>

#include "GLSLCPPBinder.h"

#include "Debug/Debug.h"

class Shader {
public:
	template<class... Args>
	struct Parameter_Reader {};

	template<class TPath, class TType, class... Args>
	struct Parameter_Reader<TPath, TType, Args...> : public Parameter_Reader<Args...> {
	public:
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
		Shader_Info(std::string path, GLenum type) : path(path), type(type) {}
		std::string path;
		GLenum type;
	};

	//=============================================================================================================================================//


		/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
			|		  CONSTRUCTORS 			|
			|_______________________________|	*/

			//Loads vertex and fragment shaders on creation.
	template<class ...Infos>
	Shader(Infos ...infos) :
		OR(this,
			DY::V(
			&shaderProgramID,
			&shaderIDs,
			&uniformLocationCache,
			&setUniform),
			DY::N(
			"shaderProgramID",
			"shaderIDs",
			"uniformLocationCache",
			"setUniform")),

		setUniform(this) {
		OB.add(OR);

		shaderProgramID = glCreateProgram();
		
		LOG::Ctor::debug<Shader>(this, &shaderProgramID, std::format("Shader program created with id: {}", shaderProgramID));

		Parameter_Reader<Infos...> reader(this, infos...);

		if (!shaderProgram_link())
			populateUniformCache();
	};

	Shader(const Shader& other) = delete;

	Shader(Shader&& other) :
		OR(other.OR),

		setUniform(this), uniformLocationCache(other.uniformLocationCache) {
		OB.add(OR);

		shaderProgramID = other.shaderProgramID;
		shaderIDs = other.shaderIDs;
		other.shaderProgramID = 0;
		other.shaderIDs.clear(); 
	}

	~Shader() {
		for (GLuint id : shaderIDs) {
			glDeleteShader(id);
		}

		LOG::Ctor::debug(this, &shaderProgramID, std::format("Shader program destroyed with id: {}", shaderProgramID));
		glDeleteProgram(shaderProgramID);
	}


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		 CLASS FUNCTIONS 		|
		|_______________________________|	*/


	void loadShader(std::string path, GLenum type);

	void populateUniformCache();


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		WRAPPER FUNCTIONS 		|
		|_______________________________|	*/

		//Runs the shader program.
	void use();

	size_t getShaderID();

private:

	//=============================================================================================================================================//

		/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
			|		WRAPPER STRUCTURES 		|
			|_______________________________|	*/


	GLuint shaderProgramID;

	std::vector<GLuint> shaderIDs;


	//=============================================================================================================================================//

		/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
			|		 CLASS FUNCTIONS 		|
			|_______________________________|	*/



		/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
			|		WRAPPER FUNCTIONS 		|
			|_______________________________|	*/

	//Attatches shader to the shader program.
	void shaderProgram_addShader(GLuint id);

	//Assembles the shader program.
	//Returns true on error.
	bool shaderProgram_link();

	//Compiles shader
	//Returns true on error.
	bool shader_compile(GLuint id, const char** code);

	//Checks for shader program error.
	//Returns true on error.
	bool shaderProgram_catchError();

	//Checks for shader error.
	//Returns true if an error occured in the shader.
	bool shader_catchError(GLuint id);

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		UTILITY FUNCTIONS 		|
		|_______________________________|	*/

	//Takes file contents and converts it to string.
	//Returns contents of a file in string format.
	std::string file_to_string(std::string path);

	//Wraps OpenGL int vector(iv) functions. !!(Warning: May be incomplete!)
	//Returns an integer representing parameters pertaining to OpenGL's object status.
	template<class T>
	int getDebugInfo(T openGLFunctioniv, unsigned int id, GLenum pname);

	//Wraps OpenGL info log functions. !!(Warning: May be incomplete!)
	template<class T>
	void getErrorMessage(T openGLFunctionInfoLog, unsigned int id, int logSize, std::string errorMessageAppend);

	//=============================================================================================================================================//

		/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
			|			VARIABLES			|
			|_______________________________|	*/

	// shader is required for setting default values on lookup where uniforms get optimized away
	struct ShaderLocation {
		GLint loc = -1;
		operator GLint() { return loc; }
	};

	std::unordered_map<std::string, ShaderLocation> uniformLocationCache;

	struct UniformFunctor {
	public:
		UniformFunctor(Shader* shader) : shader(shader) {}

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

public:
	UniformFunctor setUniform;


	inline static auto CR = DY::ClassRegister <
		&loadShader,
		&populateUniformCache,
		&use,
		&getShaderID,
		&shaderProgram_addShader,
		&shaderProgram_link,
		&shader_compile,
		&shaderProgram_catchError,
		&shader_catchError,
		&file_to_string,
		&getDebugInfo<decltype(glGetProgramiv)>,
		&getDebugInfo<decltype(glGetShaderiv)>,
		&getErrorMessage<decltype(glGetProgramInfoLog) > ,
		&getErrorMessage<decltype(glGetShaderInfoLog)>>(
			"loadShader",
			"populateUniformCache",
			"use",
			"getShaderID",
			"shaderProgram_addShader",
			"shaderProgram_link",
			"shader_compile",
			"shaderProgram_catchError",
			"shader_catchError",
			"file_to_string",
			"getDebugInfo<decltype(glGetProgramiv)>",
			"getDebugInfo<decltype(glGetShaderiv)>",
			"getErrorMessage<decltype(glGetProgramInfoLog)>",
			"getErrorMessage<decltype(glGetShaderInfoLog)>");

	DY::ObjectRegister<Shader,
		decltype(shaderProgramID),
		decltype(shaderIDs),
		decltype(uniformLocationCache),
		decltype(setUniform)> OR;

	inline static auto CB = DY::ClassBinder(CR);
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();


	struct LOG {
	public:
		using Class = DY_LINK<decltype(CB), decltype(OB), DY::No_FB, DY::No_VB>::Class;
		using Ctor = DY_LINK<decltype(CB), decltype(OB), DY::No_FB, DY::No_VB>::Ctor;
	};
};
