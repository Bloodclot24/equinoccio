#include "Mutex.h"

	Mutex::Mutex(){
		pthread_mutexattr_t atrr;
		pthread_mutexattr_init(&atrr);
		pthread_mutex_init(&mutex,&atrr);
	}
	Mutex::~Mutex(){
		this->unlock();
		pthread_mutex_destroy(&mutex);
	}
	int Mutex::lock(){
		return pthread_mutex_lock(&mutex);
	}
	int Mutex::unlock(){
		return pthread_mutex_unlock(&mutex);
	}
