#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include<vector>
#include <iostream>
#include <array>
#include <unordered_map>
//#include <algorithm>

#include<string> 
#include<typeinfo>
#include<type_traits>

class UniversalTypeInterface;
class CodeLine;
class UniversalTypeInfo;

class CodeSymbol;
class CodeVariable;
class CodeType;
class CodeValue;
//////////////////
////Main Class////
//////////////////

using namespace std::string_literals;

inline const std::vector<std::string> symbolList{
		" + "s,
		" - ",
		" * ",
		" / ",
		"(",
		")",
		"[",
		"]",
		"{",
		"}",
		" = ",
		" ",
		";",
		", "
};
inline const std::vector<std::string> typeList{
	"bool",
	"int",
	"float",
	"double",
	"vec2",
	"vec3",
	"vec4",
	"mat2",
	"mat3",
	"mat4"
};

class ShaderCode
{
public:
	ShaderCode();

	void unbind();
	void bind();


	std::string getCode();
	void setShaderProgramID(size_t id);

	constexpr static size_t variableContainerSize = 1000;

	static ShaderCode* activeShaderCode;

	std::vector<CodeLine> codeLines;

	static std::array<UniversalTypeInfo, variableContainerSize> variables;


	static std::string getTypeName(size_t typeID);
	static size_t getNextVariableID();
	static UniversalTypeInfo* storeVariable(size_t typeID);
	static bool firstInitialization;
	static void initializeStaticVariables();
private:

	enum class GLSLQualifier {
		CONSTANT,
		UNIFORM,
		LAYOUT,
		LAYOUT_LOCATION,
		LAYOUT_BINDING,
		VEC2,
		VEC3,
		VEC4,


		GLSLType_SIZE
	};
	std::string version;
	//std::vector<ShaderCodeLine> linesOfCode;

	size_t shaderCodeID;
	size_t shaderProgramID;

		

	static std::array<std::string, 1> deliminators;
	static size_t shaderCodeEnumerator;
	static std::unordered_map<size_t, std::string> typeIDToName;
	static size_t variableEnumerator;

	enum class GLSLType {
		BOOL,
		INT,
		FLOAT,
		DOUBLE,
		VEC2,
		VEC3,
		VEC4,
		MAT2,
		MAT3,
		MAT4
		/*
		SAMPLER1D,
		SAMPLER2D,
		SAMPLER3D,
		SAMPLERCUBE,
		SAMPLER2DRECT,
		SAMPLER1DARRAY,
		SAMPLER2DARRAY,
		SAMPLERCUBEARRAY,
		SAMPLERBUFFER,
		SAMPLER2DMS,
		SAMPLER2DMSARRAY,



		SAMPLER1DSHADOW,
		SAMPLER2DSHADOW,
		SAMPLERCUBESHADOW,
		SAMPLER2DRECTSHADOW,
		SAMPLER1DARRAYSHADOW,
		SAMPLER2DARRAYSHADOW,
		SAMPLERCUBEARRAYSHADOW,





		BVEC2,
		BVEC3,
		BVEC4,
		IVEC2,
		IVEC3,
		IVEC4,
		UVEC2,
		UVEC3,
		UVEC4,
		DVEC2,
		DVEC3,
		DVEC4,

		ISAMPLER1D,
		ISAMPLER2D,
		ISAMPLER3D,
		ISAMPLERCUBE,
		USAMPLER1D,
		USAMPLER2D,
		USAMPLER3D,
		USAMPLERCUBE,
		*/
	};

	enum class Symbol {
		ADDITION,
		SUBTRACTION,
		MULTIPLICATION,
		DIVISION,
		PARANTHESIS_LEFT,
		PARANTHESIS_RIGHT,
		SQUAREBRACKET_LEFT,
		SQUAREBRACKET_RIGHT,
		CURLYBRACKET_LEFT,
		CURLYBRACKET_RIGHT,
		ASSIGNMENT,
		SPACE,
		SEMICOLON,
		COMMA,

		Symbol_SIZE
	};



