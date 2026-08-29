#pragma once
#include <cstdint> // uint32_t
#include <vector>
#include "Shader.h"
#include <entt.hpp>
#include <glm/glm.hpp>
#include "../core/EngineContext.h"


namespace nothing
{


	// Vertice per debug draw
	struct DebugVertex
	{

		glm::vec3 position;

	};


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


		// Debug draw
		void DebugDrawLine(const glm::vec3& start, const glm::vec3& end);

	private:

		Shader*  defaultShader_ = nullptr;
		Shader*  debugShader_   = nullptr;
		uint32_t m_debugVAO_    = 0;
		uint32_t m_debugVBO_    = 0;


		float bgR_ = 1.0f;
		float bgG_ = 1.0f;
		float bgB_ = 1.0f;


		float aspectRatioN_ = 4.0f;
		float aspectRatioD_ = 3.0f;


		std::vector<DebugVertex> allDebugVertices_;


		EngineContext* ctx_ = nullptr;
		
	};

}