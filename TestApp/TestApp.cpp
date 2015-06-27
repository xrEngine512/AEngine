// TestApp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "../Engine/SystemThreadPool.h"
#include "../ApproxCommonSystem/FnPtr.h"
#include <iostream>
#define WIN_32_LEAN_AND_MEAN
#include <Windows.h>
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
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Start" << endl;
	system("pause");
	test1();
	test2();
	test3();
	cout << "Difference between ThreadPool and std::thread is "<< (cycles2 - cycles1)*100 / (cycles2) << " %" << endl;
	system("pause");
	return 0;
}

