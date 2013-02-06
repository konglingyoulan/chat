#include "tcp_socket.h"
#include "iocp.h"

tcp_socket::tcp_socket(SOCKET sock/* = INVALID_SOCKET*/, iocp *io/* = NULL*/)
: handle_(sock)
, io_(io)
{
	if (!valid())
	{
		create();
	}
}

tcp_socket::~tcp_socket(void)
{
	close();
}

bool tcp_socket::create()
{
	if (valid())
	{
		return false;
	}
	handle_ = socket(AF_INET, SOCK_STREAM, 0);
	return handle_ != INVALID_SOCKET;
}

bool tcp_socket::close()
{
	if (valid())
	{
		closesocket(handle_);
		handle_ = INVALID_SOCKET;
		return true;
	}
	return false;
}

bool tcp_socket::async_send()
{
	if (to_send_msgs_.empty())
	{
		return false;
	}
	message_wrapper msg = to_send_msgs_.front();
	WSABUF buf = {0};
	buf.buf = msg.get_message()->get_send_buf()->buf + msg.get_send_offset();
	buf.len = msg.get_message()->get_send_buf()->len - msg.get_send_offset();
	DWORD bytes_sent = 0;
	int ret = WSASend(handle_, &buf, 1, &bytes_sent, 0, (LPOVERLAPPED)io_, NULL);
	switch (ret)
	{
	case 0:
		{
			if (bytes_sent < buf.len)
			{
				msg.set_send_offset(msg.get_send_offset() + bytes_sent);
			}
			else
			{
				to_send_msgs_.pop_front();
			}
		}
		break;
	case WSA_IO_PENDING:
		//do nothing
		break;
	default:
		//sth. is wrong.
		return false;
	}
	return true;
}

bool tcp_socket::async_recv()
{
	WSABUF buf = {0};
	DWORD bytes_recved = 0;
	DWORD flags = 0;
	//message前4字节标识整个message的大小
	if (!recving_msg_.valid())
	{
		recving_msg_ = new message_wrapper;
		if (!recving_msg_.valid())
		{
			return false;
		}
		recving_msg_->set_state(message_wrapper::STATE_RECVING_HEAD);
	}
	u_long buf_len = 0;
	switch (recving_msg_->get_state())
	{
	case message_wrapper::STATE_RECVING_HEAD:
		{
			buf_len = sizeof(u_long) - recving_msg_->get_recv_offset();
			buf.buf = new char[buf_len];
			buf.len = buf_len;
		}
		break;
	case message_wrapper::STATE_RECVING_CONTENT:
		{
			buf.buf = recving_msg_->get_message()->get_recv_buf()->buf 
				+ recving_msg_->get_recv_offset();
			buf.len = recving_msg_->get_message()->get_recv_buf()->len
				- recving_msg_->get_recv_offset();
		}
		break;
	default:
		return false;
	}
	int ret = WSARecv(handle_, &buf, 1, &bytes_recved, &flags, (LPOVERLAPPED)io_, NULL);
	switch (ret)
	{
	//以下代码是不是放在message类中更合适？
	//像asio一样，每一个读写操作完成后回调一个函数？
	//或者使用信号槽机制改善代码结构？
	case 0:			
		{
			switch (recving_msg_->get_state())
			{
			case message_wrapper::STATE_RECVING_HEAD:
				{
					if (bytes_recved == buf_len)	//4字节头读完
					{
						recving_msg_->set_state(message_wrapper::STATE_RECVING_CONTENT);	
						//是时候为message::recv_buf_分配空间了

					}
					else
					{
						//u_long用Big Endian字节序传输，把得到的字节放到正确的偏移处
						//更新到message:;bytes_length_保存

					}
				}
				break;
			case message_wrapper::STATE_RECVING_CONTENT:
				{
					if (bytes_recved == buf_len)	//整条信息读完
					{
						recving_msg_->set_state(message_wrapper::STATE_NONE);	
						//分发到其它socket类的to_send_msgs_
						//删除recving_msg_

					}
					else
					{
						//

					}
					//将收到的消息片段附加到缓冲区后边

				}
				break;
			}
			recving_msg_->set_recv_offset(recving_msg_->get_recv_offset() + bytes_recved);
		}
		break;
	case WSA_IO_PENDING:
		//do nothing
		break;
	default:
		return false;
	}
	return true;
}

bool tcp_socket::listen(int backlog)
{
	if (!valid())
	{
		return false;
	}
	return ::listen(handle_, backlog) == 0;
}

tcp_socket* tcp_socket::accept()
{
	sockaddr_in sin = {0};
	int sin_len = sizeof(sin);
	SOCKET s = ::accept(handle_, (sockaddr*)&sin, &sin_len);
	tcp_socket *ts = new tcp_socket(s, io_);
	return ts;
}

void tcp_socket::add_send_message(shared_ptr<message>& msg)
{
	message_wrapper msg_outer(msg);
	to_send_msgs_.push_back(msg_outer);
}