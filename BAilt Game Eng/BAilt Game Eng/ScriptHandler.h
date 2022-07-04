#pragma once

#include "wren.hpp"

void writeFn(WrenVM* vm, const char* text)
{
	printf("%s", text);
}

class ScriptHandler
{
public:
	ScriptHandler();
	~ScriptHandler();

	void RunTestCode();

private:
	WrenVM* m_WrenVirtualMachine_ptr;
};

//Default constructor to initialize class
ScriptHandler::ScriptHandler()
{
	//Wren VM stores its own coppy of the config struct so it can be a local variable.
	WrenConfiguration WrenConfiguration;

	wrenInitConfiguration(&WrenConfiguration);
	WrenConfiguration.writeFn = &writeFn;

	m_WrenVirtualMachine_ptr = wrenNewVM(&WrenConfiguration);
}

//Deafult constructor to initialize class
ScriptHandler::~ScriptHandler()
{
	wrenFreeVM(m_WrenVirtualMachine_ptr);
}

void ScriptHandler::RunTestCode()
{
	WrenInterpretResult result = wrenInterpret(
		m_WrenVirtualMachine_ptr,
		"my_module",
		"System.print(\"I am running in a VM!\")");
}