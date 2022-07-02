#pragma once

#include "raylib.h"

class BaseObject3D
{
public:
	BaseObject3D();

	void Render();
	
	virtual void Update();

	bool* GetStaticStatusPTR() { return &m_isStatic; }
	void SetStaticStatus(bool* staticStausIn) { m_isStatic = staticStausIn; }

	bool* GetUpdateStatusPTR() { return &m_update; }
	void SetUpdateStatus(bool* updateStausIn) { m_update = updateStausIn; }

private:

	bool m_isStatic = true;
	bool m_update = true;
};

