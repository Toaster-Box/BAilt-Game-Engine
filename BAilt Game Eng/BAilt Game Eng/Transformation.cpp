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


void Transformation::RotateAxisAngle(float& angle, Vector3& axis)
{
	axis = Vector3Normalize(axis);
	Quaternion tempQuat = QuaternionFromAxisAngle(axis, angle);

	m_right = Vector3RotateByQuaternion(m_right, tempQuat);
	m_forward = Vector3RotateByQuaternion(m_forward, tempQuat);
	m_up = Vector3RotateByQuaternion(m_up, tempQuat);

	Quaternion newOrientation = QuaternionMultiply(tempQuat, m_orientation);

	m_orientation = newOrientation;

	//std::cout << "Forward: " << m_forward.x << " " << m_forward.y << " " << m_forward.z << std::endl;
}

void Transformation::LookAt(Vector3 targetDir, Vector3 desiredUp)
{
	targetDir = Vector3Normalize(Vector3Negate(targetDir));
	desiredUp = Vector3Normalize(desiredUp);
	targetDir.x = -targetDir.x;
	desiredUp.x = -desiredUp.x;

	Vector3 worldForward = { 0.0f, 1.0f, 0.0f };
	Vector3 worldUp = { 0.0f, 0.0f, 1.0f };
	Vector3 worldRight = { 1.0f, 0.0f, 0.0f };

	Quaternion rotQuat1 = QuaternionFromVector3ToVector3(worldForward, targetDir);
	rotQuat1 = QuaternionNormalize(rotQuat1);

	m_right = Vector3CrossProduct(targetDir, desiredUp);
	desiredUp = Vector3CrossProduct(m_right, targetDir);
	desiredUp = Vector3Normalize(desiredUp);

	Vector3 newUp = Vector3RotateByQuaternion(worldUp, rotQuat1);

	Quaternion rotQuat2 = QuaternionFromVector3ToVector3(newUp, desiredUp);
	rotQuat2 = QuaternionNormalize(rotQuat2);



	m_orientation = QuaternionMultiply(rotQuat2, rotQuat1);

	m_orientation = QuaternionNormalize(m_orientation);

	if (QuaternionLength(m_orientation) < 0.1)
	{
		m_orientation = { 0.0f, 0.0f, -1.0f, 0.0f };
	}

	m_forward = Vector3RotateByQuaternion(worldForward, m_orientation);
	m_up = Vector3RotateByQuaternion(worldUp, m_orientation);
	m_right = Vector3RotateByQuaternion(worldRight, m_orientation);
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


void Transformation::LookAtLerp(Vector3& targetDir, Vector3& desiredUp, float deltaPercent)
{
	Quaternion initialOrientation = m_orientation;

	LookAt(targetDir, desiredUp);

	Quaternion targetorientation = m_orientation;

	Quaternion lerpedTarget;
	lerpedTarget.x = ((targetorientation.x - initialOrientation.x) * deltaPercent) + initialOrientation.x;
	lerpedTarget.y = ((targetorientation.y - initialOrientation.y) * deltaPercent) + initialOrientation.y;
	lerpedTarget.z = ((targetorientation.z - initialOrientation.z) * deltaPercent) + initialOrientation.z;
	lerpedTarget.w = ((targetorientation.w - initialOrientation.w) * deltaPercent) + initialOrientation.w;

	m_orientation = QuaternionNormalize(lerpedTarget);
}


void Transformation::LookAtPosLerp(Vector3& targetPos, Vector3& desiredUp, float deltaPercent)
{
	Quaternion initialOrientation = m_orientation;

	LookAtPos(targetPos, desiredUp);

	Quaternion targetorientation = m_orientation;

	Quaternion lerpedTarget;
	lerpedTarget.x = ((targetorientation.x - initialOrientation.x) * deltaPercent) + initialOrientation.x;
	lerpedTarget.y = ((targetorientation.y - initialOrientation.y) * deltaPercent) + initialOrientation.y;
	lerpedTarget.z = ((targetorientation.z - initialOrientation.z) * deltaPercent) + initialOrientation.z;
	lerpedTarget.w = ((targetorientation.w - initialOrientation.w) * deltaPercent) + initialOrientation.w;

	m_orientation = QuaternionNormalize(lerpedTarget);
}


void Transformation::UpdateTransformation(Matrix* transformationMat_ptr) 
{
	m_right = Vector3Normalize(m_right);
	m_forward = Vector3Normalize(m_forward);
	m_up = Vector3Normalize(m_up);


	Matrix R = QuaternionToMatrix(m_orientation);
	Matrix T = MatrixTranslate(-m_position.x, m_position.y, m_position.z);
	Matrix S = MatrixScale(m_scale.x, m_scale.y, m_scale.z);

	Matrix TRS = MatrixMultiply(MatrixMultiply(S, R), T);

	*transformationMat_ptr = TRS;
}
