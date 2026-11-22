#include "Framebuffer.h"
#include "../utils/Log.h"


// =================================================


void nothing::Framebuffer::Init(int genTexWidth, int genTexHeight, bool storeDepth, bool hdr)
{

	glGenFramebuffers(1, &framebufferID_);
	glGenTextures(1, &generatedTextureID_);


	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID_);
	glBindTexture(GL_TEXTURE_2D, generatedTextureID_);


	GLint format   = storeDepth ? GL_DEPTH_COMPONENT : GL_RGB;
	GLint type     = storeDepth ? GL_FLOAT : GL_UNSIGNED_BYTE;
	GLint wrapMode = storeDepth ? GL_REPEAT : GL_CLAMP_TO_EDGE;


	if (!hdr)
	{

		glTexImage2D(GL_TEXTURE_2D, 0, format, genTexWidth, genTexHeight, 0, format, type, NULL);

	}
	else
	{

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, genTexWidth, genTexHeight, 0, GL_RGBA, GL_FLOAT, NULL);

	}


	GLint nearestOrLinear = storeDepth ? GL_NEAREST : GL_LINEAR;


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, nearestOrLinear);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, nearestOrLinear);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	


	if (storeDepth)
	{

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, generatedTextureID_, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

	}
	else
	{

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, generatedTextureID_, 0);
		glGenRenderbuffers(1, &renderbufferID_);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, genTexWidth, genTexHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferID_);

	}


	GLenum fboStats = glCheckFramebufferStatus(GL_FRAMEBUFFER);


	if (fboStats != GL_FRAMEBUFFER_COMPLETE)
	{

		nothing::LogError("Framebuffer err. : " + std::to_string(fboStats));

	}
	else
	{

		nothing::LogInfo("Created framebuffer");

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