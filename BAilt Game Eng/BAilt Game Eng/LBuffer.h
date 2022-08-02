#pragma once

#include <iostream>
#include <vector>

#include "GBuffer.h"
#include "CameraWrapper3D.h"

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

// Light Data Structures
struct BaseLight
{
	bool castsShadows = true;
	bool emitsPhotons = true;
	Vector3 color = {1.0f, 1.0f, 1.0f};
	float ambientIntensity = 0.3;
	float diffuseIntensity = 0.03f;
};

struct DirectionalLight : BaseLight
{
	Vector3 direction = { 0.5f, 0.0f, -1.0f };
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
	float cutOffAngle = DEG2RAD * 45.0f;
};

// Shader data structures
struct BaseLightShader
{
	Shader shader;
	int cameraPositionLoc;

	int colorLoc;
	int ambientIntensityLoc;
	int diffuseIntensityLoc;
};

struct DirectionalLightShader : BaseLightShader
{
	int directionLoc;
};

struct PointLightShader : BaseLightShader
{
	int positionLoc;
	int constantAttLoc;
	int linearAttLoc;
	int exponentialAttLoc;
};

struct SpotLightShader : PointLightShader
{
	int directionLoc;
	int cutOffAngleLoc;
};

// Buffer contents
struct LBufferData
{
	Texture DirectLighting;
	Texture IndirectLighting;
	Texture FinalIlluminatedScene;
	Texture Depth;
	unsigned int screenWidth;
	unsigned int screenHeight;
	unsigned int bufferId;
};


class LBuffer
{
public:
	LBuffer(GBuffer* GBufferIn_ptr);

	void UpdateLBuffer(Camera3D& cameraIn);

	Texture GetFinalIlluminationBuffer() { return m_LBufferData.DirectLighting; }

private:
	void CalcDirectLighting(Camera3D& cameraIn);
	void CalcIndirectLighting(Camera3D& cameraIn);

	void BindForWriting();
	void BindForReading();

	DirectionalLightShader m_DirectionalLightShader;
	PointLightShader m_PointLightShader;
	SpotLightShader m_SpotLightShader;

	GBuffer* m_GBuffer_ptr;

	LBufferData m_LBufferData;

	std::vector<DirectionalLight> m_DirectionalLightContainer;
	std::vector<PointLight> m_PointLightContainer;
	std::vector<SpotLight> m_SpotLightContainer;

	Material m_pointlLightShaderMat;
	Material m_spotLightShaderMat;

	Model m_isosphereModel;
	Model m_coneModel;
	Model m_planeModel;

	CameraWrapper3D m_dirLightCam;
};
