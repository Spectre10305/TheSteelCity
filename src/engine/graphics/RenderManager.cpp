#include "RenderManager.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include "../utils/Log.h"
#include "../utils/RandomGen.h"
#include <stb_image.h>
#include "../core/InputManager.h"
#include "../core/SceneManager.h"
#include "../core/Filesystem.h"
#include "../game/components/Object3D.h"
#include "../game/components/Transform.h"
#include "../game/components/Camera.h"
#include "../game/components/Tags.h"
#include "../game/components/UIDebugValues.h"


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


// =================================================


void nothing::RenderManager::Init(EngineContext& ctx)
{

	ctx_ = &ctx;


	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_SCISSOR_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);


	auto rootPath = ctx_->filesystem->GetRootPath();


	std::string defaultVSName = rootPath + "\\assets\\engine\\shaders\\default_shader_vert.glsl";
	std::string defaultFSName = rootPath + "\\assets\\engine\\shaders\\default_shader_frag.glsl";
	std::string debugVSName   = rootPath + "\\assets\\engine\\shaders\\debug_draw_vert.glsl";
	std::string debugFSName   = rootPath + "\\assets\\engine\\shaders\\debug_draw_frag.glsl";


	// Shader di default
	defaultShader_ = new nothing::Shader(defaultVSName.c_str(), defaultFSName.c_str());
	defaultShader_->Use();
	defaultShader_->SetUniform("lightsNum", 4);


	// Shader di debug
	debugShader_ = new nothing::Shader(debugVSName.c_str(), debugFSName.c_str());


	for (int i = 0; i < 4; i++)
	{

		float posX = nothing::Random::Float(-3.0f, 3.0f);
		float posZ = nothing::Random::Float(-3.0f, 3.0f);


		Light l;
		
		l.Init(i, { posX, 2.0f ,posZ }, 0.05f, 0.8f, 1.0f, 1.0f, 0.35f, 0.44f);
		testLights_.emplace_back(l);

	}


#pragma region DEBUG_DRAW


	glGenVertexArrays(1, &m_debugVAO_);
	glGenBuffers(1, &m_debugVBO_);


	glBindVertexArray(m_debugVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, m_debugVBO_);


	// Quindi si alloca un buffer vuoto con GL_DYNAMIC_DRAW, cambia ogni frame
	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);


	// layout: posizione (location = 0)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));


	// layout: colore (location = 1)
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));


#pragma endregion

}


// =================================================


void nothing::RenderManager::Update(double deltaTime)
{

	using namespace nothing::components;


	glEnable(GL_FRAMEBUFFER_SRGB);

	
	auto& uiDbgVals = ctx_->sceneManager->GetCompFromCtx<UIDebugValues>();


	defaultShader_->Use();
	defaultShader_->SetUniform("mat.diffuse", 0);
	defaultShader_->SetUniform("mat.specular", glm::vec3(uiDbgVals.gSpecularIntensity));
	defaultShader_->SetUniform("mat.shininess", uiDbgVals.gShininess);



	for (auto& lightObj : testLights_)
	{

		float ambFac = uiDbgVals.gLightAmbientFac;
		float difFac = uiDbgVals.gLightDiffuseFac;
		float speFac = uiDbgVals.gLightSpecularFac;
		float cnst   = uiDbgVals.gLightConstant;
		float lin    = uiDbgVals.gLightLinear;
		float qua    = uiDbgVals.gLightQuadratic;


		lightObj.Update(defaultShader_, ambFac, difFac, speFac, cnst, lin, qua);

	}


	auto mainCamView = ctx_->sceneManager->registry.view<Camera, MainCameraTag>();
	

	for (auto [ent, cam] : mainCamView.each())
	{
		
		cam.viewMatrix       = glm::mat4(1.0f);
		cam.viewMatrix       = glm::lookAt(cam.position, cam.position + cam.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		cam.projectionMatrix = glm::mat4(1.0f);
		cam.projectionMatrix = glm::perspective(glm::radians(cam.fov), aspectRatioN_ / aspectRatioD_, 0.1f, 100.0f);


		defaultShader_->SetUniform("projection", cam.projectionMatrix);
		defaultShader_->SetUniform("view", cam.viewMatrix);
		defaultShader_->SetUniform("cameraPos", cam.position);

		
		debugShader_->Use();
		debugShader_->SetUniform("projection", cam.projectionMatrix);
		debugShader_->SetUniform("view", cam.viewMatrix);

	}


	defaultShader_->Use();


	auto objtrview = ctx_->sceneManager->registry.view<nothing::components::Object3D, nothing::components::Transform>();


	for (auto [ent, obj3D, tr] : objtrview.each())
	{

		glm::mat4 model = glm::mat4(1.0f);


		model  = glm::translate(model, tr.position);
		model *= glm::mat4_cast(tr.rotation);
		model  = glm::scale    (model, glm::vec3(1.0f, 1.0f, 1.0f));
		

		defaultShader_->SetUniform("model", model);
		


		glBindTexture(GL_TEXTURE_2D, obj3D.textureID);
		glBindVertexArray(obj3D.meshVAO);
		glDrawElements(GL_TRIANGLES, obj3D.numVertices, GL_UNSIGNED_INT, 0);

	}



	debugShader_->Use();
	debugShader_->SetUniform("model", glm::mat4(1.0f));


	DebugDrawLine({ 0.0f, 0.0f, 0.0f }, { 0.0f, 10.0f, 0.0f });


	glBindVertexArray(m_debugVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, m_debugVBO_);


	// Nothing
	glBufferData(GL_ARRAY_BUFFER, allDebugVertices_.size() * sizeof(DebugVertex), allDebugVertices_.data(), GL_DYNAMIC_DRAW);
	glDrawArrays(GL_LINES, 0, allDebugVertices_.size());


	allDebugVertices_.clear();


	glDisable(GL_FRAMEBUFFER_SRGB);

}


// =================================================


void nothing::RenderManager::Shutdown()
{

	delete defaultShader_;

}


// =================================================


void nothing::RenderManager::ClearScreen()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(bgR_, bgG_, bgB_, 1.0f);

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


void nothing::RenderManager::DebugDrawLine(const glm::vec3& start, const glm::vec3& end)
{

	allDebugVertices_.push_back({ start });
	allDebugVertices_.push_back({ end });

}


// =================================================