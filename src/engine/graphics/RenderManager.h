#pragma once
#include <cstdint> // uint32_t
#include <vector>
#include "Shader.h"
#include <entt.hpp>
#include <glm/glm.hpp>
#include <string>
#include "../core/EngineContext.h"


namespace nothing
{


	// Vertice per debug draw
	struct DebugVertex
	{

		glm::vec3 position;

	};


	// Test luci
	class Light
	{

	public:

		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 ambient  = glm::vec3(0.0f);
		glm::vec3 diffuse  = glm::vec3(0.0f);
		glm::vec3 specular = glm::vec3(0.0f);


		float constant  = 0.0f;
		float linear    = 0.0f;
		float quadratic = 0.0f;


		int index = 0;


		void Init(int idx, glm::vec3 position_, float ambientIntensity, float diffuseIntensity, float specularIntensity, float cnst, float lnr, float qua)
		{

			index     = idx;
			position  = position_;
			ambient   = glm::vec3(ambientIntensity);
			diffuse   = glm::vec3(diffuseIntensity);
			specular  = glm::vec3(specularIntensity);
			constant  = cnst;
			linear    = lnr;
			quadratic = qua;

		}


		void Update(Shader* s, float ambientIntensity, float diffuseIntensity, float specularIntensity, float cnst, float lnr, float qua)
		{

			ambient = glm::vec3(ambientIntensity);
			diffuse = glm::vec3(diffuseIntensity);
			specular = glm::vec3(specularIntensity);
			constant = cnst;
			linear = lnr;
			quadratic = qua;


			std::string lPos = "pointLights[" + std::to_string(index) + std::string("].position");
			std::string lAmb = "pointLights[" + std::to_string(index) + std::string("].ambient");
			std::string lDif = "pointLights[" + std::to_string(index) + std::string("].diffuse");
			std::string lSpe = "pointLights[" + std::to_string(index) + std::string("].specular");
			std::string lCns = "pointLights[" + std::to_string(index) + std::string("].constant");
			std::string lLnr = "pointLights[" + std::to_string(index) + std::string("].linear");
			std::string lQua = "pointLights[" + std::to_string(index) + std::string("].quadratic");


			s->SetUniform(lPos.c_str(), position);
			s->SetUniform(lAmb.c_str(), ambient);
			s->SetUniform(lDif.c_str(), diffuse);
			s->SetUniform(lSpe.c_str(), specular);
			s->SetUniform(lCns.c_str(), constant);
			s->SetUniform(lLnr.c_str(), linear);
			s->SetUniform(lQua.c_str(), quadratic);

		}

	};


	// SCOPO: Gestione rendering principale
	class RenderManager
	{

	public:

		void Init(EngineContext& ctx);
		void Update(double deltaTime);
		void Shutdown();


		// Lo usiamo per cancellare il buffer dello schermo diretamente all'inizio del frame
		// senza aspettare l'update del RenderManager
		void ClearScreen();
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
		std::vector<Light> testLights_;


		EngineContext* ctx_ = nullptr;
		
	};

}