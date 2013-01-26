#ifndef COMMON_TCP_SOCKET_H
#define COMMON_TCP_SOCKET_H

#include <list>
#include "message_wrapper.h"

using std::list;

class tcp_socket
{
public:
	tcp_socket(SOCKET sock = INVALID_SOCKET);
	virtual ~tcp_socket(void);

	bool create();
	bool close();
	bool async_send();
	bool send();
	bool async_recv();
	bool recv();

	bool valid() {return handle_ != INVALID_SOCKET;}

protected:
	SOCKET handle_;
	list<message_wrapper> to_send_msgs_;
	shared_ptr<message> recving_msg_;
};

#endif
