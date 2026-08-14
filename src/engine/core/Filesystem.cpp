#include "Filesystem.h"
#include <Windows.h>
#include "../utils/Log.h"


// =================================================


void nothing::Filesystem::Init()
{

	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::filesystem::path temp = buffer;
	rootPath_ = temp.parent_path().parent_path().parent_path();


	texturesFolderPath_ = rootPath_ / "assets\\game\\textures\\";
	models3DFolderPath_ = rootPath_ / "assets\\game\\models3D\\";
	audiosFolderPath_   = rootPath_ / "assets\\game\\audio\\";
	mapsFolderPath_     = rootPath_ / "maps\\";


	nothing::LogInfo("Root path: " + rootPath_.string());
	nothing::LogInfo("Textures path: " + texturesFolderPath_.string());
	nothing::LogInfo("Models 3D path: " + models3DFolderPath_.string());
	nothing::LogInfo("Audios path: " + audiosFolderPath_.string());
	nothing::LogInfo("Maps path: " + mapsFolderPath_.string());

}


// =================================================


std::string nothing::Filesystem::GetTexturePathFromName(const std::string& name)
{

	std::string res = texturesFolderPath_.string() + std::string(name);
	return res;

}


// =================================================


std::string nothing::Filesystem::GetModel3DPathFromName(const std::string& name)
{

	std::string res = models3DFolderPath_.string() + std::string(name);
	return res;

}


// =================================================


std::string nothing::Filesystem::GetAudioPathFromName(const std::string& name)
{

	std::string res = audiosFolderPath_.string() + std::string(name);
	return res;

}


// =================================================


std::string nothing::Filesystem::GetMapPath(const std::string& name)
{

	std::string res = mapsFolderPath_.string() + std::string(name);
	return res;

}


// =================================================