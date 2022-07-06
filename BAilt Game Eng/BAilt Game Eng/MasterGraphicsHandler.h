#pragma once

#include <string>

#include "ConfigLoader.h"
#include "ObjectHandler2D.h"
#include "ObjectHandler3D.h"
#include "GraphicsHandler2D.h"
#include "GraphicsHandler3D.h"

#include "raylib.h"

class MasterGraphicsHandler
{
public:
	MasterGraphicsHandler(ConfigLoader& ConfLoaderIn, ObjectHandler2D& ObjHandler2DIn, ObjectHandler3D& ObjHandler3DIn, std::string& WindowName);

	void UpdateScreen();
	void ReloadConfig();

private:

	ConfigLoader* m_ConfigLoader_ptr;

	GraphicsHandler2D* m_GraphicsHandler2D_ptr;
	GraphicsHandler3D* m_GraphicsHandler3D_ptr;
};

//Default constructor to initialize class
MasterGraphicsHandler::MasterGraphicsHandler(ConfigLoader& ConfLoaderIn, ObjectHandler2D& ObjHandler2DIn, ObjectHandler3D& ObjHandler3DIn,  std::string& WindowName)
{
	//Allow future access to global variables set through the config files
	m_ConfigLoader_ptr = &ConfLoaderIn;

	//Swap window name string into a char*
	char* windowNameArr = const_cast<char*>(WindowName.c_str());

	//Initialize a window with global variables and target frame rate
	InitWindow(*m_ConfigLoader_ptr->GetConfigScreenWidth(), *m_ConfigLoader_ptr->GetConfigScreenHeight(), windowNameArr);
	SetTargetFPS(*m_ConfigLoader_ptr->GetConfigTargetFPS());

	//Create and initialize 2D and 3D graphics handlers
	m_GraphicsHandler2D_ptr = new GraphicsHandler2D(ObjHandler2DIn);
	m_GraphicsHandler3D_ptr = new GraphicsHandler3D(ObjHandler3DIn);
}

//Main Function which gets run every frame
void MasterGraphicsHandler::UpdateScreen()
{
	//Begin frame buffer
	BeginDrawing();

	// clear screen to draw new objects
	ClearBackground(LIGHTGRAY);

	//Draw 3D Components and then Draw 2D components on top
	m_GraphicsHandler3D_ptr->Update();
	m_GraphicsHandler2D_ptr->Update();

	//End frame buffer and swap buffers
	EndDrawing();
}

//Reload new global variables during runtime
void MasterGraphicsHandler::ReloadConfig()
{

}