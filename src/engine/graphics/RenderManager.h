#pragma once
#include <cstdint> // uint32_t
#include "Shader.h"


namespace nothing
{

	// SCOPO: Gestione rendering principale
	class RenderManager
	{

	public:

		void Init();
		void Update();
		void Shutdown();


		void ResizeGLViewport(int w, int h);

	private:

		uint32_t VBO_;
		uint32_t VAO_;
		uint32_t EBO_;


		Shader* testShader = nullptr;
		
	};

}