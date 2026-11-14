#include "RenderManager.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include "../utils/Log.h"
#include "../utils/RandomGen.h"
#include <stb_image.h>
#include "../core/InputManager.h"
#include "../core/SceneManager.h"
#include "../core/Filesystem.h"
#include "../core/WindowManager.h"
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


	//glEnable(GL_SCISSOR_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);


	auto rootPath = ctx_->filesystem->GetRootPath();


	std::string defaultVSName = rootPath + "\\assets\\engine\\shaders\\default_shader_vert.glsl";
	std::string defaultFSName = rootPath + "\\assets\\engine\\shaders\\default_shader_frag.glsl";
	std::string debugVSName   = rootPath + "\\assets\\engine\\shaders\\debug_draw_vert.glsl";
	std::string debugFSName   = rootPath + "\\assets\\engine\\shaders\\debug_draw_frag.glsl";
	std::string screenVSName  = rootPath + "\\assets\\engine\\shaders\\default_screen_vert.glsl";
	std::string screenFSName  = rootPath + "\\assets\\engine\\shaders\\default_screen_frag.glsl";


	// Shader di default
	defaultShader_ = new nothing::Shader(defaultVSName.c_str(), defaultFSName.c_str());
	defaultShader_->Use();
	defaultShader_->SetUniform("lightsNum", 4);


	// Shader di debug
	debugShader_ = new nothing::Shader(debugVSName.c_str(), debugFSName.c_str());


	// Screen shader per framebuffer post-processing
	screenShader_ = new nothing::Shader(screenVSName.c_str(), screenFSName.c_str());
	screenShader_->Use();
	screenShader_->SetUniform("screenTexture", 0);


	// Test luci
	for (int i = 0; i < 4; i++)
	{

		float posX = nothing::Random::Float(-3.0f, 3.0f);
		float posZ = nothing::Random::Float(-3.0f, 3.0f);


		Light l;
		
		l.Init(i, { posX, 2.0f ,posZ }, 0.05f, 0.8f, 1.0f, 1.0f, 0.35f, 0.44f);
		testLights_.emplace_back(l);

	}


#pragma region DEBUG_DRAW


	glGenVertexArrays(1, &debugVAO_);
	glGenBuffers(1, &debugVBO_);


	glBindVertexArray(debugVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, debugVBO_);


	// Quindi si alloca un buffer vuoto con GL_DYNAMIC_DRAW, cambia ogni frame
	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);


	// layout: posizione (location = 0)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));


	// layout: colore (location = 1)
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));


#pragma endregion


	int wWidth = 0;
	int wHeight = 0;
	ctx_->windowManager->GetWindowSize(wWidth, wHeight);
	testPostProcessFB.Init(wWidth, wHeight);


	/*
	glGenFramebuffers(1, &testFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, testFrameBuffer);

	glGenTextures(1, &testFrameBufferTexture);
	glBindTexture(GL_TEXTURE_2D, testFrameBufferTexture);


	int wWidth  = 0;
	int wHeight = 0;
	ctx_->windowManager->GetWindowSize(wWidth, wHeight);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wWidth, wHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, testFrameBufferTexture, 0);


	glGenRenderbuffers(1, &testFrameBufferRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, testFrameBufferRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, wWidth, wHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, testFrameBufferRenderBuffer);


	auto fboStats = glCheckFramebufferStatus(GL_FRAMEBUFFER);


	if (fboStats != GL_FRAMEBUFFER_COMPLETE)
	{

		nothing::LogInfo("Framebuffer err. : " + std::to_string(fboStats));

	}
	*/


	float screenRectVerts[] =
	{

		 1.0f, -1.0f,   1.0f, 0.0f,
		-1.0f, -1.0f,   0.0f, 0.0f,
		-1.0f,  1.0f,   0.0f, 1.0f,

		 1.0f,  1.0f,   1.0f, 1.0f,
		 1.0f, -1.0f,   1.0f, 0.0f,
		-1.0f,  1.0f,   0.0f, 1.0f

	};


	glGenVertexArrays(1, &testFrameBufferRectVAO);
	glGenBuffers(1, &testFrameBufferRectVBO);
	glBindVertexArray(testFrameBufferRectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, testFrameBufferRectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenRectVerts), &screenRectVerts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}


// =================================================


void nothing::RenderManager::Update(double deltaTime)
{

	using namespace nothing::components;


	glEnable(GL_DEPTH_TEST);
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


	// Prendiamo l'entità della telecamera e poi il componente della Camera, così evitiamo un ciclo for inutile
	auto mainCamView = ctx_->sceneManager->registry.view<Camera, MainCameraTag>();
	entt::entity camEntity = mainCamView.back();
	auto& cam = ctx_->sceneManager->registry.get<Camera>(camEntity);


	cam.viewMatrix       = glm::mat4(1.0f);
	cam.viewMatrix       = glm::lookAt(cam.position, cam.position + cam.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	cam.projectionMatrix = glm::mat4(1.0f);
	cam.projectionMatrix = glm::perspective(glm::radians(cam.fov), aspectRatioN_ / aspectRatioD_, 0.1f, 100.0f);


	defaultShader_->SetUniform("projection", cam.projectionMatrix);
	defaultShader_->SetUniform("view", cam.viewMatrix);
	defaultShader_->SetUniform("cameraPos", cam.position);


	// Disegnamo tutti gli oggetti 3D
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


	// Elementi di debug
	debugShader_->Use();
	debugShader_->SetUniform("model", glm::mat4(1.0f));
	debugShader_->SetUniform("projection", cam.projectionMatrix);
	debugShader_->SetUniform("view", cam.viewMatrix);


	DebugDrawLine({ 0.0f, 0.0f, 0.0f }, { 0.0f, 10.0f, 0.0f });


	glBindVertexArray(debugVAO_);
	glBindBuffer(GL_ARRAY_BUFFER, debugVBO_);


	// Nothing
	glBufferData(GL_ARRAY_BUFFER, allDebugVertices_.size() * sizeof(DebugVertex), allDebugVertices_.data(), GL_DYNAMIC_DRAW);
	glDrawArrays(GL_LINES, 0, allDebugVertices_.size());


	allDebugVertices_.clear();


	testPostProcessFB.EndRender();


	// Disegna lo screen rect del framebuffer di post processing
	glDisable(GL_DEPTH_TEST);
	screenShader_->Use();
	glBindVertexArray(testFrameBufferRectVAO);
	glBindTexture(GL_TEXTURE_2D, testPostProcessFB.GetGeneratedTextureID());


	// HACK: impostiamo il winding a CW, disegnamo il rettangolo e rimettiamo a CCW.
	// Sì, solo perché non ho voglia di controllare l'ordine dei vertici lol.
	glFrontFace(GL_CW);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFrontFace(GL_CCW);


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

	//glBindFramebuffer(GL_FRAMEBUFFER, testFrameBuffer);
	testPostProcessFB.BeginRender();
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