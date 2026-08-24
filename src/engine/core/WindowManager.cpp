#include "WindowManager.h"
#include "../utils/Log.h"
#include <glad/glad.h>


// =================================================


bool nothing::WindowManager::Init()
{

	if (!SDL_Init(SDL_INIT_VIDEO))
	{

		std::string initErr = SDL_GetError();
		nothing::LogInfo("Couldn't initialize SDL: " + initErr);
		return false;

	}


	windowWidth_ = 1240;
	windowHeight_ = 720;
	windowTitle_ = "The Steel City";
	windowFlags_ = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
	

	window_ = SDL_CreateWindow(windowTitle_.c_str(), windowWidth_, windowHeight_, windowFlags_);


	if (window_ == NULL)
	{

		std::string winErr = SDL_GetError();
		nothing::LogInfo("Couldn't create window: " + winErr);
		SDL_Quit();
		return false;

	}


	glContext_ = SDL_GL_CreateContext(window_);
	SDL_GL_MakeCurrent(window_, glContext_);


	// Dobbiamo inizializzare GLAD nel WindowManager altrimenti ResourceManager::InitDefaults
	// non può caricare le texture di default, dato che usa le funzioni OpenGL
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{

		nothing::LogError("Failed to initialize GLAD");
		return false;

	}
	else
	{

		nothing::LogInfo("GLAD initialized successfully");
		nothing::LogInfo("OpenGL version: " + std::string((const char*)glGetString(GL_VERSION)));
		nothing::LogInfo("Vendor: " + std::string((const char*)glGetString(GL_VENDOR)));

	}


	int32_t ctxFlags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &ctxFlags);


	if (ctxFlags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{

		nothing::LogInfo("Using OpenGL debug context");
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(nothing::GlDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	}
	

	SDL_SetWindowPosition(window_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_MaximizeWindow(window_);
	//SDL_SetWindowFullscreen(window_, true);


	return true;

}


// =================================================


void nothing::WindowManager::Shutdown()
{

	SDL_GL_DestroyContext(glContext_);
	SDL_DestroyWindow(window_);
	SDL_Quit();

}


// =================================================


bool nothing::WindowManager::ShouldCloseGame()
{

	return shouldCloseGame_;

}


// =================================================


SDL_Window* nothing::WindowManager::GetWindowPtr()
{

	return window_;

}


// =================================================


SDL_GLContext* nothing::WindowManager::GetGLContext()
{

	return &glContext_;

}


// =================================================


void nothing::WindowManager::SetWindowSize(int w, int h)
{

	SDL_SetWindowSize(window_, w, h);

}


// =================================================


void nothing::WindowManager::GetWindowSize(int& _w, int& _h)
{

	int w, h;


	SDL_GetWindowSize(window_, &w, &h);
	windowWidth_ = w;
	windowHeight_ = h;


	_w = windowWidth_;
	_h = windowHeight_;

}


// =================================================


void nothing::WindowManager::SetWindowTitle(const char* newTitle)
{

	SDL_SetWindowTitle(window_, newTitle);

}


// =================================================


void nothing::WindowManager::SetMouseInvisible()
{

	SDL_SetWindowRelativeMouseMode(window_, true);

}


// =================================================


void nothing::WindowManager::SetMouseVisible(bool snapToWindowCenter)
{

	SDL_SetWindowRelativeMouseMode(window_, false);


	if (snapToWindowCenter)
	{

		int w = 0;
		int h = 0;


		SDL_GetWindowSize(window_, &w, &h);


		float windowXCenterPos = static_cast<float>(w / 2);
		float windowYCenterPos = static_cast<float>(h / 2);


		SDL_WarpMouseInWindow(window_, windowXCenterPos, windowYCenterPos);

	}

}


// =================================================


void nothing::WindowManager::SwapBuffers()
{

	SDL_GL_SwapWindow(window_);

}


// =================================================