#include "RenderManager.h"
#include <glad/glad.h>
#include <SDL3/SDL.h>
#include "../utils/Log.h"
#include <stb_image.h>
#include "../core/InputManager.h"
#include "../core/SceneManager.h"


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


	const char* vsName = "D:\\TheSteelCity\\assets\\engine\\shaders\\default_shader_vert.glsl";
	const char* fsName = "D:\\TheSteelCity\\assets\\engine\\shaders\\default_shader_frag.glsl";


	defaultShader_ = new nothing::Shader(vsName, fsName);

}


// =================================================


void nothing::RenderManager::Update(double deltaTime)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(bgR_, bgG_, bgB_, 1.0f);

	
	defaultShader_->Use();
	defaultShader_->SetUniform("tex", 0);


	auto& cam = ctx_->sceneManager->GetCompFromCtx<Camera>();


	cam.viewMatrix       = glm::mat4(1.0f);
	cam.projectionMatrix = glm::mat4(1.0f);
	cam.viewMatrix       = glm::lookAt(cam.position, cam.position + cam.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	cam.projectionMatrix = glm::perspective(glm::radians(90.0f), aspectRatioN_ / aspectRatioD_, 0.1f, 100.0f);
	defaultShader_->SetUniform("projection", cam.projectionMatrix);
	defaultShader_->SetUniform("view", cam.viewMatrix);


	auto objtrview = ctx_->sceneManager->registry.view<nothing::Object3D, nothing::Transform>();


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