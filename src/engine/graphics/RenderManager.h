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
		void SetBackgroundColor(float r, float g, float b);
		void SetAspectRatio(int n, int d);

	private:

		uint32_t VBO_;
		uint32_t VAO_;
		uint32_t EBO_;


		Shader* testShader = nullptr;


		float f = 0.0f;


		float bgR_ = 1.0f;
		float bgG_ = 1.0f;
		float bgB_ = 1.0f;


		float aspectRatioN_ = 4.0f;
		float aspectRatioD_ = 3.0f;
		
	};

}