	class UniversalTypeInfo {
	public:
		UniversalTypeInfo(GLSLType type) : typeNameID(type), variableName(typeList[static_cast<size_t>(type)] + std::to_string(ShaderCode::getNextVariableID())) {}
		std::string getVariableName() {
			return variableName;
		}
		GLSLType getType() {
			return typeNameID;
		}
	private:
		GLSLType typeNameID;
		std::string variableName;
	};

	class CodeSegment {
	public:
		virtual std::string getCode() = 0;
	};

	class CodeVariable : public CodeSegment {
	public:
		CodeVariable() {}
		CodeVariable(UniversalTypeInterface variable);
		std::string getCode() override {
			return variable->getVariableName();
		}
	private:
		UniversalTypeInfo* variable;
	};

	class CodeSymbol : public CodeSegment {
	public:
		CodeSymbol(Symbol symbol) : symbol(symbolList[static_cast<size_t>(symbol)]) {
		}
		virtual std::string getCode() override {
			return symbol;
		}
	private:
		const std::string symbol;
	};

	class CodeValue : public CodeSegment {
	public:
		CodeValue() {}
		CodeValue(float v) : value(std::to_string(v)) {}
		CodeValue(double v) : value(std::to_string(v)) {}
		CodeValue(int v) : value(std::to_string(v)) {}
		CodeValue(unsigned int v) : value(std::to_string(v)) {}
		CodeValue(bool v) : value(std::to_string(v)) {}
		virtual std::string getCode() override {
			return value;
		}
	private:
		const std::string value;
	};

	class CodeType : public CodeSegment {
	public:
		CodeType(GLSLType type) : type(typeList[static_cast<size_t>(type)]) {
		}
		virtual std::string getCode() override {
			return type;
		}
	private:
		const std::string type;
	};

	class CodeMember : public CodeSegment {
	public:
		//CodeMember(CodeMember& member) : type(member.type), name(member.name) {}
		CodeMember(UniversalTypeInterface* type, std::string name) : type(type), name(name) {}

		virtual std::string getCode() override;
		UniversalTypeInterface* getInterface() {
			return type;
		}
		std::string getName() {
			return name;
		}
	private:
		UniversalTypeInterface* type;
		std::string name;
	};

	struct SegmentContainer {
	public:
		SegmentContainer() {
			if (ShaderCode::firstInitialization) {

			}
		}

		SegmentContainer add(Symbol type) {
			codeSymbols.push_back(type);
			codeSegments.push_back(&codeSymbols.back());
			return *this;
		}
		SegmentContainer add(GLSLType type) {
			codeTypes.push_back(type);
			codeSegments.push_back(&codeTypes.back());
			return *this;
		}
		SegmentContainer add(CodeVariable type) {
			codeVariables.push_back(type);
			codeSegments.push_back(&codeVariables.back());
			return *this;
		}
		SegmentContainer add(CodeValue type) {
			codeValues.push_back(type);
			codeSegments.push_back(&codeValues.back());
			return *this;
		}
		SegmentContainer add(CodeMember type) {
			codeMembers.push_back(type);
			codeSegments.push_back(&codeMembers.back());
			return *this;
		}
		SegmentContainer add(CodeLine type);
		std::vector<CodeSegment*>& getSegmentList() {
			return codeSegments;
		}

		static void initializeStaticVariables() {
			codeTypes.reserve(segmentAllocationSize);
			codeVariables.reserve(segmentAllocationSize);
			codeSymbols.reserve(segmentAllocationSize);
			codeValues.reserve(segmentAllocationSize);
		}


	private:
		std::vector<CodeSegment*> codeSegments;
		static std::vector<CodeType> codeTypes;
		static std::vector<CodeVariable> codeVariables;
		static std::vector<CodeSymbol> codeSymbols;
		static std::vector<CodeValue> codeValues;
		static std::vector<CodeMember> codeMembers;

		static constexpr size_t segmentAllocationSize = 100;
	};


