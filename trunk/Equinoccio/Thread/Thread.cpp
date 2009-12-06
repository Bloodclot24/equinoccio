#include "Thread.h"

Thread::Thread() {
	thread = NULL;
}

Thread::~Thread() {}

int Thread::execute(){
	return pthread_create(&thread,NULL,start_routine,(void*)this);
}

void* Thread::start_routine(void* arg){
	Thread* p = (Thread*) arg;
	return p->run();
}

void Thread::exit(){
	pthread_exit(NULL);
}

void* Thread::join(){
	pthread_join(thread,NULL);
	return NULL;
}
