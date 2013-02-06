#ifndef COMMON_MESSAGE_WRAPPER_H
#define COMMON_MESSAGE_WRAPPER_H

#include "message.h"
#include "shared_ptr.h"

class message_wrapper
{
public:
	message_wrapper();
	message_wrapper(shared_ptr<message>& msg);
	virtual ~message_wrapper(void);

	enum
	{
		STATE_NONE,
		STATE_RECVING_HEAD,
		STATE_RECVING_CONTENT,
		STATE_SENDING
	};

	message* get_message() const;
	void set_message(shared_ptr<message>& msg);
	u_long get_send_offset() const;
	void set_send_offset(u_long offset);
	u_long get_recv_offset() const;
	void set_recv_offset(u_long offset);
	int get_state() const;
	void set_state(int state);

private:
	shared_ptr<message> msg_;
	u_long send_offset_;
	u_long recv_offset_;
	int state_;
};

#endif
