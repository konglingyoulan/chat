#ifndef COMMON_SHARED_PTR_H
#define COMMON_SHARED_PTR_H

template <class T>
class shared_ptr
{
public:
	shared_ptr(T *o = NULL)
	: obj(o)
	{
		count = new size_t;
		if (obj)
		{
			*count = 1;
		} 
		else
		{
			*count = 0;
		}
	}

	template <class Y>
	shared_ptr(Y *o)
	: obj(o)
	{
		count = new size_t;
		*count = 1;
	}

	~shared_ptr()
	{
		release();
	}

	shared_ptr(const shared_ptr &rhs)
	: obj(rhs.obj)
	, count(rhs.count)
	{
		if (obj)
		{
			++*count;
		}
	}

	shared_ptr& operator=(const shared_ptr &rhs)
	{
		if (obj == rhs.obj)
		{
			return *this;
		}
		
		release();
		obj = rhs.obj;
		count = rhs.count;
		++*count;
		return *this;
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

	T* get() {return obj;}
	const T* get() const {return obj;}

private:
	void release()
	{
		if (count)
		{
			--*count;
			if (*count == 0)
			{
				delete obj;
				delete count;
			}
		}
	}

	T *obj;
	size_t *count;
};

template<class T>
bool operator==(const shared_ptr<T> &lhs, const shared_ptr<T> &rhs)
{
	return lhs.get() == rhs.get();
}

template<class T>
bool operator!=(const shared_ptr<T> &lhs, const shared_ptr<T> &rhs)
{
	return !(lhs == rhs);
}

#endif