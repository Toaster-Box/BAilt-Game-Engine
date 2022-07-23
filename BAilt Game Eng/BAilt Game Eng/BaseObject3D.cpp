#include "BaseObject3D.h"

//Default constructor to initialize class
BaseObject3D::BaseObject3D(unsigned int objIndexIn, unsigned int modelIndexIn)
{
	m_objectIndex = objIndexIn;
	m_modelIndex = modelIndexIn;

	for  (int i = 0; i < 11;  i++)
	{
		m_materialIndicies.push_back(0);
	}
}

//Function that contains functionality of object. Intended to be overloaded by inherited classes
void BaseObject3D::Update()
{

}
