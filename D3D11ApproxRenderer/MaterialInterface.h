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
			projectionMatrix(100, "Proj_Matrix",SCENE_CONSTANT, "this matrix handles calculations for perspective view"),
			lightDirection(101, "Light_Dir", SCENE_CONSTANT, "direction of light ray"),
			ambientColor(102, "Ambient_Color", SCENE_CONSTANT, "Color of ambient light"),
			diffuseColor(103, "Diffuse_Color", SCENE_CONSTANT, "Color of light")
		{
		}
	};
	struct PerFrame : AlignedObject
	{
		MaterialVariable<XMMATRIX> viewMatrix;
		MaterialVariable<XMFLOAT3> cameraPos;
		PerFrame() : 
			viewMatrix(104, "View_Matrix",PER_FRAME, "View matrix describes camera properties"),
			cameraPos(105, "Camera_Pos",PER_FRAME)
		{			
		}
	};
	struct PerObject : AlignedObject
	{
		MaterialVariable<XMMATRIX> modelMatrix;
		MaterialVariable<XMMATRIX> MVPMatrix;
		PerObject() :
			modelMatrix(106, "Model_Matrix", PER_OBJECT, "Describes current object's transformation"),
			MVPMatrix(108, "MVP_Matrix", PER_OBJECT,"Model-View-Projection Matrix")
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