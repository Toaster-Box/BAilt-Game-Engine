#pragma once

#include <iostream>
#include <vector>
#include <any>
#include <string>
#include <sstream>

//#include "ConfigLoader.h"
#include "ObjectHandler3D.h"
#include "BaseObject3D.h"
//#include "ObjectHandler2D.h"
//#include "BaseObject2D.h"

#include "wren.hpp"
#include "raylib.h"

class ScriptHandler
{
public:
	ScriptHandler();
	~ScriptHandler();

	void RunBootScript();

	void Update();

	bool LoadScript(std::string& ModuleName, std::string& FileName);

	
	WrenHandle* GetClassHandle(std::string& ModuleName, std::string ClassName);
	bool RunMethodNoArgs(WrenHandle* ClassHandle, WrenHandle* MethodHandle);

	void SetTimeStepPTR(double* timeStepIn_ptr) { m_localTimeSetp_ptr = timeStepIn_ptr; }
	void SetObjHandler3DPTR(ObjectHandler3D* ObjHandlerIn_ptr) { m_LocalObjHandler3D_ptr = ObjHandlerIn_ptr; }
	void SetScriptFileDirectory(std::string* filePathIn_ptr) { m_LocalScriptFileDirectory_ptr = filePathIn_ptr; }

private:

	//Wren callback definitions
	static WrenHandle* GetMethodHandle(WrenVM* vm, std::string& Signature);
	static WrenLoadModuleResult LoadModule(WrenVM* vm, const char* moduleFileName);
	static WrenForeignClassMethods BindForeignClass(WrenVM* vm, const char* moduleName, const char* className);
	static WrenForeignMethodFn BindForeignMethod(WrenVM* vm, const char* moduleName, const char* className, bool isStatic, const char* signature);

	//Function wrappers for Wren specific calls
	static void WriteFn(WrenVM* vm, const char* text);
	static void ErrorFn(WrenVM* vm, WrenErrorType ErrorType, const char* moduleName, const int line, const char* message);

	static void GetTimeStep(WrenVM* vm);
	static void AddClassToContainer(WrenVM* vm);
	static void AddClassInstanceToContainer(WrenVM* vm);

	static void WrenMathSin(WrenVM* vm);
	static void WrenMathCos(WrenVM* vm);
	static void WrenMathTan(WrenVM* vm);

	static void CreateObject3D(WrenVM* vm);
	static void GetObject3DPosition(WrenVM* vm);
	static void SetObject3DPosition(WrenVM* vm);
	static void SetObject3DPositionFromList(WrenVM* vm);

	
	int m_slotCount = 0;

	WrenVM* m_WrenVirtualMachine_ptr;

	double* m_localTimeSetp_ptr;
	ObjectHandler3D* m_LocalObjHandler3D_ptr;
	std::string* m_LocalScriptFileDirectory_ptr;

	static double* m_timeStep_ptr;
	static ObjectHandler3D* m_ObjHandler3D_ptr;
	static std::string* m_ScriptFileDirectory_ptr;

	static std::vector<WrenHandle*> m_ModuleHandleContainer;
	static std::vector<WrenHandle*> m_ClassHandleContainer;
	static std::vector<WrenHandle*> m_MethodHandleContainer;
};
	


//Wont work in the .cpp for some fucking reason
ObjectHandler3D* ScriptHandler::m_ObjHandler3D_ptr = NULL;
