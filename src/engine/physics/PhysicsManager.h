#pragma once
#include <box3d/box3d.h>


namespace nothing
{

	// SCOPO: Gestione della fisica, powered by Box3D.
	class PhysicsManager
	{
	public:

		void Init();
		void Update(double deltaTime);
		void Shutdown();

	private:

		b3WorldId worldID_{};
		float timeStep_ = 1.0f / 60.0f;
		int subSteps_ = 4;


		b3BodyId bodyId{};
		
	};

}