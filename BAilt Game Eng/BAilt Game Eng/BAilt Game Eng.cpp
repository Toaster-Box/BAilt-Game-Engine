
#include <iostream>

#include "ConfigLoader.h"

#include "MasterGraphicsHandler.h"
#include "ObjectHandler3D.h"
#include "ObjectHandler2D.h"

#include "ScriptHandler.h"

#include "raylib.h"



int main()
{
	std::string ConfigFilePath = "./config.txt";
	std::string WindowName = "BAilt Engine";
	std::string baseDirectory = "D:/";
	std::string missingTexture = "D:/MissingTexture.png";

	double previousTime = GetTime();
	double* timeStep_ptr = new double;

	ConfigLoader MainConfigLoader(ConfigFilePath);

	static ObjectHandler2D MainObjHandler2D;
	static ObjectHandler3D MainObjHandler3D;
	static MasterGraphicsHandler MainMasterGraphicsHandler(&MainConfigLoader, &MainObjHandler2D, &MainObjHandler3D, WindowName);

	static ScriptHandler MainScriptHandler;



	//Prevent window from closing when ESC is pressed
	SetExitKey(NULL);

	//Give the timestep an initial value
	*timeStep_ptr = previousTime;

	//avoiding possible raylib exception
	MainObjHandler3D.AddTextureToContainer(missingTexture);

	//init Script Handler, will probably all be moved to constructor sometime. for now cutting down on constructor args
	MainScriptHandler.SetConfigLoaderPTR(&MainConfigLoader);
	MainScriptHandler.SetTimeStepPTR(timeStep_ptr);
	MainScriptHandler.SetMasterGraphicsHandlerPTR(&MainMasterGraphicsHandler);
	MainScriptHandler.SetObjHandler3DPTR(&MainObjHandler3D);
	
	MainScriptHandler.RunBootScript();

	//Main update loop
	while (!WindowShouldClose())
	{
		//Update scripts then objects then screen
		MainScriptHandler.Update();

		MainObjHandler3D.Update();

		MainMasterGraphicsHandler.UpdateScreen();

		//Update time step
		*timeStep_ptr = GetTime() - previousTime;
		previousTime = GetTime();
	}
}
