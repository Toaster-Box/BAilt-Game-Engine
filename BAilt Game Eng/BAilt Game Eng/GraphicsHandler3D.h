#pragma once

#include "CameraWrapper3D.h"
#include "BaseObject3D.h"
#include "ObjectHandler3D.h"

#include "raylib.h"
namespace
{
	class GraphicsHandler3D
	{
	public:
		GraphicsHandler3D(ObjectHandler3D* ObjHandler3DIn_ptr, ConfigLoader* ConfigLoaderIn_ptr);

		CameraWrapper3D* GetCameraWrapperPTR() { return &m_SceneCamera; }

		void Update();

	private:

		ConfigLoader* m_ConfigLoader_ptr;

		ObjectHandler3D* m_ObjectHandler3D_ptr;

		CameraWrapper3D m_SceneCamera;
	};
}
///Default constructor to initialize class
GraphicsHandler3D::GraphicsHandler3D(ObjectHandler3D* ObjHandler3DIn_ptr, ConfigLoader* ConfigLoaderIn_ptr)
{
	m_ConfigLoader_ptr = ConfigLoaderIn_ptr;

	m_ObjectHandler3D_ptr = ObjHandler3DIn_ptr;

	//Init Camera
	m_SceneCamera.SetFOV(*m_ConfigLoader_ptr->GetCameraFOV());
	m_SceneCamera.SetProjection(CAMERA_PERSPECTIVE);

	//Vector3 posVec = Vector3{ 0.0f, 0.0f, 5.0f };
	//Vector3 targetVec = Vector3{ 0.0f, 0.0f, -1.0f };
	//Vector3 upVec = Vector3{ 0.0f, 1.0f, 0.0f };
	//m_SceneCamera.SetCameraPosition(posVec);
	//m_SceneCamera.LookAt(targetVec, upVec);
}

//Main function which gets run every frame
void GraphicsHandler3D::Update()
{
	BeginMode3D(*m_SceneCamera.GetCameraPTR());

	m_ObjectHandler3D_ptr->Render(true);

	EndMode3D();
}