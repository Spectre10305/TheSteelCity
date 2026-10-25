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


		static int32_t Int32(int32_t min, int32_t max)
		{

			std::uniform_int_distribution<int32_t> distr(min, max);
			return distr(gRng);

		}


		// =================================================


		static uint64_t Int64(uint64_t min, uint64_t max)
		{

			std::uniform_int_distribution<uint64_t> distr(min, max);
			return distr(gRng);

		}


		// =================================================


		static float_t Float(float_t min, float_t max)
		{

			std::uniform_real_distribution<float_t> distr(min, max);
			return distr(gRng);

		}


		// =================================================

	};
}