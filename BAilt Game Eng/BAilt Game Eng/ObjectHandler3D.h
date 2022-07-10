#pragma once

#include <string>
#include <vector>

#include "BaseObject3D.h"

//I don't know why but the anonymous namspace prevents overdefinition linker errors
namespace
{
	class ObjectHandler3D
	{
	public:
		ObjectHandler3D();

		void Render(bool isStatic);
		void Update();

		unsigned int CreateObject(std::string& fileName);
		void DeleteObject();

		BaseObject3D* GetObjectPTR(unsigned int Index);

	private:
		std::vector<BaseObject3D*> m_ObjContainer3d;
	};
}


//Default constructor to initialize class
ObjectHandler3D::ObjectHandler3D()
{

}

//Function to draw all objects in cointainer. Primarily for use in GraphicsHandler3D
void ObjectHandler3D::Render(bool isStatic)
{
	for (unsigned int i = 0; i < m_ObjContainer3d.size(); i++)
	{
		//Only render if static/dynamic bools match
		if (isStatic == m_ObjContainer3d[i]->GetStaticStatus())
		{
			m_ObjContainer3d[i]->Render();
		}
	}
}

//Update funcionality of each object in container
void ObjectHandler3D::Update()
{
	for (unsigned int i = 0; i < m_ObjContainer3d.size(); i++)
	{
		//Only run update function if object has it enabled
		if (m_ObjContainer3d[i]->GetUpdateStatus())
		{
			m_ObjContainer3d[i]->Update();
		}
	}
}

//Create new object from parameters and add it to container. Returns pointer to object
unsigned int ObjectHandler3D::CreateObject(std::string& fileName)
{
	BaseObject3D* NewObj = new BaseObject3D(fileName, m_ObjContainer3d.size());

	m_ObjContainer3d.push_back(NewObj);
	
	return m_ObjContainer3d.back()->GetIndex();
}

//Delete object from conainter and free up memory
void ObjectHandler3D::DeleteObject()
{

}

BaseObject3D* ObjectHandler3D::GetObjectPTR(unsigned int Index)
{
	//Prevent access violations
	if ( Index <= m_ObjContainer3d.size())
	{
		return m_ObjContainer3d[Index];
	}
	else
	{
		return NULL;
	}
}