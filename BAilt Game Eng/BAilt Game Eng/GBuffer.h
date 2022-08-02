#pragma once

#include "raylib.h"
#include "rlgl.h"

struct GBufferData
{
	Texture WorldPos;
	Texture Depth;
	Texture Normal;
	Texture Albedo;
	Texture Metalness;
	Texture Roughness;
	Texture Emissive;
	unsigned int screenWidth;
	unsigned int screenHeight;
	unsigned int bufferId;
};


class GBuffer
{
public:
	GBuffer(int screenWidth, int screenHeight);

	Texture GetWorldPosTex() { return m_GBufferData.WorldPos; }
	Texture GetDepthTex() { return m_GBufferData.Depth; }
	Texture GetNormalTex() { return m_GBufferData.Normal; }
	Texture GetAlbedoTex() { return m_GBufferData.Albedo; }
	Texture GetMetalnessTex() { return m_GBufferData.Metalness; }
	Texture GetRoughnessTex() { return m_GBufferData.Roughness; }
	Texture GetEmissiveTex() { return m_GBufferData.Emissive; }

	unsigned int GetWorldPosTexID() { return m_GBufferData.WorldPos.id; }
	unsigned int GetDepthTexID() { return m_GBufferData.Depth.id; }
	unsigned int GetNormalTexID() { return m_GBufferData.Normal.id; }
	unsigned int GetAlbedoTexID() { return m_GBufferData.Albedo.id; }
	unsigned int GetMetalnessTexID() { return m_GBufferData.Metalness.id; }
	unsigned int GetRoughnessTexID() { return m_GBufferData.Roughness.id; }
	unsigned int GetEmissiveTexID() { return m_GBufferData.Emissive.id; }

	unsigned int GetBufferWidth() { return m_GBufferData.screenWidth; }
	unsigned int GetBufferHeight() { return m_GBufferData.screenHeight; }

	void BindForWriting();
	void BindForReading();

private:

	GBufferData m_GBufferData;
};

