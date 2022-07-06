#pragma once

#include <iostream>
#include <vector>
#include <any>

//#include "ConfigLoader.h"
#include "ObjectHandler3D.h"
//#include "BaseObject3D.h"
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

	void RunTestCode();

	//void SetScriptFileDirectory(std::string& filePathIn) { m_scriptFileDirectory = const_cast<char*>(filePathIn.c_str()); }
	void SetObjHandler3DPTR(ObjectHandler3D* ObjHandlerIn_ptr) { m_LocalObjHandler3D_ptr = ObjHandlerIn_ptr; }

	//Memory wrappers, welcome to one line hell. Now get out. 
	//Disassociates wren functions from wren types (WrenType and WrenHandle), along with enfocing the use of the handler objects VM
	//handles are just nonspecific pointers, I think

	void AddSlots(int* addedSlotsIn_ptr) { m_slotCount += *addedSlotsIn_ptr; wrenEnsureSlots(m_WrenVirtualMachine_ptr, m_slotCount); }
	void RemoveSlots(int* removedSlotsIn_ptr) { m_slotCount -= *removedSlotsIn_ptr; wrenEnsureSlots(m_WrenVirtualMachine_ptr, m_slotCount); }
	void SetSlotCount(int* slotCountIn_ptr) { m_slotCount = *slotCountIn_ptr; wrenEnsureSlots(m_WrenVirtualMachine_ptr, m_slotCount); }
	int* GetSlotCountPTR() { return &m_slotCount; }
	int GetTrueSlotCount() { return wrenGetSlotCount(m_WrenVirtualMachine_ptr); }

	void SetSlotBool(int slotNumber, bool* BoolIn_ptr) { wrenSetSlotBool(m_WrenVirtualMachine_ptr, slotNumber, *BoolIn_ptr); }
	void SetSlotDouble(int slotNumber, double* doubleIn_ptr) { wrenSetSlotDouble(m_WrenVirtualMachine_ptr, slotNumber, *doubleIn_ptr); }
	void SetSlotNull(int slotNumber) { wrenSetSlotNull(m_WrenVirtualMachine_ptr, slotNumber); }
	void SetSlotString(int slotNumber, std::string& StringIn_ptr) { wrenSetSlotString(m_WrenVirtualMachine_ptr, slotNumber, const_cast<char*>(StringIn_ptr.c_str())); }
	void SetSlotList(int slotNumber) { wrenSetSlotNewList(m_WrenVirtualMachine_ptr, slotNumber); }

	void InsertInList(int listSlot, int index, int elementSlot) { wrenInsertInList(m_WrenVirtualMachine_ptr, listSlot, index, elementSlot); }

	bool GetSlotBool(int slotNumber) { return wrenGetSlotBool(m_WrenVirtualMachine_ptr, slotNumber); }
	double GetSlotDouble(int slotNumber) { return wrenGetSlotDouble(m_WrenVirtualMachine_ptr, slotNumber); }
	std::string GetSlotString(int slotNumber) { return wrenGetSlotString(m_WrenVirtualMachine_ptr, slotNumber); }
	int GetSlotType(int slotNumber) { return wrenGetSlotType(m_WrenVirtualMachine_ptr, slotNumber); } //techically returns an enum of WrenType

	std::any GetSlotHandle(int slotNumber) {
		m_HandlePTRContainer.push_back((std::any)wrenGetSlotHandle(m_WrenVirtualMachine_ptr, slotNumber));
		return m_HandlePTRContainer[m_HandlePTRContainer.size() - 1];
	} //I will never have a programming job and that's a good thing
	void SetSlotHandle(int slotNumber, std::any HandleIn_ptr) { wrenSetSlotHandle(m_WrenVirtualMachine_ptr, slotNumber, std::any_cast<WrenHandle*>(HandleIn_ptr)); }
	void ReleaseHandle(std::any HandleIn_ptr) { wrenReleaseHandle(m_WrenVirtualMachine_ptr, std::any_cast<WrenHandle*>(HandleIn_ptr)); }


private:

	//Wren callback definitions
	static WrenLoadModuleResult LoadModule(WrenVM* vm, const char* moduleFileName);
	static WrenForeignMethodFn BindForeignMethod(WrenVM* vm, const char* moduleName, const char* className, bool isStatic, const char* signature);

	//Function wrappers for Wren specific calls
	static void WriteFn(WrenVM* vm, const char* text);
	static void ErrorFn(WrenVM* vm, WrenErrorType ErrorType, const char* moduleName, const int line, const char* message);
	static void CreateObject3D(WrenVM* vm);


	int m_slotCount = 0;

	WrenVM* m_WrenVirtualMachine_ptr;

	ObjectHandler3D* m_LocalObjHandler3D_ptr;

	// Cointains WrenHandle*
	std::vector<std::any> m_HandlePTRContainer;

	static ObjectHandler3D* m_ObjHandler3D_ptr;
	static std::string* m_scriptFileDirectory_ptr;
};
	

//Wont work in the .cpp for some fucking reason
ObjectHandler3D* ScriptHandler::m_ObjHandler3D_ptr = NULL;

