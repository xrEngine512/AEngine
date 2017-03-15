#pragma once
#include <memory>
#include <cstring>

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

		template<class T, size_t N>
		void* ZeroMemStatic(T (&object)[N])
		{
			return memset(object, 0, sizeof(T)*N);
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
					free(ToData);
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

					free(ToData);
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

		template<template<class Y, class Deleter> class Ptr_type, class Obj_type, typename Deleter>
		inline Ptr_type<Obj_type, Deleter> manage(Obj_type* object_ptr, Deleter deleter){
            return Ptr_type<Obj_type, Deleter>(object_ptr, deleter);
        };
	}
}