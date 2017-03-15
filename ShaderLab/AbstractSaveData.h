#pragma once

#include <ApproxSystemErrors.h>
#include <cstring>

namespace ASL
{
	struct ShaderParamInfo;
	struct TextureInfo;
	struct RuntimeBufferInfo;

	template<typename T>
	struct is_simple {
		constexpr static bool value = std::is_fundamental<T>::value || std::is_enum<T>::value;
	};

	struct AbstractSaveData 
	{
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
		virtual ~AbstractSaveData()
		{
			delete[] serializedBuf;
			serializedBuf = nullptr;
		}

#pragma region PublicInterface
		virtual void const* Serialize(int& size) = 0;

		virtual void Deserialize(const void* buf, size_t size) = 0;

		virtual int SerializedSize()const = 0;
		
		template<class ...Ts>
		static inline void const* Serialize(int& size, const Ts&... objects)
		{
			void* pData;
			size = Serialization(pData, objects...);
			return pData;
		}

		template<class ...Ts>
		static inline void Deserialize(const void* buf, int size, Ts&... objects)
		{
			return Deserialization(buf, size, objects...);
		}
		
		void CleanSerializedBuffer()
		{
			delete[] serializedBuf;
			serializedBuf = nullptr;
			m_size = 0;
		}
#pragma endregion

#pragma region ProtectedMethods
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
		static void Deserialization(const void* buf, size_t size, Ts&... objects)
		{
			if (size)
			{
				int read = CalculateSize(buf, objects...);
				if (read != size)
					throw ApproxException("Ошибка десериализации");
				ProcessDeserialization(static_cast<const char*>(buf), objects...);
			}
			
		}
		template<class T>
		static inline int CalculateSize(const T& obj)
		{
			return reqSize(obj);
		}
		template<class T, class ...Ts>
		static inline int CalculateSize(const T& obj, const Ts&... objects)
		{
			return reqSize(obj) + CalculateSize<Ts...>(objects...);
		}
		const char* SerializedBuf()const
		{
			return serializedBuf;
		}
#pragma endregion

#pragma region PrivateMethods
	private:
		template<class ...Ts>
		static int Serialization(void*& pData, const Ts&... objects)
		{
			auto size = CalculateSize(objects...);
			pData = new char[size];

			ProcessSerialization(static_cast<char*>(pData), objects...);

			return size;
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

		template<class T>
		static inline void ProcessDeserialization(const char* buf, T& obj)
		{
			CopyD(buf, obj);
		}

		template<class T, class ...Ts>
		static inline void ProcessDeserialization(const char* buf, T& obj, Ts&... objects)
		{
			ProcessDeserialization<Ts...>(buf + CopyD(buf, obj), objects...);
		}

		template<class T>
		static inline void ProcessSerialization(char* ptr, const T& obj)
		{
			CopyS(ptr, obj);
		}

		template<class T, class ...Ts>
		static inline void ProcessSerialization(char* ptr, const T& obj, const Ts&... objects)
		{
			ProcessSerialization<Ts...>(ptr + CopyS(ptr, obj), objects...);
		}

		//Copy Serialization methods begin
		template<class T>
		inline int CopyS(int shift, const T& obj)
		{
			return CopyS(serializedBuf + shift, obj);
		}

		template<typename saveData>
		static inline typename std::enable_if<std::is_base_of<AbstractSaveData, saveData>::value, int>::type CopyS(char* buf, saveData& obj)
		{
			AbstractSaveData* data = dynamic_cast<AbstractSaveData*>(&obj);
			int size;
			auto ptr = data->Serialize(size);
			size += sizeof(int);
			memcpy(buf, &size, sizeof(int));
			memcpy(buf + sizeof(int), ptr, size - sizeof(int));
			data->CleanSerializedBuffer();
			return size;
		}
		
		template<typename T>
		static inline typename std::enable_if<is_simple<T>::value, int>::type CopyS(char* buf, const T& obj)
		{
			memcpy(buf, &obj, sizeof(T));
			return sizeof(T);
		}
		
		static inline int CopyS(char *buf, const std::string& obj)
		{
			int size = reqSize(obj);
			memcpy(buf, &size, sizeof(int));
			memcpy(buf + sizeof(int), obj.c_str(), size - sizeof(int));
			return size;
		}

		static inline int CopyS(char *buf, const std::pair<const void*, int>& obj)
		{
			memcpy(buf, &obj.second, sizeof(int));
			memcpy(buf + sizeof(int), obj.first, obj.second);
			return obj.second + sizeof(int);
		}

		template<class T>
		static inline typename std::enable_if<is_simple<T>::value, int>::type CopyS(char *buf, const std::vector<T>& obj)
		{
			int size = reqSize(obj);
			memcpy(buf, &size, sizeof(int));
			memcpy(buf + sizeof(int), obj.data(), size - sizeof(int));
			return size;
		}
		template<class T>
		static inline typename std::enable_if<!is_simple<T>::value, int>::type CopyS(char *buf, const std::vector<T>& objects)
		{
			int size = reqSize(objects);
			memcpy(buf, &size, sizeof(int));
			int shift = 0;
			for (auto obj : objects)
				shift += CopyS(buf + shift + sizeof(int), obj);

			return size;
		}
		//Copy Serialization methods end

		//Copy Deserialization methods begin
		template<class T>
		static inline typename std::enable_if<is_simple<T>::value, int>::type CopyD(const void* buf, T& obj)
		{
			memcpy(&obj, buf, sizeof(T));
			return sizeof(T);
		}

		static inline int CopyD(const void* buf, std::string& obj)
		{
			int size = *static_cast<const int*>(buf) - sizeof(int);
			obj.reserve(size);
			obj.assign(size, '0');
			memcpy(&obj[0], static_cast<const char*>(buf)+sizeof(int), size);
			return size + sizeof(int);
		}

		template<typename T>
		static inline typename std::enable_if<is_simple<T>::value, int>::type CopyD(const void* buf, std::vector<T>& obj)
		{
			int size = *static_cast<const int*>(buf)-sizeof(int);
			obj.reserve(size / sizeof(T));
			obj.assign(obj.capacity(), 0);
			memcpy(obj.data(), static_cast<const char*>(buf) + sizeof(int) , size);
			return size + sizeof(int);
		}

		template<typename T>
		static inline typename std::enable_if<!is_simple<T>::value, int>::type CopyD(const void* buf, std::vector<T>& objects)
		{
			int size = *static_cast<const int*>(buf);
			int remainedSize = size - sizeof(int);
			while (remainedSize != 0)
			{
				T obj;
				remainedSize -= CopyD(static_cast<const char*>(buf) + size - remainedSize, obj);
				objects.push_back(obj);
				if (remainedSize < 0)
					throw ApproxException("Ошибка десериализации при работе с вектором.");
			}
			return size;
		}

		template<typename saveData>
		static inline typename std::enable_if<std::is_base_of<AbstractSaveData, saveData>::value, int>::type CopyD(const void* buf, saveData& obj)
		{
			AbstractSaveData* data = dynamic_cast<AbstractSaveData*>(&obj);
			const int size = *static_cast<const int*>(buf);
			data->Deserialize(static_cast<const char*>(buf)+sizeof(int), size - sizeof(int));
			return size;
		}
		//Copy Deserialization methods end

		template<typename simpleType>
		static inline typename std::enable_if<is_simple<simpleType>::value, int>::type reqSize(simpleType)
		{
			return sizeof(simpleType);
		}
		
		template<typename simpleType>
		static inline typename std::enable_if<is_simple<simpleType>::value, int>::type reqSize(const void*, simpleType)
		{
			return sizeof(simpleType);
		}

		template<typename saveData>
		static inline typename std::enable_if<std::is_base_of<AbstractSaveData, saveData>::value, int>::type reqSize(const saveData& Data)
		{
			return Data.SerializedSize() + sizeof(int);
		}

		static inline int reqSize(const std::string& obj)
		{
			return obj.size() + sizeof(int);
		}

		template<class T>
		static inline typename std::enable_if<is_simple<T>::value, int>::type reqSize(const std::vector<T>& obj)
		{
			return obj.size() * sizeof(T) + sizeof(int);
		}

		template<class T>
		static inline typename std::enable_if<!(is_simple<T>::value), int>::type reqSize(const std::vector<T>& objects)
		{
			int res = 0;
			for (auto obj : objects)
			{
				res += reqSize(obj);
			}
			return res + sizeof(int);
		}

		static inline int reqSize(const std::pair<const void*, int>& obj)
		{
			return obj.second + sizeof(int);
		}

		template<typename notSimpleType>
		static inline typename std::enable_if<!(is_simple<notSimpleType>::value), int>::type reqSize(const void* buf, const notSimpleType&)
		{
			return *static_cast<const int*>(buf);
		}

		template<class T>
		static inline int CalculateSize(const void* buf, const T& obj)
		{
			return reqSize(buf, obj);
		}
		template<class T, class ...Ts>
		static inline int CalculateSize(const void* buf, const T& obj, const Ts&... objects)
		{
			int shift = reqSize(buf, obj);
			return shift + CalculateSize<Ts...>(static_cast<const char*>(buf) + shift, objects...);
		}
#pragma endregion

		char*			serializedBuf = nullptr;
		int				m_size = 0;
	};
}