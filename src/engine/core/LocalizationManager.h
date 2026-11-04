#pragma once
#include "EngineContext.h"
#include <unordered_map>
#include <string>


namespace nothing
{

	enum class Lang
	{

		IT,
		EN

	};


	// Gestione localizzazione, sia per UI che per dialoghi in gioco
	class LocalizationManager
	{

	public:

		void Init(EngineContext& ctx, Lang language);
		void Shutdown();
		void InitMapDialogueFile();
		void DeleteMapFileDialogues();
		std::string& GetDialogueString(const std::string& key);
		std::string& GetUIText(const std::string& key);


		Lang currentLanguage = Lang::IT;

	private:

		std::string defaultText_ = "NULL";
		std::unordered_map<std::string, std::string> UITexts_;
		std::unordered_map<std::string, std::string> currentMapDialogues_;


		EngineContext* ctx_ = nullptr;

	};

}