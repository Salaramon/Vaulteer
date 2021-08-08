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

#include "DebugLogger.h"
#include "DebugAliases.h"

class Shader : public DebugLogger<Shader>
{
public:
	

//=============================================================================================================================================//


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		  CONSTRUCTORS 			|
		|_______________________________|	*/

	//Loads vertex and fragment shaders on creation.
	Shader(std::string pathGSGLVertexCode, std::string pathGSGLFragmentCode);

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

	
	//Creates a new shader program on creation.
	struct ShaderProgram {
		ShaderProgram() {
			id = glCreateProgram();
		}
		~ShaderProgram() {
			glDeleteProgram(id);
		}
		unsigned int id;
	};

	//Shader interface.
	struct Shader_Shader {
		~Shader_Shader() {
			glDeleteShader(id);
		}
		unsigned int id; 
	};

	//Creates a new vertex shader on creation.
	struct Shader_Vertex : public Shader_Shader {
		Shader_Vertex(){
			id = glCreateShader(GL_VERTEX_SHADER);
		}
	};
	
	//Creates a new fragment shader on creation.
	struct Shader_Fragment : public Shader_Shader {
		Shader_Fragment() {
			id = glCreateShader(GL_FRAGMENT_SHADER);
		}
	};


//=============================================================================================================================================//

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		 CLASS FUNCTIONS 		|
		|_______________________________|	*/
	 
	//Constructs the shader program.
	void setup(std::string pathGSGLVertexCode, std::string pathGSGLFragmentCode);


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		WRAPPER FUNCTIONS 		|
		|_______________________________|	*/

	//Attatches shader to the shader program.
	void shaderProgram_addShader(Shader_Shader* shader);

	//Assembles the shader program.
	//Returns false on error.
	bool shaderProgram_link();

	//Compiles shader
	//Returns false on error.
	bool shader_compile(Shader_Shader*shader, const char** code);

	//Checks for shader program error.
	//Returns false on error.
	bool shaderProgram_catchError();

	//Checks for shader error.
	//Returns false in an error occured in the shader.
	bool shader_catchError(Shader_Shader*shader);

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

	//Holds and manages the shader program.
	ShaderProgram shaderProgram;
	