	class CodeLine {
	public:
		//Initialization constructors.
		CodeLine() {}
		CodeLine(SegmentContainer segmentContainer) : segmentContainer(segmentContainer) {}

		//Conversion constructors.
		CodeLine(CodeMember& type) {
			segmentContainer.add(type);
		}

		std::string getCode() {
			std::string codeLine;
			for (auto& codeSegment : segmentContainer.getSegmentList()) {
				codeLine += codeSegment->getCode();
			}

			return codeLine;
		}

		template<class TCodeType>
		CodeLine operator<<(TCodeType type) {
			return segmentContainer.add(type);
		}



		CodeLine operator*(CodeMember& type) {
			CodeLine newCodeLine;
			newCodeLine = *this << Symbol::MULTIPLICATION << type;
			return newCodeLine;
		}

		CodeLine operator/(CodeMember& type) {
			CodeLine newCodeLine;
			newCodeLine = *this << Symbol::DIVISION << type;
			return newCodeLine;
		}

		CodeLine operator*(UniversalTypeInterface& type);

		CodeLine operator+(UniversalTypeInterface& type);

		CodeLine operator/(UniversalTypeInterface& type);

		CodeLine operator-(UniversalTypeInterface& type);

		SegmentContainer& getSegmentContainer() {
			return segmentContainer;
		}

	private:
		SegmentContainer segmentContainer;
	};
	/*
	inline SegmentContainer::add(CodeLine type) {
		codeSegments.insert(codeSegments.end(), type.getSegmentContainer().getSegmentList().begin(), type.getSegmentContainer().getSegmentList().end());
		return *this;
	}
	*/


	class UniversalTypeInterface {
	public:
		UniversalTypeInterface() {}
		UniversalTypeInterface(UniversalTypeInfo* type);

		UniversalTypeInfo* getInfo() {
			return type;
		}
		GLSLType getType() {
			return getInfo()->getType();
		}
		std::string getVariableName() {
			return  type->getVariableName();
		}
		std::string getTypeName() {
			return typeList[static_cast<size_t>(type->getType())];
		}
	protected:
		UniversalTypeInfo* type;
	};

	ShaderCode::CodeLine CodeLine::operator*(UniversalTypeInterface& type)
	{
		CodeLine newCodeLine;
		newCodeLine = *this << Symbol::MULTIPLICATION << type;
		return newCodeLine;
	}

	inline CodeLine CodeLine::operator+(UniversalTypeInterface& type) {
		CodeLine newCodeLine;
		newCodeLine = *this << Symbol::ADDITION << type;
		return newCodeLine;
	}

	inline CodeLine CodeLine::operator/(UniversalTypeInterface& type) {
		CodeLine newCodeLine;
		newCodeLine = *this << Symbol::DIVISION << type;
		return newCodeLine;
	}

	inline CodeLine CodeLine::operator-(UniversalTypeInterface& type) {
		CodeLine newCodeLine;
		newCodeLine = *this << Symbol::SUBTRACTION << type;
		return newCodeLine;
	}

	inline CodeVariable::CodeVariable(UniversalTypeInterface variable) : variable(variable.getInfo()) {}


	template<class Type, const size_t id>
	class UniversalType : public UniversalTypeInterface {
	public:
		UniversalType() : UniversalTypeInterface(ShaderCode::storeVariable(id)) {}

		void operator=(const CodeLine codeLine) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << *this << Symbol::ASSIGNMENT << codeLine << Symbol::SEMICOLON;
			ShaderCode::activeShaderCode->codeLines.push_back(newCodeLine);
		}

