#pragma once
#include <memory>

namespace ApproxCommon
{
	namespace Memory
	{
		template<class T>
		void* ZeroMem(T* object)
		{
			return memset(object, 0, sizeof(*object));
		}

		template<class T>
		void* ZeroMem(T object)
		{
			return memset(&object, 0, sizeof(object));
		}

		class CopyDataOwnershipPolicy
		{
		protected:
			template <typename T1, typename T2>
			static inline void Copy(T1* &ToData, size_t _size, const T2* FromData)
			{
				if (ToData)
				{					
					if (FromData)
					{
						if (memcmp(ToData, FromData, _size) == 0)
							return;
					}
					delete ToData;
				}

				ToData = malloc(_size);
				memcpy(ToData, FromData, _size);
			}

			template <class T>
			static inline void Copy(T &ToData, const T &FromData)
			{
				memcpy(&ToData, &FromData, sizeof(T));
			}

			template <class T1, class T2>
			static inline void Copy(T1* &ToData, const T2 &FromData)
			{
				if (ToData)
				{
					if (memcmp(ToData, &FromData, sizeof(T2)) == 0)
						return;

					delete ToData;
				}

				ToData = malloc(sizeof(T2));
				memcpy(ToData, &FromData, sizeof(T2));
			}
			static inline void Delete(void *&Data)
			{
				if (Data)
				{
					free(Data);
					Data = nullptr;
				}
			}
		};
		class MoveDataOwnershipPolicy
		{
		protected:			
			static inline void Copy(void* &Data, size_t, void* &pData)
			{
				Data = pData;
				pData = nullptr;
			}

			static inline void Delete(void *&Data)
			{
				free(Data);
				Data = nullptr;
			}
		};
	}
}