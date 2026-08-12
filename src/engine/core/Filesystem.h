#pragma once
#include <string>
#include <filesystem>


namespace nothing
{

	// SCOPO: Gestione dei file e dei percorsi di file
	class Filesystem
	{

	public:

		void Init();
		std::string GetTexturePathFromName(const char* name);
		std::string GetModel3DPathFromName(const char* name);
		std::string GetAudioPathFromName(const char* name);
		std::string GetMapPath(const char* name);

	private:

		std::filesystem::path rootPath_;
		std::filesystem::path texturesFolderPath_;
		std::filesystem::path models3DFolderPath_;
		std::filesystem::path audiosFolderPath_;
		std::filesystem::path mapsFolderPath_;

	};

}