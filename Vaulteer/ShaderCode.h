#pragma once

#include<vector>
#include <iostream>

#include<string> 
#include<typeinfo>

namespace shr {

	////////////////////////////
	////Forward declarations////
	////////////////////////////
	template<class GLSLType>
	class Uniform;
	template<class GLSLType>
	class Input;
	template<class GLSLType>
	class Output;

	struct _texture;
	struct _gl_Position;

	struct sampler2D;
	struct sFloat;
	struct vec2;


	///////////////////////////////////////////
	////Identifiers containing variable IDs////
	///////////////////////////////////////////

	//Non-r-value identifiers.
	class InputIdentifier {
	public:
		InputIdentifier() {
			id++;
		}
	protected:
		static size_t id;
	};
	class UniformIdentifier {
	public:
		UniformIdentifier() {
			id++;
		}
	protected:
		static size_t id;
	};
	class OutputIdentifier {
	public:
		OutputIdentifier() {
			id++;
		}
	protected:
		static size_t id;
	};
	class vec4Identifier {
	public:
		vec4Identifier() {
			id++;
		}
	protected:
		static size_t id;
	};

	//r-value identifiers.
	class sFloatIdentifier {
	public:
		sFloatIdentifier() {
			id++;
		}
		sFloatIdentifier(float f) {

		}
	protected:
		static size_t id;
	};



	//////////////////////
	////Type interface////
	//////////////////////
	class GLSLTypesInterface
	{
	public:
		std::string codeName;
		std::string codeGLSLType;
		std::string codeDeclaration;
	};



	////////////////////////////////////
	////Operation output generalizer////
	////////////////////////////////////
	class ShaderCodeLine
	{
	public:
		std::string generateCode() {
			std::string code;
			if (deliminators.empty()) {
				code = types[0]->codeName;
			}
			else {
				for (size_t i = 0; i < deliminators.size(); i++) {
					code += types[i]->codeName + deliminators[i];
				}
				if (deliminators.size() != types.size()) {
					code += types.back()->codeName;
				}
			}

			return code;
		}

		template<class GLSLType>
		ShaderCodeLine operator*(Uniform<GLSLType>& line) {
			std::string delim(" * ");
			if (deliminators.size() == types.size()) {
				deliminators.back() += delim;
			}
			else {
				deliminators.push_back(delim);
			}
			types.push_back(&line);

			return *this;
		}

		ShaderCodeLine operator*(ShaderCodeLine line) {
			std::string delim(" * ");
			if (deliminators.size() == types.size()) {
				deliminators.back() += delim;
			}
			else {
				deliminators.push_back(delim);
			}
			deliminators.insert(deliminators.end(), line.deliminators.begin(), line.deliminators.end());
			types.insert(types.end(), line.types.begin(), line.types.end());

			return *this;
		}

		std::vector<GLSLTypesInterface*> types;
		std::vector<std::string> deliminators;
	};



	//Needs to be protected
	template<class StructType>
	inline std::string structStripper() {
		return std::string(typeid(StructType).name()).substr(12);
	}



	//////////////////
	////Main Class////
	//////////////////
	class ShaderCode
	{
	public:
		ShaderCode();

		void unbind();
		void bind();
		size_t getNumberOfUniforms();
		size_t getNumberOfOutputs();
		size_t getNumberOfInputs();
		size_t getNumberOfOther();
		void addUniform(GLSLTypesInterface* typeCode);
		void addOutput(GLSLTypesInterface* typeCode);
		void addInput(GLSLTypesInterface* typeCode);
		void addOther(GLSLTypesInterface* typeCode);
		void addCodeLine(ShaderCodeLine shaderCodeLine);
		std::string getCode();

		static ShaderCode* activeShaderCode;
		std::vector<GLSLTypesInterface> internalVariables;
		std::vector<GLSLTypesInterface*> other;
	private:
		std::string version;
		
