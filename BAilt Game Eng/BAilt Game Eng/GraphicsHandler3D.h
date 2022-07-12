#pragma once

#include "BaseObject3D.h"
#include "ObjectHandler3D.h"

#include "raylib.h"

class GraphicsHandler3D
{
public:
	GraphicsHandler3D(ObjectHandler3D* ObjHandler3DIn_ptr, ConfigLoader* ConfigLoaderIn_ptr);

	void Update();

private:

	ConfigLoader* m_ConfigLoader_ptr;

	ObjectHandler3D* m_ObjectHandler3D_ptr;

	Camera3D m_SceneCamera;
};

///Default constructor to initialize class
GraphicsHandler3D::GraphicsHandler3D(ObjectHandler3D* ObjHandler3DIn_ptr, ConfigLoader* ConfigLoaderIn_ptr)
{
	m_ConfigLoader_ptr = ConfigLoaderIn_ptr;

	m_ObjectHandler3D_ptr = ObjHandler3DIn_ptr;

	//Init Camera

	m_SceneCamera.position.x = 0.0f;
	m_SceneCamera.position.y = 0.0f;
	m_SceneCamera.position.z = 5.0f;

	m_SceneCamera.target.x = 0.0f;
	m_SceneCamera.target.y = 0.0f;
	m_SceneCamera.target.z = -1.0f;

	m_SceneCamera.up.x = 0.0f;
	m_SceneCamera.up.y = 1.0f;
	m_SceneCamera.up.z = 0.0f;

	m_SceneCamera.fovy = *m_ConfigLoader_ptr->GetCameraFOV();

	m_SceneCamera.projection = CAMERA_PERSPECTIVE;
}

//Main function which gets run every frame
void GraphicsHandler3D::Update()
{
	if (IsKeyDown(KEY_UP))
	{
		m_SceneCamera.position.y += 0.01f;
		m_SceneCamera.target.y = m_SceneCamera.position.y;
		m_SceneCamera.target.z = -1.0f;
	}
	if (IsKeyDown(KEY_DOWN))
	{
		m_SceneCamera.position.y -= 0.01f;
		m_SceneCamera.target.y = m_SceneCamera.position.y;
		m_SceneCamera.target.z = -1.0f;
	}
	if (IsKeyDown(KEY_LEFT))
	{
		m_SceneCamera.position.x -= 0.01f;
		m_SceneCamera.target.x = m_SceneCamera.position.x;
		m_SceneCamera.target.z = -1.0f;
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		m_SceneCamera.position.x += 0.01f;
		m_SceneCamera.target.x = m_SceneCamera.position.x;
		m_SceneCamera.target.z = -1.0f;
	}

	BeginMode3D(m_SceneCamera);

	m_ObjectHandler3D_ptr->Render(true);

	EndMode3D();
}