#pragma once

#include "BaseObject3D.h"
#include "ObjectHandler3D.h"

#include "raylib.h"

class GraphicsHandler3D
{
public:
	GraphicsHandler3D(ObjectHandler3D* ObjHandler3DIn_ptr);

	void Update();

private:

	ObjectHandler3D* m_ObjectHandler3D_ptr;

	Camera3D m_SceneCamera;
};

///Default constructor to initialize class
GraphicsHandler3D::GraphicsHandler3D(ObjectHandler3D* ObjHandler3DIn_ptr)
{
	m_ObjectHandler3D_ptr = ObjHandler3DIn_ptr;

	//Init Camera
	m_SceneCamera.position.x = 3.0f;
	m_SceneCamera.position.y = 0.0f;
	m_SceneCamera.position.z = 0.0f;

	m_SceneCamera.target.x = -1.0f;
	m_SceneCamera.target.y = 0.0f;
	m_SceneCamera.target.z = 0.0f;

	m_SceneCamera.up.x = 0.0f;
	m_SceneCamera.up.y = 0.0f;
	m_SceneCamera.up.z = 1.0f;

	m_SceneCamera.fovy = 75.0f;

	m_SceneCamera.projection = CAMERA_PERSPECTIVE;
}

//Main function which gets run every frame
void GraphicsHandler3D::Update()
{
	BeginMode3D(m_SceneCamera);

	m_ObjectHandler3D_ptr->Render(true);

	EndMode3D();
}