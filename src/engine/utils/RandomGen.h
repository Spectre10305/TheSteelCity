#pragma once
#include <random>
#include "Log.h"

namespace nothing
{

	static std::mt19937 gRng;


	// Generatore di numeri casuali, serviva anche scrivere questo commento??
	class Random
	{

	public:


		static void RandomizeSeed()
		{

			std::random_device randDvc;
			unsigned int seed = randDvc();
			gRng.seed(seed);
			nothing::LogInfo("New random seed: " + std::to_string(seed));
			
		}


		// =================================================


		static int Int(int min, int max)
		{

			std::uniform_int_distribution<int> distr(min, max);
			return distr(gRng);

		}


		// =================================================


		static float Float(float min, float max)
		{

			std::uniform_real_distribution<float> distr(min, max);
			return distr(gRng);

		}


		// =================================================

	};
}