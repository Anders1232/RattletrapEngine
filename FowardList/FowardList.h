#ifndef FOWARD_LIST
#define FOWARD_LIST

#include <pthread.h>
#include <semaphore.h>

struct fowardListElement
{
	void* data;
	struct fowardListElement *next;
};
typedef struct fowardListElement FowardListElement;

struct fowardList
{
	pthread_mutex_t mutex;
	sem_t semaphore;
	FowardListElement *begin;
	FowardListElement *end;
};
typedef struct fowardList FowardList;

FowardList* NewFowardList(void);
void FowardListAddElement(FowardList*, void *element);
void* FowardListGetElement(FowardList*);
FowardList* DeleteFowardList(FowardList*);

#endif
