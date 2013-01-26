#ifndef COMMON_SCOPED_PTR_H
#define COMMON_SCOPED_PTR_H

#include "noncopyable.h"

template <class T>
class scoped_ptr : private noncopyable
{
public:
	scoped_ptr(const T *o = NULL)
	: obj(o)
	{}

	~scoped_ptr()
	{
		if (obj)
		{
			delete obj;
		}
	}

	T* operator->() const
	{
		return obj;
	}

	T& operator*() const
	{
		return *obj;
	}

	bool valid() const {return obj != NULL;}

	void reset(T *o = NULL)
	{
		if (obj != o)
		{
			swap(o);
		}
	}

	void swap(const scoped_ptr &rhs)
	{
		T *tmp = rhs->obj;
		rhs->obj = obj;
		obj = tmp;
	}

private:
	T *obj;
};

template <class T>
void swap(const scoped_ptr<T>& x, const scoped_ptr<T>& y)
{
	x.swap(y);
}

#endif