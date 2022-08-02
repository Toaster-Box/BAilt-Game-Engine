#pragma once

#include "CameraWrapper3D.h"
#include "Transformation.h"
#include "ObjectHandler3D.h"
#include "GBuffer.h"
#include "LBuffer.h"

#include "raylib.h"
#include "rlgl.h"
namespace
{
	class GraphicsHandler3D
	{
	public:
		GraphicsHandler3D(ObjectHandler3D* ObjHandler3DIn_ptr, ConfigLoader* ConfigLoaderIn_ptr);

		CameraWrapper3D* GetCameraWrapperPTR() { return &m_SceneCamera; }

		void GeometryPass();
		void LightPass();
		void PostProcessingPass();
		void DrawFrameToScreen();

	private:

		ConfigLoader* m_ConfigLoader_ptr;
		ObjectHandler3D* m_ObjectHandler3D_ptr;

		CameraWrapper3D m_SceneCamera;

		GBuffer* m_GBuffer_ptr;
		LBuffer* m_LBuffer_ptr;

		Shader m_GeometryPassShader;
	};
}
///Default constructor to initialize class
GraphicsHandler3D::GraphicsHandler3D(ObjectHandler3D* ObjHandler3DIn_ptr, ConfigLoader* ConfigLoaderIn_ptr)
{
	m_ConfigLoader_ptr = ConfigLoaderIn_ptr;

	m_ObjectHandler3D_ptr = ObjHandler3DIn_ptr;

	//Init camera
	m_SceneCamera.SetFOV(*m_ConfigLoader_ptr->GetCameraFOV());
	m_SceneCamera.SetProjection(CAMERA_PERSPECTIVE);

	//Init buffers
	m_GBuffer_ptr = new GBuffer(*m_ConfigLoader_ptr->GetConfigScreenWidth(), *m_ConfigLoader_ptr->GetConfigScreenHeight());
	m_LBuffer_ptr = new LBuffer(m_GBuffer_ptr);

	//Init Shader
	m_GeometryPassShader = LoadShader("D:/GBufferShader.vs", "D:/GBufferShader.fs");
	m_GeometryPassShader.locs[SHADER_LOC_MAP_NORMAL] = rlGetLocationUniform(m_GeometryPassShader.id, "normalTex");
	m_GeometryPassShader.locs[SHADER_LOC_MAP_ROUGHNESS] = rlGetLocationUniform(m_GeometryPassShader.id, "roughnessTex");
	m_GeometryPassShader.locs[SHADER_LOC_MAP_EMISSION] = rlGetLocationUniform(m_GeometryPassShader.id, "emissiveTex");

	m_ObjectHandler3D_ptr->SetGeometryPassShaderPTR(m_GeometryPassShader);
}

//Main function which gets run every frame
void GraphicsHandler3D::GeometryPass()
{
	m_ObjectHandler3D_ptr->UpdateAllTransforms();

	m_GBuffer_ptr->BindForWriting();

	BeginMode3D(*m_SceneCamera.GetCameraPTR());

	m_ObjectHandler3D_ptr->Render(true, false);

	EndMode3D();

	m_GBuffer_ptr->BindForReading();
}

void GraphicsHandler3D::LightPass()
{
	m_LBuffer_ptr->UpdateLBuffer(*m_SceneCamera.GetCameraPTR());
}


void GraphicsHandler3D::PostProcessingPass()
{

}


void GraphicsHandler3D::DrawFrameToScreen()
{
	BeginDrawing();

	DrawTexture(m_LBuffer_ptr->GetFinalIlluminationBuffer(), 0, 0, WHITE);

	EndDrawing();

}
