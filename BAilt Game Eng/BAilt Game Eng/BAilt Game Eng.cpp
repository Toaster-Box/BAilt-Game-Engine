
#include <iostream>

#include "ConfigLoader.h"

#include "MasterGraphicsHandler.h"
#include "ObjectHandler3D.h"
#include "ObjectHandler2D.h"

#include "ScriptHandler.h"

#include "raylib.h"

//Temp includes
#include "BaseObject3D.h"
//End Temp

std::string configFilePath = "./config.txt";
std::string windowName = "BAilt Engine";

ConfigLoader MainConfigLoader(configFilePath);
ObjectHandler2D MainObjHandler2D;
ObjectHandler3D MainObjHandler3D;
MasterGraphicsHandler MainMasterGraphicsHandler( &MainConfigLoader, &MainObjHandler2D, &MainObjHandler3D, windowName);

ScriptHandler MainScriptHandler;


int main()
{
	//Prevent window from closing when ESC is pressed
	SetExitKey(NULL);

	std::string TestModel = "C:/Users/halla10/Documents/3 CODING/tempresources/BasicMonkey.obj";
	BaseObject3D* TempObj = MainObjHandler3D.CreateObject(TestModel);

	//Temp Code
	MainScriptHandler.RunTestCode();
	//End Temp

	//Main update loop
	while (!WindowShouldClose())
	{
		MainMasterGraphicsHandler.UpdateScreen();

		MainObjHandler3D.Update();
	}
}
