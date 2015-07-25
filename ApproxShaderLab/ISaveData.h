#pragma once

#include <ApproxSystemErrors.h>

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

		virtual void const* Serialize(int& size) = 0;

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
		template<class ...Ts>
		int Serialization(const Ts&... objects)
		{
			if (serializedBuf)
				delete[] serializedBuf;
			int size = CalculateSize(objects...);
			serializedBuf = new char[size];
			
			ProcessSerialization(0, objects...);

			return size;
		}

		template<class ...Ts>
		void Deserialization(void* buf, size_t size, Ts&... objects)
		{
			int read = CalculateSize(buf, objects...);
			if (read > size)
				throw ApproxException(L"Ошибка десериализации");
			ProcessDeserialization(static_cast<char*>(buf), objects...);
			
		}

		const char* SerializedBuf()const
		{
			return serializedBuf;
		}

	private:
		template<class T>
		inline void ProcessDeserialization(char* buf, T& obj)
		{
			Copy(buf, obj);
		}

		template<class T, class ...Ts>
		inline void ProcessDeserialization(char* buf, T& obj, Ts&... objects)
		{
			ProcessDeserialization<Ts...>(buf + Copy(buf, obj), objects...);
		}

		template<class T>
		inline void ProcessSerialization(int shift, const T& obj)
		{
			Copy(shift, obj);
		}

		template<class T, class ...Ts>
		inline void ProcessSerialization(int shift, const T& obj, const Ts&... objects)
		{
			Copy(shift, obj);
			shift += reqSize(obj);
			ProcessSerialization<Ts...>(shift, objects...);
		}

		template<class T>
		inline int Copy(void* buf, T& obj)
		{
			memcpy(&obj, buf, sizeof(T));
			return sizeof(T);
		}

		template<>
		inline int Copy(void* buf, std::string& obj)
		{
			int size = *static_cast<int*>(buf);
			obj.reserve(size);
			obj.assign(size, '0');
			//obj.copy(static_cast<char*>(buf) + sizeof(int),size);
			memcpy(&obj[0], static_cast<char*>(buf)+sizeof(int), size);
			return size + sizeof(int);
		}

		template<class T>
		inline int Copy(void* buf, std::vector<T>& obj)
		{
			int size = *static_cast<int*>(buf);
			obj.reserve(size / sizeof(T));
			obj.assign(obj.capacity(), 0);
			memcpy(obj.data(), static_cast<char*>(buf) + sizeof(int) , size);
			return size + sizeof(int);
		}

		template<class T>
		inline void Copy(int shift, const T& obj)
		{
			memcpy(serializedBuf + shift, &obj, reqSize(obj));
		}

		template<>
		inline void Copy(int shift, const std::string& obj)
		{
			int size = obj.size();
			memcpy(serializedBuf + shift, &size, sizeof(int));
			memcpy(serializedBuf + shift + sizeof(int), obj.c_str(), reqSize(obj) - sizeof(int));
		}

		template<>
		inline void Copy(int shift, const std::pair<const void*, int>& obj)
		{
			memcpy(serializedBuf + shift, &obj.second, sizeof(int));
			memcpy(serializedBuf + shift + sizeof(int), obj.first, obj.second);
		}

		template<class T>
		inline void Copy(int shift, const std::vector<T>& obj)
		{
			int size = obj.size() * sizeof(int);
			memcpy(serializedBuf + shift, &size, sizeof(int));
			memcpy(serializedBuf + shift + sizeof(int), obj.data(), obj.size() * sizeof(T));
		}

		template<typename simpleType>
		inline typename std::enable_if<std::is_fundamental<simpleType>::value || std::is_enum<simpleType>::value, int>::type reqSize(simpleType)
		{
			return sizeof(simpleType);
		}

		template<typename simpleType>
		inline typename std::enable_if<std::is_fundamental<simpleType>::value || std::is_enum<simpleType>::value, int>::type reqSize(void* , simpleType)
		{
			return sizeof(simpleType);
		}

		inline int reqSize(const std::string& obj)
		{
			return obj.size() + sizeof(int);
		}

		template<class T>
		inline int reqSize(const std::vector<T>& obj)
		{
			return obj.size() * sizeof(T) + sizeof(int);
		}

		inline int reqSize(const std::pair<const void*, int>& obj)
		{
			return obj.second + sizeof(int);
		}

		template<typename notSimpleType>
		inline typename std::enable_if<!(std::is_fundamental<notSimpleType>::value || std::is_enum<notSimpleType>::value), int>::type reqSize(void* buf, notSimpleType&)
		{
			return *static_cast<int*>(buf) + sizeof(int);
		}

		template<class T>
		inline int CalculateSize(const T& obj)
		{
			return reqSize(obj);
		}
		template<class T, class ...Ts>
		inline int CalculateSize(const T& obj, const Ts&... objects)
		{
			return reqSize(obj) + CalculateSize<Ts...>(objects...);
		}

		template<class T>
		inline int CalculateSize(void* buf, const T& obj)
		{
			return reqSize(buf, obj);
		}
		template<class T, class ...Ts>
		inline int CalculateSize(void* buf, const T& obj, const Ts&... objects)
		{
			int shift = reqSize(buf, obj);
			return shift + CalculateSize<Ts...>(static_cast<char*>(buf) + shift, objects...);
		}

		char* serializedBuf = nullptr;
	};
}