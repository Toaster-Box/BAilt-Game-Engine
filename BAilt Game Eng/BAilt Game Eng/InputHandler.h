#pragma once

#include <vector>

#include "ConfigLoader.h"

#include "raylib.h"

namespace
{
	class InputHandler
	{
	public:
		InputHandler();
		~InputHandler();

		void UpdateLocalScreenSize();

		void Update();

		std::vector<int>* GetInputBufferPTR() { return &m_inputBuffer; }
		float GetMouseDeltaX() { return m_mouseDeltaX; }
		float GetMouseDeltaY() { return m_mouseDeltaY; }
		float GetMousePosX() { return m_mousePosX; }
		float GetMousePosY() { return m_mousePosY; }
		float GetMouseWheelDelta() { return m_mouseWheelDelta; }
		bool GetGamePadMode() { return m_gamePadMode; }

		void SetConfigLoaderPTR(ConfigLoader* confIn_ptr) { m_configLoader_ptr = confIn_ptr; UpdateLocalScreenSize(); }
		void SetLockMouse(bool boolIn) { m_lockMouse = boolIn; }
		void SetShowMouse(bool boolIn) { m_showMouse = boolIn; }

	private:

		std::vector<int> m_inputBuffer;
		std::vector<int> m_prevInputBuffer;

		float m_mouseDeltaX = 0.0;
		float m_mouseDeltaY = 0.0;
		float m_mousePosX = 0.0;
		float m_mousePosY = 0.0;
		float m_mouseWheelDelta = 0.0f;


		bool m_lockMouse = false;
		bool m_showMouse = true;

		bool m_gamePadMode = false;

		float m_localScreenWidth = 1.0f;
		float m_localScreenHeight = 1.0f;

		//used to normalize mouse pos
		ConfigLoader* m_configLoader_ptr;

		const static unsigned short numberOfInputChecksPerDevice = 10;
	};
}

InputHandler::InputHandler()
{

}
InputHandler::~InputHandler()
{

}


void InputHandler::UpdateLocalScreenSize()
{
	m_localScreenWidth = (float) *m_configLoader_ptr->GetConfigScreenWidth();
	m_localScreenHeight = (float) *m_configLoader_ptr->GetConfigScreenHeight();
}


void InputHandler::Update()
{
	//clear input buffer
	m_prevInputBuffer = m_inputBuffer;
	m_inputBuffer.clear();

	//Calc and set mouse vars
	m_mouseDeltaX = (GetMouseX() / m_localScreenWidth) - m_mousePosX;
	m_mouseDeltaY = (GetMouseY() / m_localScreenHeight) - m_mousePosY;

	m_mousePosX = GetMouseX() / m_localScreenWidth;
	m_mousePosY = GetMouseY() / m_localScreenHeight;

	m_mouseWheelDelta = GetMouseWheelMove();

	//Check Previous buffer for held keys
	for (unsigned int i = 0; i < m_prevInputBuffer.size(); i++)
	{
		if (IsKeyDown(m_prevInputBuffer[i]))
		{
			m_inputBuffer.push_back(m_prevInputBuffer[i]);
		}
	}

	//Loop over all mouse buttons, since it can use an iskeydown it isn't compared to the previous buffer 
	for (unsigned int i = 0; i < 7; i++)
	{
		if (IsMouseButtonDown(i))
		{
			m_inputBuffer.push_back(i);
		}
	}

	//put currently pressed keys in buffer
	for (unsigned int i = 0; i < numberOfInputChecksPerDevice; i++)
	{	
		int input = GetKeyPressed();

		if (input != 0)
		{
			m_inputBuffer.push_back(input);
			
		}
		else
		{
			i = numberOfInputChecksPerDevice;
		}
	}

	//makes sure at least a non input is in the input buffer
	if (m_inputBuffer.size() == 0)
	{
		m_inputBuffer.push_back(500);
	}

	////Uncomment for input debug
	//std::cout << "X: " << m_mousePosX << ", +" << m_mouseDeltaX;
	//std::cout << " Y: " << m_mousePosY << ", +" << m_mouseDeltaY << " W:" << m_mouseWheelDelta << " KEYS: ";
	//for (unsigned int i = 0; i < m_inputBuffer.size(); i++)
	//{
	//	std::cout << m_inputBuffer[i] << ", ";
	//}
	//std::cout << std::endl;

	if (m_lockMouse)
	{
		m_mousePosX = 32.0f;
		m_mousePosY = 32.0f;
	}

	if (!m_showMouse)
	{
		HideCursor();
	}
	else
	{
		ShowCursor();
	}
}