#pragma once

#include <glad/glad.h>

#include "DebugLogger.h"
#include "DebugAliases.h"

class OpenGL
{
public:
	inline static void APIENTRY debugCallback(GLenum source​, GLenum type​, GLuint id​,
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

	static void enableDepthTest();
	static void disableDepthTest();
	static void enableDebugOutput(GLenum mode = 0);
	static void disableDebugOutput();
	static void enableCullFace();
	static void enableCullFace(GLenum faces);
	static void disableCullFace();
	static void enableDirectDebugMessageing();
	static void disableDirectDebugMessageing();

	inline static DebugLogger<OpenGL> log;
	inline static bool directMessaging = false;

	inline static GLenum debugModes = 0;

};

