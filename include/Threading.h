#ifndef THREADING_H
#define THREADING_H

#include <thread>
#include <pthread.h>
#include <semaphore.h>
#include <sys/sysinfo.h>
#include "FowardList.h"
#include "State.h"

namespace Rattletrap {
	class Threading {
		public:
			Threading(void);
			~Threading(void);
			void EarlyUpdate(std::std::vector<std::unique_ptr<GameObject>> &gameObjects);
			void Update(std::std::vector<std::unique_ptr<GameObject>> &gameObjects);
			void LateUpdate(std::std::vector<std::unique_ptr<GameObject>> &gameObjects);
		private:
			struct ThreadContex{
				pthread_mutex_t mutex;
				int remainingGOsCounter;
				sem_t mutexForEndInform;
				FowardList* fw;
			};
			ThreadContex early, mid, late;
			
			void CreateEarlyThreadPool(void);
			void CreateMidThreadPool(void);
			void CreateLateThreadPool(void);
			
			void* EarlyUpdateThread(void *nothing);
			void* MidUpdateThread(void *nothing);
			void* LateUpdateThread(void *nothing);
			
			void DestroyEarlyThreadPool(void);
			void DestroyMidThreadPool(void);
			void DestroyLateThreadPool(void);
			
			void Update(ThreadContex &contex, std::std::vector<std::unique_ptr<GameObject>> &gameObjects);
			
			std::vector<pthread_t> earlyThreads;
			std::vector<pthread_t> midThreads;
			std::vector<pthread_t> LateThreads;
	}
}


#endif
