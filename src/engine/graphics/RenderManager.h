#pragma once
#include <cstdint> // uint32_t
#include <vector>
#include "Shader.h"
#include "../core/InputManager.h"


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


	// SCOPO: Gestione rendering principale
	class RenderManager
	{

	public:

		void Init();
		void Update(InputManager& input);
		void Shutdown();


		void ResizeGLViewport(int w, int h);
		void SetBackgroundColor(float r, float g, float b);
		void SetAspectRatio(int n, int d);


		// Test creazione mesh 3D
		Mesh CreateCubeMesh();
		uint32_t CreateTexture(const char* texFileName);

	private:

		uint32_t VBO_;
		uint32_t VAO_;
		uint32_t EBO_;


		Shader* testShader_ = nullptr;


		float bgR_ = 1.0f;
		float bgG_ = 1.0f;
		float bgB_ = 1.0f;


		float aspectRatioN_ = 4.0f;
		float aspectRatioD_ = 3.0f;


		std::vector<Mesh> meshes_;
		std::vector<uint32_t> textures_;
		
	};

}