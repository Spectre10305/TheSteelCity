#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h> // per GlDebugOutput


namespace nothing
{


	// Codici ANSI semplici per colore
	inline constexpr const char* G_COLOR_GREEN  = "\033[32m";
	inline constexpr const char* G_COLOR_YELLOW = "\033[33m";
	inline constexpr const char* G_COLOR_RED    = "\033[31m";
	inline constexpr const char* G_COLOR_RESET  = "\033[0m";



	// =================================================


	inline void LogInfo(const std::string& message)
	{

		std::cout << G_COLOR_GREEN << "[INFO]: " << message << G_COLOR_RESET << std::endl;

	}


	// =================================================


	inline void LogWarning(const std::string& message)
	{

		std::cout << G_COLOR_YELLOW << "[WARNING]: " << message << G_COLOR_RESET << std::endl;

	}


	// =================================================


	inline void LogError(const std::string& message)
	{

		std::cout << G_COLOR_RED << "[ERROR]: " << message << G_COLOR_RESET << std::endl;

	}


	// =================================================


	// Utility per mostrare i vettori, message può essere vuoto ("")
	inline void LogInfoVector(const std::string& message, float x, float y, float z)
	{

		if (message.empty())
		{

			std::cout << G_COLOR_GREEN << "[INFO]: " << x << ", " << y << ", " << z << G_COLOR_RESET << std::endl;

		}
		else
		{

			std::cout << G_COLOR_GREEN << "[INFO]: " << message << x << ", " << y << ", " << z << G_COLOR_RESET << std::endl;

		}

	}


	// =================================================


	inline float ToKilobytes(size_t bytes)
	{

		double res = bytes / 1024.0;
		return static_cast<float>(res);

	}


	// =================================================


	inline void GLAPIENTRY GlDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{

		if (id == 131185 || id == 131169) return;
		LogInfo("[GL message]" + std::string(message));

	}


	// =================================================

}