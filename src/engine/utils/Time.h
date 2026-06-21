#pragma once
#include <SDL3/SDL.h>
#include <chrono>


namespace nothing
{

	// Variabile per memorizzare il tempo di inizio
	static std::chrono::time_point<std::chrono::high_resolution_clock> gStartTime;


	//=============================================================


	inline void StartTimer()
	{

		gStartTime = std::chrono::high_resolution_clock::now();

	}


	//=============================================================


	inline double ElapsedMS()
	{

		auto now = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration<double, std::milli>(now - gStartTime);
		return duration.count();

	}


	//=============================================================


	inline double ElapsedS()
	{

		auto now = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration<double>(now - gStartTime);
		return duration.count();

	}


	//=============================================================


	inline std::time_t PrintCurrentTime() //NOTA: questo è il tempo in ore, minuti e secondi
	{

		std::time_t current = std::time(nullptr);
		return current;

	}


	//=============================================================


	inline double GetSDL3Time()
	{

		static const Uint64 freq = SDL_GetPerformanceFrequency();
		static const Uint64 start = SDL_GetPerformanceCounter();


		Uint64 now = SDL_GetPerformanceCounter();
		return static_cast<double>(now - start) / freq;

	}


	//=============================================================


	inline double CalcDeltaTime()
	{

		static const Uint64 freq = SDL_GetPerformanceFrequency();
		static Uint64 last = SDL_GetPerformanceCounter();


		Uint64 now = SDL_GetPerformanceCounter();
		double delta = static_cast<double>(now - last) / freq;
		last = now;


		return delta;

	}


	//=============================================================


	inline double SimpleSineWave(double amplitude, double frequency, double phaseShift = 0.0)
	{

		return amplitude * sin(2.0 * 3.141592653589793 * frequency * GetSDL3Time() + phaseShift);

	}


	//=============================================================

}