#ifndef COMMON_THREAD_H
#define COMMON_THREAD_H

#include <Windows.h>

typedef unsigned (_stdcall *thread_func)(void*);

class thread
{
public:
	thread(void);
	~thread(void);

	void create(thread_func, void*);

private:
	unsigned handle_;
};

#endif