		CodeLine operator*(UniversalTypeInterface& type) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << *this << Symbol::MULTIPLICATION << type;
			return newCodeLine;
		}

		CodeLine operator-(UniversalTypeInterface& type) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << *this << Symbol::SUBTRACTION << type;
			return newCodeLine;
		}

		CodeLine operator+(UniversalTypeInterface& type) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << *this << Symbol::ADDITION << type;;
			return newCodeLine;
		}

		CodeLine operator/(UniversalTypeInterface& type) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << *this << Symbol::DIVISION << type;;
			return newCodeLine;
		}

		CodeLine operator-(CodeLine type) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << *this << Symbol::SUBTRACTION << type;;
			return newCodeLine;
		}

		CodeLine operator*(CodeLine type) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << *this << Symbol::MULTIPLICATION << type;;
			return newCodeLine;
		}

		CodeLine operator+(CodeLine type) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << *this << Symbol::ADDITION << type;;
			return newCodeLine;
		}

		CodeLine operator/(CodeLine type) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << *this << Symbol::DIVISION << type;;
			return newCodeLine;
		}

		/*
		template<class ParameterType>
		CodeLine operator()(ParameterType& type1, ParameterType& type2);

		template<class ParameterType>
		CodeLine operator()(ParameterType& type1, ParameterType& type2, ParameterType& type3);

		template<class ParameterType>
		CodeLine operator()(ParameterType& type1, ParameterType& type2, ParameterType& type3, ParameterType& type4);
		*/
	};


	template<class Type, const size_t id>
	class FundamentalType : public UniversalType<Type, id> {
	public:

		//using UniversalType<Type, id>::operator=;
		using UniversalType<Type, id>::operator*;
		using UniversalType<Type, id>::operator/;
		using UniversalType<Type, id>::operator+;
		using UniversalType<Type, id>::operator-;

		void operator=(const float&& type) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << *this << Symbol::ASSIGNMENT << type << Symbol::SEMICOLON;
			ShaderCode::activeShaderCode->codeLines.push_back(newCodeLine);
		}

		CodeLine operator*(float&& type) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << *this << Symbol::MULTIPLICATION << type;
			return newCodeLine;
		}

		CodeLine operator+(float&& type) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << *this << Symbol::ADDITION << type;;
			return newCodeLine;
		}

		CodeLine operator/(float&& type) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << *this << Symbol::DIVISION << type;;
			return newCodeLine;
		}

		CodeLine operator-(float&& type) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << *this << Symbol::SUBTRACTION << type;;
			return newCodeLine;
		}

		CodeLine operator()(float&& type)
		{
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << this->getType() << Symbol::PARANTHESIS_LEFT << type << Symbol::PARANTHESIS_RIGHT;
			return newCodeLine;
		}

		//static constexpr size_t ID = id;

	};


	template<class StorageType, class Type, const size_t id>
	class VectorType : public UniversalType<Type, id> {
	public:

		CodeMember x = CodeMember(this, ".x");
		CodeMember y = CodeMember(this, ".y");
		CodeMember z = CodeMember(this, ".z");

		CodeMember xx = CodeMember(this, ".xx");
		CodeMember yx = CodeMember(this, ".yx");
		CodeMember zx = CodeMember(this, ".zx");
		CodeMember xy = CodeMember(this, ".xx");
		CodeMember yy = CodeMember(this, ".yx");
		CodeMember zy = CodeMember(this, ".zx");
		CodeMember xz = CodeMember(this, ".xx");
		CodeMember yz = CodeMember(this, ".yx");
		CodeMember zz = CodeMember(this, ".zx");

		CodeMember xxx = CodeMember(this, ".xxx");
		CodeMember yxx = CodeMember(this, ".yxx");
		CodeMember zxx = CodeMember(this, ".zxx");
		CodeMember xyx = CodeMember(this, ".xxx");
		CodeMember yyx = CodeMember(this, ".yxx");
		CodeMember zyx = CodeMember(this, ".zxx");
		CodeMember xzx = CodeMember(this, ".xxy");
		CodeMember yzx = CodeMember(this, ".yxy");
		CodeMember zzx = CodeMember(this, ".zxy");
		CodeMember xxy = CodeMember(this, ".xxy");
		CodeMember yxy = CodeMember(this, ".yxy");
		CodeMember zxy = CodeMember(this, ".zxy");
		CodeMember xyy = CodeMember(this, ".xxy");
		CodeMember yyy = CodeMember(this, ".yxy");
		CodeMember zyy = CodeMember(this, ".zxy");
		CodeMember xzy = CodeMember(this, ".xxz");
		CodeMember yzy = CodeMember(this, ".yxz");
		CodeMember zzy = CodeMember(this, ".zxz");
		CodeMember xxz = CodeMember(this, ".xxz");
		CodeMember yxz = CodeMember(this, ".yxz");
		CodeMember zxz = CodeMember(this, ".zxz");
		CodeMember xyz = CodeMember(this, ".xxz");
		CodeMember yyz = CodeMember(this, ".yxz");
		CodeMember zyz = CodeMember(this, ".zxz");
		CodeMember xzz = CodeMember(this, ".xxz");
		CodeMember yzz = CodeMember(this, ".yxz");
		CodeMember zzz = CodeMember(this, ".zxz");

		CodeMember xxxx = CodeMember(this, ".xxxx");
		CodeMember yxxx = CodeMember(this, ".yxxx");
		CodeMember zxxx = CodeMember(this, ".zxxx");
		CodeMember xyxx = CodeMember(this, ".xxxx");
		CodeMember yyxx = CodeMember(this, ".yxxx");
		CodeMember zyxx = CodeMember(this, ".zxxx");
		CodeMember xzxx = CodeMember(this, ".xxyx");
		CodeMember yzxx = CodeMember(this, ".yxyx");
		CodeMember zzxx = CodeMember(this, ".zxyx");
		CodeMember xxyx = CodeMember(this, ".xxyx");
		CodeMember yxyx = CodeMember(this, ".yxyx");
		CodeMember zxyx = CodeMember(this, ".zxyx");
		CodeMember xyyx = CodeMember(this, ".xxyx");
		CodeMember yyyx = CodeMember(this, ".yxyx");
		CodeMember zyyx = CodeMember(this, ".zxyx");
		CodeMember xzyx = CodeMember(this, ".xxzx");
		CodeMember yzyx = CodeMember(this, ".yxzx");
		CodeMember zzyx = CodeMember(this, ".zxzx");
		CodeMember xxzx = CodeMember(this, ".xxzx");
		CodeMember yxzx = CodeMember(this, ".yxzx");
		CodeMember zxzx = CodeMember(this, ".zxzx");
		CodeMember xyzx = CodeMember(this, ".xxzx");
		CodeMember yyzx = CodeMember(this, ".yxzx");
		CodeMember zyzx = CodeMember(this, ".zxzx");
		CodeMember xzzx = CodeMember(this, ".xxzx");
		CodeMember yzzx = CodeMember(this, ".yxzx");
		CodeMember zzzx = CodeMember(this, ".zxzx");
		CodeMember xxxy = CodeMember(this, ".xxxy");
		CodeMember yxxy = CodeMember(this, ".yxxy");
		CodeMember zxxy = CodeMember(this, ".zxxy");
		CodeMember xyxy = CodeMember(this, ".xxxy");
		CodeMember yyxy = CodeMember(this, ".yxxy");
		CodeMember zyxy = CodeMember(this, ".zxxy");
		CodeMember xzxy = CodeMember(this, ".xxyy");
		CodeMember yzxy = CodeMember(this, ".yxyy");
		CodeMember zzxy = CodeMember(this, ".zxyy");
		CodeMember xxyy = CodeMember(this, ".xxyy");
		CodeMember yxyy = CodeMember(this, ".yxyy");
		CodeMember zxyy = CodeMember(this, ".zxyy");
		CodeMember xyyy = CodeMember(this, ".xxyy");
		CodeMember yyyy = CodeMember(this, ".yxyy");
		CodeMember zyyy = CodeMember(this, ".zxyy");
		CodeMember xzyy = CodeMember(this, ".xxzy");
		CodeMember yzyy = CodeMember(this, ".yxzy");
		CodeMember zzyy = CodeMember(this, ".zxzy");
		CodeMember xxzy = CodeMember(this, ".xxzy");
		CodeMember yxzy = CodeMember(this, ".yxzy");
		CodeMember zxzy = CodeMember(this, ".zxzy");
		CodeMember xyzy = CodeMember(this, ".xxzy");
		CodeMember yyzy = CodeMember(this, ".yxzy");
		CodeMember zyzy = CodeMember(this, ".zxzy");
		CodeMember xzzy = CodeMember(this, ".xxzy");
		CodeMember yzzy = CodeMember(this, ".yxzy");
		CodeMember zzzy = CodeMember(this, ".zxzy");
		CodeMember xxxz = CodeMember(this, ".xxxz");
		CodeMember yxxz = CodeMember(this, ".yxxz");
		CodeMember zxxz = CodeMember(this, ".zxxz");
		CodeMember xyxz = CodeMember(this, ".xxxz");
		CodeMember yyxz = CodeMember(this, ".yxxz");
		CodeMember zyxz = CodeMember(this, ".zxxz");
		CodeMember xzxz = CodeMember(this, ".xxyz");
		CodeMember yzxz = CodeMember(this, ".yxyz");
		CodeMember zzxz = CodeMember(this, ".zxyz");
		CodeMember xxyz = CodeMember(this, ".xxyz");
		CodeMember yxyz = CodeMember(this, ".yxyz");
		CodeMember zxyz = CodeMember(this, ".zxyz");
		CodeMember xyyz = CodeMember(this, ".xxyz");
		CodeMember yyyz = CodeMember(this, ".yxyz");
		CodeMember zyyz = CodeMember(this, ".zxyz");
		CodeMember xzyz = CodeMember(this, ".xxzz");
		CodeMember yzyz = CodeMember(this, ".yxzz");
		CodeMember zzyz = CodeMember(this, ".zxzz");
		CodeMember xxzz = CodeMember(this, ".xxzz");
		CodeMember yxzz = CodeMember(this, ".yxzz");
		CodeMember zxzz = CodeMember(this, ".zxzz");
		CodeMember xyzz = CodeMember(this, ".xxzz");
		CodeMember yyzz = CodeMember(this, ".yxzz");
		CodeMember zyzz = CodeMember(this, ".zxzz");
		CodeMember xzzz = CodeMember(this, ".xxzz");
		CodeMember yzzz = CodeMember(this, ".yxzz");
		CodeMember zzzz = CodeMember(this, ".zxzz");


		template<class Type1, class Type2>
		CodeLine operator()(Type1 type1, Type2 type2) {
			CodeLine newCodeLine;
			std::cout << typeid(Type1).name() << " " << typeid(Type2).name() << std::endl;
			newCodeLine = newCodeLine << this->getType() << Symbol::PARANTHESIS_LEFT << type1 << Symbol::COMMA << type2 << Symbol::PARANTHESIS_RIGHT;
			return newCodeLine;
		}

		template<class Type1, class Type2, class Type3>
		CodeLine operator()(Type1 type1, Type2 type2, Type3 type3) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << this->getType() << Symbol::PARANTHESIS_LEFT << type1 << Symbol::COMMA << type2 << Symbol::COMMA << type3 << Symbol::PARANTHESIS_RIGHT;
			return newCodeLine;
		}

		template<class Type1, class Type2, class Type3, class Type4>
		CodeLine operator()(Type1 type1, Type2 type2, Type3 type3, Type4 type4) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << this->getType() << Symbol::PARANTHESIS_LEFT << type1 << Symbol::COMMA << type2 << Symbol::COMMA << type3 << Symbol::COMMA << type4 << Symbol::PARANTHESIS_RIGHT;
			return newCodeLine;
		}

	};

	template<class StorageType, class Type, const size_t id>
	class MatrixType : public UniversalType<Type, id> {
	public:

		template<class Type1, class Type2>
		CodeLine operator()(Type1 type1, Type2 type2) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << this->getType() << Symbol::PARANTHESIS_LEFT << type1 << Symbol::COMMA << type2 << Symbol::PARANTHESIS_RIGHT;
			return newCodeLine;
		}

		template<class Type1, class Type2, class Type3>
		CodeLine operator()(Type1 type1, Type2 type2, Type3 type3) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << this->getType() << Symbol::PARANTHESIS_LEFT << type1 << Symbol::COMMA << type2 << Symbol::COMMA << type3 << Symbol::PARANTHESIS_RIGHT;
			return newCodeLine;
		}

		template<class Type1, class Type2, class Type3, class Type4>
		CodeLine operator()(Type1 type1, Type2 type2, Type3 type3, Type4 type4) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << this->getType() << Symbol::PARANTHESIS_LEFT << type1 << Symbol::COMMA << type2 << Symbol::COMMA << type3 << Symbol::COMMA << type4 << Symbol::PARANTHESIS_RIGHT;
			return newCodeLine;
		}

		template<class Type1, class Type2, class Type3, class Type4, class Type5, class Type6>
		CodeLine operator()(Type1 type1, Type2 type2, Type3 type3, Type4 type4, Type5 type5, Type6 type6) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << this->getType() << Symbol::PARANTHESIS_LEFT
				<< type1 << Symbol::COMMA << type2 << Symbol::COMMA << type3 << Symbol::COMMA << type4 << Symbol::COMMA
				<< type5 << Symbol::COMMA << type6
				<< Symbol::PARANTHESIS_RIGHT;
			return newCodeLine;
		}

		template<class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8>
		CodeLine operator()(Type1 type1, Type2 type2, Type3 type3, Type4 type4, Type5 type5, Type6 type6, Type7 type7, Type8 type8) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << this->getType() << Symbol::PARANTHESIS_LEFT
				<< type1 << Symbol::COMMA << type2 << Symbol::COMMA << type3 << Symbol::COMMA << type4 << Symbol::COMMA
				<< type5 << Symbol::COMMA << type6 << Symbol::COMMA << type7 << Symbol::COMMA << type8
				<< Symbol::PARANTHESIS_RIGHT;
			return newCodeLine;
		}

		template<class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10>
		CodeLine operator()(Type1 type1, Type2 type2, Type3 type3, Type4 type4, Type5 type5, Type6 type6, Type7 type7, Type8 type8, Type9 type9, Type10 type10) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << this->getType() << Symbol::PARANTHESIS_LEFT
				<< type1 << Symbol::COMMA << type2 << Symbol::COMMA << type3 << Symbol::COMMA << type4 << Symbol::COMMA
				<< type5 << Symbol::COMMA << type6 << Symbol::COMMA << type7 << Symbol::COMMA << type8
				<< type9 << Symbol::COMMA << type10 << Symbol::COMMA
				<< Symbol::PARANTHESIS_RIGHT;
			return newCodeLine;
		}

		template<class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12>
		CodeLine operator()(Type1 type1, Type2 type2, Type3 type3, Type4 type4, Type5 type5, Type6 type6, Type7 type7, Type8 type8, Type9 type9, Type10 type10, Type11 type11, Type12 type12) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << this->getType() << Symbol::PARANTHESIS_LEFT
				<< type1 << Symbol::COMMA << type2 << Symbol::COMMA << type3 << Symbol::COMMA << type4 << Symbol::COMMA
				<< type5 << Symbol::COMMA << type6 << Symbol::COMMA << type7 << Symbol::COMMA << type8 << Symbol::COMMA
				<< type9 << Symbol::COMMA << type10 << Symbol::COMMA << type11 << Symbol::COMMA << type12
				<< Symbol::PARANTHESIS_RIGHT;
			return newCodeLine;
		}

		template<class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14>
		CodeLine operator()(Type1 type1, Type2 type2, Type3 type3, Type4 type4, Type5 type5, Type6 type6, Type7 type7, Type8 type8, Type9 type9, Type10 type10, Type11 type11, Type12 type12, Type13 type13, Type14 type14) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << this->getType() << Symbol::PARANTHESIS_LEFT
				<< type1 << Symbol::COMMA << type2 << Symbol::COMMA << type3 << Symbol::COMMA << type4 << Symbol::COMMA
				<< type5 << Symbol::COMMA << type6 << Symbol::COMMA << type7 << Symbol::COMMA << type8 << Symbol::COMMA
				<< type9 << Symbol::COMMA << type10 << Symbol::COMMA << type11 << Symbol::COMMA << type12 << Symbol::COMMA
				<< type13 << Symbol::COMMA << type14
				<< Symbol::PARANTHESIS_RIGHT;
			return newCodeLine;
		}

		template<class Type1, class Type2, class Type3, class Type4, class Type5, class Type6, class Type7, class Type8, class Type9, class Type10, class Type11, class Type12, class Type13, class Type14, class Type15, class Type16>
		CodeLine operator()(Type1 type1, Type2 type2, Type3 type3, Type4 type4, Type5 type5, Type6 type6, Type7 type7, Type8 type8, Type9 type9, Type10 type10, Type11 type11, Type12 type12, Type13 type13, Type14 type14, Type15 type15, Type16 type16) {
			CodeLine newCodeLine;
			newCodeLine = newCodeLine << this->getType() << Symbol::PARANTHESIS_LEFT
				<< type1 << Symbol::COMMA << type2 << Symbol::COMMA << type3 << Symbol::COMMA << type4 << Symbol::COMMA
				<< type5 << Symbol::COMMA << type6 << Symbol::COMMA << type7 << Symbol::COMMA << type8 << Symbol::COMMA
				<< type9 << Symbol::COMMA << type10 << Symbol::COMMA << type11 << Symbol::COMMA << type12 << Symbol::COMMA
				<< type13 << Symbol::COMMA << type14 << Symbol::COMMA << type15 << Symbol::COMMA << type16
				<< Symbol::PARANTHESIS_RIGHT;
			return newCodeLine;
		}
	};

	struct Bool : public FundamentalType<Bool, 0> { Bool(bool type) {} using UniversalType<Bool, 0>::operator=; };
	struct Int : public FundamentalType<Int, 1> { Int(int type) {} using UniversalType<Int, 1>::operator=; };
	struct Float : public FundamentalType<Float, 2> { Float(float type) {} using UniversalType<Float, 2>::operator=; };
	struct Double : public FundamentalType<Double, 3> { Double(double type) {} using UniversalType<Double, 3>::operator=; };

	template<class StorageType> struct Vec2 : public VectorType<StorageType, Vec2<StorageType>, 4> { using UniversalType<Vec2<StorageType>, 4>::operator=; };
	template<class StorageType> struct Vec3 : public VectorType<StorageType, Vec3<StorageType>, 5> { using UniversalType<Vec3<StorageType>, 5>::operator=; };
	template<class StorageType> struct Vec4 : public VectorType<StorageType, Vec4<StorageType>, 6> { using UniversalType<Vec4<StorageType>, 6>::operator=; };

	template<class StorageType> struct Mat2 : public MatrixType<StorageType, Mat2<StorageType>, 7> { using UniversalType<Mat2<StorageType>, 7>::operator=; };
	template<class StorageType> struct Mat3 : public MatrixType<StorageType, Mat3<StorageType>, 8> { using UniversalType<Mat3<StorageType>, 8>::operator=; };
	template<class StorageType> struct Mat4 : public MatrixType<StorageType, Mat4<StorageType>, 9> { using UniversalType<Mat4<StorageType>, 9>::operator=; };

	inline Vec2<Float> vec2;
	inline Vec3<Float> vec3;
	inline Mat3<Float> mat3;


};


using shr  = ShaderCode;

//Have Action types such as swizzle use the type system as above. Such that the type inherits the universal type and its interface type and when used it will call it will add the swizzle and attached type functions seperately.
//Many functions currently do the same thing despite being for different operators, is there a way to operator overload once and have the related types used them regardless?
//Have constructors return their respective types which then can be fed to an operator

//Use array instead of vector, maybe later try constexpr compiler to count number of objects created during the program.