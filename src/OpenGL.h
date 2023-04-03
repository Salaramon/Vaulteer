#pragma once

#include <glad/glad.h>


struct GLBlendModes {
	inline static constexpr GLenum Zero = GL_ZERO;
	inline static constexpr GLenum One = GL_ONE;
	inline static constexpr GLenum SourceColor = GL_SRC_COLOR;
	inline static constexpr GLenum OneMinusSourceColor = GL_ONE_MINUS_SRC_COLOR;
	inline static constexpr GLenum DestColor = GL_DST_COLOR;
	inline static constexpr GLenum OneMinusDestColor = GL_ONE_MINUS_DST_COLOR;
	inline static constexpr GLenum SourceAlpha = GL_SRC_ALPHA;
	inline static constexpr GLenum OneMinusSourceAlpha = GL_ONE_MINUS_SRC_ALPHA;
	inline static constexpr GLenum DestAlpha = GL_DST_ALPHA;
	inline static constexpr GLenum OneMinusDestAlpha = GL_ONE_MINUS_DST_ALPHA;
	inline static constexpr GLenum ConstantColor = GL_CONSTANT_COLOR;
	inline static constexpr GLenum OneMinusConstantColor = GL_ONE_MINUS_CONSTANT_COLOR;
	inline static constexpr GLenum ConstantAlpha = GL_CONSTANT_ALPHA;
	inline static constexpr GLenum OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA;
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

	//Debug enumeration
	inline static const GLenum SYNC = 0x01;

	static void initialize();
	static void initializeSTBI();

	static void enableDepthTest();
	static void disableDepthTest();
	static void enableStencilTest();
	static void disableStencilTest();
	static void enableDebugOutput(GLenum mode = 0);
	static void disableDebugOutput();
	static void enableCullFace();
	static void enableCullFace(GLenum faces);
	static void disableCullFace();
	static void enableDirectDebugMessageing();
	static void disableDirectDebugMessageing();
	static void enableSRGB();
	static void disableSRGB();

	static void enableBlending();
	static void disableBlending();
	static void setBlendMode(GLenum sourceFactor, GLenum destFactor);
	static void setBlendMode(GLint target, GLenum sourceFactor, GLenum destFactor);

	inline static bool directMessaging = false;

	inline static GLenum debugModes = 0;
};
