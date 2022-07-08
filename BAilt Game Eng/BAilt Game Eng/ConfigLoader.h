#pragma once

#include <string>

#include "raylib.h"
#include "wren.hpp"

class ConfigLoader
{
public:
	ConfigLoader(std::string& fileName);
	~ConfigLoader();

	//Return true for success, false for failure
	bool LoadConfig(std::string& fileName);
	bool CreateConfig(std::string& fileName);
	bool SaveConfig() { return CreateConfig(*m_fileName_ptr); }

	//Graphics getters and setters
	int* GetConfigScreenWidth() { return &m_screenWidth; }
	int* GetConfigScreenHeight() { return &m_screenHeight; }
	int* GetConfigTargetFPS() { return &m_targetFPS; }
	bool* GetConfigFullScreenMode() { return &m_fullScreenMode; }

	//Wren getters and setters

private:

	bool m_configUpdated = false;

	std::string* m_fileName_ptr;

	//make sure variables are initialized with their default values

	//Graphics config
	int m_screenWidth = 1280;
	int m_screenHeight = 720;
	int m_targetFPS = 9999;

	bool m_fullScreenMode = false;

	//Wren config
};

