#pragma once

#include <glad/glad.h>

#include "Debug/DebugLogger.h"
#include "Debug/DebugAliases.h"

class OpenGL {
public:
	static void APIENTRY debugCallback(GLenum source​, GLenum type​, GLuint id​,
	                                   GLenum severity​, GLsizei length​, const GLchar* message​, const void* userParam​) {
		DebugLogger<FUNCTION> log(ObjectAlias::OpenGLMessage);
		if (directMessaging) {
			std::cout << std::string(message​) + '\n' << std::endl;
		}
		log.debug(std::string(message​) + '\n', MessageAlias::OpenGLMessage);
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

	inline static DebugLogger<OpenGL> log;
	inline static bool directMessaging = false;

	inline static GLenum debugModes = 0;

	struct BlendModes {
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
};
