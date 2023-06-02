#include "vpch.h"
#include "OpenGL.h"

#include <stb_image.h>

void OpenGL::initialize() {
	glDebugMessageCallback(debugCallback, nullptr);
	
	depthTest(true);
	stencilTest(true);
	SRGB(true);
	debugOutput(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	directDebugMessaging(true);
	
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
	//stbi_set_flip_vertically_on_load(true);
}

void OpenGL::depthMask(bool enabled) {
	glDepthMask(enabled);
}

void OpenGL::depthTest(bool enabled) {
	if (enabled) glEnable(GL_DEPTH_TEST);
	else         glDisable(GL_DEPTH_TEST);
}

void OpenGL::stencilTest(bool enabled) {
	if (enabled) glEnable(GL_STENCIL_TEST);
	else         glDisable(GL_STENCIL_TEST);
}

void OpenGL::debugOutput(GLenum mode) {
	if (mode) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(mode);
	}
	else {
		glDisable(GL_DEBUG_OUTPUT);
	}
	debugModes = mode;
}

void OpenGL::cullFace(GLenum faces) {
	if (faces) {
		glEnable(GL_CULL_FACE);
		glCullFace(faces);
	}
	else glDisable(GL_CULL_FACE);
}

void OpenGL::directDebugMessaging(bool enabled) {
	directMessaging = enabled;
}

void OpenGL::SRGB(bool enabled) {
	if (enabled) glEnable(GL_FRAMEBUFFER_SRGB);
	else		 glDisable(GL_FRAMEBUFFER_SRGB);
}

void OpenGL::blending(bool enabled) {
	if (enabled) glEnable(GL_BLEND);
	else		 glDisable(GL_BLEND);
}

void OpenGL::setBlendMode(GLenum sourceFactor, GLenum destFactor) {
	glBlendFunc(sourceFactor, destFactor);
}

void OpenGL::setBlendMode(GLint target, GLenum sourceFactor, GLenum destFactor) {
	glBlendFunci(target, sourceFactor, destFactor);
}