		std::vector<GLSLTypesInterface*> uniforms;
		std::vector<GLSLTypesInterface*> inputs;
		std::vector<GLSLTypesInterface*> outputs;
		std::vector<ShaderCodeLine> mainLines;
		size_t shaderCodeID;
		static size_t shaderCodeEnumerator;
	};



	
	//////////////////////////////
	////GLSLType type wrappers////
	//////////////////////////////
	
	//GLSL specific functional wrapper types
	template<class GLSLType>
	class Uniform : public GLSLTypesInterface, public UniformIdentifier
	{
	public:
		Uniform() {
			codeName = "uniform" + std::to_string(id);
			codeGLSLType = structStripper<GLSLType>();
			codeDeclaration = "uniform " + codeGLSLType + " " + codeName;
			ShaderCode::activeShaderCode->addUniform(this);
		}

		template<class _GLSLType>
		ShaderCodeLine operator*(Uniform<_GLSLType>& uniform) {
			ShaderCodeLine code;
			code.deliminators = { " * " };
			code.types = { &uniform, this };
			return code;
		}

		ShaderCodeLine operator*(ShaderCodeLine line) {
			ShaderCodeLine code;
			code.deliminators = { " * " };
			code.types = { this };
			code.deliminators.insert(code.deliminators.end(), line.deliminators.begin(), line.deliminators.end());
			code.types.insert(code.types.end(), line.types.begin(), line.types.end());
			return code;
		}
	};

	template<class GLSLType>
	class Output : public GLSLTypesInterface, public OutputIdentifier
	{
	public:
		Output() {
			codeName = "output" + std::to_string(id);
			codeGLSLType = structStripper<GLSLType>();
			codeDeclaration = "out " + codeGLSLType + " " + codeName;
			ShaderCode::activeShaderCode->addOutput(this);
		}

		//void operator*(ShaderCodeLine shaderCodeLine);
		void operator=(Input<GLSLType>& line) {
			ShaderCodeLine code;
			code.types = { this };
			code.deliminators = { " = " };
			code.types.push_back(&line);
			ShaderCode::activeShaderCode->addCodeLine(code);
		}

		void operator=(ShaderCodeLine shaderCodeLine) {
			ShaderCodeLine code;
			code.types = { this };
			code.deliminators = { " = " };
			code.types.insert(code.types.end(), shaderCodeLine.types.begin(), shaderCodeLine.types.end());
			code.deliminators.insert(code.deliminators.end(), shaderCodeLine.deliminators.begin(), shaderCodeLine.deliminators.end());
			ShaderCode::activeShaderCode->addCodeLine(code);
		}
	};

	template<class GLSLType>
	class Input : public GLSLTypesInterface, public InputIdentifier
	{
	public:
		Input() {
			codeName = "input" + std::to_string(id);
			codeGLSLType = structStripper<GLSLType>();
			codeDeclaration = "in " + codeGLSLType + " " + codeName;
			ShaderCode::activeShaderCode->addInput(this);
		}

		template<class _GLSLType>
		Input(Output<_GLSLType>& output) {
			codeGLSLType = structStripper<GLSLType>();
			if (codeGLSLType == output.codeGLSLType) {
				codeName = "input" + std::to_string(ShaderCode::activeShaderCode->getNumberOfUniforms());
				codeName = output.codeName + codeName;
				output.codeName = codeName;
				output.codeDeclaration = "out " + output.codeGLSLType + " " + output.codeName;
				codeDeclaration = "in " + codeGLSLType + " " + codeName;
				ShaderCode::activeShaderCode->addInput(this);
			}
			else {
				throw "Input type does not match output type.";
			}
		}
	};


	//GLSL specific functional modifier wrapper types
	template<class shrType>
	class Location;
	template<class GLSLType>
	class Location<Input<GLSLType>> : public Input<GLSLType>, public GLSLType {
	public:
		Location<Input<GLSLType>>() {
			
			locationID = Input<GLSLType>::InputIdentifier::id;
			Input<GLSLType>::GLSLTypesInterface::codeDeclaration = "layout (location = " + std::to_string(Input<GLSLType>::InputIdentifier::id) + ") " + Input<GLSLType>::GLSLTypesInterface::codeDeclaration;
		}
		size_t id(){
			return locationID;
		}
		size_t size() {
			return GLSLType::typeSize;
		}
	private:
		size_t locationID;
	};

