#include "RenderManager.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include "../utils/Log.h"
#include <stb_image.h>


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// =================================================


void nothing::RenderManager::Init(ResourceManager& resourcesManager)
{

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);


	const char* vsName = "D:\\TheSteelCity\\assets\\engine\\shaders\\default_shader_vert.glsl";
	const char* fsName = "D:\\TheSteelCity\\assets\\engine\\shaders\\default_shader_frag.glsl";


	defaultShader_ = new nothing::Shader(vsName, fsName);
	defaultShader_->Use();
	defaultShader_->SetUniform("colR", 0.5f);
	defaultShader_->SetUniform("colG", 0.6f);
	defaultShader_->SetUniform("colB", 0.3f);


	resourcesManager.CreateTexture("D:\\TheSteelCity\\assets\\game\\textures\\tex_floor_wood_1.png");
	resourcesManager.CreateTexture("D:\\TheSteelCity\\assets\\game\\textures\\tex_wall_bricks_1.png");


	int num = 20;
	meshes_.reserve(20 * 20);


	for (int x = 0; x < num; x++)
	{

		for (int z = 0; z < num; z++)
		{

			static int i = 0;


			i++;


			Mesh m = CreateCubeMesh(1.0f + static_cast<float>(i), 1.0f + static_cast<float>(i), 1.0f + static_cast<float>(i));
			m.position = glm::vec3(static_cast<float>(x) * 2.0f + static_cast<float>(i), 0.0f, static_cast<float>(z) * 2.0f + static_cast<float>(i));


			if (i % 2 == 0)
			{

				m.texture = resourcesManager.GetTextureIDFromName("tex_floor_wood_1");

			}
			else
			{

				m.texture = resourcesManager.GetTextureIDFromName("tex_wall_bricks_1");

			}
			
			
			meshes_.emplace_back(m);

		}

	}


	size_t meshesSz = meshes_.size() * sizeof(Mesh);
	nothing::LogInfo("Meshes size (KB): " + std::to_string(nothing::ToKilobytes(meshesSz)));
	

}


// =================================================


void nothing::RenderManager::Update(InputManager& input, double deltaTime)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(bgR_, bgG_, bgB_, 1.0f);


	static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	static glm::vec3 cameraOrientation = glm::vec3(0.0f, 0.0f, -1.0f);
	static glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	static float     cameraSpeed = 0.05f;


#pragma region INPUT


	if (input.IsActionHeld(GameAction::MoveForward))
	{

		cameraPos += cameraSpeed * cameraOrientation;

	}


	if (input.IsActionHeld(GameAction::MoveBackward))
	{

		cameraPos -= cameraSpeed * cameraOrientation;

	}


	if (input.IsActionHeld(GameAction::MoveLeft))
	{

		cameraPos += cameraSpeed * -glm::normalize(glm::cross(cameraOrientation, up));

	}


	if (input.IsActionHeld(GameAction::MoveRight))
	{

		cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraOrientation, up));

	}


	if (input.IsActionHeld(GameAction::RotateUp))
	{

		cameraOrientation = glm::rotate(cameraOrientation, glm::radians(1.0f), glm::normalize(glm::cross(cameraOrientation, up)));

	}


	if (input.IsActionHeld(GameAction::RotateDown))
	{

		cameraOrientation = glm::rotate(cameraOrientation, glm::radians(-1.0f), glm::normalize(glm::cross(cameraOrientation, up)));

	}


	if (input.IsActionHeld(GameAction::RotateLeft))
	{

		cameraOrientation = glm::rotate(cameraOrientation, glm::radians(1.0f), up);

	}


	if (input.IsActionHeld(GameAction::RotateRight))
	{

		cameraOrientation = glm::rotate(cameraOrientation, glm::radians(-1.0f), up);

	}


#pragma endregion

	
	defaultShader_->Use();
	defaultShader_->SetUniform("tex", 0);

	
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);


	//model = glm::rotate(model, glm::radians(rot), glm::vec3(1.0f, 0.5f, 0.3f));
	view = glm::lookAt(cameraPos, cameraPos + cameraOrientation, up);
	proj = glm::perspective(glm::radians(90.0f), aspectRatioN_ / aspectRatioD_, 0.1f, 100.0f);


	defaultShader_->SetUniform("projection", proj);
	defaultShader_->SetUniform("view", view);


	for (auto& mesh : meshes_)
	{

		glm::mat4 model = glm::mat4(1.0f);


		model = glm::translate(model, mesh.position);
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));


		defaultShader_->SetUniform("model", model);


		glBindTexture(GL_TEXTURE_2D, mesh.texture);
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_INT, 0);

	}

}


// =================================================


void nothing::RenderManager::Shutdown()
{

	delete defaultShader_;

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


nothing::Mesh nothing::RenderManager::CreateCubeMesh(float width, float height, float depth)
{

	float W = width;
	float H = height;
	float D = depth;


	float vertices[] =
	{
		// FRONT (+Z)
		0.0f, 0.0f, D,     0.0f, 0.0f,
		W,    0.0f, D,     W,    0.0f,
		W,    H,    D,     W,    H,
		0.0f, H,    D,     0.0f, H,

		// BACK (-Z)
		W,    0.0f, 0.0f,  0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,  W,    0.0f,
		0.0f, H,    0.0f,  W,    H,
		W,    H,    0.0f,  0.0f, H,

		// LEFT (-X)
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		0.0f, 0.0f, D,     D,    0.0f,
		0.0f, H,    D,     D,    H,
		0.0f, H,    0.0f,  0.0f, H,

		// RIGHT (+X)
		W,    0.0f, D,     0.0f, 0.0f,
		W,    0.0f, 0.0f,  D,    0.0f,
		W,    H,    0.0f,  D,    H,
		W,    H,    D,     0.0f, H,

		// TOP (+Y)
		0.0f, H,    D,     0.0f, 0.0f,
		W,    H,    D,     W,    0.0f,
		W,    H,    0.0f,  W,    D,
		0.0f, H,    0.0f,  0.0f, D,

		// BOTTOM (-Y)
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		W,    0.0f, 0.0f,  W,    0.0f,
		W,    0.0f, D,     W,    D,
		0.0f, 0.0f, D,     0.0f, D
	};


	unsigned int indices[] =
	{

		0, 1, 2,  2, 3, 0,        // front
		4, 5, 6,  6, 7, 4,        // back
		8, 9, 10, 10, 11, 8,      // left
		12, 13, 14, 14, 15, 12,   // right
		16, 17, 18, 18, 19, 16,   // top
		20, 21, 22, 22, 23, 20    // bottom

	};


	Mesh res{};


	glGenVertexArrays(1, &res.VAO);
	glGenBuffers(1, &res.VBO);
	glGenBuffers(1, &res.EBO);
	glBindVertexArray(res.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, res.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// Hardcoded per ora
	res.numIndices = 36;


	return res;

}


// =================================================