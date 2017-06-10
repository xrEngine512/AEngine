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
		virtual void const* Serialize(uint64_t& size) = 0;

		virtual void Deserialize(const void* buf, uint64_t size) = 0;

		virtual uint64_t SerializedSize()const = 0;
		
		template<class ...Ts>
		static inline void const* Serialize(uint64_t& size, const Ts&... objects)
		{
			void* pData;
			size = Serialization(pData, objects...);
			return pData;
		}

		template<class ...Ts>
		static inline void Deserialize(const void* buf, uint64_t size, Ts&... objects)
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
		uint64_t Serialization(const Ts&... objects)
		{
			if (serializedBuf)
				delete[] serializedBuf;
			m_size = CalculateSize(objects...);
			serializedBuf = new char[m_size];
			
			ProcessSerialization(uint64_t(0), objects...);

			return m_size;
		}

		template<class ...Ts>
		static void Deserialization(const void* buf, uint64_t size, Ts&... objects)
		{
			if (size)
			{
				auto read = CalculateSize(buf, objects...);
				if (read != size)
					throw approx_exception("Ошибка десериализации");
				ProcessDeserialization(static_cast<const char*>(buf), objects...);
			}
			
		}
		template<class T>
		static inline uint64_t CalculateSize(const T& obj)
		{
			return reqSize(obj);
		}
		template<class T, class ...Ts>
		static inline uint64_t CalculateSize(const T& obj, const Ts&... objects)
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
		static uint64_t Serialization(void*& pData, const Ts&... objects)
		{
			auto size = CalculateSize(objects...);
			pData = new char[size];

			ProcessSerialization(static_cast<char*>(pData), objects...);

			return size;
		}

		template<class T>
		inline void ProcessSerialization(uint64_t shift, const T& obj)
		{
			CopyS(shift, obj);
		}

		template<class T, class ...Ts>
		inline void ProcessSerialization(uint64_t shift, const T& obj, const Ts&... objects)
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
		inline uint64_t CopyS(uint64_t shift, const T& obj)
		{
			return CopyS(serializedBuf + shift, obj);
		}

		template<typename saveData>
		static inline typename std::enable_if<std::is_base_of<AbstractSaveData, saveData>::value, uint64_t>::type CopyS(char* buf, saveData& obj)
		{
			AbstractSaveData* data = dynamic_cast<AbstractSaveData*>(&obj);
			uint64_t size;
			auto ptr = data->Serialize(size);
			size += sizeof(uint64_t);
			memcpy(buf, &size, sizeof(uint64_t));
			memcpy(buf + sizeof(uint64_t), ptr, size - sizeof(uint64_t));
			data->CleanSerializedBuffer();
			return size;
		}
		
		template<typename T>
		static inline typename std::enable_if<is_simple<T>::value, uint64_t>::type CopyS(char* buf, const T& obj)
		{
			memcpy(buf, &obj, sizeof(T));
			return sizeof(T);
		}
		
		static inline uint64_t CopyS(char *buf, const std::string& obj)
		{
			uint64_t size = reqSize(obj);
			memcpy(buf, &size, sizeof(uint64_t));
			memcpy(buf + sizeof(uint64_t), obj.c_str(), size - sizeof(uint64_t));
			return size;
		}

		static inline uint64_t CopyS(char *buf, const std::pair<const void*, uint64_t>& obj)
		{
			memcpy(buf, &obj.second, sizeof(uint64_t));
			memcpy(buf + sizeof(uint64_t), obj.first, obj.second);
			return obj.second + sizeof(uint64_t);
		}

		template<class T>
		static inline typename std::enable_if<is_simple<T>::value, uint64_t>::type CopyS(char *buf, const std::vector<T>& obj)
		{
			uint64_t size = reqSize(obj);
			memcpy(buf, &size, sizeof(uint64_t));
			memcpy(buf + sizeof(uint64_t), obj.data(), size - sizeof(uint64_t));
			return size;
		}
		template<class T>
		static inline typename std::enable_if<!is_simple<T>::value, uint64_t>::type CopyS(char *buf, const std::vector<T>& objects)
		{
			uint64_t size = reqSize(objects);
			memcpy(buf, &size, sizeof(uint64_t));
			uint64_t shift = 0;
			for (auto obj : objects)
				shift += CopyS(buf + shift + sizeof(uint64_t), obj);

			return size;
		}
		//Copy Serialization methods end

		//Copy Deserialization methods begin
		template<class T>
		static inline typename std::enable_if<is_simple<T>::value, uint64_t>::type CopyD(const void* buf, T& obj)
		{
			memcpy(&obj, buf, sizeof(T));
			return sizeof(T);
		}

		static inline uint64_t CopyD(const void* buf, std::string& obj)
		{
			uint64_t size = *static_cast<const uint64_t*>(buf) - sizeof(uint64_t);
			obj.reserve(size);
			obj.assign(size, '0');
			memcpy(&obj[0], static_cast<const char*>(buf)+sizeof(uint64_t), size);
			return size + sizeof(uint64_t);
		}

		template<typename T>
		static inline typename std::enable_if<is_simple<T>::value, uint64_t>::type CopyD(const void* buf, std::vector<T>& obj)
		{
			uint64_t size = *static_cast<const uint64_t*>(buf)-sizeof(uint64_t);
			obj.reserve(size / sizeof(T));
			obj.assign(obj.capacity(), 0);
			memcpy(obj.data(), static_cast<const char*>(buf) + sizeof(uint64_t) , size);
			return size + sizeof(uint64_t);
		}

		template<typename T>
		static inline typename std::enable_if<!is_simple<T>::value, uint64_t>::type CopyD(const void* buf, std::vector<T>& objects)
		{
			uint64_t size = *static_cast<const uint64_t*>(buf);
			uint64_t remainedSize = size - sizeof(uint64_t);
			while (remainedSize != 0)
			{
				T obj;
				auto copied = CopyD(static_cast<const char*>(buf) + size - remainedSize, obj);
				objects.push_back(obj);
				if (remainedSize < copied)
					throw approx_exception("Ошибка десериализации при работе с вектором.");
				remainedSize -= copied;
			}
			return size;
		}

		template<typename saveData>
		static inline typename std::enable_if<std::is_base_of<AbstractSaveData, saveData>::value, uint64_t>::type CopyD(const void* buf, saveData& obj)
		{
			AbstractSaveData* data = dynamic_cast<AbstractSaveData*>(&obj);
			const uint64_t size = *static_cast<const uint64_t*>(buf);
			data->Deserialize(static_cast<const char*>(buf)+sizeof(uint64_t), size - sizeof(uint64_t));
			return size;
		}
		//Copy Deserialization methods end

		template<typename simpleType>
		static inline typename std::enable_if<is_simple<simpleType>::value, uint64_t>::type reqSize(simpleType)
		{
			return sizeof(simpleType);
		}
		
		template<typename simpleType>
		static inline typename std::enable_if<is_simple<simpleType>::value, uint64_t>::type reqSize(const void*, simpleType)
		{
			return sizeof(simpleType);
		}

		template<typename saveData>
		static inline typename std::enable_if<std::is_base_of<AbstractSaveData, saveData>::value, uint64_t>::type reqSize(const saveData& Data)
		{
			return Data.SerializedSize() + sizeof(uint64_t);
		}

		static inline uint64_t reqSize(const std::string& obj)
		{
			return obj.size() + sizeof(uint64_t);
		}

		template<class T>
		static inline typename std::enable_if<is_simple<T>::value, uint64_t>::type reqSize(const std::vector<T>& obj)
		{
			return obj.size() * sizeof(T) + sizeof(uint64_t);
		}

		template<class T>
		static inline typename std::enable_if<!(is_simple<T>::value), uint64_t>::type reqSize(const std::vector<T>& objects)
		{
			uint64_t res = 0;
			for (auto obj : objects)
			{
				res += reqSize(obj);
			}
			return res + sizeof(uint64_t);
		}

		static inline uint64_t reqSize(const std::pair<const void*, uint64_t>& obj)
		{
			return obj.second + sizeof(uint64_t);
		}

		template<typename notSimpleType>
		static inline typename std::enable_if<!(is_simple<notSimpleType>::value), uint64_t>::type reqSize(const void* buf, const notSimpleType&)
		{
			return *static_cast<const uint64_t*>(buf);
		}

		template<class T>
		static inline uint64_t CalculateSize(const void* buf, const T& obj)
		{
			return reqSize(buf, obj);
		}
		template<class T, class ...Ts>
		static inline uint64_t CalculateSize(const void* buf, const T& obj, const Ts&... objects)
		{
			uint64_t shift = reqSize(buf, obj);
			return shift + CalculateSize<Ts...>(static_cast<const char*>(buf) + shift, objects...);
		}
#pragma endregion

		char*			serializedBuf = nullptr;
		uint64_t				m_size = 0;
	};
}