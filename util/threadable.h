#ifndef _THREADABLE_H
#define _THREADABLE_H

class Threadable {
public:
	virtual void run(void* arg) = 0;
};

#endif