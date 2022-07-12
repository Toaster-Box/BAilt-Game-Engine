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

	void Pitch(float& angle);

	void Yaw(float& angle);

	void Roll(float& angle);

	void RotateAxisAngle(float& angle, const Vector3& axis);

	void LookAt(Vector3& targetDir, Vector3& desiredUp);
	void LookAtPos(Vector3& targetPos, Vector3& desiredUp);

protected:

	void UpdateTransformation(Matrix* transformationMat_ptr);

	Vector3 m_scale = { 1.0f, 1.0f, 1.0f };

	Vector3 m_position = { 0.0f, 0.0f, 0.0f };

	Vector3 m_forward = { 0.0f, 1.0f, 0.0f };
	Vector3 m_right = { .0f, 0.0f, 0.0f };
	Vector3 m_up = { 0.0f, 0.0f, 1.0f };

private:
};


