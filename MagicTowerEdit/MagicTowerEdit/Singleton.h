#pragma  once
#include <assert.h>

template<class T>
class Singleton
{
public:
	Singleton(){}

	virtual ~Singleton(){}

	inline static void sCreate()
	{
		assert(smInstance == NULL);
		smInstance = new T;
		assert(smInstance);
	}

	inline static bool sHasInstance()
	{
		return smInstance != NULL;
	}

	inline static void sDestroy()
	{
		if (smInstance)
		{
			delete static_cast<T*>(smInstance);
			smInstance = NULL;
		}
	}

	inline static T& sInstance()
	{
		assert(smInstance != NULL);
		return *static_cast<T*>(smInstance);
	}

	inline static T* sInstancePtr()
	{
		assert(smInstance!=NULL);
		return static_cast<T*>(smInstance);
	}
private:
	static void* smInstance;
};

template<class T>
void * Singleton<T>::smInstance = NULL;