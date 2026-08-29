#include "RenderManager.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include "../utils/Log.h"
#include <stb_image.h>
#include "../core/InputManager.h"
#include "../core/SceneManager.h"
#include "../game/components/Object3D.h"
#include "../game/components/Transform.h"
#include "../game/components/Camera.h"
#include "../game/components/Tags.h"


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// =================================================


void nothing::RenderManager::Init(EngineContext& ctx)
{

	ctx_ = &ctx;


	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_SCISSOR_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);


	// Shader di default
	const char* defVsName = "D:\\TheSteelCity\\assets\\engine\\shaders\\default_shader_vert.glsl";
	const char* defFsName = "D:\\TheSteelCity\\assets\\engine\\shaders\\default_shader_frag.glsl";
	

	defaultShader_ = new nothing::Shader(defVsName, defFsName);


	// Shader di debug
	const char* debVsName = "D:\\TheSteelCity\\assets\\engine\\shaders\\debug_draw_vert.glsl";
	const char* debFsName = "D:\\TheSteelCity\\assets\\engine\\shaders\\debug_draw_frag.glsl";


	debugShader_ = new nothing::Shader(debVsName, debFsName);


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


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(bgR_, bgG_, bgB_, 1.0f);

	
	defaultShader_->Use();
	defaultShader_->SetUniform("tex", 0);


	auto mainCamView = ctx_->sceneManager->registry.view<Camera, MainCameraTag>();
	

	for (auto [ent, cam] : mainCamView.each())
	{
		
		cam.viewMatrix = glm::mat4(1.0f);
		cam.projectionMatrix = glm::mat4(1.0f);
		cam.viewMatrix = glm::lookAt(cam.position, cam.position + cam.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		cam.projectionMatrix = glm::perspective(glm::radians(cam.fov), aspectRatioN_ / aspectRatioD_, 0.1f, 100.0f);
		defaultShader_->SetUniform("projection", cam.projectionMatrix);
		defaultShader_->SetUniform("view", cam.viewMatrix);

		
		debugShader_->Use();
		debugShader_->SetUniform("projection", cam.projectionMatrix);
		debugShader_->SetUniform("view", cam.viewMatrix);
		

	}


	defaultShader_->Use();


	auto objtrview = ctx_->sceneManager->registry.view<nothing::components::Object3D, nothing::components::Transform>();


	for (auto [ent, obj3D, tr] : objtrview.each())
	{

		glm::mat4 model = glm::mat4(1.0f);


		model = glm::translate(model, tr.position);
		model = glm::rotate   (model, glm::radians(tr.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate   (model, glm::radians(tr.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate   (model, glm::radians(tr.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale    (model, glm::vec3(1.0f, 1.0f, 1.0f));


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


	// Aggiunge il debug della fisica
	//glBufferData(GL_ARRAY_BUFFER, debugDraw.m_allRP3DTris.size() * sizeof(DebugVertex), debugDraw.m_allRP3DTris.data(), GL_DYNAMIC_DRAW);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe
	//glDrawArrays(GL_TRIANGLES, 0, debugDraw.m_allRP3DTris.size());
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // No wireframe


	allDebugVertices_.clear();

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


void nothing::RenderManager::DebugDrawLine(const glm::vec3& start, const glm::vec3& end)
{

	allDebugVertices_.push_back({ start });
	allDebugVertices_.push_back({ end });

}


// =================================================