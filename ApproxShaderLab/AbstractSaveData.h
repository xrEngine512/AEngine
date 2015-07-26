#pragma once

#include <ApproxSystemErrors.h>

namespace ASL
{
	struct ShaderParamInfo;
	struct TextureInfo;
	struct RuntimeBufferInfo;

	template<typename T>
	struct is_simple
		: std::_Cat_base<std::is_fundamental<T>::value || std::is_enum<T>::value>
	{
	};

	struct AbstractSaveData 
	{
		void CleanSerializedBuffer()
		{
			delete[] serializedBuf;
			serializedBuf = nullptr;
			m_size = 0;
		}
		AbstractSaveData(){}
		AbstractSaveData(const AbstractSaveData& obj)
		{
			if (this != &obj)
			{
				delete[] serializedBuf;
				serializedBuf = nullptr;
				if (obj.m_size)
				{
					serializedBuf = new char[obj.m_size];
					memcpy(serializedBuf, obj.serializedBuf, m_size);
				}
			}
		}
		virtual void const* Serialize(int& size) = 0;

		virtual void Deserialize(const void* buf, size_t size) = 0;

		virtual int SerializedSize()const = 0;

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
		virtual ~AbstractSaveData()
		{
			delete[] serializedBuf;
			serializedBuf = nullptr;
		}
	protected:
		template<class ...Ts>
		int Serialization(const Ts&... objects)
		{
			if (serializedBuf)
				delete[] serializedBuf;
			m_size = CalculateSize(objects...);
			serializedBuf = new char[m_size];
			
			ProcessSerialization(0, objects...);

			return m_size;
		}

		template<class ...Ts>
		void Deserialization(const void* buf, size_t size, Ts&... objects)
		{
			if (size)
			{
				int read = CalculateSize(buf, objects...);
				if (read > size)
					throw ApproxException(L"Ошибка десериализации");
				ProcessDeserialization(static_cast<const char*>(buf), objects...);
			}
			
		}
		template<class T, class ...Ts>
		inline int CalculateSize(const T& obj, const Ts&... objects)const
		{
			return reqSize(obj) + CalculateSize<Ts...>(objects...);
		}
		const char* SerializedBuf()const
		{
			return serializedBuf;
		}

	private:
		template<class T>
		inline void ProcessDeserialization(const char* buf, T& obj)
		{
			CopyD(buf, obj);
		}

		template<class T, class ...Ts>
		inline void ProcessDeserialization(const char* buf, T& obj, Ts&... objects)
		{
			ProcessDeserialization<Ts...>(buf + CopyD(buf, obj), objects...);
		}

		template<class T>
		inline void ProcessSerialization(int shift, const T& obj)
		{
			CopyS(shift, obj);
		}

		template<class T, class ...Ts>
		inline void ProcessSerialization(int shift, const T& obj, const Ts&... objects)
		{
			shift += CopyS(shift, obj);
			ProcessSerialization<Ts...>(shift, objects...);
		}

		//Copy Serialization methods begin
		template<typename saveData>
		inline typename std::enable_if<std::is_base_of<AbstractSaveData, saveData>::value, int>::type CopyS(int shift, saveData& obj)
		{
			AbstractSaveData* data = dynamic_cast<AbstractSaveData*>(&obj);
			int size;
			auto ptr = data->Serialize(size);
			size += sizeof(int);
			memcpy(serializedBuf + shift, &size, sizeof(int));
			memcpy(serializedBuf + shift + sizeof(int), ptr, size - sizeof(int));
			data->CleanSerializedBuffer();
			return size;
		}

		template<typename T>
		inline typename std::enable_if<is_simple<T>::value,int>::type CopyS(int shift, const T& obj)
		{
			memcpy(serializedBuf + shift, &obj, sizeof(T));
			return sizeof(T);
		}

		inline int CopyS(int shift, const std::string& obj)
		{
			int size = reqSize(obj);
			memcpy(serializedBuf + shift, &size, sizeof(int));
			memcpy(serializedBuf + shift + sizeof(int), obj.c_str(), size - sizeof(int));
			return size;
		}

		inline int CopyS(int shift, const std::pair<const void*, int>& obj)
		{
			memcpy(serializedBuf + shift, &obj.second, sizeof(int));
			memcpy(serializedBuf + shift + sizeof(int), obj.first, obj.second);
			return obj.second + sizeof(int);
		}

