#pragma once
#include <miniaudio.h>
#include <memory> // std::unique_ptr
#include "../core/EngineContext.h"


namespace nothing
{

	// SCOPO: Gestione audio principale
	class AudioManager
	{
	public:

		bool Init(EngineContext& ctx);
		void Update();
		void Shutdown();

	private:

		std::unique_ptr<ma_engine> audioEngine_;
		std::unique_ptr<ma_sound> testSound_;


		EngineContext* ctx_ = nullptr;

	};

}