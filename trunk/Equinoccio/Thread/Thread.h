#ifndef THREAD_H_
#define THREAD_H_

#include  <pthread.h>

class Thread {
public:
	Thread();
	virtual ~Thread();
	void* join();
	int execute();
	void exit();
	virtual void* run()=0;
private:
	pthread_t thread;
	static void* start_routine(void*);
};

#endif /* THREAD_H_ */
