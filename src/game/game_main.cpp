#include <iostream>
#include <entt.hpp>
#include "components/PlayerInput.h"
#include "components/Camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>


// =================================================


extern "C" __declspec(dllexport)
void TestGameInit()
{

	std::cout << "Init Game Code\n";

}


// =================================================


extern "C" __declspec(dllexport)
void TestGameUpdate(entt::registry& reg)
{
	
	auto& cam = reg.ctx().get<nothing::components::Camera>();
	auto& input = reg.ctx().get<nothing::components::PlayerInput>();
	static glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);


	if (input.moveForward == 1.0f)
	{

		cam.position += cam.speed * cam.rotation;

	}


	if (input.moveBackwards == 1.0f)
	{

		cam.position -= cam.speed * cam.rotation;

	}


	if (input.moveLeft == 1.0f)
	{

		cam.position += cam.speed * -glm::normalize(glm::cross(cam.rotation, up));

	}


	if (input.moveRight == 1.0f)
	{

		cam.position += cam.speed * glm::normalize(glm::cross(cam.rotation, up));

	}


	if (input.rotateUp == 1.0f)
	{

		cam.rotation = glm::rotate(cam.rotation, glm::radians(cam.rotSens), glm::normalize(glm::cross(cam.rotation, up)));

	}


	if (input.rotateDown == 1.0f)
	{

		cam.rotation = glm::rotate(cam.rotation, glm::radians(-cam.rotSens), glm::normalize(glm::cross(cam.rotation, up)));

	}


	if (input.rotateLeft == 1.0f)
	{

		cam.rotation = glm::rotate(cam.rotation, glm::radians(cam.rotSens), up);

	}


	if (input.rotateRight == 1.0f)
	{

		cam.rotation = glm::rotate(cam.rotation, glm::radians(-cam.rotSens), up);

	}

}


// =================================================


extern "C" __declspec(dllexport)
void TestGameShutdown()
{

	std::cout << "Shutdown Game Code\n";

}


// =================================================