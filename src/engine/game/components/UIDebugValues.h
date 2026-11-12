#pragma once
#include <glm/glm.hpp>


namespace nothing
{

	namespace components
	{

		// Contenitore globale di valori modificabili dal sistema UI
		struct UIDebugValues
		{

			float playerWalkSpeed = 0.0f;
			float playerRunSpeed  = 0.0f;
			int   playerHealth    = 0;


			// RenderManager
			float gSpecularIntensity = 1.0;
			float gShininess         = 32.0f;


			float gLightAmbientFac   = 0.1f;
			float gLightDiffuseFac   = 0.8f;
			float gLightSpecularFac  = 1.0f;
			float gLightConstant     = 1.0f;
			float gLightLinear       = 0.35f;
			float gLightQuadratic    = 0.44f;

		};

	}

}