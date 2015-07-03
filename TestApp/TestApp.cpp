// TestApp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <immintrin.h>
#include <iostream>
#include <DirectXMath.h>

using namespace std;
using namespace DirectX;
#define WIN_32_LEAN_AND_MEAN
#include <Windows.h>
/*#include "../Engine/SystemThreadPool.h"
#include "../ApproxCommonSystem/FnPtr.h"

long long cycles =0 , cycles1 = 0, cycles2 = 0;

class ThreadTest
{
	ExitCode Thread1()
	{
		++cycles1;
		return RUNNING;
	}
	virtual ExitCode Thread2()
	{
		++cycles;
		return RUNNING;
	}
public:
	IFnPtr* Get()
	{
		return new FnPtr<ThreadTest>(&ThreadTest::Thread1, this);
	}
	IFnPtr* GetVirtual()
	{
		return new FnPtr<ThreadTest>(&ThreadTest::Thread2, this);
	}
};

void test1()
{
	cycles = 0;
	ThreadTest test;
	SystemThreadPool pool;
	pool.Initialize();
	pool.AddTask(test.Get());
	pool.Run();
	Sleep(5000);
	pool.Shutdown();
	cout << "Using ThreadPool for non virtual function-->" << cycles1 << " cycles" << endl;
	system("pause");
}

void test2()
{
	cycles = 0;
	ThreadTest test;
	SystemThreadPool pool;
	pool.Initialize();
	pool.AddTask(test.GetVirtual());
	pool.Run();
	Sleep(5000);
	pool.Shutdown();
	cout << "Using ThreadPool for virtual function-->" << cycles << " cycles" << endl;
	system("pause");
}

void test3()
{
	cycles = 0;
	volatile bool run = true;
	thread t([&]()
	{
		while (run)
		{
			cycles2++;
		}
	});
	Sleep(5000);
	run = false;
	if (t.joinable())
		t.join();
	cout << "Using std::thread for non virtual function-->" << cycles2 << " cycles" << endl;
}*/



__declspec(align(32)) struct AMMATRIX
{
	void* operator new(size_t size)
	{
		return _aligned_malloc(size, 32);
	}
		void operator delete(void* ptr)
	{
		_aligned_free(ptr);
	}
	void* operator new[](size_t size)
	{
		return _aligned_malloc(size, 32);
	}
		void operator delete[](void* ptr)
	{
		_aligned_free(ptr);
	}
	AMMATRIX() = default;
	AMMATRIX(__m256 half1, __m256 half2)
	{
		
		doubleRows[0] = half1;
		doubleRows[1] = half2;
	}
	AMMATRIX(float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24,
		float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44)
	{
		doubleRows[0] = _mm256_setr_ps(a11, a12, a13, a14, a21, a22, a23, a24);
		doubleRows[1] = _mm256_setr_ps(a31, a32, a33, a34, a41, a42, a43, a44);
	}
	union
	{
		struct
		{
			float a11, a12, a13, a14;
			float a21, a22, a23, a24;
			float a31, a32, a33, a34;
			float a41, a42, a43, a44;
		};
		float m[4][4];
		__m128 Rows[4];
		__m256 doubleRows[2];
	};

	#define _mm256_combine(u,d) _mm256_insertf128_ps(_mm256_castps128_ps256(u), d, 1)

	static inline __m256 twolincomb_AVX_8(__m256 A01, const AMMATRIX &B)
	{
		__m256 result; 
		result = _mm256_mul_ps(_mm256_broadcast_ps(&B.Rows[0]),_mm256_permute_ps(A01, 0x00));
		result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_permute_ps(A01, 0x55), _mm256_broadcast_ps(&B.Rows[1])));
		result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_permute_ps(A01, 0xaa), _mm256_broadcast_ps(&B.Rows[2])));
		result = _mm256_add_ps(result, _mm256_mul_ps(_mm256_permute_ps(A01, 0xff), _mm256_broadcast_ps(&B.Rows[3])));
		return result;
	}


	static AMMATRIX __vectorcall Multiply(const AMMATRIX &M1, const AMMATRIX &M2)
	{
		AMMATRIX MRes;

		/*__m256 Rows = M1.doubleRows[0];

		auto vX = _mm256_permute_ps(Rows, 0x00);
		auto vY = _mm256_permute_ps(Rows, 0x55);
		auto vZ = _mm256_permute_ps(Rows, 0xaa);
		auto vW = _mm256_permute_ps(Rows, 0xff); 

		vX = _mm256_mul_ps(vX, _mm256_broadcast_ps(&M2.Rows[0]));
		vY = _mm256_mul_ps(vY, _mm256_broadcast_ps(&M2.Rows[1]));
		vZ = _mm256_mul_ps(vZ, _mm256_broadcast_ps(&M2.Rows[2]));
		vW = _mm256_mul_ps(vW, _mm256_broadcast_ps(&M2.Rows[3]));

		vX = _mm256_add_ps(vX, vZ);
		vY = _mm256_add_ps(vY, vW);
		vX = _mm256_add_ps(vX, vY);

		MRes.doubleRows[0] = vX;//_mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(vX, _mm256_broadcast_ps(&M2.Rows[0])), _mm256_mul_ps(vZ, _mm256_broadcast_ps(&M2.Rows[2]))), _mm256_add_ps(_mm256_mul_ps(vY, _mm256_broadcast_ps(&M2.Rows[1])), _mm256_mul_ps(vW, _mm256_broadcast_ps(&M2.Rows[3]))));

		Rows = M1.doubleRows[1];

		vX = _mm256_permute_ps(Rows, 0x00);
		vY = _mm256_permute_ps(Rows, 0x55);
		vZ = _mm256_permute_ps(Rows, 0xaa);
		vW = _mm256_permute_ps(Rows, 0xff);

		vX = _mm256_mul_ps(vX, _mm256_broadcast_ps(&M2.Rows[0]));
		vY = _mm256_mul_ps(vY, _mm256_broadcast_ps(&M2.Rows[1]));
		vZ = _mm256_mul_ps(vZ, _mm256_broadcast_ps(&M2.Rows[2]));
		vW = _mm256_mul_ps(vW, _mm256_broadcast_ps(&M2.Rows[3]));

		vX = _mm256_add_ps(vX, vZ);
		vY = _mm256_add_ps(vY, vW);
		vX = _mm256_add_ps(vX, vY);

		MRes.doubleRows[1] = vX;//_mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(vX, _mm256_broadcast_ps(&M2.Rows[0])), _mm256_mul_ps(vZ, _mm256_broadcast_ps(&M2.Rows[2]))), _mm256_add_ps(_mm256_mul_ps(vY, _mm256_broadcast_ps(&M2.Rows[1])), _mm256_mul_ps(vW, _mm256_broadcast_ps(&M2.Rows[3]))));;
		*/
		MRes.doubleRows[0] = twolincomb_AVX_8(M1.doubleRows[0], M2);
		MRes.doubleRows[1] = twolincomb_AVX_8(M1.doubleRows[1], M2);

		return MRes;
	}
	AMMATRIX __vectorcall operator*(const AMMATRIX& M2) const
	{
		return Multiply(*this ,M2);
	}
};

