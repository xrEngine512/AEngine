#pragma once
#include "MaterialVariable.h"
#include "AlignedObject.h"
#include "IShader.h"

#include <ApproxMath.h>

#include <unordered_set>

using namespace Math;

namespace MatInterface
{
	struct SceneConstants : AlignedObject
	{
		MaterialVariable<AMatrix> projectionMatrix;
		MaterialVariable<AVector3> lightDirection;
		MaterialVariable<AVector> ambientColor;
		MaterialVariable<AVector> diffuseColor;

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
		MaterialVariable<AMatrix> viewMatrix;
		MaterialVariable<AVector3> cameraPos;
		PerFrame() : 
			viewMatrix(104, "View_Matrix",PER_FRAME, "View matrix describes camera properties"),
			cameraPos(105, "Camera_Pos",PER_FRAME)
		{			
		}
		void NotifyAll()
		{
			for (auto shader : m_Subscribers)
			{
				shader->UpdateSceneConstantsBuffers();
			}
		}
	private:
		std::unordered_set<ShaderSystem::IShader*> m_Subscribers;
	};
	struct PerObject : AlignedObject
	{
		MaterialVariable<AMatrix> modelMatrix;
		MaterialVariable<AMatrix> MVPMatrix;
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
		void CalculateMVPAfterTranspose()
		{
			perObjectData.MVPMatrix = sceneConstants.projectionMatrix * perFrameData.viewMatrix * perObjectData.modelMatrix;
		}
		void CalculateMVPBeforeTranspose()
		{
			perObjectData.MVPMatrix = perObjectData.modelMatrix * perFrameData.viewMatrix * sceneConstants.projectionMatrix;
		}
	};
}