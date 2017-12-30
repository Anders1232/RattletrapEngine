#ifndef THREADING_H
#define THREADING_H

#include <thread>
#include <pthread.h>
#include <semaphore.h>
#include "FowardList.h"
#include "State.h"

#ifdef _WIN32
	#include <Windows.h>
	#include <Winbase.h>
#elif __APPLE__
	#include <sys/sysinfo.h>
	//mac
#elif __linux__
	#include <sys/sysinfo.h>
#else
	#error "Unknown compiler"
#endif

namespace RattletrapEngine {

	class Threading {
		public:
			static void Init(void);
			static void Destroy(void);
			static void EarlyUpdate(std::vector<std::unique_ptr<GameObject>> &gameObjects);
			static void Update(std::vector<std::unique_ptr<GameObject>> &gameObjects);
			static void LateUpdate(std::vector<std::unique_ptr<GameObject>> &gameObjects);
		private:
			struct ThreadContex{
				pthread_mutex_t mutex;
				int remainingGOsCounter;
				sem_t mutexForEndInform;
				FowardList* fw;
			};
			static ThreadContex early, mid, late;
			
			static void CreateEarlyThreadPool(void);
			static void CreateMidThreadPool(void);
			static void CreateLateThreadPool(void);
			
			static void* EarlyUpdateThread(void *nothing);
			static void* MidUpdateThread(void *nothing);
			static void* LateUpdateThread(void *nothing);
			
			static void DestroyEarlyThreadPool(void);
			static void DestroyMidThreadPool(void);
			static void DestroyLateThreadPool(void);
			
			static void Update(ThreadContex &contex, std::vector<std::unique_ptr<GameObject>> &gameObjects);
			
			static std::vector<pthread_t> earlyThreads;
			static std::vector<pthread_t> midThreads;
			static std::vector<pthread_t> lateThreads;
	};

}

#endif
