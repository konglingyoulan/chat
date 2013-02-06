#include "iocp.h"

iocp::iocp(void)
{
	create();
}

iocp::~iocp(void)
{
}

bool iocp::create()
{
	completion_handle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	return completion_handle_ != NULL;
}

bool iocp::attach_socket(const tcp_socket &sock)
{
	if (!sock.valid() || !valid())
	{
		return false;
	}
	if (CreateIoCompletionPort((HANDLE)SOCKET(sock), completion_handle_,
		(ULONG_PTR)&sock, 0) != completion_handle_)
	{
		return false;
	}

	clients_.push_back(shared_ptr<tcp_socket>(const_cast<tcp_socket*>(&sock)));
	return true;
}

tcp_socket* iocp::wait()
{
	if (!valid())
	{
		return NULL;
	}

	tcp_socket *sock;
	message *msg;
	DWORD bytes;
	if (GetQueuedCompletionStatus(completion_handle_, &bytes, (PULONG_PTR)&sock, (LPOVERLAPPED*)&msg, INFINITE) != FALSE)
	{
		return sock;
	}
	return NULL;
}