		template<class T>
		inline typename std::enable_if<is_simple<T>::value, int>::type CopyS(int shift, const std::vector<T>& obj)
		{
			int size = reqSize(obj);
			memcpy(serializedBuf + shift, &size, sizeof(int));
			memcpy(serializedBuf + shift + sizeof(int), obj.data(), size - sizeof(int));
			return size;
		}
		template<class T>
		inline typename std::enable_if<!is_simple<T>::value, int>::type CopyS(int shift, const std::vector<T>& objects)
		{
			int size = reqSize(objects);
			memcpy(serializedBuf + shift, &size, sizeof(int));
			for (auto obj : objects)
			{
				shift += CopyS(shift + sizeof(int), obj);
			}
			return size;
		}

		//Copy Serialization methods end

		//Copy Deserialization methods begin
		template<class T>
		inline typename std::enable_if<is_simple<T>::value, int>::type CopyD(const void* buf, T& obj)
		{
			memcpy(&obj, buf, sizeof(T));
			return sizeof(T);
		}

		inline int CopyD(const void* buf, std::string& obj)
		{
			int size = *static_cast<const int*>(buf) - sizeof(int);
			obj.reserve(size);
			obj.assign(size, '0');
			//obj.copy(static_cast<char*>(buf) + sizeof(int),size);
			memcpy(&obj[0], static_cast<const char*>(buf)+sizeof(int), size);
			return size + sizeof(int);
		}

		template<typename T>
		inline typename std::enable_if<is_simple<T>::value, int>::type CopyD(const void* buf, std::vector<T>& obj)
		{
			int size = *static_cast<const int*>(buf)-sizeof(int);
			obj.reserve(size / sizeof(T));
			obj.assign(obj.capacity(), 0);
			memcpy(obj.data(), static_cast<const char*>(buf) + sizeof(int) , size);
			return size + sizeof(int);
		}

		template<typename T>
		inline typename std::enable_if<!is_simple<T>::value, int>::type CopyD(const void* buf, std::vector<T>& objects)
		{
			int size = *static_cast<const int*>(buf);
			int remainedSize = size;
			while (remainedSize != 0)
			{
				T obj;
				remainedSize -= CopyD(static_cast<const char*>(buf)+sizeof(int) + size - remainedSize, obj);
				objects.push_back(obj);
				if (remainedSize < 0)
					throw ApproxException(L"Ошибка десериализации при работе с вектором.");
			}
			return size;
		}

		template<typename saveData>
		inline typename std::enable_if<std::is_base_of<AbstractSaveData, saveData>::value, int>::type CopyD(const void* buf, saveData& obj)
		{
			AbstractSaveData* data = dynamic_cast<AbstractSaveData*>(&obj);
			const int size = *static_cast<const int*>(buf);
			data->Deserialize(static_cast<const char*>(buf)+sizeof(int), size - sizeof(int));
			return size;
		}
		//Copy Deserialization methods end

		template<typename simpleType>
		inline typename std::enable_if<is_simple<simpleType>::value, int>::type reqSize(simpleType)const
		{
			return sizeof(simpleType);
		}
		
		template<typename simpleType>
		inline typename std::enable_if<is_simple<simpleType>::value, int>::type reqSize(const void*, simpleType)const
		{
			return sizeof(simpleType);
		}

		template<typename saveData>
		inline typename std::enable_if<std::is_base_of<AbstractSaveData, saveData>::value, int>::type reqSize(const saveData& Data)const
		{
			return Data.SerializedSize() + sizeof(int);
		}

		inline int reqSize(const std::string& obj)const
		{
			return obj.size() + sizeof(int);
		}

		template<class T>
		inline typename std::enable_if<is_simple<T>::value, int>::type reqSize(const std::vector<T>& obj)const
		{
			return obj.size() * sizeof(T) + sizeof(int);
		}

		template<class T>
		inline typename std::enable_if<!(is_simple<T>::value), int>::type reqSize(const std::vector<T>& objects)const
		{
			int res = 0;
			for (auto obj : objects)
			{
				res += reqSize(obj);
			}
			return res + sizeof(int);
		}

		inline int reqSize(const std::pair<const void*, int>& obj)const
		{
			return obj.second + sizeof(int);
		}

		template<typename notSimpleType>
		inline typename std::enable_if<!(is_simple<notSimpleType>::value), int>::type reqSize(const void* buf, const notSimpleType&)const
		{
			return *static_cast<const int*>(buf);
		}

		template<class T>
		inline int CalculateSize(const T& obj)const
		{
			return reqSize(obj);
		}
		

		template<class T>
		inline int CalculateSize(const void* buf, const T& obj)const
		{
			return reqSize(buf, obj);
		}
		template<class T, class ...Ts>
		inline int CalculateSize(const void* buf, const T& obj, const Ts&... objects)const
		{
			int shift = reqSize(buf, obj);
			return shift + CalculateSize<Ts...>(static_cast<const char*>(buf) + shift, objects...);
		}

		char* serializedBuf = nullptr;
		int m_size = 0;
	};
}