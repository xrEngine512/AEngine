#pragma once
#include "MaterialVariable.h"
#include "AlignedObject.h"
#include <DirectXMath.h>

using namespace DirectX;
namespace MatInterface
{
	struct SceneConstants : AlignedObject
	{
		MaterialVariable<XMMATRIX> projectionMatrix;
		MaterialVariable<XMFLOAT3> lightDirection;
		MaterialVariable<XMFLOAT4> ambientColor;
		MaterialVariable<XMFLOAT4> diffuseColor;
		SceneConstants() : 
			projectionMatrix(100, "Proj_Matrix", "this matrix handles calculations for perspective view"),
			lightDirection(101, "Light_Dir", "direction of light ray"),
			ambientColor(102, "Ambient_Color","Color of ambient light"),
			diffuseColor(103, "Diffuse_Color","Color of light")
		{
		}
	};
	struct PerFrame : AlignedObject
	{
		MaterialVariable<XMMATRIX> viewMatrix;
		MaterialVariable<XMFLOAT3> cameraPos;
		PerFrame() : 
			viewMatrix(104, "View_Matrix", "View matrix describes camera properties"),
			cameraPos(105, "Camera_Pos")
		{			
		}
	};
	struct PerObject : AlignedObject
	{
		MaterialVariable<XMMATRIX> modelMatrix;
		MaterialVariable<XMMATRIX> MVPMatrix;
		PerObject() :
			modelMatrix(106, "Model_Matrix", "Describes current object's transformation"),
			MVPMatrix(108, "MVP_Matrix", "Model-View-Projection Matrix")
		{			
		}
	};

	struct MaterialInterface
	{
		SceneConstants	sceneConstants;
		PerFrame		perFrameData;
		PerObject		perObjectData;
	};
}