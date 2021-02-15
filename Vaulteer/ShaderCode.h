#pragma once

#include<vector>

#include<string>

namespace shr {

	template<class GLSLType>
	class Uniform;
	template<class GLSLType>
	class Input;


	class ShaderCodeLine
	{
	public:
		ShaderCodeLine();
		ShaderCodeLine(std::string code);

		std::string generatedCode;

		template<class GLSLType>
		ShaderCodeLine operator*(Uniform<GLSLType> line);
		ShaderCodeLine operator*(ShaderCodeLine line);
	};

	template<class GLSLType>
	inline ShaderCodeLine ShaderCodeLine::operator*(Uniform<GLSLType> line)
	{
		return ShaderCodeLine(generatedCode + " * " + line.codeName);
	}

	inline ShaderCodeLine ShaderCodeLine::operator*(ShaderCodeLine line)
	{
		return ShaderCodeLine(generatedCode + " * " + line.generatedCode);
	}

	class GLSLTypesInterface
	{
	public:
		std::string codeName;
		std::string codeGLSLType;
		std::string codeDeclaration;
		size_t id;
	};

	
	struct vec2 {};
	struct vec3 {};
	struct vec4 {
		template<class GLSLType>
		inline ShaderCodeLine operator()(Input<GLSLType>& input, float f);
	};

	template<class GLSLType>
	inline ShaderCodeLine vec4::operator()(Input<GLSLType>& input, float f)
	{
		return ShaderCodeLine(input.codeGLSLType + "(" + input.codeName + ", " + std::to_string(f) + ")");
	}

	struct mat4 {};

	struct gl_Position {
		inline void operator=(ShaderCodeLine shaderCodeLine);
	};

	inline vec4 Vec4;
	inline gl_Position glPosition;


	class ShaderCode
	{
	public:
		ShaderCode();

		void unbind();
		void bind();
		size_t getNumberOfUniforms();
		size_t getNumberOfOutputs();
		size_t getNumberOfInputs();
		void addUniform(GLSLTypesInterface typeCode);
		void addOutput(GLSLTypesInterface typeCode);
		void addInput(GLSLTypesInterface typeCode);
		void addCodeLine(ShaderCodeLine shaderCodeLine);
		std::string getCode();

		static ShaderCode* activeShaderCode;

	private:
		std::string version;
		std::vector<GLSLTypesInterface> uniforms;
		std::vector<GLSLTypesInterface> inputs;
		std::vector<GLSLTypesInterface> outputs;
		std::vector<ShaderCodeLine> mainLines;
		size_t shaderCodeID;
		static size_t shaderCodeEnumerator;
	};



	void gl_Position::operator=(ShaderCodeLine shaderCodeLine)
	{
		ShaderCode::activeShaderCode->addCodeLine(std::string("gl_position") + std::string(" = ") + shaderCodeLine.generatedCode + std::string(";"));
	}


	template<class StructType>
	std::string structStripper() {
		return std::string(typeid(StructType).name()).substr(12);
	}


	template<class GLSLType>
	class Input : public GLSLTypesInterface
	{
	public:
		Input();
	};

	template<class GLSLType>
	inline Input<GLSLType>::Input()
	{
		codeName = "input" + std::to_string(ShaderCode::activeShaderCode->getNumberOfInputs());
		codeGLSLType = structStripper<GLSLType>();
		codeDeclaration = "layout(location = " + std::to_string(ShaderCode::activeShaderCode->getNumberOfInputs()) + ") in " + codeGLSLType + " " + codeName;
		ShaderCode::activeShaderCode->addInput(*this);
	}



	template<class GLSLType>
	class Uniform : public GLSLTypesInterface
	{
	public:
		Uniform();

		template<class _GLSLType>
		ShaderCodeLine operator*(Uniform<_GLSLType>& uniform);

		ShaderCodeLine operator*(ShaderCodeLine line);
	};

	template<class GLSLType>
	inline Uniform<GLSLType>::Uniform()
	{
		codeName = "uniform" + std::to_string(ShaderCode::activeShaderCode->getNumberOfUniforms());
		codeGLSLType = structStripper<GLSLType>();
		codeDeclaration = "uniform " + codeGLSLType + " " + codeName;
		ShaderCode::activeShaderCode->addUniform(*this);
	}

	template<class GLSLType>
	inline ShaderCodeLine Uniform<GLSLType>::operator*(ShaderCodeLine line)
	{
		return ShaderCodeLine(codeName + " * " + line.generatedCode);
	}

	template<class GLSLType>
	template<class _GLSLType>
	inline ShaderCodeLine Uniform<GLSLType>::operator*(Uniform<_GLSLType>& uniform)
	{
		return ShaderCodeLine(codeName + " * " + uniform.codeName);
	}



	template<class GLSLType>
	class Output : public GLSLTypesInterface
	{
	public:
		Output();

		void operator*(ShaderCodeLine& shaderCodeLine);
		void operator=(Input<GLSLType>& shaderCodeLine);
	};

	template<class GLSLType>
	inline Output<GLSLType>::Output()
	{
		codeName = "output" + std::to_string(ShaderCode::activeShaderCode->getNumberOfOutputs());
		codeGLSLType = structStripper<GLSLType>();
		codeDeclaration = "out " + codeGLSLType + " " + codeName;
		ShaderCode::activeShaderCode->addOutput(*this);
	}

	template<class GLSLType>
	inline void Output<GLSLType>::operator*(ShaderCodeLine& shaderCodeLine)
	{
		ShaderCode::activeShaderCode->addCodeLine(codeName + " = " + shaderCodeLine.generatedCode + ";");
	}

	template<class GLSLType>
	inline void Output<GLSLType>::operator=(Input<GLSLType>& input)
	{
		ShaderCode::activeShaderCode->addCodeLine(codeName + " = " + input.codeName + ";");
	}
}