#pragma once

#include <string>
#include <vector>

#include "Transformation.h"

#include "raylib.h"
#include "raymath.h"

class BaseObject3D : public Transformation
{
public:
	BaseObject3D(unsigned int objIndexIn, unsigned int modelIndexIn);

	virtual void Update();

	void UpdateTransform() { UpdateTransformation(&m_Transform); }
	Matrix* GetTransform() { return &m_Transform; }

	bool GetStaticStatus() { return m_isStatic; }
	void SetStaticStatus(bool& staticStausIn) { m_isStatic = staticStausIn; }

	bool GetUpdateStatus() { return m_update; }
	void SetUpdateStatus(bool& updateStausIn) { m_update = updateStausIn; }

	unsigned int GetIndex() { return m_objectIndex; }

	unsigned int GetModelIndex() { return m_modelIndex; }
	void SetModelIndex(unsigned int indexIn) { m_modelIndex = indexIn; }

	unsigned int GetTextureMapIndex(unsigned int materialIndex) { return m_materialIndicies[materialIndex]; }
	void SetTextureMapIndex(unsigned int materialIndex, unsigned int indexIn) { m_materialIndicies[materialIndex] = indexIn; }


private:

	unsigned int m_objectIndex;
	unsigned int m_modelIndex;

	std::vector<int> m_materialIndicies;


	bool m_isStatic = true;
	bool m_update = true;

	
	Matrix m_Transform;
};

