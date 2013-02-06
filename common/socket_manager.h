#ifndef COMMON_SOCKET_MANAGER_H
#define COMMON_SOCKET_MANAGER_H

#include "noncopyable.h"

class socket_manager : private noncopyable
{
public:
	socket_manager(void);
	~socket_manager(void);
};

#endif
