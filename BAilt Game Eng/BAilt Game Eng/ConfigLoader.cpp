
#include "ConfigLoader.h"

#include <iostream>
#include <string>

#include "raylib.h"

ConfigLoader::ConfigLoader(std::string& fileName)
{
	bool successfullFileLoad = false;

	//raylib file functions require char*
	char* fileNameArr = const_cast<char*>(fileName.c_str());

	//if the config file is found then load the values if not then create a file with the default values
	if (FileExists(fileNameArr))
	{
		LoadConfig(fileName);
	}
	else
	{
		CreateConfig(fileName);
	}

}


void ConfigLoader::LoadConfig(std::string& fileName)
{

}


void ConfigLoader::CreateConfig(std::string& fileName)
{

}

