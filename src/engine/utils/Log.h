#pragma once
#include <string>
#include <iostream>


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


	inline float ToKilobytes(size_t bytes)
	{

		double res = bytes / 1024.0;
		return static_cast<float>(res);

	}


	// =================================================

}