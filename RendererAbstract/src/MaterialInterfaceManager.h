#pragma once
#include "MemoryMultiplexer.h"
#include "RendererCommonDefinitions.h"

#include <vector>
#include <string>

namespace MatInterface
{
	class MaterialInterfaceManager
	{
		std::vector <IMaterialVariable*> m_materialVariables;
		std::string m_ver;

		MaterialInterfaceManager();
		~MaterialInterfaceManager();
	public:
		
		void RegisterVariable(IMaterialVariable*);
		MaterialInterfaceInfo GetMaterialInterfaceInfo()const;
		std::vector<std::pair<void*, ShaderSystem::GenericType>> GetMaterialVariablesPtr()const;
		std::vector<std::pair<void*, ShaderSystem::GenericType>> GetMaterialVariablesPtr(const std::vector<int>& IDs)const;
		static ShaderSystem::GenericType NormalizeType(const std::string& type);
		static MaterialInterfaceManager* Instance();
	};

	extern MaterialInterfaceManager* g_materialInterfaceManager;
}