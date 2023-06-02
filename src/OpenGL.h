#pragma once

#include <glad/glad.h>


enum GLBlendModes {
	Zero = GL_ZERO,
	One = GL_ONE,
	SourceColor = GL_SRC_COLOR,
	OneMinusSourceColor = GL_ONE_MINUS_SRC_COLOR,
	DestColor = GL_DST_COLOR,
	OneMinusDestColor = GL_ONE_MINUS_DST_COLOR,
	SourceAlpha = GL_SRC_ALPHA,
	OneMinusSourceAlpha = GL_ONE_MINUS_SRC_ALPHA,
	DestAlpha = GL_DST_ALPHA,
	OneMinusDestAlpha = GL_ONE_MINUS_DST_ALPHA,
	ConstantColor = GL_CONSTANT_COLOR,
	OneMinusConstantColor = GL_ONE_MINUS_CONSTANT_COLOR,
	ConstantAlpha = GL_CONSTANT_ALPHA,
	OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA,
};

class OpenGL {
public:
	static void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
			return;

		const char* severityStr = [severity] {
			switch (severity) {
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
			case GL_DEBUG_SEVERITY_LOW: return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
			}
		}();
		const char* srcStr = [source] {
			switch (source) {
			case GL_DEBUG_SOURCE_API: return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
			case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
			case GL_DEBUG_SOURCE_OTHER: return "OTHER";
			}
		}();
		const char* typeStr = [type] {
			switch (type) {
			case GL_DEBUG_TYPE_ERROR: return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER: return "MARKER";
			case GL_DEBUG_TYPE_OTHER: return "OTHER";
			}
		}();
		std::cout << srcStr << ", " << typeStr << ", " << severityStr /* << ", " << id */ << ": " << message << '\n';
	}

	//Face culling enumerations
	inline static const GLenum FRONT = GL_FRONT;
	inline static const GLenum BACK = GL_BACK;
	inline static const GLenum FRONT_AND_BACK = GL_FRONT_AND_BACK;

	static void initialize();

	static void depthMask(bool enabled);
	static void depthTest(bool enabled);
	static void stencilTest(bool enabled);

	static void directDebugMessaging(bool enabled);
	static void SRGB(bool enabled);
	static void blending(bool enabled);

	static void debugOutput(GLenum mode);
	static void cullFace(GLenum faces);

	static void setBlendMode(GLenum sourceFactor, GLenum destFactor);
	static void setBlendMode(GLint target, GLenum sourceFactor, GLenum destFactor);

	inline static bool directMessaging = false;

	inline static GLenum debugModes = GL_NONE;
};
