
#include "ConfigLoader.h"

#include <iostream>
#include <string>

#include "raylib.h"

ConfigLoader::ConfigLoader(std::string& fileName)
{
	bool successfullFileLoad = false;

	//Save the filename for use later
	m_fileName_ptr = &fileName;

	//raylib file functions require char*
	char* fileNameArr = const_cast<char*>(fileName.c_str());

	//if the config file is found then load the values if not then create a file with the default values
	if (FileExists(fileNameArr))
	{
		LoadConfig(fileName);

		successfullFileLoad = true;
	}
	else
	{
		CreateConfig(fileName);

		successfullFileLoad = false;
	}

}

//Default deconstructor, save the changed config if it has changed.
ConfigLoader::~ConfigLoader()
{
	if (m_configUpdated)
	{
		SaveConfig();
	}
}


bool ConfigLoader::LoadConfig(std::string& fileName)
{
	bool success = false;

	return success;
}


bool ConfigLoader::CreateConfig(std::string& fileName)
{
	bool success = false;

	return success;
}

