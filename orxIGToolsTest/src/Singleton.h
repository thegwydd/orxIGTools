#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <assert.h>
#include <memory>

//////////////////////////////////////////////////////////////////////////
//! Singleton base class
//! \param T	Class type to make singleton
template <typename T>
class Singleton
	{
	protected:
		Singleton(void) {};
		~Singleton(void) {};

	protected:
		//! Static instance of the class
		static std::shared_ptr<T> ms_Singleton;

	public:
		//! Returns a reference to the instance
		static T & Instance(void)
			{
			if (ms_Singleton == nullptr) ms_Singleton = make_shared<T>();
			assert(ms_Singleton); 
			return (*ms_Singleton);
			}

		//! returns a pointer to the instance
		static std::shared_ptr<T> InstancePtr(void)
			{
			assert(ms_Singleton); 
			return ms_Singleton;
			}
	};

template <class T> std::shared_ptr<T> Singleton<T>::ms_Singleton = nullptr;

#endif