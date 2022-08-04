#pragma once

#include <iostream>
#include <vector>
#include <any>
#include <string>
#include <sstream>

#include "ConfigLoader.h"
#include "MasterGraphicsHandler.h"
//#include "GraphicsHandler3D.h"
//#include "GraphicsHandler2D.h"
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
		void SetMasterGraphicsHandlerPTR(MasterGraphicsHandler* MasterGraphicsHandlerIn_ptr) { m_LocalMasterGraphicsHandler_ptr = MasterGraphicsHandlerIn_ptr; }
		void SetObjHandler3DPTR(ObjectHandler3D* ObjHandlerIn_ptr) { m_LocalObjHandler3D_ptr = ObjHandlerIn_ptr; }
		//void SetBaseDirectory(std::string* dirIn) { m_LocalBaseDirectory_ptr = dirIn; }
		void SetConfigLoaderPTR(ConfigLoader* confLoaderIn_ptr) { m_LocalConfigLoader_ptr = confLoaderIn_ptr; }

	private:

		//Wren callback definitions
		static int GetMethodHandle(WrenVM* vm, std::string& Signature);
		static WrenLoadModuleResult LoadModule(WrenVM* vm, const char* moduleFileName);
		static WrenForeignClassMethods BindForeignClass(WrenVM* vm, const char* moduleName, const char* className);
		static WrenForeignMethodFn BindForeignMethod(WrenVM* vm, const char* moduleName, const char* className, bool isStatic, const char* signature);

		//Function wrappers for Wren specific calls
		static void WriteFn(WrenVM* vm, const char* text);
		static void ErrorFn(WrenVM* vm, WrenErrorType ErrorType, const char* moduleName, const int line, const char* message);
		//Utility fns
		static void GetTimeStep(WrenVM* vm);
		static void AddClassToContainer(WrenVM* vm);
		static void AddClassInstanceToContainer(WrenVM* vm);
		static void SetDirectory(WrenVM* vm);
		//Math fns
		static void WrenMathSin(WrenVM* vm);
		static void WrenMathCos(WrenVM* vm);
		static void WrenMathTan(WrenVM* vm);
		//ConfigLoader fns
		//Input Handler
		static void GetMousePosX(WrenVM* vm);
		static void GetMousePosY(WrenVM* vm);
		static void GetMouseDeltaX(WrenVM* vm);
		static void GetMouseDeltaY(WrenVM* vm);
		static void GetMouseWheelDelta(WrenVM* vm);
		static void LockMouse(WrenVM* vm);
		static void ToggleShowMouse(WrenVM* vm);
		static void CheckKeyPressed(WrenVM* vm);
		static void CheckKeyHeld(WrenVM* vm);
		static void CheckKeyReleased(WrenVM* vm);
		static void CheckKeyUp(WrenVM* vm);
		static void ReturnKeyPressed(WrenVM* vm);
		static void CheckMouseButtonPressed(WrenVM* vm);
		static void CheckMouseButtonHeld(WrenVM* vm);
		static void CheckMouseButtonReleased(WrenVM* vm);
		static void CheckMouseButtonUp(WrenVM* vm);
		//MasterGraphicsHandler fns
		static void Camera3DPitch(WrenVM* vm);
		static void Camera3DYaw(WrenVM* vm);
		static void Camera3DRoll(WrenVM* vm);
		static void GetCamera3DPosition(WrenVM* vm);
		static void GetCamera3DForward(WrenVM* vm);
		static void GetCamera3DRight(WrenVM* vm);
		static void GetCamera3DUp(WrenVM* vm);
		static void SetCamera3DPosition(WrenVM* vm);
		static void Camera3DLookAt(WrenVM* vm);
		static void Camera3DLookAtPos(WrenVM* vm);
		static void Camera3DSetFOV(WrenVM* vm);
		//ObjectHandler3D fns
		static void CreateObject3D(WrenVM* vm);
		//BaseObject3D fns
		static void GetObject3DPosition(WrenVM* vm);
		static void SetObject3DPosition(WrenVM* vm);
		static void GetObject3DForward(WrenVM* vm);
		static void GetObject3DRight(WrenVM* vm);
		static void GetObject3DUp(WrenVM* vm);
		static void Object3DLookAt(WrenVM* vm);
		static void Object3DLookAtPos(WrenVM* vm);
		static void Object3DLookAtLerp(WrenVM* vm);
		static void Object3DLookAtPosLerp(WrenVM* vm);
		static void Object3DPitch(WrenVM* vm);
		static void Object3DYaw(WrenVM* vm);
		static void Object3DRoll(WrenVM* vm);
		static void SetObject3DTexture(WrenVM* vm);


		int m_slotCount = 0;

		WrenVM* m_WrenVirtualMachine_ptr;

		double* m_localTimeSetp_ptr;
		MasterGraphicsHandler* m_LocalMasterGraphicsHandler_ptr;
		ObjectHandler3D* m_LocalObjHandler3D_ptr;
		//std::string* m_LocalBaseDirectory_ptr;
		ConfigLoader* m_LocalConfigLoader_ptr;

		static double* m_timeStep_ptr;
		static MasterGraphicsHandler* m_MasterGraphicsHandler_ptr;
		static ObjectHandler3D* m_ObjHandler3D_ptr;
		//static std::string* m_baseDirectory_ptr;
		static ConfigLoader* m_ConfigLoader_ptr;

		static std::vector<WrenHandle*> m_ModuleHandleContainer;
		static std::vector<WrenHandle*> m_ClassHandleContainer;
		static std::vector<WrenHandle*> m_MethodHandleContainer;
	};
