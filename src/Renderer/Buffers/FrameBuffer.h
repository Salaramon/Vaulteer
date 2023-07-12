#pragma once
#include "Model/Textures/Texture2D.h"
#include "Utils/TypeDefUtils.h"


struct FBAttachmentSpecification {
	GLenum textureFormat = GL_NONE;
	GLenum minFilter = GL_LINEAR, magFilter = GL_LINEAR;
	GLenum wrapX = GL_REPEAT, wrapY = GL_REPEAT;
	GLenum depthStencilMode = GL_DEPTH_COMPONENT;
};

struct FramebufferSpecification {
	int width = 0, height = 0, layers;
	GLenum textureType;
	//uint samples = 1;

	std::vector<FBAttachmentSpecification> attachments;

	FramebufferSpecification() = default;

	FramebufferSpecification(int width, int height, GLenum textureType = GL_TEXTURE_2D, uint layers = 1)
		: width(width), height(height), textureType(textureType), layers(layers) {}

	FramebufferSpecification(int width, int height, std::initializer_list<FBAttachmentSpecification> attachments, GLenum textureType = GL_TEXTURE_2D, uint layers = 1)
		: width(width), height(height), textureType(textureType), layers(layers), attachments(attachments) {}
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
			if (attachment.textureFormat == GL_DEPTH24_STENCIL8 || 
				attachment.textureFormat == GL_DEPTH32F_STENCIL8 || 
				attachment.textureFormat == GL_STENCIL_INDEX8)
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
		glDeleteTextures(colorTextures.size(), colorTextures.data());
		glDeleteTextures(1, &depthTexture);
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
			glCreateTextures(specification.textureType, colorAttachments.size(), colorTextures.data());

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
			glCreateTextures(specification.textureType, 1, &depthTexture);

			switch (specification.textureType) {
			case GL_TEXTURE_2D:
				glTextureStorage2D(depthTexture, 1, depthAttachment.textureFormat, specification.width, specification.height);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, specification.textureType, depthTexture, 0);
				break;
				
			case GL_TEXTURE_2D_ARRAY:
				glTextureStorage3D(depthTexture, 1, depthAttachment.textureFormat, specification.width, specification.height, specification.layers);
				glNamedFramebufferTexture(fbo, GL_DEPTH_STENCIL_ATTACHMENT, depthTexture, 0);
				

				bindDepthLayer(0);
				break;

			default:
				KYSE_ASSERT(false, "texture type is not implemented!");
				break;
			}
				
			glTextureParameteri(depthTexture, GL_TEXTURE_MIN_FILTER, depthAttachment.minFilter);
			glTextureParameteri(depthTexture, GL_TEXTURE_MAG_FILTER, depthAttachment.magFilter);
			glTextureParameteri(depthTexture, GL_TEXTURE_WRAP_S, depthAttachment.wrapX);
			glTextureParameteri(depthTexture, GL_TEXTURE_WRAP_T, depthAttachment.wrapY);
			glTextureParameteri(depthTexture, GL_DEPTH_STENCIL_TEXTURE_MODE, depthAttachment.depthStencilMode);
		}

		if (!colorAttachments.empty()) {
			glDrawBuffers(colorTextures.size(), drawBuffers.data());
		}

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		KYSE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE);

		// restore default FBO
		unbind();
	}


	void resize(uint width, uint height) {
		specification.width = width;
		specification.height = height;

		invalidate();
	}

	//void growColorLayers(uint layers) {}

	void growDepthLayers(uint layers) {
		specification.layers = layers;

		GLuint temp = depthTexture;
		glCreateTextures(specification.textureType, 1, &depthTexture);

		glTextureStorage3D(depthTexture, 1, depthAttachment.textureFormat, specification.width, specification.height, specification.layers);
		glNamedFramebufferTexture(fbo, GL_DEPTH_STENCIL_ATTACHMENT, depthTexture, 0);
		bindDepthLayer(0);

		glDeleteFramebuffers(1, &temp);
	}


	void bindForWriting() const {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	}

	void bindForReading() const {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	}

	static void unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void bindColorLayer(int attachment, int i) {
		KYSE_ASSERT(attachment < colorAttachments.size());
		KYSE_ASSERT(i < specification.layers);
		glNamedFramebufferTextureLayer(fbo, GL_COLOR_ATTACHMENT0 + attachment, colorTextures[attachment], 0, i);
	}

	void bindDepthLayer(int i) {
		glNamedFramebufferTextureLayer(fbo, GL_DEPTH_STENCIL_ATTACHMENT, depthTexture, 0, i);
	}

	void bindColorTextureUnit(int unitIndex) {
		glBindTextureUnit(unitIndex, colorTextures[unitIndex]);
	}


	void clear() {
		clearColor();
		clearDepthStencil();
	}

	void clearColor(float value = 0.0) {
		for (int i = 0; i < colorAttachments.size(); i++)
 			clearColorAttachment(i, value);
	}

	void clearDepthStencil(float depth = 1.0, int stencil = 0) {
		glClearNamedFramebufferfi(fbo, GL_DEPTH_STENCIL, 0, depth, stencil);
	}

	void clearColorAttachment(int index, int value = 0) {
		KYSE_ASSERT(index < colorAttachments.size());
 		glClearNamedFramebufferiv(fbo, GL_COLOR, index, &value);
	}
	void clearColorAttachment(int index, float value = 0.0) {
		KYSE_ASSERT(index < colorAttachments.size());
 		glClearNamedFramebufferfv(fbo, GL_COLOR, index, &value);
	}


	static void copyDepth(const Framebuffer& from, const Framebuffer& to) {
		from.bindForReading();
		to.bindForWriting();
		
		glBlitFramebuffer(
			0, 0, from.specification.width, from.specification.height, 
			0, 0, to.specification.width, to.specification.height, 
			GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

		unbind();
	}

	static void copyDepthFromBackFBO(const Framebuffer& to) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		to.bindForWriting();
		
		glBlitFramebuffer(
			0, 0, to.specification.width, to.specification.height, 
			0, 0, to.specification.width, to.specification.height, 
			GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

		unbind();
	}

	static void copyDepthToBackFBO(const Framebuffer& from) {
		from.bindForWriting();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		
		glBlitFramebuffer(
			0, 0, from.specification.width, from.specification.height, 
			0, 0, from.specification.width, from.specification.height, 
			GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

		unbind();
	}
};