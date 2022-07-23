#pragma once

#include <string>
#include <vector>

#include "BaseObject3D.h"

#include "raylib.h"
//#include "rlgl.h"

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

		unsigned int AddTextureToContainer(std::string& fileName);
		void SetObjectTexture(BaseObject3D* objectIn_ptr, unsigned int textureIndex, int materialMapIndex);

		BaseObject3D* GetObjectPTR(unsigned int Index);

	private:
		void DrawObject3D(unsigned int objIndex, bool isStatic);

		std::vector<BaseObject3D*> m_ObjContainer3d;

		std::vector<Texture> m_TexContainer;
		std::vector<std::string> m_TexNameContainer;

		std::vector<Model*> m_ModelContainer;
		std::vector<std::string> m_ModelNameContainer;
		std::vector<unsigned int> m_numInstances;
	};
}


//Default constructor to initialize class
ObjectHandler3D::ObjectHandler3D()
{

}
//Function to draw all objects in cointainer. Primarily for use in GraphicsHandler3D
void ObjectHandler3D::Render(bool isStatic)
{
	//update all transforms before rendering to take advantage of processor preloading
	for (unsigned int i = 0; i < m_ObjContainer3d.size(); i++)
	{
		//Update transform of about to be rendered objects
		if (isStatic == m_ObjContainer3d[i]->GetStaticStatus())
		{
			m_ObjContainer3d[i]->UpdateTransform();
		}
	}

	for (unsigned int i = 0; i < m_ObjContainer3d.size(); i++)
	{
		DrawObject3D(i, isStatic);
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
	BaseObject3D* NewObj = new BaseObject3D(m_ObjContainer3d.size(), m_ModelContainer.size());

	Model* objModel;

	bool alreadyLoaded = false;
	unsigned int indexIfLoaded = NULL;

	for (unsigned int i = 0; i < m_ModelNameContainer.size(); i++)
	{
		if (fileName == m_ModelNameContainer[i])
		{
			indexIfLoaded = i;
			alreadyLoaded = true;
		}
	}

	if (alreadyLoaded)
	{
		std::cout << "Model: " << fileName << " is already loaded. Returning index to previously loaded model" << std::endl;

		objModel = m_ModelContainer[indexIfLoaded];
	}
	else
	{
		
		objModel = new Model;
		*objModel = LoadModel(const_cast<char*>(fileName.c_str()));
		m_ModelNameContainer.push_back(fileName);
	}

	m_ModelContainer.push_back(objModel);
	m_ObjContainer3d.push_back(NewObj);

	m_ModelContainer.back()->transform = *m_ObjContainer3d.back()->GetTransform();
	m_ModelContainer.back()->materials->maps[MATERIAL_MAP_ALBEDO].texture = m_TexContainer[0];
	
	return m_ObjContainer3d.back()->GetIndex();
}

//Delete object from conainter and free up memory
void ObjectHandler3D::DeleteObject()
{

}


unsigned int ObjectHandler3D::AddTextureToContainer(std::string& fileName)
{
	bool alreadyLoaded = false;
	unsigned int indexIfLoaded = NULL;

	for (unsigned int i = 0; i < m_TexContainer.size(); i++)
	{
		if (fileName == m_TexNameContainer[i])
		{
			indexIfLoaded = i;
			alreadyLoaded = true;
		}
	}

	if (alreadyLoaded)
	{
		std::cout << "Texture: " << fileName << " is already loaded. Returning index to previously loaded texture" << std::endl;
		return indexIfLoaded;
	}
	else
	{
		Texture tempTex = LoadTexture(const_cast<char*>(fileName.c_str()));

		m_TexContainer.push_back(tempTex);
		m_TexNameContainer.push_back(fileName);

		//returns the index to the texture in the container
		return m_TexContainer.size() - 1;
	}
}


void ObjectHandler3D::SetObjectTexture(BaseObject3D* objectIn_ptr, unsigned int textureIndex, int materialMapIndex)
{
	unsigned int safeTextureIndex;
	//Prevent access violations
	if (textureIndex <= m_TexContainer.size() - 1)
	{
		safeTextureIndex = textureIndex;
	}
	else
	{
		safeTextureIndex =  NULL;
	}

	int safeMaterialMapIndex;
	if (textureIndex <= MATERIAL_MAP_BRDF)
	{
		safeMaterialMapIndex = materialMapIndex;
	}
	else
	{
		safeMaterialMapIndex = MATERIAL_MAP_ALBEDO;
	}

	objectIn_ptr->SetTextureMapIndex(safeMaterialMapIndex, safeTextureIndex);

	m_ModelContainer[objectIn_ptr->GetModelIndex()]->materials->maps[safeMaterialMapIndex].texture = m_TexContainer[objectIn_ptr->GetTextureMapIndex(safeMaterialMapIndex)];
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


void ObjectHandler3D::DrawObject3D(unsigned int objIndex, bool isStatic)
{
	if (m_ObjContainer3d[objIndex]->GetStaticStatus() == isStatic)
	{
		m_ModelContainer[m_ObjContainer3d[objIndex]->GetModelIndex()]->transform = *m_ObjContainer3d[objIndex]->GetTransform();
		DrawModel(*m_ModelContainer[m_ObjContainer3d[objIndex]->GetModelIndex()], Vector3{ 0.0f }, 1.0f, WHITE);
	}
}