#include "BaseObject3D.h"

//Deafult constructor to initialize class
BaseObject3D::BaseObject3D(std::string& fileName)
{
	m_postitionVec.x = 0.0f;
	m_postitionVec.y = 0.0f;
	m_postitionVec.z = 0.0f;

	m_rotationQuat.x = 0.0f;
	m_rotationQuat.y = 0.0f;
	m_rotationQuat.z = 0.0f;
	m_rotationQuat.w = 0.0f;

	LoadModelFromFile(fileName);

	Texture texture = LoadTexture("C:/Users/halla10/Documents/3 CODING/tempresources/MissingTexture.png");

	m_objModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

//Draw object on screen
void BaseObject3D::Render()
{
	DrawModel(m_objModel, m_postitionVec, 1.0f, WHITE);
}

//Function that contains functionality of object. Intended to be overloaded by inherited classes
void BaseObject3D::Update()
{
	
}


void BaseObject3D::LoadModelFromFile(std::string& fileName)
{
	//raylib file functions require char*
	char* fileNameArr = const_cast<char*>(fileName.c_str());

	m_objModel = LoadModel(fileNameArr);
}