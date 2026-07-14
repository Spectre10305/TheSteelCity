#pragma once
#include <cstdint> // uint32_t
#include <vector>
#include "Shader.h"
#include "../core/InputManager.h"
#include "../core/ResourceManager.h"
#include <entt.hpp>


namespace nothing
{

	// Test mesh struct per rendering
	struct Mesh
	{

		uint32_t VAO = 0;
		uint32_t VBO = 0;
		uint32_t EBO = 0;
		uint32_t numIndices = 0;
		uint32_t texture = 0;
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	};


	struct Mesh_t
	{

		uint32_t VAO;
		uint32_t numIndices;
		uint32_t textureID;
		glm::vec3 position;

	};


	// SCOPO: Gestione rendering principale
	class RenderManager
	{

	public:

		void Init(ResourceManager& resourcesManager);
		void Update(InputManager& input, double deltaTime);
		void Shutdown();


		void ResizeGLViewport(int w, int h);
		void SetBackgroundColor(float r, float g, float b);
		void SetAspectRatio(int n, int d);


		// Test creazione mesh 3D
		Mesh CreateCubeMesh(float width, float height, float depth);

	private:

		Shader* defaultShader_ = nullptr;


		float bgR_ = 1.0f;
		float bgG_ = 1.0f;
		float bgB_ = 1.0f;


		float aspectRatioN_ = 4.0f;
		float aspectRatioD_ = 3.0f;


		std::vector<Mesh> meshes_;


		entt::registry registry_;
		
	};

}