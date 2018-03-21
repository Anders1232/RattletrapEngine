#include "Threading.h"
#include "Game.h"

namespace RattletrapEngine {
	
	Threading::ThreadContex Threading::early;
	Threading::ThreadContex Threading::mid;
	Threading::ThreadContex Threading::late;
	
	std::vector<pthread_t> Threading::earlyThreads;
	std::vector<pthread_t> Threading::midThreads;
	std::vector<pthread_t> Threading::lateThreads;
	
	void Threading::Init(void){
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
	
	void Threading::Destroy(void){
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
		GameObject *goToUpdate;\
		while(NULL != (goToUpdate= (GameObject*)FowardListGetElement( contex .fw) ) ){\
			if(goToUpdate->IsActive() ){\
				goToUpdate-> updateFunc (Game::GetInstance().GetDeltaTime());\
			}\
			pthread_mutex_lock(&( contex .mutex));\
			( contex .remainingGOsCounter)--;\
			if(0 >=  contex .remainingGOsCounter){\
				sem_post(&( contex .mutexForEndInform) );\
			}\
			pthread_mutex_unlock(&( contex .mutex));\
		}\
		pthread_exit(NULL);
	
	void* Threading::EarlyUpdateThread(void *nothing){
		UpdateThread(early,EarlyUpdate);
		return nullptr;
	}
	
	void* Threading::MidUpdateThread(void *nothing){
		UpdateThread(mid,Update);
		return nullptr;
	}
	
	void* Threading::LateUpdateThread(void *nothing){
		UpdateThread(late,LateUpdate);
		return nullptr;
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
		vector.resize(numThreads);\
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
		CreateThreadPool(lateThreads,LateUpdateThread);
	}
	
	#define DestroyThreadPool(contex,vector)\
		unsigned int numThreads= vector.size();\
		for(unsigned int i=0; i< numThreads; i++){\
			FowardListAddElement( contex .fw, NULL);\
		}\
		for(unsigned int i=0; i< numThreads; i++){\
			pthread_join(vector[i], NULL);\
		}
	//fim do define
	
	void Threading::DestroyEarlyThreadPool(void){
		DestroyThreadPool(early,earlyThreads);
	}
	
	void Threading::DestroyMidThreadPool(void){
		DestroyThreadPool(mid,midThreads);
	}
	
	void Threading::DestroyLateThreadPool(void){
		DestroyThreadPool(late,lateThreads);
	}
	
	void Threading::Update(ThreadContex &contex, std::vector<std::unique_ptr<GameObject>> &gameObjects){
		unsigned int goCount= gameObjects.size();
		contex.remainingGOsCounter= goCount;
		for(unsigned int i=0; i< goCount; i++){
			FowardListAddElement(contex.fw, &( *(gameObjects[i]) ) );
		}
		sem_wait(&(contex.mutexForEndInform) );
	}
	
	void Threading::EarlyUpdate(std::vector<std::unique_ptr<GameObject>> &gameObjects){
		Update(early, gameObjects);
	}
	void Threading::Update(std::vector<std::unique_ptr<GameObject>> &gameObjects){
		Update(mid, gameObjects);
	}
	void Threading::LateUpdate(std::vector<std::unique_ptr<GameObject>> &gameObjects){
		Update(late, gameObjects);
	}
	
}
