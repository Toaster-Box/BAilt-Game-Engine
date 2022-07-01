#pragma once

#include <string>

#include "ConfigLoader.h"

#include "raylib.h"

class MasterGraphicsHandler
{
public:
	MasterGraphicsHandler(ConfigLoader* confLoaderPTRIn, std::string& WindowName);

	void UpdateScreen();

	void SetConfigLoaderPTR(ConfigLoader* confLoaderPTRIn) { m_configLoader_ptr = confLoaderPTRIn; }

private:

	ConfigLoader* m_configLoader_ptr;
};


MasterGraphicsHandler::MasterGraphicsHandler(ConfigLoader* confLoaderPTRIn, std::string& WindowName)
{
	m_configLoader_ptr = confLoaderPTRIn;

	char* windowNameArr = const_cast<char*>(WindowName.c_str());

	InitWindow(*m_configLoader_ptr->GetConfigScreenWidth(), *m_configLoader_ptr->GetConfigScreenHeight(), windowNameArr);

	SetTargetFPS(*m_configLoader_ptr->GetConfigTargetFPS());
}


void MasterGraphicsHandler::UpdateScreen()
{
	BeginDrawing();

	ClearBackground(LIGHTGRAY);

	EndDrawing();
}