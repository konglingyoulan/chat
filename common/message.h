#ifndef COMMON_MESSAGE_H
#define COMMON_MESSAGE_H

#include <WinSock2.h>

/*	msg format:

	--------		4 bytes, msg total length
	--------		4 bytes, msg type
	--------		n bytes, msg content, could be 0
	----бнбн		
*/


class message
{
public:
	message(void);
	virtual ~message(void);

	enum
	{
		TYPE_CMD = 0x10000,
		TYPE_TEXT = 0x20000
	};
	enum
	{
		TYPE_SIGNIN = TYPE_CMD | 0x1,
		TYPE_SIGNUP = TYPE_CMD | 0x2,
		TYPE_QUIT = TYPE_CMD | 0x3,
	};

	//before send message, 
	//convert message to bytes stream.
	virtual void prepare_send() = 0;
	//when message has recved done,call this function
	//to parse bytes stream into each member.
	virtual void handle_recv() = 0;

	WSABUF* get_send_buf() const;
	WSABUF* get_recv_buf() const;

protected:
	OVERLAPPED overlapped_;
	int type_;
	u_long bytes_length_;
	WSABUF *send_buf_;
	WSABUF *recv_buf_; 
};

#endif
