#include "RenderManager.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include "../utils/Log.h"


#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl3.h"
#include "IMGUI/imgui_impl_opengl3.h"


// =================================================


void nothing::RenderManager::Init()
{

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{

		nothing::LogError("Failed to initialize GLAD");
		return;

	}
	else
	{

		nothing::LogInfo("GLAD initialized successfully");
		nothing::LogInfo("OpenGL version: " + std::string((const char*)glGetString(GL_VERSION)));
		nothing::LogInfo("Vendor: " + std::string((const char*)glGetString(GL_VENDOR)));

	}


	// TEST TRIANGOLO



	float vertices[] =
	{

		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f

	};


	unsigned int indices[] =
	{

		0, 1, 3,
		1, 2, 3

	};


	glGenVertexArrays(1, &VAO_);
	glGenBuffers(1, &VBO_);
	glGenBuffers(1, &EBO_);
	glBindVertexArray(VAO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	const char* vsName = "D:\\TheSteelCity\\assets\\engine\\shaders\\test_shader_vert.glsl";
	const char* fsName = "D:\\TheSteelCity\\assets\\engine\\shaders\\test_shader_frag.glsl";


	testShader = new nothing::Shader(vsName, fsName);
	testShader->Use();
	testShader->SetUniform("colR", 0.5f);
	testShader->SetUniform("colG", 0.6f);
	testShader->SetUniform("colB", 0.3f);

}


// =================================================


void nothing::RenderManager::Update()
{

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	
	glBindVertexArray(VAO_);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}


// =================================================


void nothing::RenderManager::Shutdown()
{

	delete testShader;

}


// =================================================


void nothing::RenderManager::ResizeGLViewport(int w, int h)
{

	glViewport(0, 0, w, h);

}


// =================================================