const int numOfElems = 1000;

double RunAM();
double RunXM();

__declspec(align(32)) union VecD
{
	__m256 vec;
	__m128 half[2];
	struct
	{
		float x;
		float y;
		float z;
		float w;
	};
	VecD __vectorcall Mul_AVX(const VecD& _vec)
	{
		VecD res;
		_mm256_store_ps(&res.x, _mm256_mul_ps(vec, _vec.vec));
		return res;
	}
	VecD __vectorcall Add_AVX(const VecD& _vec)
	{
		VecD res;
		_mm256_store_ps(&res.x, _mm256_add_ps(vec, _vec.vec));
		return res;
	}
	VecD __vectorcall Mul_SSE2(const VecD& _vec)
	{
		VecD res;

		res.half[0] = _mm_mul_ps(half[0], _vec.half[0]);
		res.half[1] = _mm_mul_ps(half[1], _vec.half[1]);

		return res;
	}
	VecD __vectorcall Add_SSE2(const VecD& _vec)
	{
		VecD res;

		res.half[0] = _mm_add_ps(half[0], _vec.half[0]);
		res.half[1] = _mm_add_ps(half[1], _vec.half[1]);

		return res;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Start" << endl;
	system("pause");
	for (int i = 0; i < 10;i++)
		RunXM();

	cout << endl;

	for (int i = 0; i < 10; i++)
		RunAM();
	/*auto resTimeXM = RunXM();

	if (resTimeAM < resTimeXM)
	{
		cout << "AMMATRIX " << resTimeXM / resTimeAM << " times faster than XMMATRIX" << endl;
	}
	else
	{
		cout << "XMMATRIX " << resTimeAM / resTimeXM << " times faster than AMMATRIX" << endl;
	}*/

	/*VecD* vecs1 = static_cast<VecD*>(_aligned_malloc(sizeof(VecD)*numOfElems, 32));
	VecD* vecs2 = static_cast<VecD*>(_aligned_malloc(sizeof(VecD)*numOfElems, 32));
	VecD* vecs3 = static_cast<VecD*>(_aligned_malloc(sizeof(VecD)*numOfElems, 32));

	LARGE_INTEGER frequency;
	LARGE_INTEGER t1, t2;

	for (int i = 0; i < 5; i++)
	{
		/*QueryPerformanceCounter(&t1);
		for (int i = 0; i < numOfElems; i++)
		{
			vecs1[i] = vecs2[i].Mul_SSE2(vecs3[i]);
		}
		QueryPerformanceCounter(&t2);
		QueryPerformanceFrequency(&frequency);
		auto resTimeAM = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		cout << "SSE2 Multiplication " << resTimeAM << endl;

		QueryPerformanceCounter(&t1);
		for (int i = 0; i < numOfElems; i++)
		{
			vecs1[i] = vecs2[i].Add_SSE2(vecs3[i]);
		}
		QueryPerformanceCounter(&t2);
		QueryPerformanceFrequency(&frequency);
		resTimeAM = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		cout << "SSE2 Adding " << resTimeAM << endl;*/

		//_mm256_zeroall();

		/*QueryPerformanceCounter(&t1);
		for (int i = 0; i < numOfElems; i++)
		{
			vecs1[i] = vecs2[i].Mul_AVX(vecs3[i]);
		}
		QueryPerformanceCounter(&t2);
		QueryPerformanceFrequency(&frequency);
		auto resTimeAM = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		cout << "AVX Multiplication " << resTimeAM << endl;

		QueryPerformanceCounter(&t1);
		for (int i = 0; i < numOfElems; i++)
		{
			vecs1[i] = vecs2[i].Add_AVX(vecs3[i]);
		}
		QueryPerformanceCounter(&t2);
		QueryPerformanceFrequency(&frequency);
		resTimeAM = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		cout << "AVX Adding " << resTimeAM << endl;
		//_mm256_zeroall();

	}

	//cout << "Difference between ThreadPool and std::thread is "<< (cycles2 - cycles1)*100 / (cycles2) << " %" << endl;
	

	_aligned_free(vecs1);
	_aligned_free(vecs2);
	_aligned_free(vecs3);*/
	system("pause");
	return 0;
}

double RunAM()
{
	LARGE_INTEGER frequency;
	LARGE_INTEGER t1, t2;

	AMMATRIX amMarices1[numOfElems];// = new AMMATRIX;
	AMMATRIX amMarices2[numOfElems];// = new AMMATRIX[numOfElems];
	AMMATRIX amMarices3[numOfElems];// = new AMMATRIX[numOfElems];

	AMMATRIX AMinitVal(1.1f, 1.2f, 1.3f, 1.4f, 2.1f, 2.2f, 2.3f, 2.4f, 3.1f, 3.2f, 3.3f, 3.4f, 4.1f, 4.2f, 4.3f, 4.4f);
	for (int i = 0; i < numOfElems; i++)
	{
		amMarices2[i] = AMinitVal;
		amMarices3[i] = AMinitVal;
	}

	memset(amMarices1, 2, sizeof(AMMATRIX) * numOfElems);


	QueryPerformanceCounter(&t1);
	for (int i = 0; i < numOfElems; i++)
	{
		//amMarices1[i] = AMMATRIX::Multiply(amMarices2[i], amMarices3[i]);
		amMarices1[i] = amMarices2[i] * amMarices3[i];
	}
	QueryPerformanceCounter(&t2);
	QueryPerformanceFrequency(&frequency);
	auto resTimeAM = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;

	cout << "AMMATRIX worked in " << resTimeAM << endl;


	/*delete[] amMarices1;
	delete[] amMarices2;
	delete[] amMarices3;*/

	return resTimeAM;
}

double RunXM()
{
	LARGE_INTEGER frequency;
	LARGE_INTEGER t1, t2;
	//allocate memory

	XMMATRIX* xmMarices1 = static_cast<XMMATRIX*>(_aligned_malloc(sizeof(XMMATRIX)*numOfElems, 16));
	XMMATRIX* xmMarices2 = static_cast<XMMATRIX*>(_aligned_malloc(sizeof(XMMATRIX)*numOfElems, 16));
	XMMATRIX* xmMarices3 = static_cast<XMMATRIX*>(_aligned_malloc(sizeof(XMMATRIX)*numOfElems, 16));

	XMMATRIX XMinitVal(1.1f, 1.2f, 1.3f, 1.4f, 2.1f, 2.2f, 2.3f, 2.4f, 3.1f, 3.2f, 3.3f, 3.4f, 4.1f, 4.2f, 4.3f, 4.4f);
	for (int i = 0; i < numOfElems; i++)
	{
		xmMarices2[i] = XMinitVal;
		xmMarices3[i] = XMinitVal;
	}
	memset(xmMarices1, 0, sizeof(XMMATRIX) * numOfElems);



	QueryPerformanceCounter(&t1);
	for (int i = 0; i < numOfElems; i++)
	{
		xmMarices1[i] = xmMarices2[i] * xmMarices3[i];
	}
	QueryPerformanceCounter(&t2);
	QueryPerformanceFrequency(&frequency);
	auto resTimeXM = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;

	cout << "XMMATRIX worked in " << resTimeXM << endl;

	_aligned_free(xmMarices1);
	_aligned_free(xmMarices2);
	_aligned_free(xmMarices3);

	return resTimeXM;
	return 1;
}