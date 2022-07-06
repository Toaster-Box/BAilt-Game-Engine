#include "ScriptHandler.h"

//Default constructor to initialize class
ScriptHandler::ScriptHandler()
{
	//Wren VM stores its own coppy of the config struct so it can be a local variable.
	WrenConfiguration WrenConfiguration;

	wrenInitConfiguration(&WrenConfiguration);
	WrenConfiguration.writeFn = &writeFn;
	WrenConfiguration.bindForeignMethodFn = &BindForeignMethod;

	m_WrenVirtualMachine_ptr = wrenNewVM(&WrenConfiguration);
}

//Deafult Deconstructor
ScriptHandler::~ScriptHandler()
{
	wrenFreeVM(m_WrenVirtualMachine_ptr);
}

void ScriptHandler::RunBootScript()
{

}

//Main function which gets run every frame
void ScriptHandler::Update()
{
	
}


void ScriptHandler::RunTestCode()
{
	WrenInterpretResult result = wrenInterpret(
		m_WrenVirtualMachine_ptr,
		"my_module",
		"System.print(\"I am running in a VM!\")");

		char* testCode = LoadFileText("C:/Users/halla10/Documents/3 CODING/tempresources/TestCode.wren");
		std::cout << testCode << std::endl;

	WrenInterpretResult test = wrenInterpret(
		m_WrenVirtualMachine_ptr,
		"main",
		testCode);

}

//The worst function
WrenForeignMethodFn ScriptHandler::BindForeignMethod(WrenVM* vm, const char* moduleName, const char* className, bool isStatic, const char* signature)
{
	if (strcmp(moduleName, "main") == 0)
	{
		//all main module components are nested in this statement

		if (strcmp(className, "ConfigLoader") == 0)
		{
			//All ConfigLoader functions are nested in this Statement

			if (true)
			{

			}
		}
		else if (strcmp(className, "MasterGraphicsHandler") == 0)
		{
			//All MasterGraphicsHandler functions are nested in this Statement

			if (true)
			{

			}
		}
		else if (strcmp(className, "ObjectHandler3D") == 0)
		{
			//All ObjectHandler3D functions are nested in this Statement

			if (isStatic && strcmp(signature, "CreateObject3D(_)") == 0)
			{
				return CreateObject3D;
			}
		}
		else if (strcmp(className, "BaseObject3D") == 0)
		{
			//All BaseObject3D functions are nested in this Statement

			if (true)
			{

			}
		}
		else if (strcmp(className, "ObjectHandler2D") == 0)
		{
			//All ObjectHandler2D functions are nested in this Statement

			if (true)
			{

			}
		}
		else if (strcmp(className, "BaseObject2D") == 0)
		{
			//All BaseObject2D functions are nested in this Statement

			if (true)
			{

			}
		}
	}
}

//All fucking wren functions are here


void ScriptHandler::writeFn(WrenVM* vm, const char* text)
{
	printf("%s", text);
}

void ScriptHandler::CreateObject3D(WrenVM* vm)
{
	std::string fileName = wrenGetSlotString(vm, 1);
	std::cout << fileName << std::endl;

	m_ObjHandler3D_ptr->CreateObject(fileName);
}