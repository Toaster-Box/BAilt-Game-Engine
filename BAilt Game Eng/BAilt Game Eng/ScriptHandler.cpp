#include "ScriptHandler.h"

//Wont work in the .h for some fucking reason
std::string* ScriptHandler::m_scriptFileDirectory_ptr = NULL;

//Default constructor to initialize class
ScriptHandler::ScriptHandler()
{
	//Wren VM stores its own coppy of the config struct so it can be a local variable.
	WrenConfiguration WrenConfiguration;

	wrenInitConfiguration(&WrenConfiguration);
	WrenConfiguration.writeFn = &WriteFn;
	WrenConfiguration.errorFn = &ErrorFn;
	WrenConfiguration.loadModuleFn = &LoadModule;
	WrenConfiguration.bindForeignMethodFn = &BindForeignMethod;

	m_WrenVirtualMachine_ptr = wrenNewVM(&WrenConfiguration);
}

//Deafult Deconstructor
ScriptHandler::~ScriptHandler()
{
	//free all handles before shutting down the VM
	for (unsigned int i = 0; i < m_HandlePTRContainer.size(); i++)
	{
		ReleaseHandle(m_HandlePTRContainer[i]);
	}

	//Shut down the VM
	wrenFreeVM(m_WrenVirtualMachine_ptr);
}

//Runs an initial script before the program enters the main loop
void ScriptHandler::RunBootScript()
{
	//I have absolutely no idea why but this line keeps the static pointer from returning to NULL 
	m_ObjHandler3D_ptr = m_LocalObjHandler3D_ptr;

	RunTestCode();
}

//Main function which gets run every frame
void ScriptHandler::Update()
{
	
}

//DELETE FOR RELEASE
void ScriptHandler::RunTestCode()
{
	WrenInterpretResult result = wrenInterpret(
		m_WrenVirtualMachine_ptr,
		"my_module",
		"System.print(\"I am running in a VM!\")");

	char* testCode = LoadFileText("C:/Users/halla10/Documents/3 CODING/tempresources/TestCode.wren");
	//std::cout << testCode << std::endl;

	

	WrenInterpretResult test = wrenInterpret(
		m_WrenVirtualMachine_ptr,
		"main",
		testCode);

}

WrenLoadModuleResult ScriptHandler::LoadModule(WrenVM* vm, const char* moduleFileName)
{
	WrenLoadModuleResult result = { NULL };

	std::string ModuleFilePathSTR = *m_scriptFileDirectory_ptr;
	ModuleFilePathSTR.append(moduleFileName);
	char* moduleFilePathsCharArr = const_cast<char*>(ModuleFilePathSTR.c_str());

	result.source = LoadFileText(moduleFilePathsCharArr);

	return result;
}

//The worst function, VM callback that returns a function pointer to the specified function. 
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


void ScriptHandler::WriteFn(WrenVM* vm, const char* text)
{
	std::cout << text;
}


void ScriptHandler::ErrorFn(WrenVM* vm, WrenErrorType ErrorType, const char* moduleName, const int line, const char* message)
{
	switch (ErrorType)
	{
		default:
		{
			std::cout << "Wren VM: Undefined wren VM error" << std::endl;
		} break;
		case WREN_ERROR_COMPILE:
		{
			std::cout << "Wren VM: [" << moduleName << " line " << line << "] [ERROR] " << message << std::endl;
		} break;
		case WREN_ERROR_STACK_TRACE:
		{
			std::cout << "Wren VM: [" << moduleName << " line " << line << "] in " << message << std::endl;
		} break;
		case WREN_ERROR_RUNTIME:
		{
			std::cout << "Wren VM: [RUNTIME ERROR] " << message << std::endl;
		} break;
	}
}


void ScriptHandler::CreateObject3D(WrenVM* vm)
{
	std::string fileName = wrenGetSlotString(vm, 1);
	//std::cout << fileName << std::endl;
	//std::cout << m_ObjHandler3D_ptr << std::endl;

	m_ObjHandler3D_ptr->CreateObject(fileName);
}