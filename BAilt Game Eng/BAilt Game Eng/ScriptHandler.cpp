#include "ScriptHandler.h"

//Wont work in the .h for some fucking reason
double* ScriptHandler::m_timeStep_ptr = NULL;
std::string* ScriptHandler::m_ScriptFileDirectory_ptr = NULL;

std::vector<WrenHandle*> ScriptHandler::m_ModuleHandleContainer;
std::vector<WrenHandle*> ScriptHandler::m_ClassHandleContainer;
std::vector<WrenHandle*> ScriptHandler::m_MethodHandleContainer;

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
	for (unsigned int i = 0; i < m_ClassHandleContainer.size(); i++)
	{
		wrenReleaseHandle(m_WrenVirtualMachine_ptr, m_ClassHandleContainer[i]);
	}
	for (unsigned int i = 0; i < m_MethodHandleContainer.size(); i++)
	{
		wrenReleaseHandle(m_WrenVirtualMachine_ptr, m_MethodHandleContainer[i]);
	}

	//Shut down the VM
	wrenFreeVM(m_WrenVirtualMachine_ptr);
}

//Runs an initial script before the program enters the main loop
void ScriptHandler::RunBootScript()
{
	//I have absolutely no idea why but these lines keep the static pointers from returning to NULL 
	//I think its because main needs to be entered before static variables can hold a value
	m_timeStep_ptr = m_localTimeSetp_ptr;
	m_ObjHandler3D_ptr = m_LocalObjHandler3D_ptr;
	m_ScriptFileDirectory_ptr = m_LocalScriptFileDirectory_ptr;

	//Get a Handle for the OnUpdate() function as the first handle in the method container
	std::string UpdateSignature = "OnUpdate()";
	GetMethodHandle(m_WrenVirtualMachine_ptr, UpdateSignature);

	std::string BootModule = "main";
	std::string BootSource = "Boot.wren";
	LoadScript(BootModule, BootSource);
}

//Main function which gets run every frame
void ScriptHandler::Update()
{
	//std::cout << m_ClassHandleContainer[0] << " : " << m_MethodHandleContainer[0] << std::endl;

	//Run all "OnUpdate()" functions
	for (unsigned int i = 0; i < m_ClassHandleContainer.size(); i++)
	{
		RunMethodNoArgs(m_ClassHandleContainer[i], m_MethodHandleContainer[0]);
	}
}


