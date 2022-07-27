#pragma once

#include "CameraWrapper3D.h"
#include "Transformation.h"
#include "ObjectHandler3D.h"
#include "GBuffer.h"

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

		GBuffer* m_MainGBuffer_ptr;

		Shader m_GeometryPassShader;
	};
}
///Default constructor to initialize class
GraphicsHandler3D::GraphicsHandler3D(ObjectHandler3D* ObjHandler3DIn_ptr, ConfigLoader* ConfigLoaderIn_ptr)
{
	m_ConfigLoader_ptr = ConfigLoaderIn_ptr;

	m_ObjectHandler3D_ptr = ObjHandler3DIn_ptr;

	//Init Cameras
	m_SceneCamera.SetFOV(*m_ConfigLoader_ptr->GetCameraFOV());
	m_SceneCamera.SetProjection(CAMERA_PERSPECTIVE);

	//Init GBuffer
	m_MainGBuffer_ptr = new GBuffer(m_ConfigLoader_ptr);

	//Init Shaders
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

	m_MainGBuffer_ptr->BindForWriting();

	BeginMode3D(*m_SceneCamera.GetCameraPTR());

	m_ObjectHandler3D_ptr->Render(true, false);

	EndMode3D();

	m_MainGBuffer_ptr->BindForReading();
}

void GraphicsHandler3D::LightPass()
{

}


void GraphicsHandler3D::PostProcessingPass()
{

}


void GraphicsHandler3D::DrawFrameToScreen()
{
	BeginDrawing();

	DrawTexture(m_MainGBuffer_ptr->GetBuffferTex(), 0, 0, WHITE);

	EndDrawing();

}
