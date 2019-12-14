#pragma once
#include <mutex>
#include <assert.h>
using namespace std;

class SingletonFinalizer
{
public:
	typedef void(*FinalizerFunc)();

	static void addFinalizer(FinalizerFunc func);
	static void finalize();
};

template <typename T>
class singleton final
{
public:
	static T& get_instance()
	{
		call_once(initFlag, create);
		assert(instance);
		return *instance;
	}

private:
	static void create()
	{
		instance = new T;
		SingletonFinalizer::addFinalizer(&singleton<T>::destroy);
	}

	static void destroy()
	{
		delete instance;
		instance = nullptr;
	}

	static once_flag initFlag;
	static T* instance;
};

template <typename T> once_flag singleton<T>::initFlag;
template <typename T> T* singleton<T>::instance = nullptr;