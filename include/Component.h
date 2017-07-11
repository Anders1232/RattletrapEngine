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
		/**
			\brief Atualiza estado do componente.
		
			Deve conter a lógica principal provida pelo componente, utilizando o gameObject que o contém sempre que necessário.
		*/
		virtual void Update(float dt)=0;
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
		virtual void Render(void){};
};

#endif
