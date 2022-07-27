#pragma once

#include "ConfigLoader.h"

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
	GBuffer(ConfigLoader* cfgLoaderIn_ptr);

	Texture GetBuffferTex() { return m_bufferData.Emissive; }

	void BindForWriting();
	void BindForReading();

private:

	ConfigLoader* m_ConfigLoader_ptr;

	GBufferData m_bufferData;
};

