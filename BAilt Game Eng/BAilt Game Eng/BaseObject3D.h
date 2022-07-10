#pragma once

#include <string>

#include "raylib.h"

class BaseObject3D
{
public:
	BaseObject3D(std::string& fileName, unsigned int indexIn);

	void Render();

	virtual void Update();

	void LoadModelFromFile(std::string& fileName);

	unsigned int GetIndex() { return m_index; }
	bool GetStaticStatus() { return m_isStatic; }
	void SetStaticStatus(bool& staticStausIn) { m_isStatic = staticStausIn; }

	bool GetUpdateStatus() { return m_update; }
	void SetUpdateStatus(bool& updateStausIn) { m_update = updateStausIn; }

	Vector3 GetPosition() { return m_postitionVec; }
	void SetPosition(Vector3& PosIn) { m_postitionVec = PosIn; }

private:
	unsigned int m_index;

	bool m_isStatic = true;
	bool m_update = true;

	Vector3 m_postitionVec;
	Quaternion m_rotationQuat;
	
	Model m_objModel;

};

