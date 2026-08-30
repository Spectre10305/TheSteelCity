#pragma once
#include <cstdint>
#include <limits.h>
#include "Log.h"


#define NOTHING_TEXTURES_MEMORY 0
#define NOTHING_MODELS3D_MEMORY 1
#define NOTHING_AUDIOS_MEMORY 2


namespace nothing
{

	inline size_t gTotalMemory         = 0;
	inline size_t gTotalTexturesMemory = 0;
	inline size_t gTotalModels3DMemory = 0;
	inline size_t gTotalAudiosMemory   = 0;


	// =================================================


	inline void AddBytes(uint32_t category, size_t bytes)
	{

		if (bytes > SIZE_MAX)
		{

			nothing::LogInfo("Too many bytes passed to AddBytes()");
			return;

		}


		switch (category)
		{

		case NOTHING_TEXTURES_MEMORY:
			gTotalTexturesMemory += bytes;
			break;

			
		case NOTHING_MODELS3D_MEMORY:
			gTotalModels3DMemory += bytes;
			break;


		case NOTHING_AUDIOS_MEMORY:
			gTotalAudiosMemory += bytes;
			break;


		default:
			break;

		}
		

		gTotalMemory += bytes;

	}


	// =================================================


	inline void RemoveBytes(uint32_t category, size_t bytes)
	{

		if (bytes > SIZE_MAX)
		{

			nothing::LogInfo("Too many bytes passed to RemoveBytes()");
			return;

		}


		switch (category)
		{

		case NOTHING_TEXTURES_MEMORY:
			gTotalTexturesMemory -= bytes;
			break;


		case NOTHING_MODELS3D_MEMORY:
			gTotalModels3DMemory -= bytes;
			break;


		case NOTHING_AUDIOS_MEMORY:
			gTotalAudiosMemory -= bytes;
			break;


		default:
			break;

		}


		gTotalMemory -= bytes;

	}


	// =================================================


	inline void InterrogateMemoryStatus()
	{

		nothing::LogInfo("==================MEMORY STATUS==================");
		nothing::LogInfo("Total memory___________(bytes): " + std::to_string(gTotalMemory));
		nothing::LogInfo("Total textures memory__(bytes): " + std::to_string(gTotalTexturesMemory));
		nothing::LogInfo("Total models 3D memory_(bytes): " + std::to_string(gTotalModels3DMemory));
		nothing::LogInfo("Total audios memory____(bytes): " + std::to_string(gTotalAudiosMemory));
		nothing::LogInfo("=================================================");

	}

}