	//GLSL specific wrapper types
	struct sampler2D {};

	//Fundamental wrapper types
	struct sFloat : public GLSLTypesInterface, public sFloatIdentifier {
	public:
		sFloat() : sFloatIdentifier() {
			codeName = "var" + std::to_string(id);
			codeGLSLType = "float";
			codeDeclaration = codeGLSLType + " " + codeGLSLType;
			ShaderCode::activeShaderCode->addOther(this);
		}
		sFloat(float value) : sFloatIdentifier(value) {
			codeName = std::to_string(value);
			codeGLSLType = "float";
			codeDeclaration = "";
			ShaderCode::activeShaderCode->internalVariables.emplace_back(*this);
			ShaderCode::activeShaderCode->addOther(&ShaderCode::activeShaderCode->internalVariables.back());
		}
	};

	//Struct wrapper types
	struct vec2 {};
	struct vec3 : public GLSLTypesInterface {
	protected:
		const size_t typeSize = 3;
	};
	struct mat4 {};
	struct vec4 : public GLSLTypesInterface, public vec4Identifier {
	public:
		vec4() {
			if (initialized) {
				codeName = "var" + std::to_string(id);
				codeGLSLType = structStripper<decltype(*this)>();
				codeDeclaration = codeGLSLType + " " + codeName;
			}
			else {
				codeName = structStripper<decltype(*this)>();
			}
		}


		template<class GLSLType>
		inline ShaderCodeLine operator()(Input<GLSLType>& input, sFloat&& f) {
			GLSLTypesInterface* storedFloat = &ShaderCode::activeShaderCode->internalVariables.back();
			ShaderCodeLine code;
			code.types = { this, &input, storedFloat };
			code.deliminators = { "(", ", ", ")" };
			return code;
		}

	private:
		bool initialized = false;
	};

	//Global variable wrapper structs
	struct _gl_Position : public GLSLTypesInterface {
	public:
		_gl_Position() {
			codeName = structStripper<decltype(*this)>().substr(1);
			codeGLSLType = "";
			codeDeclaration = codeName;
		}
		void operator=(ShaderCodeLine shaderCodeLine);
	};
	
	//Global function wrapper structs
	struct _texture : public GLSLTypesInterface {
	public:
		_texture() {
			codeName = structStripper<decltype(*this)>().substr(1);
			codeGLSLType = "";
			codeDeclaration = codeName;
		}
		inline ShaderCodeLine operator()(Uniform<sampler2D>& uniform, Input<shr::vec2>& input);
	};
	inline _texture texture;
	
	//Struct type wrapper variables for inline declaration
	inline vec4 Vec4;

	//Global variable wrapper type variables.
	inline _gl_Position gl_Position;

	//Global function wrapper operator overloads
	inline ShaderCodeLine shr::_texture::operator()(Uniform<sampler2D>& uniform, Input<shr::vec2>& input) {
		ShaderCodeLine code;
		code.deliminators = { "(", ",", ")" };
		code.types = { &texture, &uniform, &input };
		return code;
	}

	//Gloval variable wrapper operator overloads
	inline void shr::_gl_Position::operator=(ShaderCodeLine shaderCodeLine) {
		ShaderCodeLine code;
		code.types.push_back(&gl_Position);
		code.deliminators = { " = " };
		code.types.insert(code.types.end(), shaderCodeLine.types.begin(), shaderCodeLine.types.end());
		code.deliminators.insert(code.deliminators.end(), shaderCodeLine.deliminators.begin(), shaderCodeLine.deliminators.end());
		ShaderCode::activeShaderCode->addCodeLine(code);
	}

	

	///////////////////////////
	////Typedefs and usings////
	///////////////////////////

	template<class GLSLType>
	using LocIn = Location<Input<GLSLType>>;
}