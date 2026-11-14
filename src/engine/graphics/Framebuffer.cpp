#include "Framebuffer.h"
#include "../utils/Log.h"


// =================================================


void nothing::Framebuffer::Init(int windowWidth, int windowHeight)
{

	glGenFramebuffers(1, &framebufferID_);
	glGenTextures(1, &generatedTextureID_);


	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID_);
	glBindTexture(GL_TEXTURE_2D, generatedTextureID_);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, generatedTextureID_, 0);


	glGenRenderbuffers(1, &renderbufferID_);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID_);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferID_);


	GLenum fboStats = glCheckFramebufferStatus(GL_FRAMEBUFFER);


	if (fboStats != GL_FRAMEBUFFER_COMPLETE)
	{

		nothing::LogError("Framebuffer err. : " + std::to_string(fboStats));

	}


	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


// =================================================


void nothing::Framebuffer::BeginRender()
{

	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID_);

}


// =================================================


void nothing::Framebuffer::EndRender()
{

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


// =================================================


uint32_t nothing::Framebuffer::GetGeneratedTextureID()
{

	return generatedTextureID_;

}


// =================================================