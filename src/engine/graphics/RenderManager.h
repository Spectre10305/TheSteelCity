#pragma once
#include <cstdint> // uint32_t
#include <vector>
#include "Shader.h"
#include <entt.hpp>
#include "../core/EngineContext.h"


namespace nothing
{

	// SCOPO: Gestione rendering principale
	class RenderManager
	{

	public:

		void Init(EngineContext& ctx);
		void Update(double deltaTime);
		void Shutdown();


		void ResizeGLViewport(int w, int h);
		void SetBackgroundColor(float r, float g, float b);
		void SetAspectRatio(int n, int d);

	private:

		Shader* defaultShader_ = nullptr;


		float bgR_ = 1.0f;
		float bgG_ = 1.0f;
		float bgB_ = 1.0f;


		float aspectRatioN_ = 4.0f;
		float aspectRatioD_ = 3.0f;


		EngineContext* ctx_ = nullptr;
		
	};

}