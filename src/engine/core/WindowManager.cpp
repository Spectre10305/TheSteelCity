#include "WindowManager.h"
#include "../utils/Log.h"


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
	windowTitle_ = "Nothing Engine DEMO";
	windowFlags_ = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
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


	SDL_SetWindowPosition(window_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	

	glContext_ = SDL_GL_CreateContext(window_);
	SDL_GL_MakeCurrent(window_, glContext_);


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