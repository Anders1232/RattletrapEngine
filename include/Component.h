#ifndef COMPONENT_H
#define COMPONENT_H

#include "ComponentType.h"
#include "GameObject.h"
#include "ComponentType.h"

/**
	\brief Tipos de Componentes

	Enumeração com os tipos de componentes possíveis, usado no método Is que todo componente deve implementar.
*/


#ifndef GAME_OBJECT
class GameObject;
#endif


#define COMPONENT
/**
	\brief Modela os componentes.

	Componentes são objetos variados que adicionam funcionalidades variadas ao GameObject que os contém.
*/
class Component{
	public:
		Component(GameObject &associated);
		/**
			\brief Atualiza estado do componente.

			Deve conter a lógica principal provida pelo componente, utilizando o GameObject que o contém sempre que necessário.
			Sugere-se que qualquer lógica que que possa solicitar remoção de um GameObject fique no método EarlyUpdate.
		*/
		virtual void Update(float dt)=0;
		/**
			\brief Atualiza estado do componente.

			Recomendado para conter lógica que solicita reoção de GameObjects. É executado todo frame antes do Update.
		*/
		virtual void EarlyUpdate(float dt){};
		/**
			\brief Atualiza estado do componente.

			É executado todo frame após o Render.
		*/
		virtual void LateUpdate(float dt){};
		/**
			\brief Ativa ou desativa o Componente.

			Essa modificação só passa a valer a partir do frame seguinte.
			Em componentes desativados os métodos EarlyUpdate, Update, Render e LateUpdate não são chamados.
		*/
		void SetEnable(bool newValue);
		/**
			\brief Informa se o componente está ativo.

			Em componentes desativados os métodos EarlyUpdate, Update, Render e LateUpdate não são chamados.
		*/
		bool IsEnabled(void) const;
		/**
			\brief Atualiza enable.

			Esse método não deve ser sobrescrevido.
			Em componentes desativados os métodos EarlyUpdate, Update, Render e LateUpdate não são chamados.
		*/
		void UpdateEnable(void);
		/**
			\brief Verifica o subtipo de componente.
			\param type Tipo que deseja verificar se o componente é.

			Retorna verdadeiro se o componente for do tipo type, falso caso contrário.
		*/
		virtual bool Is(ComponentType type) const =0;
		/**
			\brief Destrutor

			Declaração necessária para que não houvesse undefined behaviour na deleção dos filhos de Component, pois não estando declarado seria assumido a existência do construtor default aqui(que não seria virtual)
		*/
		virtual ~Component(void){};
		virtual void Render(void) const {};
		virtual void ButtonObserver(Component* btn){};

	protected:
		GameObject &associated;
		bool enabled;/**<Informa Se o gameObject está ativo ou não*/
		bool newEnabled;/**< Informa se esse GO estará ativo no próximo frame. Feito para que o GO não mude de ativo para inativo no decorrer de um frame*/
};

#endif
