#pragma once

#include <format>

#include <DebugYourself.h>

using DY = dy::DebugYourself<dy::CONFIGURATION_STATE>;

template<class C, class O, class F, class V>
using DY_LINK = DY::Dependencies<C, O, F, V, DY::DirectLogger<DY::AlwaysFalse>>;

template<class C, class O, class F, class V>
struct _LOG {
public:
	using ALL = DY_LINK<C, O, F, V>;
	using GLOB = DY_LINK<C, O, F, V>::Global;
	using CLAS = DY_LINK<C, O, F, V>::Class;
	using CTOR = DY_LINK<C, O, F, V>::Constructor;
	using SPEC = DY_LINK<C, O, F, V>::Special;
};


namespace A_Level {
	constexpr char Constructor[] = "Constructor";
	constexpr char Main[] = "Main";
	constexpr char STBI[] = "Library_STBI";
	constexpr char GLFW[] = "Library_GLFW";
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