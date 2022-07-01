#pragma once

#include <string>

class ConfigLoader
{
public:
	ConfigLoader(std::string& fileName);

	void LoadConfig(std::string& fileName);
	void CreateConfig(std::string& fileName);

	int* GetConfigScreenWidth() { return &m_screenWidth; }
	int* GetConfigScreenHeight() { return &m_screenHeight; }
	int* GetConfigTargetFPS() { return &m_targetFPS; }
	bool* GetConfigFullScreenMode() { return &m_fullScreenMode; }

private:

	//make sure variables are initialized with their default values

	int m_screenWidth = 1280;
	int m_screenHeight = 720;
	int m_targetFPS = 60;

	bool m_fullScreenMode = false;
};

