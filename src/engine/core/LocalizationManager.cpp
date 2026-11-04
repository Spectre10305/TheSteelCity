#include "LocalizationManager.h"
#include "Filesystem.h"
#include "../utils/Log.h"
#include <nlohmann/json.hpp>
#include <fstream>


//=============================================================


void nothing::LocalizationManager::Init(EngineContext& ctx, Lang language)
{
	using namespace nlohmann;


	ctx_ = &ctx;
	currentLanguage = language;


	std::string langFileName;


	switch (currentLanguage)
	{

	case Lang::IT:
		langFileName = "\\assets\\lang_it.json";
		break;


	case Lang::EN:
		langFileName = "\\assets\\lang_en.json";
		break;


	default:
		// Fallback all'italiano per ora..
		langFileName = "\\assets\\lang_it.json";
		break;

	}


	std::ifstream langFile(ctx_->filesystem->GetRootPath() + langFileName, std::ios::in);


	try
	{

		json jsData = json::parse(langFile);


		for (auto& [key, val] : jsData.items())
		{

			if (val.is_string())
			{

				UITexts_.emplace(key, val.get<std::string>());

			}

		}

	}
	catch (json::exception e)
	{
		
		nothing::LogInfo(e.what());

	}

}


//=============================================================


void nothing::LocalizationManager::Shutdown()
{

	UITexts_.clear();

}


//=============================================================


void nothing::LocalizationManager::InitMapDialogueFile()
{

	switch (currentLanguage)
	{

	case Lang::IT:
		currentMapDialogues_.emplace("testDialog1", "Questo è un dialogo di test.");
		currentMapDialogues_.emplace("testDialog2", "Anche questo è un dialogo di test.");
		currentMapDialogues_.emplace("testDialog3", "Ma come hai potuto vedere, era diverso dal primo.");
		break;


	case Lang::EN:
		currentMapDialogues_.emplace("testDialog1", "This is a test dialogue.");
		currentMapDialogues_.emplace("testDialog2", "This is also a test dialogue");
		currentMapDialogues_.emplace("testDialog3", "But as you can see, it was different from the previous.");
		

	default:
		break;

	}
	
}


//=============================================================


void nothing::LocalizationManager::DeleteMapFileDialogues()
{

	currentMapDialogues_.clear();

}


//=============================================================


std::string& nothing::LocalizationManager::GetDialogueString(const std::string& key)
{

	if (currentMapDialogues_.find(key) != currentMapDialogues_.end())
	{

		return currentMapDialogues_.at(key);

	}
	else
	{

		return defaultText_;

	}

}


//=============================================================


std::string& nothing::LocalizationManager::GetUIText(const std::string& key)
{

	if (UITexts_.find(key) != UITexts_.end())
	{

		return UITexts_.at(key);

	}
	else
	{

		return defaultText_;

	}

}


//=============================================================