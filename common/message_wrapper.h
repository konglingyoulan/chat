#ifndef COMMON_MESSAGE_WRAPPER_H
#define COMMON_MESSAGE_WRAPPER_H

#include "message.h"
#include "shared_ptr.h"

class message_wrapper
{
public:
	message_wrapper(void);
	virtual ~message_wrapper(void);

	enum
	{
		STATE_NONE,
		STATE_RECVING,
		STATE_SENDING
	};

private:
	shared_ptr<message> msg_;
	u_long send_offset_;
	int state_;
};

#endif