bool ScriptHandler::LoadScript(std::string& ModuleName, std::string& FileName)
{
	std::string SourceFilePath = *m_ScriptFileDirectory_ptr;
	SourceFilePath.append(FileName);

	char* scriptCode = LoadFileText(const_cast<char*>(SourceFilePath.c_str()));

	WrenInterpretResult CallResult = wrenInterpret(m_WrenVirtualMachine_ptr, const_cast<char*>(ModuleName.c_str()), scriptCode);

	if (CallResult == WREN_RESULT_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}


WrenHandle* ScriptHandler::GetClassHandle(std::string& ModuleName, std::string ClassName)
{
	//Ensure a slot and load a class into it at index 0
	wrenGetVariable(m_WrenVirtualMachine_ptr, const_cast<char*>(ModuleName.c_str()), const_cast<char*>(ClassName.c_str()), 0);

	//get the handle to the class.
	WrenHandle* WrenClassHandle = wrenGetSlotHandle(m_WrenVirtualMachine_ptr, 0);

	//Store the handle locally so it can be freed in the destructorlater
	m_ClassHandleContainer.push_back(WrenClassHandle);

	return WrenClassHandle;
}

//runs a wren script that takes no arguments
bool ScriptHandler::RunMethodNoArgs(WrenHandle* ClassHandle, WrenHandle* MethodHandle)
{
	//Load the Handle to the class in slot 0
	wrenEnsureSlots(m_WrenVirtualMachine_ptr, 1);
	wrenSetSlotHandle(m_WrenVirtualMachine_ptr, 0, ClassHandle);

	WrenInterpretResult CallResult = wrenCall(m_WrenVirtualMachine_ptr, MethodHandle);

	if (CallResult == WREN_RESULT_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}


WrenHandle* ScriptHandler::GetMethodHandle(WrenVM* vm, std::string& Signature)
{
	WrenHandle* MethodHandle = wrenMakeCallHandle(vm, const_cast<char*>(Signature.c_str()));
	m_MethodHandleContainer.push_back(MethodHandle);
	return  MethodHandle;
}


WrenLoadModuleResult ScriptHandler::LoadModule(WrenVM* vm, const char* moduleFileName)
{
	WrenLoadModuleResult result = { NULL };

	std::string ModuleFilePathSTR = *m_ScriptFileDirectory_ptr;
	ModuleFilePathSTR.append(moduleFileName);
	ModuleFilePathSTR.append(".wren");
	char* moduleFilePathsCharArr = const_cast<char*>(ModuleFilePathSTR.c_str());

	result.source = LoadFileText(moduleFilePathsCharArr);

	return result;
}

//The worst function, VM callback that returns a function pointer to the specified function. 
WrenForeignMethodFn ScriptHandler::BindForeignMethod(WrenVM* vm, const char* moduleName, const char* className, bool isStatic, const char* signature)
{
	//Module isnt compared since we don't care want to restrict access to these functions based on module name

	if (strcmp(className, "Utility") == 0)
	{
		//All Utility functions are nested in this Statement

		if (isStatic && strcmp(signature, "GetTimeStep()") == 0)
		{
			return GetTimeStep;
		}
		else if (isStatic && strcmp(signature, "AddClassToContainer(_,_)") == 0)
		{
			return AddClassToContainer;
		}
	}
	else if (strcmp(className, "ConfigLoader") == 0)
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

		if (strcmp(signature, "GetObject3DPosition(_)") == 0)
		{
			return GetObject3DPosition;
		}

		if (strcmp(signature, "SetObject3DPosition(_,_,_,_)") == 0)
		{
			return SetObject3DPosition;
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


void ScriptHandler::GetTimeStep(WrenVM* vm)
{
	wrenSetSlotDouble(vm, 0, *m_timeStep_ptr);
}


void ScriptHandler::AddClassToContainer(WrenVM* vm)
{
	//slot 1 module name, slot 2 class name
	const char* moduleName = wrenGetSlotString(vm, 1);
	const char* ClassName = wrenGetSlotString(vm, 2);

	//Ensure a slot and load a class into it at index 0
	wrenEnsureSlots(vm, 1);
	
	wrenGetVariable(vm, moduleName, ClassName, 0);

	//get the handle to the class.
	WrenHandle* WrenClassHandle = wrenGetSlotHandle(vm, 0);

	//Store the handle locally so it can be freed in the destructorlater
	m_ClassHandleContainer.push_back(WrenClassHandle);
}


void ScriptHandler::CreateObject3D(WrenVM* vm)
{
	//create an object and return a pointer to it.
	std::string fileName = wrenGetSlotString(vm, 1);

	const void* objptr = static_cast<const void*>(m_ObjHandler3D_ptr->CreateObject(fileName));

	//convert void* to string
	std::stringstream objptrStringStream;
	objptrStringStream << objptr;

	wrenSetSlotString(vm, 0, const_cast<char*>(objptrStringStream.str().c_str()));
}


void ScriptHandler::GetObject3DPosition(WrenVM* vm)
{

}


void ScriptHandler::SetObject3DPosition(WrenVM* vm)
{
	//this probably sets the bar for worst function I have ever wrote
	std::stringstream objptrStringStream;
	objptrStringStream << (std::string)wrenGetSlotString(vm, 1);

	Vector3 PosVector;
	PosVector.x = wrenGetSlotDouble(vm, 2);
	PosVector.y = wrenGetSlotDouble(vm, 3);
	PosVector.z = wrenGetSlotDouble(vm, 4);

	void* objptrVoid;
	objptrStringStream >> objptrVoid;

	BaseObject3D* objptr = (BaseObject3D*)objptrVoid;

	try
	{
		objptr->SetPosition(PosVector);
	}
	catch (const std::exception& exec)
	{
		std::cout << "Invalid object pointer when setting position through wren" << std::endl;
	}
}
