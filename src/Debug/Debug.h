#pragma once

//#define DEBUG_ENABLED 1

#include <format>
#include <functional>
#include <glm/gtx/string_cast.hpp>
#include <typeinfo>

#include <DebugYourself.h>

template<class Enable = dy::State>
struct Format;

template<>
struct Format<dy::Enabled> {
public:
	template<class... Types>
	static std::string std_format(const std::_Fmt_string<Types...> fmt, Types&&... types) { return std::format(fmt, std::forward<Types>(types)...); }

	template<class genType>
	static std::string glm_to_string(const genType &x) { return glm::to_string(std::forward<const genType>(x)); }

	template<class T>
	static std::string std_to_string(const T& var) { return std::to_string(std::forward<const T>(var)); }

	template<class... Args>
	static std::string types_to_string() {
		return ((std::string(typeid(Args).name()) + " ") + ...);
	}

	template<class... Args>
	static std::string pack_to_string(Args... args) {
		return ((std::to_string(args) + " ") + ...);
	}
};

template<>
struct Format<dy::Disabled> {
public:
	template<class... Args>
	static std::nullptr_t std_format(Args...) { return nullptr; }

	template<class... Args>
	static std::nullptr_t glm_to_string(Args...) { return nullptr; }

	template<class... Args>
	static std::nullptr_t std_to_string(Args...) { return nullptr; }

	template<class... Args>
	static std::nullptr_t types_to_string() { return nullptr; }

	template<class... Args>
	static std::nullptr_t pack_to_string(Args...) { return nullptr; }
};

#ifdef DEBUG_ENABLED
	#define DEBUG_STATE dy::CONFIGURATION_STATE
#else
	#define DEBUG_STATE false
#endif

using DY = dy::DebugYourself<false, Format<>>;

template<class C, class O, class F, class V>
using DY_LINK = DY::Dependencies<C, O, F, V, DY::AlwaysTrue, DY::DirectLogger<DY::AlwaysFalse>>;

template<class C, class O, class F, class V>
struct _LOG {
public:
	using ALL = DY_LINK<C, O, F, V>;
	using GLOB = DY_LINK<C, O, F, V>::Global;
	using CLAS = DY_LINK<C, O, F, V>::Class;
	using STAT = DY_LINK<C, O, F, V>::Static;
	using CTOR = DY_LINK<C, O, F, V>::Constructor;
	using SPGL = DY_LINK<C, O, F, V>::Special::Global;
	using SPST = DY_LINK<C, O, F, V>::Special::Static;
	using SPCT = DY_LINK<C, O, F, V>::Special::Constructor;
};

using DIRLOG = _LOG<DY::No_CB, DY::No_OB, DY::No_FB, DY::No_VB>;




namespace Tag {
	namespace Class {
		namespace ShaderProgram {
			constexpr char LoadingProcedure[] = "ShaderProgram_LoadingProcedure";
		}
	}
}


namespace ObjectAlias {
	constexpr char OpenGLMessage[] = "OpenGLMessage";
	constexpr char Main[] = "Main";
	constexpr char STBI[] = "Library_STBI";
	constexpr char GLFW[] = "Library_GLFW";
}

namespace A_Severity {
	constexpr char CriticalError[] = "CriticalError";
	constexpr char Error[] = "Error";
	constexpr char Warining[] = "Warning";
	constexpr char Message[] = "Message";
}


namespace DebugUtils {
	template<class T>
	std::string arrayToString(T& arr, auto func ) {
		std::string returnString = "[";
		for (auto& e : arr) {
			returnString += func(e) + ", ";
		}
		returnString.erase(returnString.end() - 2, returnString.end());
		returnString = "]";

		return returnString;
	}
}
