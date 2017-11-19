#ifndef UPDATE_THREADS_H
#define UPDATE_THREADS_H

#include<pthread.h>
#include"FowardList.h"

struct threadData{
	int *remainingGOsCounter;
	GameObject *goToProcess;
	pthread_mutex_t *mutex;
	pthread_mutex_t *mutexForEndInform;
	FowardList* fw;
}

void* UodateThread(void* _data){
	ThreadData* data= _data;
	
}

#endif
