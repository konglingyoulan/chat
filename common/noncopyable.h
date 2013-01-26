#ifndef COMMON_NONCOPYABLE_H
#define COMMON_NONCOPYABLE_H

class noncopyable
{
protected:
	noncopyable(){}
	~noncopyable(){};
private:
	noncopyable(const noncopyable&);
	noncopyable& operator=(const noncopyable&);
};

#endif