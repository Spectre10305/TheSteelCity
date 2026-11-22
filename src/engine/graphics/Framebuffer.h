#pragma once
#include <glad/glad.h>
#include <cstdint>

namespace nothing
{

	// Wrapper per framebuffer OpenGL
	class Framebuffer
	{

	public:

		void     Init(int genTexWidth, int genTexHeight, bool storeDepth, bool hdr);
		void     BeginRender();
		void     EndRender();
		uint32_t GetGeneratedTextureID();

	private:

		uint32_t framebufferID_;
		uint32_t generatedTextureID_;
		uint32_t renderbufferID_;

	};

}