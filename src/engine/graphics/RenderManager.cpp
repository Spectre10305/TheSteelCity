#include "RenderManager.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include "../utils/Log.h"

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_sdl3.h"
#include "IMGUI/imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


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

	f += 0.5f;
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(bgR_, bgG_, bgB_, 1.0f);


	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 proj = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(90.0f), aspectRatioN_ / aspectRatioD_, 0.1f, 10.0f);


	glm::mat4 mvp = proj * view * model;


	testShader->SetUniform("MVP", mvp);

	
	glBindVertexArray(VAO_);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();


	ImGui::ShowDemoWindow();


	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
	ImGui::Begin("Testing");
	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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


void nothing::RenderManager::SetBackgroundColor(float r, float g, float b)
{

	bgR_ = r;
	bgG_ = g;
	bgB_ = b;

}


// =================================================


void nothing::RenderManager::SetAspectRatio(int n, int d)
{

	aspectRatioN_ = static_cast<float>(n);
	aspectRatioD_ = static_cast<float>(d);

}


// =================================================