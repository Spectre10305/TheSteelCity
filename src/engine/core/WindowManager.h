#pragma once
#include <SDL3/SDL.h>
#include <string>


namespace nothing
{

	// SCOPO: Gestione principale della finestra ed eventi
	class WindowManager
	{

	public:

		bool Init();
		void Shutdown();


		bool           ShouldCloseGame();
		SDL_Window*    GetWindowPtr();
		SDL_GLContext* GetGLContext();
		void           SetWindowSize(int w, int h);
		void           GetWindowSize(int& w, int& h);
		void           SetWindowTitle(const char* newTitle);
		void           SwapBuffers(); // Chiama SDL_GL_SwapWindow()

	private:

		SDL_Window*     window_       = nullptr;
		SDL_WindowFlags windowFlags_  = 0;
		SDL_GLContext   glContext_    = nullptr;


		bool shouldCloseGame_ = false;
		

		int         windowWidth_  = 0;
		int         windowHeight_ = 0;
		std::string windowTitle_ = "";

	};

}