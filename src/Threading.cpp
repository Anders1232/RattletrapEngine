#include "Threading.h"

namespace Rattletrap {
	
	Threading::Threading(void){
		pthread_mutex_init(&(early.mutex), NULL );
		pthread_mutex_init(&(mid.mutex), NULL );
		pthread_mutex_init(&(late.mutex), NULL );
		
		sem_init(&(early.mutexForEndInform), 0, 0 );
		sem_init(&(mid.mutexForEndInform), 0, 0 );
		sem_init(&(late.mutexForEndInform), 0, 0 );
		
		early.remainingGOsCounter=0;
		mid.remainingGOsCounter=0;
		late.remainingGOsCounter=0;
		
		early.fw= NewFowardList();
		mid.fw= NewFowardList();
		late.fw= NewFowardList();
	}
	
	Threading::~Threading(void){
		pthread_mutex_destroy(&(early.mutex) );
		pthread_mutex_destroy(&(mid.mutex) );
		pthread_mutex_destroy(&(late.mutex) );
		
		sem_destroy(&(early.mutexForEndInform) );
		sem_destroy(&(mid.mutexForEndInform) );
		sem_destroy(&(late.mutexForEndInform) );
		
		early.fw = DeleteFowardList(early.fw);
		mid.fw= DeleteFowardList(mid.fw);
		late.fw= DeleteFowardList(late.fw);
	}
	
	
	#define UpdateThread(contex,updateFunc)\
		GameObject *gotoUpdate;\
		while(NULL != (goToUpdate= FowardListGetElement( contex .fw) ) ){\
			if(goToUpdate->IsActive() ){\
				goToUpdate-> updateFunc (Game::GetInstance().GetDeltaTime());\
			}\
			pthread_mutex_lock(&( contex .mutex));\
			( contex .remainingGOsCounter)--;\
			if(0 >= remainingGOsCounter){\
				sem_post(&( contex .mutexForEndInform) );\
			}\
			pthread_mutex_unlock(&( contex .mutex));\
		}
	
	void* Threading::EarlyUpdateThread(void *nothing){
		UpdateThread(early,EarlyUpdate);
	}
	
	void* Threading::MidUpdateThread(void *nothing){
		UpdateThread(mid,Update);
	}
	
	void* Threading::LateUpdateThread(void *nothing){
		UpdateThread(late,LateUpdate);
	}
	
	#undef UpdateThread
	
	//código obtido em https://stackoverflow.com/questions/7341046/posix-equivalent-of-boostthreadhardware-concurrency/7341515#7341515
	//para o caso do compilador não tenha suporte para std::thread::hardware_concurrency()
	static unsigned hardware_concurrency()
	{
	#if defined(PTW32_VERSION) || defined(__hpux)
		return pthread_num_processors_np();
	#elif defined(__APPLE__) || defined(__FreeBSD__)
		int count;
		size_t size=sizeof(count);
		return sysctlbyname("hw.ncpu",&count,&size,NULL,0)?0:count;
	#elif defined(BOOST_HAS_UNISTD_H) && defined(_SC_NPROCESSORS_ONLN)
		int const count=sysconf(_SC_NPROCESSORS_ONLN);
		return (count>0)?count:0;
	#elif defined(_GNU_SOURCE)
		return get_nprocs();
	#elif _WIN32
		SYSTEM_INFO info={{0}};
		GetSystemInfo(&info);
		return info.dwNumberOfProcessors;
	#else
		return 1;
	#endif
	}
	
	#define CreateThreadPool(vector,updateMethod)\
		unsigned int numThreads= std::thread::hardware_concurrency();\
		if(0 == numThreads){\
			numThreads= hardware_concurrency();\
		}\
		vector= std::vector <pthread_t>(numThreads);\
		for(unsigned int i=0; i < numThreads; i++){\
			pthread_create(&(vector[i]), NULL, updateMethod , NULL);\
		}
	//fim do define
	
	void Threading::CreateEarlyThreadPool(void){
		CreateThreadPool(earlyThreads,EarlyUpdateThread);
	}
	
	void Threading::CreateMidThreadPool(void){
		CreateThreadPool(midThreads,MidUpdateThread);
	}
	
	void Threading::CreateLateThreadPool(void){
		CreateThreadPool(LateThreads,LateUpdateThread);
	}
	
	#define DestroyThreadPool(vector)\
		unsigned int numThreads= vector.size();\
		for(unsigned int i=0; i< numThreads; i++){\
			FowardListAddElement(&( vector .fw), NULL);\
		}\
		for(unsigned int i=0; i< numThreads; i++){\
			pthread_join(vector[i], NULL);\
		}
	//fim do define
	
	void Threading::DestroyEarlyThreadPool(void){
		DestroyThreadPool(earlyThreads);
	}
	
	void Threading::DestroyMidThreadPool(void){
		DestroyThreadPool(midThreads);
	}
	
	void Threading::DestroyLateThreadPool(void){
		DestroyThreadPool(LateThreads);
	}
	
	void Threading::Update(ThreadContex &contex, std::std::vector<std::unique_ptr<GameObject>> &gameObjects){
		unsigned int goCount= gameObjects.size();
		contex.remainingGOsCounter= goCount;
		for(int i=0; i< goCount; i++){
			FowardListAddElement(contex.fw, &( (gameObjects[i])->get() ) );
		}
		sem_wait(&(contex.mutexForEndInform) );
	}
	
	void Threading::EarlyUpdate(std::std::vector<std::unique_ptr<GameObject>> &gameObjects){
		Update(early, gameObjects);
	}
	void Threading::Update(std::std::vector<std::unique_ptr<GameObject>> &gameObjects){
		Update(mid, gameObjects);
	}
	void Threading::LateUpdate(std::std::vector<std::unique_ptr<GameObject>> &gameObjects){
		Update(late, gameObjects);
	}
	
}

