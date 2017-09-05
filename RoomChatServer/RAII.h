#pragma once
#include<WinSock2.h>
#include<mutex>
#include<iostream>
using namespace std;


class MUTEX
{
private:
	mutex mMutex;
public:
	MUTEX()
	{
		cout << "mutex ������ ȣ��" << endl;
	}
	~MUTEX()
	{
		cout << "mutex �Ҹ��� ȣ��" << endl;
	}
	void lock()
	{
		mMutex.lock();
		cout << "mutex lock �Ϸ�" << endl;
	}
	void unlock()
	{
		mMutex.unlock();
		cout << "mutex nulock �Ϸ�" << endl;
	}

};

class CRITICALSECTION
{
private:
	CRITICAL_SECTION mCS;
public:
	CRITICALSECTION()
	{
		InitializeCriticalSection(&mCS);
	}
	~CRITICALSECTION()
	{
		DeleteCriticalSection(&mCS);
		cout << "critical ��ü ����" << endl;
	}
	void lock()
	{
		EnterCriticalSection(&mCS);
		cout << "critical lock �Ϸ�" << endl;
	}
	void unlock()
	{
		LeaveCriticalSection(&mCS);
		cout << "critical unlock �Ϸ�" << endl;
	}
};



template<typename T> class ScopeLock {

	T& obj;
public:

	ScopeLock(T* o) : obj(*o)
	{
		obj.lock();
	}

	ScopeLock(T& o) : obj(o)
	{
		obj.lock();
	}

	~ScopeLock()
	{
		obj.unlock();
		//obj.~T();
	}

};
//static MUTEX RAII_MUTEX;
//static CRITICALSECTION RAII_CRITICALSECTION;