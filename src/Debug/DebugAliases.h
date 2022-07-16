#pragma once

#include <string>

namespace ObjectAlias {
	constexpr char OpenGLMessage[] = "OpenGLMessage";
	constexpr char Main[] = "Main";
	constexpr char STBI[] = "Library_STBI";
	constexpr char GLFW[] = "Library_GLFW";
}

namespace MessageAlias {
	constexpr char OpenGLMessage[] = "OpenGLMessage";

	constexpr char ShaderOutput[] = "ShaderOutput";
	constexpr char ShaderError[] = "ShaderError";

	//Break on critical error
	constexpr char CriticalError[] = "CriticalError";

	constexpr char Error[] = "Error";
	constexpr char Warining[] = "Warning";
	constexpr char Message[] = "Message";
}