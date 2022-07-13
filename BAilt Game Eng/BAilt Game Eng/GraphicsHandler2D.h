#pragma once

#include "BaseObject2D.h"
#include "ObjectHandler2D.h"

#include "raylib.h"

namespace
{
	class GraphicsHandler2D
	{
	public:
		GraphicsHandler2D(ObjectHandler2D& ObjHandler2DIn);

		void Update();

	private:

		ObjectHandler2D* m_ObjectHandler2D_ptr;
	};
}

//Deafult constructor to initialize class
GraphicsHandler2D::GraphicsHandler2D(ObjectHandler2D& ObjHandler2DIn)
{
	m_ObjectHandler2D_ptr = &ObjHandler2DIn;
}

//Main Function which gets run every frame
void GraphicsHandler2D::Update()
{

}