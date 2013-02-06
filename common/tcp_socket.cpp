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
	//messageǰ4�ֽڱ�ʶ����message�Ĵ�С
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
	//���´����ǲ��Ƿ���message���и����ʣ�
	//��asioһ����ÿһ����д������ɺ�ص�һ��������
	//����ʹ���źŲۻ��Ƹ��ƴ���ṹ��
	case 0:			
		{
			switch (recving_msg_->get_state())
			{
			case message_wrapper::STATE_RECVING_HEAD:
				{
					if (bytes_recved == buf_len)	//4�ֽ�ͷ����
					{
						recving_msg_->set_state(message_wrapper::STATE_RECVING_CONTENT);	
						//��ʱ��Ϊmessage::recv_buf_����ռ���

					}
					else
					{
						//u_long��Big Endian�ֽ����䣬�ѵõ����ֽڷŵ���ȷ��ƫ�ƴ�
						//���µ�message:;bytes_length_����

					}
				}
				break;
			case message_wrapper::STATE_RECVING_CONTENT:
				{
					if (bytes_recved == buf_len)	//������Ϣ����
					{
						recving_msg_->set_state(message_wrapper::STATE_NONE);	
						//�ַ�������socket���to_send_msgs_
						//ɾ��recving_msg_

					}
					else
					{
						//

					}
					//���յ�����ϢƬ�θ��ӵ����������

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