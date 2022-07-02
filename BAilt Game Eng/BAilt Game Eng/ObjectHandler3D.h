#pragma once

#include <string>
#include <vector>

#include "BaseObject3D.h"

class ObjectHandler3D
{
public:
	ObjectHandler3D();

	void Render(bool isStatic);
	void Update();

	void CreateObject(std::string& fileName);
	void DeleteObject();

private:

	std::vector<BaseObject3D*> m_ObjContainer3d_ptr;
};

//Deafult constructor to initialize class
ObjectHandler3D::ObjectHandler3D()
{

}

//Function to draw all objects in cointainer. Primarily for use in GraphicsHandler3D
void ObjectHandler3D::Render(bool isStatic)
{
	for (unsigned int i = 0; i < m_ObjContainer3d_ptr.size(); i++)
	{
		//Only render if static/dynamic bools match
		if (isStatic == *m_ObjContainer3d_ptr[i]->GetStaticStatusPTR())
		{
			m_ObjContainer3d_ptr[i]->Render();
		}
	}
}

//Update funcionality of each object in container
void ObjectHandler3D::Update()
{
	for (unsigned int i = 0; i < m_ObjContainer3d_ptr.size(); i++)
	{
		//Only run update function if object has it enabled
		if ( *m_ObjContainer3d_ptr[i]->GetUpdateStatusPTR())
		{
			m_ObjContainer3d_ptr[i]->Update();
		}
	}
}

//Create new object from parameters and add it to container
void ObjectHandler3D::CreateObject(std::string& fileName)
{
	BaseObject3D* NewObj = new BaseObject3D(fileName);

	m_ObjContainer3d_ptr.push_back(NewObj);
}

//Delete object from conainter and free up memory
void ObjectHandler3D::DeleteObject()
{

}