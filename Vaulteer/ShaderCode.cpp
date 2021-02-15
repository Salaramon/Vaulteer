#include "ShaderCode.h"

namespace shr {

	size_t ShaderCode::shaderCodeEnumerator;
	ShaderCode* ShaderCode::activeShaderCode = nullptr;

	ShaderCode::ShaderCode()
	{
		//See no reason to implement an id reservation system for shader code generation. Thus an ever increasing ID keeps it simple.
		shaderCodeID = shaderCodeEnumerator;
		shaderCodeEnumerator++;

		version = "#version 330 core";
	}

	void ShaderCode::unbind()
	{
		activeShaderCode = nullptr;
	}

	void ShaderCode::bind()
	{
		if (activeShaderCode == nullptr) {
			activeShaderCode = this;
		}
		else {
			throw "ShaderCode is already bound.";
		}
	}

	size_t ShaderCode::getNumberOfUniforms()
	{
		return uniforms.size();
	}

	size_t ShaderCode::getNumberOfOutputs()
	{
		return outputs.size();
	}

	size_t ShaderCode::getNumberOfInputs()
	{
		return inputs.size();
	}

	void ShaderCode::addUniform(GLSLTypesInterface typeCode)
	{
		uniforms.push_back(typeCode);
	}

	void ShaderCode::addOutput(GLSLTypesInterface typeCode)
	{
		outputs.push_back(typeCode);
	}

	void ShaderCode::addInput(GLSLTypesInterface typeCode)
	{
		inputs.push_back(typeCode);
	}

	void ShaderCode::addCodeLine(ShaderCodeLine shaderCodeLine)
	{
		mainLines.push_back(shaderCodeLine);
	}

	std::string ShaderCode::getCode()
	{
		std::string shaderCode = version + "\n\n";
		for (GLSLTypesInterface &code : inputs) {
			shaderCode += code.codeDeclaration + "\n";
		}
		shaderCode += "\n";

		for (GLSLTypesInterface& code : outputs) {
			shaderCode += code.codeDeclaration + "\n";
		}
		shaderCode += "\n";

		for (GLSLTypesInterface& code : uniforms) {
			shaderCode += code.codeDeclaration + "\n";
		}
		shaderCode += "\nvoid main()\n{\n";

		for (ShaderCodeLine& code : mainLines) {
			shaderCode += code.generatedCode + "\n";
		}
		shaderCode += "}";

		return shaderCode;
	}



	shr::ShaderCodeLine::ShaderCodeLine()
	{
	}

	shr::ShaderCodeLine::ShaderCodeLine(std::string code)
	{
		generatedCode = code;
	}

	

}