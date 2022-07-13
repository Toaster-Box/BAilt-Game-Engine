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
	WrenConfiguration.bindForeignClassFn = &BindForeignClass;

	m_WrenVirtualMachine_ptr = wrenNewVM(&WrenConfiguration);
}

//Deafult Deconstructor
ScriptHandler::~ScriptHandler()
{
	//free all handles before shutting down the VM
	for (unsigned int i = 0; i < m_MethodHandleContainer.size(); i++)
	{
		wrenReleaseHandle(m_WrenVirtualMachine_ptr, m_MethodHandleContainer[i]);
	}
	for (unsigned int i = 0; i < m_ClassHandleContainer.size(); i++)
	{
		wrenReleaseHandle(m_WrenVirtualMachine_ptr, m_ClassHandleContainer[i]);
	}
	for (unsigned int i = 0; i < m_ModuleHandleContainer.size(); i++)
	{
		wrenReleaseHandle(m_WrenVirtualMachine_ptr, m_ModuleHandleContainer[i]);
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
	m_InputHandler_ptr = m_localInputHandler_ptr;
	m_MasterGraphicsHandler_ptr = m_LocalMasterGraphicsHandler_ptr;
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
	//Run all "OnUpdate()" functions
	for (unsigned int i = 0; i < m_ClassHandleContainer.size(); i++)
	{
		//std::cout << m_ClassHandleContainer[i] << " : " << m_MethodHandleContainer[0] << std::endl;

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

//runs a wren method that takes no arguments
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


WrenForeignClassMethods ScriptHandler::BindForeignClass(WrenVM* vm, const char* moduleName, const char* className)
{
	WrenForeignClassMethods ClassMethods;
	ClassMethods.allocate = NULL;
	ClassMethods.finalize = NULL;

	if (strcmp(className, "SomeClass") == 0)
	{

	}

	return ClassMethods;
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
		else if (isStatic && strcmp(signature, "AddClassInstanceToContainer(_)") == 0)
		{
			return AddClassInstanceToContainer;
		}
	}
	else if (strcmp(className, "Math") == 0)
	{
		//All Math functions are nested in this Statement

		if (isStatic && strcmp(signature, "Sin(_)") == 0)
		{
			return WrenMathSin;
		}
		else if (isStatic && strcmp(signature, "Cos(_)") == 0)
		{
			return WrenMathCos;
		}
		else if (isStatic && strcmp(signature, "Tan(_)") == 0)
		{
			return WrenMathTan;
		}
	}
	else if (strcmp(className, "ConfigLoader") == 0)
	{
		//All ConfigLoader functions are nested in this Statement

		if (true)
		{

		}
	}
	else if (strcmp(className, "InputHandler") == 0)
	{
		//All InputHandler functions are nested in this Statement
		if (isStatic && strcmp(signature, "GetInputBuffer()") == 0)
		{
			return GetInputBuffer;
		}
		else if (isStatic && strcmp(signature, "GetMouseDeltaX()") == 0)
		{
			return GetMouseDeltaX;
		}
		else if (isStatic && strcmp(signature, "GetMouseDeltaY()") == 0)
		{
			return GetMouseDeltaY;
		}
		else if (isStatic && strcmp(signature, "GetMouseWheelDelta()") == 0)
		{
			return GetMouseWheelDelta;
		}
		else if (isStatic && strcmp(signature, "GetMousePosX()") == 0)
		{
			return GetMousePosX;
		}
		else if (isStatic && strcmp(signature, "GetMousePosY()") == 0)
		{
			return GetMousePosY;
		}
		else if (isStatic && strcmp(signature, "SetLockMouse(_)") == 0)
		{
			return SetLockMouse;
		}
		else if (isStatic && strcmp(signature, "SetShowMouse(_)") == 0)
		{
			return SetShowMouse;
		}
	}
	else if (strcmp(className, "MasterGraphicsHandler") == 0)
	{
		//All MasterGraphicsHandler functions are nested in this Statement

		if (isStatic && strcmp(signature, "Camera3DPitch(_)") == 0)
		{
			return Camera3DPitch;
		}
		else if (isStatic && strcmp(signature, "Camera3DYaw(_)") == 0)
		{
			return Camera3DYaw;
		}
		else if (isStatic && strcmp(signature, "Camera3DRoll(_)") == 0)
		{
			return Camera3DRoll;
		}
		else if (isStatic && strcmp(signature, "GetCamera3DForward()") == 0)
		{
			return GetCamera3DForward;
		}
		else if (isStatic && strcmp(signature, "GetCamera3DRight()") == 0)
		{
			return GetCamera3DRight;
		}
		else if (isStatic && strcmp(signature, "GetCamera3DUp()") == 0)
		{
			return GetCamera3DUp;
		}
		else if (isStatic && strcmp(signature, "GetCamera3DPosition()") == 0)
		{
			return GetCamera3DPosition;
		}
		else if (isStatic && strcmp(signature, "SetCamera3DPosition(_,_,_)") == 0)
		{
			return SetCamera3DPosition;
		}
		else if (isStatic && strcmp(signature, "SetCamera3DPosition(_)") == 0)
		{
			return SetCamera3DPositionFromList;
		}
		else if (isStatic && strcmp(signature, "Camera3DLookAt(_,_,_,_,_,_)") == 0)
		{
			return Camera3DLookAt;
		}
		else if (isStatic && strcmp(signature, "Camera3DLookAt(_,_)") == 0)
		{
			return Camera3DLookAtFromList;
		}
		else if (isStatic && strcmp(signature, "Camera3DLookAtPos(_,_,_,_,_,_)") == 0)
		{
			return Camera3DLookAtPos;
		}
		else if (isStatic && strcmp(signature, "Camera3DLookAtPos(_,_)") == 0)
		{
			return Camera3DLookAtPosFromList;
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

		if (isStatic && strcmp(signature, "GetObject3DPosition(_)") == 0)
		{
			return GetObject3DPosition;
		}
		else if (isStatic && strcmp(signature, "SetObject3DPosition(_,_)") == 0)
		{
			return SetObject3DPositionFromList;
		}
		else if (isStatic && strcmp(signature, "SetObject3DPosition(_,_,_,_)") == 0)
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

//Utility
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
void ScriptHandler::AddClassInstanceToContainer(WrenVM* vm)
{
	//get the handle to slot one and hope it's a class.
	WrenHandle* WrenClassHandle = wrenGetSlotHandle(vm, 1);

	//Store the handle locally so it can be freed in the destructorlater
	m_ClassHandleContainer.push_back(WrenClassHandle);
}

//math
void ScriptHandler::WrenMathSin(WrenVM* vm)
{
	double var = wrenGetSlotDouble(vm, 1);
	var = sin(var);
	wrenSetSlotDouble(vm, 0, var);
}
void ScriptHandler::WrenMathCos(WrenVM* vm)
{
	double var = wrenGetSlotDouble(vm, 1);
	var = cos(var);
	wrenSetSlotDouble(vm, 0, var);
}
void ScriptHandler::WrenMathTan(WrenVM* vm)
{
	double var = wrenGetSlotDouble(vm, 1);
	var = tan(var);
	wrenSetSlotDouble(vm, 0, var);
}

//InputHandler
void ScriptHandler::GetMousePosX(WrenVM* vm)
{
	float posX = m_InputHandler_ptr->GetMousePosX();
	wrenSetSlotDouble(vm, 0, posX);
}
void ScriptHandler::GetMousePosY(WrenVM* vm)
{
	float posY = m_InputHandler_ptr->GetMousePosY();
	wrenSetSlotDouble(vm, 0, posY);
}
void ScriptHandler::GetMouseDeltaX(WrenVM* vm)
{
	wrenSetSlotDouble(vm, 0, m_InputHandler_ptr->GetMouseDeltaX());
}
void ScriptHandler::GetMouseDeltaY(WrenVM* vm)
{
	wrenSetSlotDouble(vm, 0, m_InputHandler_ptr->GetMouseDeltaY());
}
void ScriptHandler::GetMouseWheelDelta(WrenVM* vm)
{
	float wheelDelta = m_InputHandler_ptr->GetMouseWheelDelta();
	wrenSetSlotDouble(vm, 0, wheelDelta);
}
void ScriptHandler::SetLockMouse(WrenVM* vm)
{
	bool isLocked = wrenGetSlotBool(vm, 1);
	m_InputHandler_ptr->SetLockMouse(isLocked);
}
void ScriptHandler::SetShowMouse(WrenVM* vm)
{
	bool isShown = wrenGetSlotBool(vm, 1);
	m_InputHandler_ptr->SetLockMouse(isShown);
}
void ScriptHandler::GetInputBuffer(WrenVM* vm)
{
	std::vector<int> localInputBuffer = *m_InputHandler_ptr->GetInputBufferPTR();

	//need a fuckton of slots, indicies 0-17 most likely
	wrenEnsureSlots(vm, localInputBuffer.size() + 1);

	wrenSetSlotNewList(vm, 0);

	//load all input into slots and into the list
	for (unsigned int i = 0; i < localInputBuffer.size(); i++)
	{
		//slot index is i + 1 becasue the list is in 0
		wrenSetSlotDouble(vm, i + 1, localInputBuffer[i]);
		wrenInsertInList(vm, 0, i, i + 1);
	}
}

//MasterGraphicshandler
void ScriptHandler::Camera3DPitch(WrenVM* vm)
{
	float angle = wrenGetSlotDouble(vm, 1);
	m_MasterGraphicsHandler_ptr->GetGraphicsHandler3DPTR()->GetCameraWrapperPTR()->Pitch(angle);
}
void ScriptHandler::Camera3DYaw(WrenVM* vm)
{
	float angle = wrenGetSlotDouble(vm, 1);
	m_MasterGraphicsHandler_ptr->GetGraphicsHandler3DPTR()->GetCameraWrapperPTR()->Yaw(angle);
}
void ScriptHandler::Camera3DRoll(WrenVM* vm)
{
	float angle = wrenGetSlotDouble(vm, 1);
	m_MasterGraphicsHandler_ptr->GetGraphicsHandler3DPTR()->GetCameraWrapperPTR()->Roll(angle);
}
void ScriptHandler::GetCamera3DPosition(WrenVM* vm)
{
	Vector3 posVec = m_MasterGraphicsHandler_ptr->GetGraphicsHandler3DPTR()->GetCameraWrapperPTR()->GetCameraPosition();

	//Ensure slots since we are using a lot of them, 5 for indicies 0-4
	wrenEnsureSlots(vm, 5);

	//return vecot as a list in slot 0
	wrenSetSlotNewList(vm, 0);

	wrenSetSlotDouble(vm, 2, 0.0f - posVec.x);
	wrenSetSlotDouble(vm, 3, posVec.y);
	wrenSetSlotDouble(vm, 4, posVec.z);

	wrenInsertInList(vm, 0, 0, 2);
	wrenInsertInList(vm, 0, 1, 3);
	wrenInsertInList(vm, 0, 2, 4);
}
void ScriptHandler::GetCamera3DForward(WrenVM* vm)
{
	Vector3 forwardVec = m_MasterGraphicsHandler_ptr->GetGraphicsHandler3DPTR()->GetCameraWrapperPTR()->GetCameraForward();

	//Ensure slots since we are using a lot of them, 5 for indicies 0-4
	wrenEnsureSlots(vm, 5);

	//return vecot as a list in slot 0
	wrenSetSlotNewList(vm, 0);

	wrenSetSlotDouble(vm, 2, 0.0f - forwardVec.x);
	wrenSetSlotDouble(vm, 3, forwardVec.y);
	wrenSetSlotDouble(vm, 4, forwardVec.z);

	wrenInsertInList(vm, 0, 0, 2);
	wrenInsertInList(vm, 0, 1, 3);
	wrenInsertInList(vm, 0, 2, 4);
}
void ScriptHandler::GetCamera3DRight(WrenVM* vm)
{
	Vector3 rightVec = m_MasterGraphicsHandler_ptr->GetGraphicsHandler3DPTR()->GetCameraWrapperPTR()->GetCameraRight();

	//Ensure slots since we are using a lot of them, 5 for indicies 0-4
	wrenEnsureSlots(vm, 5);

	//return vecot as a list in slot 0
	wrenSetSlotNewList(vm, 0);

	wrenSetSlotDouble(vm, 2, 0.0f - rightVec.x);
	wrenSetSlotDouble(vm, 3, rightVec.y);
	wrenSetSlotDouble(vm, 4, rightVec.z);

	wrenInsertInList(vm, 0, 0, 2);
	wrenInsertInList(vm, 0, 1, 3);
	wrenInsertInList(vm, 0, 2, 4);
}
void ScriptHandler::GetCamera3DUp(WrenVM* vm)
{
	Vector3 upVec = m_MasterGraphicsHandler_ptr->GetGraphicsHandler3DPTR()->GetCameraWrapperPTR()->GetCameraUp();

	//Ensure slots since we are using a lot of them, 5 for indicies 0-4
	wrenEnsureSlots(vm, 5);

	//return vecot as a list in slot 0
	wrenSetSlotNewList(vm, 0);

	wrenSetSlotDouble(vm, 2, 0.0f - upVec.x);
	wrenSetSlotDouble(vm, 3, upVec.y);
	wrenSetSlotDouble(vm, 4, upVec.z);

	wrenInsertInList(vm, 0, 0, 2);
	wrenInsertInList(vm, 0, 1, 3);
	wrenInsertInList(vm, 0, 2, 4);
}
void ScriptHandler::SetCamera3DPosition(WrenVM* vm)
{
	float xPos = wrenGetSlotDouble(vm, 1);
	float yPos = wrenGetSlotDouble(vm, 2);
	float zPos = wrenGetSlotDouble(vm, 3);

	Vector3 posVec = { xPos, yPos, zPos };

	m_MasterGraphicsHandler_ptr->GetGraphicsHandler3DPTR()->GetCameraWrapperPTR()->SetCameraPosition(posVec);
}
void ScriptHandler::SetCamera3DPositionFromList(WrenVM* vm)
{
	//need slots 0-4
	wrenEnsureSlots(vm, 5);

	wrenGetListElement(vm, 1, 0, 2);
	wrenGetListElement(vm, 1, 1, 3);
	wrenGetListElement(vm, 1, 2, 4);

	Vector3 posVec;
	posVec.x = wrenGetSlotDouble(vm, 2);
	posVec.y = wrenGetSlotDouble(vm, 3);
	posVec.z = wrenGetSlotDouble(vm, 4);

	m_MasterGraphicsHandler_ptr->GetGraphicsHandler3DPTR()->GetCameraWrapperPTR()->SetCameraPosition(posVec);
}
void ScriptHandler::Camera3DLookAt(WrenVM* vm)
{
	//need slots 0-4
	wrenEnsureSlots(vm, 7);

	float xTarget = wrenGetSlotDouble(vm, 1);
	float yTarget = wrenGetSlotDouble(vm, 2);
	float zTarget = wrenGetSlotDouble(vm, 3);

	float xUp = wrenGetSlotDouble(vm, 4);
	float yUp = wrenGetSlotDouble(vm, 5);
	float zUp = wrenGetSlotDouble(vm, 6);

	Vector3 targetVec = { xTarget, yTarget, zTarget };
	Vector3 upVec = { xUp, yUp, zUp };

	m_MasterGraphicsHandler_ptr->GetGraphicsHandler3DPTR()->GetCameraWrapperPTR()->LookAt(targetVec, upVec);
}
void ScriptHandler::Camera3DLookAtFromList(WrenVM* vm)
{
	//need slots 0-8
	wrenEnsureSlots(vm, 9);

	wrenGetListElement(vm, 1, 0, 3);
	wrenGetListElement(vm, 1, 1, 4);
	wrenGetListElement(vm, 1, 2, 5);

	wrenGetListElement(vm, 2, 0, 6);
	wrenGetListElement(vm, 2, 1, 7);
	wrenGetListElement(vm, 2, 2, 8);

	Vector3 targetVec;
	targetVec.x = wrenGetSlotDouble(vm, 3);
	targetVec.y = wrenGetSlotDouble(vm, 4);
	targetVec.z = wrenGetSlotDouble(vm, 5);

	Vector3 upVec;
	upVec.x = wrenGetSlotDouble(vm, 6);
	upVec.y = wrenGetSlotDouble(vm, 7);
	upVec.z = wrenGetSlotDouble(vm, 8);
	upVec.z = wrenGetSlotDouble(vm, 8);

	m_MasterGraphicsHandler_ptr->GetGraphicsHandler3DPTR()->GetCameraWrapperPTR()->LookAt(targetVec, upVec);
}
void ScriptHandler::Camera3DLookAtPos(WrenVM* vm)
{
	//need slots 0-4
	wrenEnsureSlots(vm, 7);

	float xTarget = wrenGetSlotDouble(vm, 1);
	float yTarget = wrenGetSlotDouble(vm, 2);
	float zTarget = wrenGetSlotDouble(vm, 3);

	float xUp = wrenGetSlotDouble(vm, 4);
	float yUp = wrenGetSlotDouble(vm, 5);
	float zUp = wrenGetSlotDouble(vm, 6);

	Vector3 targetVec = { xTarget, yTarget, zTarget };
	Vector3 upVec = { xUp, yUp, zUp };

	m_MasterGraphicsHandler_ptr->GetGraphicsHandler3DPTR()->GetCameraWrapperPTR()->LookAtPos(targetVec, upVec);
}
void ScriptHandler::Camera3DLookAtPosFromList(WrenVM* vm)
{
	//need slots 0-8
	wrenEnsureSlots(vm, 9);

	wrenGetListElement(vm, 1, 0, 3);
	wrenGetListElement(vm, 1, 1, 4);
	wrenGetListElement(vm, 1, 2, 5);

	wrenGetListElement(vm, 2, 0, 6);
	wrenGetListElement(vm, 2, 1, 7);
	wrenGetListElement(vm, 2, 2, 8);

	Vector3 targetVec;
	targetVec.x = wrenGetSlotDouble(vm, 3);
	targetVec.y = wrenGetSlotDouble(vm, 4);
	targetVec.z = wrenGetSlotDouble(vm, 5);

	Vector3 upVec;
	upVec.x = wrenGetSlotDouble(vm, 6);
	upVec.y = wrenGetSlotDouble(vm, 7);
	upVec.z = wrenGetSlotDouble(vm, 8);
	upVec.z = wrenGetSlotDouble(vm, 8);

	m_MasterGraphicsHandler_ptr->GetGraphicsHandler3DPTR()->GetCameraWrapperPTR()->LookAtPos(targetVec, upVec);
}

//ObjectHandler3D
void ScriptHandler::CreateObject3D(WrenVM* vm)
{
	//create an object and return its Identifier
	std::string fileName = wrenGetSlotString(vm, 1);

	unsigned int createdObjIndex = m_ObjHandler3D_ptr->CreateObject(fileName);

	wrenSetSlotDouble(vm, 0, createdObjIndex);
}

//BaseObject3D
void ScriptHandler::GetObject3DPosition(WrenVM* vm)
{
	unsigned int objIndex = (unsigned int)wrenGetSlotDouble(vm, 1);

	BaseObject3D* obj_ptr = NULL;
	obj_ptr = m_ObjHandler3D_ptr->GetObjectPTR(objIndex);

	Vector3 Position;

	//prevent trying to access a null pointer
	if (obj_ptr != NULL)
	{
		Position = obj_ptr->GetPosition();
	}
	else
	{
		std::cout << "Error getting object position" << std::endl;

		Position.x = 0;
		Position.y = 0;
		Position.z = 0;
	}

	//Ensure slots since we are using a lot of them, 5 for indicies 0-4
	wrenEnsureSlots(vm, 5);

	//return vecot as a list in slot 0
	wrenSetSlotNewList(vm, 0);

	wrenSetSlotDouble(vm, 2, 0.0f - Position.x);
	wrenSetSlotDouble(vm, 3, Position.y);
	wrenSetSlotDouble(vm, 4, Position.z);

	wrenInsertInList(vm, 0, 0, 2);
	wrenInsertInList(vm, 0, 1, 3);
	wrenInsertInList(vm, 0, 2, 4);
}
void ScriptHandler::SetObject3DPosition(WrenVM* vm)
{
	//this used to have a string to void*...

	unsigned int objIndex = (unsigned int)wrenGetSlotDouble(vm, 1);
	
	Vector3 PosVector;
	PosVector.x = wrenGetSlotDouble(vm, 2);
	PosVector.y = wrenGetSlotDouble(vm, 3);
	PosVector.z = wrenGetSlotDouble(vm, 4);

	BaseObject3D* obj_ptr = NULL;
	obj_ptr = m_ObjHandler3D_ptr->GetObjectPTR(objIndex);

	//prevent trying to access a null pointer
	if (obj_ptr != NULL)
	{
		obj_ptr->SetPosition(PosVector);
	}
	else
	{
		std::cout << "Error setting object position" << std::endl;
	}
}
void ScriptHandler::SetObject3DPositionFromList(WrenVM* vm)
{
	//need slots 0-5
	wrenEnsureSlots(vm, 6);

	wrenGetListElement(vm, 2, 0, 3);
	wrenGetListElement(vm, 2, 1, 4);
	wrenGetListElement(vm, 2, 2, 5);

	unsigned int objIndex = (unsigned int)wrenGetSlotDouble(vm, 1);

	Vector3 posVec;
	posVec.x = wrenGetSlotDouble(vm, 3);
	posVec.y = wrenGetSlotDouble(vm, 4);
	posVec.z = wrenGetSlotDouble(vm, 5);

	BaseObject3D* obj_ptr = NULL;
	obj_ptr = m_ObjHandler3D_ptr->GetObjectPTR(objIndex);

	//prevent trying to access a null pointer
	if (obj_ptr != NULL)
	{
		obj_ptr->SetPosition(posVec);
	}
	else
	{
		std::cout << "Error setting object position" << std::endl;
	}
}