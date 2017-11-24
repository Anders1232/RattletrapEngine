#include "FowardList.h"
#include <stdlib.h>
#include <stdio.h>

FowardList* NewFowardList(void)
{
	FowardList *ret= malloc(sizeof(FowardList));
	if(NULL == ret)
	{
		fprintf(stderr, "[ERROR] %s | %s:%d\t\t Error allocating foward list.", __FILE__, __func__, __LINE__);
		return NULL;
	}
//	ret->mutex= malloc (sizeof(pthread_mutex_t));
/*	if(NULL == ret->mutex)
	{
		fprintf(stderr, "[ERROR] %s | %s:%d\t\t Error allocating foward list.", __FILE__, __func__, __LINE__);
		return NULL;
	}*/
	pthread_mutex_init(&(ret->mutex), NULL);
	sem_init(&(ret->semaphore), 0, 0);
	ret->begin= NULL;
	ret->end = NULL;
	return ret;
}

void FowardListAddElement(FowardList*fw, void *element)
{
	pthread_mutex_lock(&(fw->mutex));
	FowardListElement *newElement= malloc(sizeof(FowardListElement));
	if(NULL == newElement)
	{
		fprintf(stderr, "[ERROR] %s | %s:%d\t\t Error allocating foward list.", __FILE__, __func__, __LINE__);
		return;
	}
	newElement->next=NULL;
	newElement->data= element;
	if(fw->begin== NULL)
	{
		fw->begin= newElement;
	}
	else
	{
		fw->end->next= newElement;
	}
	fw->end= newElement;
	sem_post(&(fw->semaphore));
	pthread_mutex_unlock(&(fw->mutex));
}

void* FowardListGetElement(FowardList* fw)
{
	sem_wait(&(fw->semaphore) );
	pthread_mutex_lock(&(fw->mutex));
	FowardListElement *toDelete= fw->begin;
	void *ret= NULL;
	if(NULL != toDelete)
	{
		fw->begin= fw->begin->next;
		ret= toDelete->data;
		free(toDelete);
	}
//	if()
	pthread_mutex_unlock(&(fw->mutex));
	return ret;
}

FowardList* DeleteFowardList(FowardList *fw)
{
	pthread_mutex_destroy(&(fw->mutex));
	sem_destroy(&(fw->semaphore));
	FowardListElement *aux= fw->begin;
	FowardListElement *aux2;
	while(NULL != aux)
	{
		aux2= aux->next;
		free(aux);
		aux=aux2;
	}
	free(fw);
	return NULL;
}
