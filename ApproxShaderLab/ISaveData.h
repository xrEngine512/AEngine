#pragma once

namespace ASL
{
	struct ShaderParamInfo;
	struct TextureInfo;
	struct RuntimeBufferInfo;

	struct ISaveData 
	{
		void CleanSerializedBuffer()
		{
			delete[] serializedBuf;
			serializedBuf = nullptr;
		}

		virtual void* Serialize(int& size) = 0;

		virtual void Deserialize(void* buf, size_t size) = 0;

		virtual ShaderParamInfo* ToShaderParameterInfo()
		{
			return nullptr;
		}
		virtual TextureInfo* ToTextureInfo()
		{
			return nullptr;
		}
		virtual RuntimeBufferInfo* ToRuntimeBufferInfo()
		{
			return nullptr;
		}
		~ISaveData()
		{
		}
	protected:
					
		char* serializedBuf = nullptr;
	};
}