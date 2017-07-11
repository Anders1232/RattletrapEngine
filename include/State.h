#ifndef STATE_H
#define STATE_H

#include <vector>
#include <memory>
#include "InputManager.h"
#include "Resources.h"

#include "GameObject.h"

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
			\param dt intervalo de tempo transcorrido desde a última atualização.

			Aqui deve ser colocado o código que atualiza o estado do jogo, utilizando dos dados contidos no inputManager, câmera e outras paster que forem julgadas necessárias.
			Pode-se fazer o uso do UpdateArray() por praticidade.
		*/
		virtual void Update(float dt)=0;
		/**
			\brief Renderiza os elmentos do jogo

			Aqui deve ser colocado o código que rederiza os elementos jogo na ordem que for desejada.
			Pode-se fazer o uso do RenderArray por praticidade.
		*/
		virtual void Render(void) const =0;
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
		/**
			\brief Atualiza o estado de cada GameObject no vetor de GameObjects.
			\param dt Tempo transcorrido desde a última atualização.

			O vetor de GameObjects é percorrido completamente e para cada gameObject o método Update é chamado passando como argumento dt.
			Se algum GameObject for considerado morto(IsDead retornar verdadeiro) esse GameObject é removido do vetor de GameObjects.
		*/
		virtual void UpdateArray(float dt);
		/**
			\brief Renderiza cada GameObject no vetor de GameObjects.

			Percorre todos os GameObjects do vetor de GameObjects chamando o método Render de cada um deles.
		*/
		virtual void RenderArray(void) const;
		bool popRequested;/**< Armazena a informação se o state solicitou desempilhamento.*/
		bool quitRequested;/**< Armazena a informação se o state solicitou que o jogo seja fechado.*/
		std::vector<std::unique_ptr<GameObject>> objectArray;/**< Vetor de GameObjects.*/
};

#include "InputManager.h"

#endif // STATE_H
