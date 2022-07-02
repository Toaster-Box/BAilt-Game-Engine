#pragma once

#include <string>

#include "raylib.h"

class BaseObject3D
{
public:
	BaseObject3D(std::string& fileName);

	void Render();

	virtual void Update();

	void LoadModelFromFile(std::string& fileName);

	bool* GetStaticStatusPTR() { return &m_isStatic; }
	void SetStaticStatus(bool* staticStausIn) { m_isStatic = staticStausIn; }

	bool* GetUpdateStatusPTR() { return &m_update; }
	void SetUpdateStatus(bool* updateStausIn) { m_update = updateStausIn; }

private:

	bool m_isStatic = true;
	bool m_update = true;

	Vector3 m_postitionVec;
	Quaternion m_rotationQuat;
	
	Model m_objModel;

};

