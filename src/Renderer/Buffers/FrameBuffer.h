#pragma once
#include "Model/Textures/Texture2D.h"
#include "Utils/TypeDefUtils.h"


struct FBAttachmentSpecification {
	GLenum textureFormat = GL_NONE;
	GLenum minFilter = GL_LINEAR, magFilter = GL_LINEAR;
	GLenum wrapX = GL_REPEAT, wrapY = GL_REPEAT;
};

struct FramebufferSpecification {
	uint width = 0, height = 0;
	uint samples = 1;
	
	std::vector<FBAttachmentSpecification> attachments;

	FramebufferSpecification() = default;

	FramebufferSpecification(uint width, uint height, uint samples = 1) : width(width), height(height), samples(samples) {}

	FramebufferSpecification(uint width, uint height, std::initializer_list<FBAttachmentSpecification> attachments, uint samples = 1)
		: width(width), height(height), samples(samples), attachments(std::move(attachments)) {}
};


class Framebuffer {
public:
	GLuint fbo = 0;
	FramebufferSpecification specification;

	std::vector<FBAttachmentSpecification> colorAttachments;
	FBAttachmentSpecification depthAttachment;

	std::vector<GLuint> colorTextures;
	GLuint depthTexture;

	Framebuffer(FramebufferSpecification spec) : specification(std::move(spec)) {
		for (auto& attachment : specification.attachments) {
			if (attachment.textureFormat == GL_DEPTH24_STENCIL8 || attachment.textureFormat == GL_DEPTH32F_STENCIL8)
				depthAttachment = attachment;
			else
				colorAttachments.push_back(attachment);
		}

		invalidate();
	}

	Framebuffer(Framebuffer&& other) noexcept : fbo(other.fbo), specification(other.specification) {
		other.fbo = 0;
	}

	~Framebuffer() {
		// 12 hours were wasted here by copypasting glDeleteBuffers from Buffer.h
		glDeleteFramebuffers(1, &fbo);
	}

	void invalidate() {
		if (fbo) {
			glDeleteFramebuffers(1, &fbo);
			glDeleteTextures(colorTextures.size(), colorTextures.data());
			glDeleteTextures(1, &depthTexture);
			
			colorTextures.clear();
			depthTexture = 0;
		}
		
		glCreateFramebuffers(1, &fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

		std::vector<GLenum> drawBuffers(colorAttachments.size(), GL_NONE);

		if (!colorAttachments.empty()) {
			colorTextures.resize(colorAttachments.size());
			glCreateTextures(GL_TEXTURE_2D, colorAttachments.size(), colorTextures.data());

			for (int i = 0; i < colorAttachments.size(); i++) {
				FBAttachmentSpecification& attachment = colorAttachments[i];
				glTextureStorage2D(colorTextures[i], 1, attachment.textureFormat, specification.width, specification.height);
				
				glTextureParameteri(colorTextures[i], GL_TEXTURE_MIN_FILTER, attachment.minFilter);
				glTextureParameteri(colorTextures[i], GL_TEXTURE_MAG_FILTER, attachment.magFilter);
				glTextureParameteri(colorTextures[i], GL_TEXTURE_WRAP_S, attachment.wrapX);
				glTextureParameteri(colorTextures[i], GL_TEXTURE_WRAP_T, attachment.wrapY);

				GLenum attachmentVal = GL_COLOR_ATTACHMENT0 + i;
				glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentVal, GL_TEXTURE_2D, colorTextures[i], 0);
				drawBuffers[i] = attachmentVal;
			}
		}

		if (depthAttachment.textureFormat != GL_NONE) {
			glCreateTextures(GL_TEXTURE_2D, 1, &depthTexture);
			
			glTextureStorage2D(depthTexture, 1, depthAttachment.textureFormat, specification.width, specification.height);
			
			glTextureParameteri(depthTexture, GL_TEXTURE_MIN_FILTER, depthAttachment.minFilter);
			glTextureParameteri(depthTexture, GL_TEXTURE_MAG_FILTER, depthAttachment.magFilter);
			glTextureParameteri(depthTexture, GL_TEXTURE_WRAP_S, depthAttachment.wrapX);
			glTextureParameteri(depthTexture, GL_TEXTURE_WRAP_T, depthAttachment.wrapY);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
		}

		if (!colorAttachments.empty()) {
			glDrawBuffers(colorTextures.size(), drawBuffers.data());
		}

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		assert(status == GL_FRAMEBUFFER_COMPLETE);

		// restore default FBO
		unbind();
	}


	void resize(uint width, uint height) {
		specification.width = width;
		specification.height = height;

		invalidate();
	}


	void bindForWriting() const {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	}

	void bindForReading() const {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	}

	void unbind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void bindTextureUnit(int unitIndex) {
		glBindTextureUnit(unitIndex, colorTextures[unitIndex]);
	}

	void clear() {
		clearColor();
		clearDepthStencil();
	}

	void clearColor(float value = 0.0) {
		for (int i = 0; i < colorAttachments.size(); i++)
 			clearAttachment(i, value);
	}

	void clearDepthStencil(float depth = 1.0, int stencil = 0) {
		glClearNamedFramebufferfi(fbo, GL_DEPTH_STENCIL, 0, depth, stencil);
	}

	void clearAttachment(int index, int value = 0) {
		assert(index < colorAttachments.size());
 		glClearNamedFramebufferiv(fbo, GL_COLOR, index, &value);
	}
	void clearAttachment(int index, float value = 0.0) {
		assert(index < colorAttachments.size());
 		glClearNamedFramebufferfv(fbo, GL_COLOR, index, &value);
	}


	static void copyDepth(const Framebuffer& from, const Framebuffer& to) {
		from.bindForReading();
		to.bindForWriting();
		
		glBlitFramebuffer(
			0, 0, from.specification.width, from.specification.height, 
			0, 0, to.specification.width, to.specification.height, 
			GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

		to.unbind();
	}

	static void copyDepthFromBackFBO(const Framebuffer& to) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		to.bindForWriting();
		
		glBlitFramebuffer(
			0, 0, to.specification.width, to.specification.height, 
			0, 0, to.specification.width, to.specification.height, 
			GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

		to.unbind();
	}

	static void copyDepthToBackFBO(const Framebuffer& from) {
		from.bindForWriting();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		
		glBlitFramebuffer(
			0, 0, from.specification.width, from.specification.height, 
			0, 0, from.specification.width, from.specification.height, 
			GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

		from.unbind();
	}
};