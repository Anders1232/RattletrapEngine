/**
	Esse código foi feito com base em exemplo presente no artigo:
	https://www.codeproject.com/Articles/35230/Observer-Pattern-as-C-Component
*/

#include <vector>
#include <algorithm>//find()

#include "Error.h"

template<typename SourceType, typename EventArgType>
class Event{
	protected:
		typedef void (*EventHandler)(SourceType *source, EventArgType eventArg, void* context);

		class SubscriberRecord{
			private:
				EventHandler handlerProc;//método a ser chamado
				void* handlerContext; //ponteiro ao objeto que contem o método

				SubscriberRecord(EventHandler handler, void* context = 0):
					handlerProc(handler),
					handlerContext(context)
					{}

				friend class Event; //Agora, a classe Event pode acessar os atributos privados e protegidos de SubscrierRecord
		};

		std::vector<SubscriberRecord> Subscribers; //objetos e respectivos métodos cadastrados no evento
		SourceType *eventSource;//ponteiro ao objeto que gerou o evento

		public:
			Event(SourceType *source):
				eventSource(source)
				{}
			~Event(){};

			//Adiciona uma função (desligada de objeto) a lista de inscrições
			void operator +=(EventHandler handler){
				Subscribers.push_back(SubscriberRecord(handler));
			}

			//Remove uma função (desligada de objeto) a lista de inscrições
			void operator -=(EventHandler handler){
			    //vector<Event<SourceType,EventArgType>::SubscribeRecord>::iterator é um escopo dependente
				typename std::vector<SubscriberRecord>::iterator it = find(Subscribers.begin(), Subscribers.end(), handler);
				if(it != Subscribers.end()){
					Subscribers.erase(it);
				}
			}

			//Adiciona método e (opcional) objeto à lista de inscrições
			void Subscribe(EventHandler handler, void* context = 0){
				Subscribers.push_back(SubscriberRecord(handler, context));
			}

			//Remove método e (opcional) objeto à lista de inscrições
			void Unsubscribe(EventHandler handler, void* context = 0){
				typename std::vector<SubscriberRecord>::iterator it = find(Subscribers.begin(), Subscribers.end(), SubscriberRecord(handler, context));
				if(it != Subscribers.end()){
					Subscribers.erase(it);
				}
			}

			//Momento da chamada dos métodos inscritos
			void FireEvent(EventArgType eventArg){
			    DEBUG_PRINT("inicio");
				for(unsigned int i = 0; i < Subscribers.size(); i++){
					Subscribers[i].handlerProc(eventSource, eventArg, Subscribers[i].handlerContext);
				}
				DEBUG_PRINT("fim");
			}
};
