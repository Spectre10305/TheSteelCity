#pragma once
#include <cstdint>


namespace nothing
{

	namespace components
	{

		struct Object3D
		{

			uint32_t meshVAO = 0;
			uint32_t numVertices = 0;
			uint32_t textureID = 0;

		};

	}

}