#ifndef COMMON_TCP_SOCKET_H
#define COMMON_TCP_SOCKET_H

#include <list>
#include "message_wrapper.h"

using std::list;

class iocp;
class tcp_socket
{
public:
	tcp_socket(SOCKET sock = INVALID_SOCKET, iocp *io = NULL);
	virtual ~tcp_socket(void);

	bool create();
	bool close();
	bool async_send();
	bool send();
	bool async_recv();
	bool recv();
	bool listen(int backlog);
	tcp_socket* accept();

	void add_send_message(shared_ptr<message>& msg);

	operator SOCKET() const {return handle_;};

	bool valid() const {return handle_ != INVALID_SOCKET;}

protected:
	SOCKET handle_;
	sockaddr_in addr_;
	iocp *io_;
	list<message_wrapper> to_send_msgs_;
	shared_ptr<message_wrapper> recving_msg_;
};

#endif
