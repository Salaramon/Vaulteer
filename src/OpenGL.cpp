#include "vpch.h"
#include "OpenGL.h"

#include <stb_image.h>

void OpenGL::initialize() {
	glDebugMessageCallback(debugCallback, nullptr);
	//log.debug("OpenGL debug callback set.\n", "glDebugMessageCallback");
}

void OpenGL::initializeSTBI() {
	stbi_set_flip_vertically_on_load(true);
}

void OpenGL::enableDepthTest() {
	glEnable(GL_DEPTH_TEST);
	//log.debug("GL_DEPTH_TEST enabled.\n", "glEnable()");
}

void OpenGL::disableDepthTest() {
	glDisable(GL_DEPTH_TEST);
	//log.debug("GL_DEPTH_TEST glDisable.\n", "glDisable()");
}

void OpenGL::enableStencilTest() {
	// query 
	/*GLint drawFboId = 0;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);*/
	glEnable(GL_STENCIL_TEST);
	//log.debug("GL_STENCIL_TEST glEnable.\n", "glEnable()");
}

void OpenGL::disableStencilTest() {
	glDisable(GL_STENCIL_TEST);
	//log.debug("GL_STENCIL_TEST glDisable.\n", "glDisable()");
}

void OpenGL::enableDebugOutput(GLenum mode) {
	glEnable(GL_DEBUG_OUTPUT);
	//log.debug("GL_DEBUG_OUTPUT enabled.\n", "glEnable()");
	debugModes = mode;
	if (mode & SYNC) {
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		//log.debug("GL_DEBUG_OUTPUT_SYNCHRONOUS enabled.\n", "glEnable()");
	}
}

void OpenGL::disableDebugOutput() {
	glDisable(GL_DEBUG_OUTPUT);
	//log.debug("GL_DEBUG_OUTPUT disabled.\n", "glDisable()");
	if (debugModes & SYNC) {
		glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		//log.debug("GL_DEBUG_OUTPUT_SYNCHRONOUS disabled.\n", "glDisable()");
	}
	debugModes = 0;
}

void OpenGL::enableCullFace(GLenum faces) {
	glEnable(GL_CULL_FACE);
	//log.debug("GL_CULL_FACE enabled.\n", "glEnable()");
	if (faces == (FRONT | BACK)) {
		glCullFace(GL_FRONT_AND_BACK);
		//log.debug("Cullface set to GL_FRONT_AND_BACK.\n", "glCullFace()");
	}
	else {
		glCullFace(faces);
		//log.debug("Cullface set: " + std::to_string(faces) + "\n", "glCullFace()");
	}
}

void OpenGL::enableCullFace() {
	glEnable(GL_CULL_FACE);
	//log.debug("GL_CULL_FACE enabled.\n", "glEnable()");
}

void OpenGL::disableCullFace() {
	glDisable(GL_CULL_FACE);
	//log.debug("GL_CULL_FACE disabled.\n", "glDisable()");
}

void OpenGL::enableDirectDebugMessageing() {
	directMessaging = true;
	//log.debug("Direct messaging enabled: Messages will be directly printed to the console and logged.\n", "enableDirectDebugMessageing()");
}

void OpenGL::disableDirectDebugMessageing() {
	directMessaging = false;
	//log.debug("Direct messaging disabled: Messages will only be logged.\n", "disableDirectDebugMessageing()");
}

void OpenGL::enableSRGB() {
	glEnable(GL_FRAMEBUFFER_SRGB);
}

void OpenGL::disableSRGB() {
	glDisable(GL_FRAMEBUFFER_SRGB);
}

void OpenGL::enableBlending() {
	glEnable(GL_BLEND);
}

void OpenGL::disableBlending() {
	glDisable(GL_BLEND);
}

void OpenGL::setBlendMode(GLenum sourceFactor, GLenum destFactor) {
	glBlendFunc(sourceFactor, destFactor);
}

void OpenGL::setBlendMode(GLint target, GLenum sourceFactor, GLenum destFactor) {
	glBlendFunci(target, sourceFactor, destFactor);
}
