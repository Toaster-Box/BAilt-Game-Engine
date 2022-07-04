
#include <iostream>

#include "ConfigLoader.h"

#include "MasterGraphicsHandler.h"
#include "ObjectHandler3D.h"
#include "ObjectHandler2D.h"

#include "raylib.h"

std::string configFilePath = "./config.txt";
std::string windowName = "BAilt Engine";

ConfigLoader MainConfigLoader(configFilePath);
ObjectHandler2D MainObjHandler2D;
ObjectHandler3D MainObjHandler3D;
MasterGraphicsHandler MainMasterGraphicsHandler( &MainConfigLoader, &MainObjHandler2D, &MainObjHandler3D, windowName);


int main()
{
	//Prevent window from closing when ESC is pressed
	SetExitKey(NULL);

	std::string TestModel = "C:/Users/halla10/Documents/3 CODING/tempresources/BasicMonkey.obj";
	MainObjHandler3D.CreateObject(TestModel);

	//Main update loop
	while (!WindowShouldClose())
	{
		MainMasterGraphicsHandler.UpdateScreen();

		MainObjHandler3D.Update();
	}
}
