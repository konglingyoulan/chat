#include "thread.h"
#include <process.h>

thread::thread(void)
: handle_(-1)
{}

thread::~thread(void)
{
	if (handle_)
	{
		_endthreadex(handle_);
	}
}

void thread::create(thread_func func, void *param)
{
	handle_ = _beginthreadex(NULL, 0, func, param, 0, NULL);
}