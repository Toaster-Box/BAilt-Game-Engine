#pragma once

#include "raylib.h"
#include "raymath.h"
namespace
{
	class CameraWrapper3D
	{
	public:
		CameraWrapper3D();

		void Pitch(float& angle);
		void Yaw(float& angle);
		void Roll(float& angle);

		void LookAt(Vector3& targetIn, Vector3& upIn);
		void LookAtPos(Vector3& targetIn, Vector3& upIn);

		Vector3	GetCameraPosition() { return m_rayCamera.position; }
		Vector3	GetCameraForward() { return m_forward; }
		Vector3	GetCameraRight() { return (m_left); }
		Vector3	GetCameraUp() { return Vector3Negate(m_rayCamera.up); }
		void SetCameraPosition(Vector3& posIn); 
		void SetFOV(float fovIn) { m_rayCamera.fovy = fovIn; }

		Camera3D* GetCameraPTR() { return &m_rayCamera; }

		void SetProjection(CameraProjection proj) { m_rayCamera.projection = proj; }

	private:

		void RotateAxisAngle(float& angle, const Vector3& axis);

		Vector3 m_left;
		Vector3 m_forward;

		Camera3D m_rayCamera;
	};
}

CameraWrapper3D::CameraWrapper3D()
{
	Vector3 position = { 0.0f, 3.0f, 0.0f };

	m_rayCamera.position = position;

	Vector3 forward = { 0.0f, -1.0f, 0.0f };
	Vector3 up = { 0.0f, 0.0f, 1.0f };
	Vector3 right = Vector3CrossProduct(up, Vector3Negate(forward));

	m_rayCamera.target = forward;
	m_rayCamera.up = up;
	m_left = right;
}


void CameraWrapper3D::LookAt(Vector3& targetIn, Vector3& upIn)
{
	//save Vector for later
	m_forward = targetIn;

	m_rayCamera.target.x = (0.0f - targetIn.x) + m_rayCamera.position.x;
	m_rayCamera.target.y = targetIn.y + m_rayCamera.position.y;
	m_rayCamera.target.z = targetIn.z + m_rayCamera.position.z;
	m_rayCamera.up = upIn;

	

	//Make x right handed
	//m_rayCamera.target.x = 0.0f - m_rayCamera.target.x; currently not needed
	m_rayCamera.up.x = 0.0f - m_rayCamera.up.x;
}


void CameraWrapper3D::LookAtPos(Vector3& targetIn, Vector3& upIn)
{
	//save Vector for later
	m_forward = Vector3Subtract(targetIn, m_rayCamera.position);

	m_rayCamera.target = targetIn;
	m_rayCamera.up = upIn;

	//Make x right handed
	m_rayCamera.target.x = 0.0f - m_rayCamera.target.x;
	m_rayCamera.up.x = 0.0f - m_rayCamera.up.x;
}

void CameraWrapper3D::SetCameraPosition(Vector3& posIn)
{ 
	//convert to right handed system
	m_rayCamera.position = posIn; 
	m_rayCamera.position.x = 0.0f - m_rayCamera.position.x;

	//update target to move with camera
	m_rayCamera.target = Vector3Add(m_forward, m_rayCamera.position);
}


void CameraWrapper3D::Pitch(float& angle)
{
	RotateAxisAngle(angle, m_left);
}


void CameraWrapper3D::Yaw(float& angle)
{
	RotateAxisAngle(angle, m_rayCamera.up);
}


void CameraWrapper3D::Roll(float& angle)
{
	RotateAxisAngle(angle, m_forward);
}


