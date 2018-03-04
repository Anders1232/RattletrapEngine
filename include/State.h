#ifndef STATE_H
#define STATE_H

#include <vector>
#include <memory>
#include "InputManager.h"
#include "Resources.h"

#include "GameObject.h"

namespace RattletrapEngine {

	/**
		\brief Classe virtual que modela um estado do jogo

		State provê uma interface sobre a qual a lógica do jogo pode ser desenvolvida e rodada sobre a engine. Um jogo pode ter vários estados, solictando que seja desempilhado ou que novos estados sejam empilhados. A engine os gerencia como uma pilha.
	*/
	class State {
		public:
			/**
				\brief Construtor

				Inicializa as variáveis protegidas. Para fins de praticidade recomenda-se que seus filhos chamem esse construtor caso não queiram inicializar essas variáveis por si mesmo.
			*/
			State(void);
			/**
				\brief Destrutor
				\todo Verificar necessidade de fazer um destrutor virtual.

				Força os filhos a implementarem um destrutor.
			*/
			virtual ~State(void);
			/**
				\brief Atualiza os estado do jogo
				\param dt intervalo de tempo transcorrido desde o último frame.

				Aqui deve ser colocado o código que atualiza o estado do jogo, utilizando dos dados contidos no inputManager, câmera e outras coisas que forem julgadas necessárias.
			*/
			virtual void Update(float dt)=0;
			/**
				\brief Atualiza os estado do jogo
				\param dt intervalo de tempo transcorrido desde o último frame.

				Aqui deve ser colocado o código que atualiza o estado do jogo, utilizando dos dados contidos no inputManager, câmera e outras coisas que forem julgadas necessárias.
				É chamadado antes do Update.
			*/
			virtual void EarlyUpdate(float dt);
			/**
				\brief Atualiza os estado do jogo
				\param dt intervalo de tempo transcorrido desde o último frame.

				Aqui deve ser colocado o código que atualiza o estado do jogo, utilizando dos dados contidos no inputManager, câmera e outras coisas que forem julgadas necessárias.
				É chamadado depois do Render.
			*/
			virtual void LateUpdate(float dt);
			/**
				\brief Atualiza active.
			
				Esse método não deve ser sobrescrevido. Também atualiza o status enabled de seus Componenetes.
				Em GameObjects desativados os métodos EarlyUpdate, Update, Render e LateUpdate não são chamados.
			*/
			virtual void UpdateActive(void);
			/**
				\brief Deleta GameObjects.
			
				Deleta GameObjects que solicitaram deleção.
			*/
			void DeleteRequested(void);
			/**
				\brief Renderiza os elmentos do jogo

				Aqui deve ser colocado o código que rederiza os elementos jogo na ordem que for desejada.
			*/
			virtual void Render(void) const;
			/**
				\brief Pausa o State

				Esse método realizar as ações necessárias para pausar o State para que outro State seja empilhado sobre ele.
			*/
			virtual void Pause(void)=0;
			/**
				\brief Despausa o State

				Esse método realizar as ações necessárias para o State seja despausado pois o State que estava sobre ele na pilha foi desempilhado.
			*/
			virtual void Resume(void)=0;
			/**
				\brief Adiciona um GameObject ao vetor de GameObjects

				No processo o ponteiro para esse gameobject é transformado em um unique_ptr.
			*/
			virtual void AddObject(GameObject *object);
			/**
				\brief Informa se o state deve ser desempilhado.

				Verdadeiro se ele deve ser desempilhado, falso caso contrário.
			*/
			bool PopRequested(void);
			/**
				\brief Informa se o state solicitou que o jogo seja fechado.

				Verdadeiro se o State solicitou o encerramento do programa, falso caso contrário.
			*/
			bool QuitRequested(void);
			/** 
				\brief Carrega os assets do estado
				
				Carrega os assets do estado.
			*/
			virtual void LoadAssets(void) const = 0;
		protected:
			bool popRequested;/**< Armazena a informação se o state solicitou desempilhamento.*/
			bool quitRequested;/**< Armazena a informação se o state solicitou que o jogo seja fechado.*/
			std::vector<std::unique_ptr<GameObject>> objectArray;/**< Vetor de GameObjects.*/
	};

}

#include "InputManager.h"

#endif // STATE_H
