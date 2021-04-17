#include "ShaderCode.h"

shr::ShaderCode* shr::ShaderCode::activeShaderCode;
std::array<shr::UniversalTypeInfo, shr::ShaderCode::variableContainerSize> shr::ShaderCode::variables;
bool shr::ShaderCode::firstInitialization = true;

std::array<std::string, 1> shr::ShaderCode::deliminators;
size_t shr::ShaderCode::shaderCodeEnumerator = 0;
std::unordered_map<size_t, std::string> shr::ShaderCode::typeIDToName;
size_t shr::ShaderCode::variableEnumerator = 0;

std::vector<shr::CodeType> shr::SegmentContainer::codeTypes;
std::vector<shr::CodeVariable> shr::SegmentContainer::codeVariables;
std::vector<shr::CodeSymbol> shr::SegmentContainer::codeSymbols;
std::vector<shr::CodeValue> shr::SegmentContainer::codeValues;
std::vector<shr::CodeMember> shr::SegmentContainer::codeMembers;


namespace shr {

	ShaderCode::ShaderCode()
	{
		initializeStaticVariables();
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
	
	std::string ShaderCode::getCode()
	{
		std::string shaderCode;
		for (auto& codeLine : codeLines) {
			shaderCode += codeLine.getCode() + "\n";
		}
		return shaderCode;
	}
	
	void ShaderCode::setShaderProgramID(size_t id)
	{
		shaderCodeID = id;
	}

    std::string ShaderCode::getTypeName(size_t typeID)
    {
        return typeIDToName.find(typeID)->second;
    }

	size_t ShaderCode::getNextVariableID()
	{
		return variableEnumerator++;
	}

	UniversalTypeInfo* ShaderCode::storeVariable(size_t typeID)
	{
		initializeStaticVariables();
		variables[variableEnumerator - 1] = std::move(UniversalTypeInfo(static_cast<GLSLType>(typeID)));
		return &variables[variableEnumerator - 1];
		
	}

	
	void ShaderCode::initializeStaticVariables()
	{
		if (firstInitialization){
			firstInitialization = false;
			SegmentContainer::initializeStaticVariables();
		}
		
	}

	


}

ShaderCode::SegmentContainer ShaderCode::SegmentContainer::add(CodeLine type) {
	codeSegments.insert(codeSegments.end(), type.getSegmentContainer().getSegmentList().begin(), type.getSegmentContainer().getSegmentList().end());
	return *this;
}

ShaderCode::UniversalTypeInterface::UniversalTypeInterface(UniversalTypeInfo* type) : type(type) {
	if (!ShaderCode::firstInitialization) {
		CodeLine newCodeLine;
		newCodeLine = newCodeLine << type->getType() << Symbol::SPACE << *this << Symbol::SEMICOLON;
		ShaderCode::activeShaderCode->codeLines.push_back(newCodeLine);
	}
}

std::string ShaderCode::CodeMember::getCode()
{
	return std::string(type->getVariableName() + name);
}
