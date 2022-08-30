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

	bool GetFurStatus() { return m_isFur; }
	void SetFurStatus(bool& staticStausIn) { m_isFur = staticStausIn; }

	bool GetTransparencyStatus() { return m_hasTransparency; }
	void SetTransparencyStatus(bool& transparencyStatusIn) { m_hasTransparency = transparencyStatusIn; }

	bool GetUpdateStatus() { return m_update; }
	void SetUpdateStatus(bool& updateStausIn) { m_update = updateStausIn; }

	bool GetShadowStatus() { return m_castsShadows; }
	void SetShadowStatus(bool& updateStausIn) { m_castsShadows = updateStausIn; }

	unsigned int GetIndex() { return m_objectIndex; }

	unsigned int GetModelIndex() { return m_modelIndex; }
	void SetModelIndex(unsigned int indexIn) { m_modelIndex = indexIn; }

	unsigned int GetTextureMapIndex(unsigned int materialIndex) { return m_materialIndicies[materialIndex]; }
	void SetTextureMapIndex(unsigned int materialIndex, unsigned int indexIn) { m_materialIndicies[materialIndex] = indexIn; }


private:

	unsigned int m_objectIndex;
	unsigned int m_modelIndex;

	std::vector<int> m_materialIndicies;


	bool m_isFur = false;
	bool m_hasTransparency = false;
	bool m_update = false;
	bool m_castsShadows = true;

	
	Matrix m_Transform;
};

