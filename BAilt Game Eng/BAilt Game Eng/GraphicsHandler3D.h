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
};

//Deafult constructor to initialize class
GraphicsHandler3D::GraphicsHandler3D(ObjectHandler3D* ObjHandler3DIn_ptr)
{
	m_ObjectHandler3D_ptr = ObjHandler3DIn_ptr;
}

//Main function which gets run every frame
void GraphicsHandler3D::Update()
{

}