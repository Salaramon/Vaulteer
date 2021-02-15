#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>
#include <vector>

#include "ShaderCode.h"

class Shader
{
public:

//=============================================================================================================================================//


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		  CONSTRUCTORS 			|
		|_______________________________|	*/

	//Loads vertex and fragment shaders on creation.
	Shader(std::string pathGSGLVertexCode, std::string pathGSGLFragmentCode);
	Shader(shr::ShaderCode& shaderVertexCode, std::string pathGSGLFragmentCode);
	Shader(shr::ShaderCode& shaderVertexCode, shr::ShaderCode& shaderFragmentCode);

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		WRAPPER FUNCTIONS 		|
		|_______________________________|	*/

	//Runs the shader program.
	void use();

	//Sets the uniform variable in the shader program. !!(Incomplete: Missing overloads for more types)
	void setUniform(std::string name, int value);

	//Sets the matrix transform variable in the shader program. !!(Incomplete: Missing overloads for more types)
	void setMatrix(const std::string &name, const glm::mat4& mat) const;


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
	
};