	struct UniformFunctor : public DebugLogger<Shader> {
	public:
		UniformFunctor(Shader* shader) : shader(shader) {}
		void operator()(Binder::UniformInfo uniform, GLfloat value1) {													debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + std::to_string(value1) + ")\n", "glUniform1f");																										glUniform1f(glGetUniformLocation(shader->shaderProgram.id, uniform.name), value1); }
		void operator()(Binder::UniformInfo uniform, GLfloat value1, GLfloat value2) {									debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + std::to_string(value1) + ", " + std::to_string(value2) + ")\n", "glUniform2f");																		glUniform2f(glGetUniformLocation(shader->shaderProgram.id, uniform.name), value1, value2); }
		void operator()(Binder::UniformInfo uniform, GLfloat value1, GLfloat value2, GLfloat value3) {					debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + std::to_string(value1) + ", " + std::to_string(value2) + ", " + std::to_string(value3) + ")\n", "glUniform3f");										glUniform3f(glGetUniformLocation(shader->shaderProgram.id, uniform.name), value1, value2, value3); }
		void operator()(Binder::UniformInfo uniform, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4) {	debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + std::to_string(value1) + ", " + std::to_string(value2) + ", " + std::to_string(value3) + ", " + std::to_string(value4) + ")\n", "glUniform4f");		glUniform4f(glGetUniformLocation(shader->shaderProgram.id, uniform.name), value1, value2, value3, value4); }
		void operator()(Binder::UniformInfo uniform, GLint value1) {
			debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + std::to_string(value1) + ")\n", "glUniform1i");																										
			int loc = glGetUniformLocation(shader->shaderProgram.id, uniform.name);
			glUniform1i(loc, value1); 
		}
		void operator()(Binder::UniformInfo uniform, GLint value1, GLint value2) {										debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + std::to_string(value1) + ", " + std::to_string(value2) + ")\n", "glUniform2i");																		glUniform2i(glGetUniformLocation(shader->shaderProgram.id, uniform.name), value1, value2); }
		void operator()(Binder::UniformInfo uniform, GLint value1, GLint value2, GLint value3) {						debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + std::to_string(value1) + ", " + std::to_string(value2) + ", " + std::to_string(value3) + ")\n", "glUniform3i");										glUniform3i(glGetUniformLocation(shader->shaderProgram.id, uniform.name), value1, value2, value3); }
		void operator()(Binder::UniformInfo uniform, GLint value1, GLint value2, GLint value3, GLint value4) {			debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + std::to_string(value1) + ", " + std::to_string(value2) + ", " + std::to_string(value3) + ", " + std::to_string(value4) + ")\n", "glUniform4i");		glUniform4i(glGetUniformLocation(shader->shaderProgram.id, uniform.name), value1, value2, value3, value4); }
		void operator()(Binder::UniformInfo uniform, GLuint value1) {	
			debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + std::to_string(value1) + ")\n", "glUniform1ui");	
			int loc = glGetUniformLocation(shader->shaderProgram.id, uniform.name);
			glUniform1ui(loc, value1); 
		}
		void operator()(Binder::UniformInfo uniform, GLuint value1, GLuint value2) {									debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + std::to_string(value1) + ", " + std::to_string(value2) + ")\n", "glUniform2ui");																	glUniform2ui(glGetUniformLocation(shader->shaderProgram.id, uniform.name), value1, value2); }
		void operator()(Binder::UniformInfo uniform, GLuint value1, GLuint value2, GLuint value3) {						debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + std::to_string(value1) + ", " + std::to_string(value2) + ", " + std::to_string(value3) + ")\n", "glUniform3ui");									glUniform3ui(glGetUniformLocation(shader->shaderProgram.id, uniform.name), value1, value2, value3); }
		void operator()(Binder::UniformInfo uniform, GLuint value1, GLuint value2, GLuint value3, GLuint value4) {		debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + std::to_string(value1) + ", " + std::to_string(value2) + ", " + std::to_string(value3) + ", " + std::to_string(value4) + ")\n", "glUniform4ui");	glUniform4ui(glGetUniformLocation(shader->shaderProgram.id, uniform.name), value1, value2, value3, value4); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::vec1& value) {							debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniform1fv");																										glUniform1fv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, &value[0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::vec2& value) {							debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniform2fv");																										glUniform2fv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, &value[0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::vec3& value) {							debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniform3fv");																										glUniform3fv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, &value[0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::vec4& value) {							debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniform4fv");																										glUniform4fv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, &value[0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::ivec1& value) {							debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniform1iv");																										glUniform1iv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, &value[0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::ivec2& value) {							debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniform2iv");																										glUniform2iv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, &value[0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::ivec3& value) {							debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniform3iv");																										glUniform3iv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, &value[0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::ivec4& value) {							debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniform4iv");																										glUniform4iv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, &value[0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::uvec1& value) {							debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniform1uiv");																									glUniform1uiv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, &value[0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::uvec2& value) {							debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniform2uiv");																									glUniform2uiv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, &value[0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::uvec3& value) {							debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniform3uiv");																									glUniform3uiv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, &value[0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, const glm::uvec4& value) {							debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniform4uiv");																									glUniform4uiv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, &value[0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat2& value) {		debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniformMatrix2fv");																								glUniformMatrix2fv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, transpose, &value[0][0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat3& value) {		debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniformMatrix3fv");																								glUniformMatrix3fv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, transpose, &value[0][0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat4& value) {		
			debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + std::to_string(count) + ")\n", "glUniformMatrix4fv");

			bool i = glIsProgram(shader->shaderProgram.id);
			int loc = glGetUniformLocation(shader->shaderProgram.id, uniform.name);
			glUniformMatrix4fv(loc, count, transpose, &value[0][0]);
		}
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat2x3& value) {	debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniformMatrix2x3fv");																								glUniformMatrix2x3fv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, transpose, &value[0][0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat3x2& value) {	debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniformMatrix3x2fv");																								glUniformMatrix3x2fv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, transpose, &value[0][0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat2x4& value) {	debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniformMatrix2x4fv");																								glUniformMatrix2x4fv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, transpose, &value[0][0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat4x2& value) {	debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniformMatrix4x2fv");																								glUniformMatrix4x2fv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, transpose, &value[0][0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat3x4& value) {	debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniformMatrix3x4fv");																								glUniformMatrix3x4fv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, transpose, &value[0][0]); }
		void operator()(Binder::UniformInfo uniform, GLsizei count, GLboolean transpose, const glm::mat4x3& value) {	debug("Uniform " + std::string(uniform.name) + " for shader: " + std::to_string(shader->getObjectKey()) + ". Set to (" + glm::to_string(value) + ")\n", "glUniformMatrix4x3fv");																								glUniformMatrix4x3fv(glGetUniformLocation(shader->shaderProgram.id, uniform.name), count, transpose, &value[0][0]); }
	private:
		Shader* shader;
	};

public:
	UniformFunctor setUniform;

	inline static char** Using;

};
