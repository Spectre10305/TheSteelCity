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
		std::string GetTexturePathFromName(const std::string& name); // Nome + ".png"
		std::string GetModel3DPathFromName(const std::string& name); // Nome + ".obj"
		std::string GetAudioPathFromName(const std::string& name); // Nome + ".ogg" / ".wav"
		std::string GetMapPath(const std::string& name);

	private:

		std::filesystem::path rootPath_;
		std::filesystem::path texturesFolderPath_;
		std::filesystem::path models3DFolderPath_;
		std::filesystem::path audiosFolderPath_;
		std::filesystem::path mapsFolderPath_;

	};

}