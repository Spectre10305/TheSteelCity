#include "AudioManager.h"
#include "../core/Filesystem.h"
#include "../core/SceneManager.h"
#include "../game/components/Tags.h"
#include "../game/components/Transform.h"
#include "../utils/Log.h"


// =================================================


bool nothing::AudioManager::Init(EngineContext& ctx)
{

	ctx_ = &ctx;


	audioEngine_ = std::make_unique<ma_engine>();


	ma_result engineInitResult = ma_engine_init(NULL, audioEngine_.get());


	if (engineInitResult != MA_SUCCESS)
	{

		nothing::LogError("Cannot initialize miniaudio ma_engine");
		return false;

	}


	// Test
	auto filePath = ctx_->filesystem->GetAudioPathFromName("test.wav");

	testSound_ = std::make_unique<ma_sound>();
	ma_sound_init_from_file(audioEngine_.get(), filePath.c_str(), MA_SOUND_FLAG_LOOPING, nullptr, nullptr, testSound_.get());
	ma_sound_set_position(testSound_.get(), 0.0f, 0.0f, 0.0f);

	ma_sound_start(testSound_.get());


	return true;

}


// =================================================


void nothing::AudioManager::Update()
{

	using namespace nothing::components;


	auto playerView = ctx_->sceneManager->registry.view<Transform, PlayerTag>();


	for (auto [ent, tr] : playerView.each())
	{

		ma_engine_listener_set_position(audioEngine_.get(), 0, tr.position.x, tr.position.y, tr.position.z);
		ma_engine_listener_set_direction(audioEngine_.get(), 0, 0.0f, 0.0f, -1.0f);

	}


	ma_sound_set_position(testSound_.get(), 0.0f, 0.0f, 0.0f);
	
}


// =================================================


void nothing::AudioManager::Shutdown()
{

	ma_engine_uninit(audioEngine_.get());
	audioEngine_.reset();

}


// =================================================