void CameraWrapper3D::RotateAxisAngle(float& angle, const Vector3& axis)
{
	Vector3 upT = m_rayCamera.up;

	Vector3 normAxis = Vector3Normalize(axis);
	float x = normAxis.x;
	float y = normAxis.y;
	float z = normAxis.z;

	float s = sinf(angle);
	float c = cosf(angle);
	float nc = 1.0f - c;

	Vector3 newLeft;
	Vector3 newForward;
	Vector3 newUp;

	Matrix R1;

	R1.m0 = c + (x * x * nc);		R1.m4 = (x * y * nc) - (z * s);	R1.m8 = (x * z * nc) + (y * s);		R1.m12 = 0.0f;
	R1.m1 = (y * x * nc) + (z * s);	R1.m5 = c + (y * y * nc);		R1.m9 = (y * z * nc) - (x * s);		R1.m13 = 0.0f;
	R1.m2 = (z * x * nc) - (y * s);	R1.m6 = (z * y * nc) + (x * s);	R1.m10 = c + (z * z * nc);			R1.m14 = 0.0f;
	R1.m3 = 0.0f;					R1.m7 = 0.0f;					R1.m12 = 0.0f;						R1.m15 = 1.0f;

	Matrix R2;

	R2.m0 = m_left.x;		R2.m4 = m_left.y;		R2.m8 = m_left.z;		R2.m12 = 0.0f;
	R2.m1 = m_forward.x;	R2.m5 = m_forward.y;	R2.m9 = m_forward.z;	R2.m13 = 0.0f;
	R2.m2 = upT.x;			R2.m6 = upT.y;			R2.m10 = upT.z;			R2.m14 = 0.0f;
	R2.m3 = 0.0f;			R2.m7 = 0.0f;			R2.m11 = 0.0f;			R2.m15 = 1.0f;

	Matrix R3 = MatrixMultiply(R1, R2);

	newLeft.x = R3.m0;
	newLeft.y = R3.m4;
	newLeft.z = R3.m8;

	newForward.x = R3.m1;
	newForward.y = R3.m5;
	newForward.z = R3.m9;

	newUp.x = R3.m2;
	newUp.y = R3.m6;
	newUp.z = R3.m10;
	
	m_left = newLeft;
	m_rayCamera.target = Vector3Add(newForward, m_rayCamera.position);
	m_rayCamera.up = newUp;

	//for forward preservation.
	m_forward = newForward;
}

//saving for later

//void Transformation::LookAtLerp(Vector3& targetDir, Vector3& desiredUp, float deltaPercent)
//{
//	Vector3 normTargetDr;
//	normTargetDr = Vector3Normalize(targetDir);
//	//normTargetDr.x = -normTargetDr.x;
//
//	Vector3 normDesiredUp;
//
//	Vector3 reversedForward;
//	reversedForward = Vector3Negate(m_forward);
//
//	Vector3 lerpedTarget;
//	lerpedTarget.x = ((normTargetDr.x - reversedForward.x) * deltaPercent) + reversedForward.x;
//	lerpedTarget.y = ((normTargetDr.y - reversedForward.y) * deltaPercent) + reversedForward.y;
//	lerpedTarget.z = ((normTargetDr.z - reversedForward.z) * deltaPercent) + reversedForward.z;
//
//	std::cout << "Forward: " << m_forward.x << " " << m_forward.y << " " << m_forward.z << std::endl;
//	std::cout << "target: " << normTargetDr.x << " " << normTargetDr.y << " " << normTargetDr.z << std::endl;
//
//	LookAt(lerpedTarget, desiredUp);
//}
//
//
//void Transformation::LookAtPosLerp(Vector3& targetPos, Vector3& desiredUp, float deltaPercent)
//{
//	Vector3 moddedTarget;
//	moddedTarget.x = targetPos.x - m_position.x;
//	moddedTarget.y = targetPos.y - m_position.y;
//	moddedTarget.z = targetPos.z - m_position.z;
//
//	moddedTarget = Vector3Normalize(moddedTarget);
//
//	Vector3 reversedForward;
//	reversedForward = Vector3Negate(m_forward);
//
//	Vector3 lerpedTarget;
//	lerpedTarget.x = ((moddedTarget.x - reversedForward.x) * deltaPercent) + reversedForward.x;
//	lerpedTarget.y = ((moddedTarget.y - reversedForward.y) * deltaPercent) + reversedForward.y;
//	lerpedTarget.z = ((moddedTarget.z - reversedForward.z) * deltaPercent) + reversedForward.z;
//
//	std::cout << "Forward: " << lerpedTarget.x << " " << lerpedTarget.y << " " << lerpedTarget.z << std::endl;
//
//	if (Vector3LengthSqr(lerpedTarget) > 0.0001)
//	{
//		LookAt(lerpedTarget, desiredUp);
//	}
//}