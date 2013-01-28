#ifndef COMMON_IOCP_H
#define COMMON_IOCP_H

#include "tcp_socket.h"

class iocp
{
public:
	iocp(void);
	virtual ~iocp(void);

	bool create();
	bool attach_socket(const tcp_socket &sock);
	tcp_socket* wait();

	bool valid() const {return completion_handle_ != NULL;}

protected:
	HANDLE completion_handle_;
	list<shared_ptr<tcp_socket> > clients_;
};

#endif