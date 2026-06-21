#pragma once
#include <cstdint> // uint32_t


namespace nothing
{

	// Wrapper per le funzioni OpenGL per gli shaders
	class Shader
	{

	public:

		Shader(const char* vertFileName, const char* fragFileName);
		~Shader();


		void     Use();
		uint32_t GetProgramID();


		void SetUniform(const char* unifName, int newVal);
		void SetUniform(const char* unifName, float newVal);
		void SetUniform(const char* unifName, double newVal);

	private:

		uint32_t programID_;

		// "type" deve essere "VERTEX", "FRAGMENT" o "PROGRAM"
		void CheckErrs(uint32_t shader, const char* type);

	};

}