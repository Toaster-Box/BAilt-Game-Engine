#pragma once

#include <iostream>

#include <math.h>
#include <cmath>

#include "raylib.h"
#include "raymath.h"


class Transformation
{
public:
	Transformation();
	~Transformation();

	Vector3 GetPosition() { return m_position; }
	void SetPosition(Vector3& PosIn) { m_position = PosIn; }
	Vector3 GetForward() { return m_forward; }
	Vector3 GetRight() { return m_right; }
	Vector3 GetUp() { return m_up; }


	void Pitch(float& angle);
	void Yaw(float& angle);
	void Roll(float& angle);
	void RotateAxisAngle(float& angle, Vector3& axis);

	void LookAt(Vector3 targetDir, Vector3 desiredUp);
	void LookAtPos(Vector3& targetPos, Vector3& desiredUp);
	void LookAtLerp(Vector3& targetDir, Vector3& desiredUp, float deltaPercent);
	void LookAtPosLerp(Vector3& targetPos, Vector3& desiredUp, float deltaPercent);

	void UpdateTransformation(Matrix* transformationMat_ptr);

protected:

	Quaternion m_orientation = {0.0f, 0.0f, -1.0f, 0.0f};


	Vector3 m_scale = { 1.0f, 1.0f, 1.0f };

	Vector3 m_position = { 0.0f, 0.0f, 0.0f };

	Vector3 m_forward = { 0.0f, 1.0f, 0.0f };
	Vector3 m_right = { -1.0f, 0.0f, 0.0f }; 
	Vector3 m_up = { 0.0f, 0.0f, 1.0f };

private:
};
