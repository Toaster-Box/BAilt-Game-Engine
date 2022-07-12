#include "Transformation.h"

Transformation::Transformation()
{
	LookAt(m_forward, m_up);
}


Transformation::~Transformation()
{

}

void Transformation::Pitch(float& angle)
{
	RotateAxisAngle(angle, m_right);
}


void Transformation::Yaw(float& angle)
{
	RotateAxisAngle(angle, m_up);
}


void Transformation::Roll(float& angle)
{
	RotateAxisAngle(angle, m_forward);
}


void Transformation::RotateAxisAngle(float& angle, const Vector3& axis)
{
}

void Transformation::LookAt(Vector3& targetDir, Vector3& desiredUp)
{
	Vector3	negforward = Vector3Negate(targetDir);
	Vector3 right = Vector3CrossProduct(desiredUp, negforward);
	Vector3 NewUp = Vector3CrossProduct(negforward, right);

	m_forward = targetDir;
	m_right = right;
	m_up = NewUp;
}


void Transformation::LookAtPos(Vector3& targetPos, Vector3& desiredUp)
{
	//vector3subtraction is a bad function
	Vector3 moddedTarget;
	moddedTarget.x = targetPos.x - m_position.x;
	moddedTarget.y = targetPos.y - m_position.y;
	moddedTarget.z = targetPos.z - m_position.z;

	if (Vector3LengthSqr(moddedTarget) > 0.0001)
	{
		LookAt(moddedTarget, desiredUp);
	}
	else
	{
		LookAt(targetPos, desiredUp);
	}
}


void Transformation::UpdateTransformation(Matrix* transformationMat_ptr) 
{
	Matrix R;

	m_forward = Vector3Normalize(m_forward);
	m_right = Vector3Normalize(m_right);
	m_up = Vector3Normalize(m_up);

	R.m0 = m_right.x;	R.m4 = m_right.y;	R.m8 = m_right.z;	R.m12 = 0.0f;
	R.m1 = m_forward.x;	R.m5 = m_forward.y;	R.m9 = m_forward.z;	R.m13 = 0.0f;
	R.m2 = m_up.x;		R.m6 = m_up.y;		R.m10 = m_up.z;		R.m14 = 0.0f;
	R.m3 = 0.0f;		R.m7 = 0.0f;		R.m11 = 0.0f;		R.m15 = 1.0f;

	Matrix T;

	T.m0 = 1.0f;					T.m4 = 0.0f;					T.m8 = 0.0f;					T.m12 = -m_position.x;
	T.m1 = 0.0f;					T.m5 = 1.0f;					T.m9 = 0.0f;					T.m13 = m_position.y;
	T.m2 = 0.0f;					T.m6 = 0.0f;					T.m10 = 1.0f;					T.m14 = m_position.z;
	T.m3 = 0.0f;					T.m7 = 0.0f;					T.m11 = 0.0f;					T.m15 = 1.0f;

	Matrix S;

	S.m0 = 1.0f / m_scale.x;	S.m4 = 0.0f;				S.m8 = 0.0f;				S.m12 = 0.0f;
	S.m1 = 0.0f;				S.m5 = 1.0f / m_scale.y;	S.m9 = 0.0f;				S.m13 = 0.0f;
	S.m2 = 0.0f;				S.m6 = 0.0f;				S.m10 = 1.0f / m_scale.z;	S.m14 = 0.0f;
	S.m3 = 0.0f;				S.m7 = 0.0f;				S.m11 = 0.0f;				S.m15 = 1.0f;

	Matrix TRS = MatrixMultiply(MatrixMultiply(S, R), T);

	*transformationMat_ptr = TRS;
}