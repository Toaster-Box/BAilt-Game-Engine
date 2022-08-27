#pragma once

#include "raylib.h"

// Light Data Structures
struct BaseLight
{
	bool castsShadows = true;
	bool dynamicShadowUpdate = false;
	bool emitsPhotons = false;
	bool volumeteric = false;
	Vector3 color = { 1.0f, 1.0f, 1.0f };
	float ambientIntensity = 0.0;
	float diffuseIntensity = 1.0f;
};

struct DirectionalLight : BaseLight
{
	Vector3 direction = { 0.0f, 0.0f, -1.0f };
};

struct PointLight : BaseLight
{
	Vector3 position = { 0.0f, 0.0f, 0.0f };
	float constantAtt = 0.5f;
	float linearAtt = 0.0f;
	float exponentioalAtt = 0.05f;
};

struct SpotLight : PointLight
{
	Vector3 direction = { 0.0f, 0.0f, -1.0f };
	float innerCutOffAngle = DEG2RAD * 45.0f;
	float outerCutOffAngle = DEG2RAD * 50.0f;
};