#ifndef DRAGANDDROP_H
#define DRAGANDDROP_H

#include "Component.h"
#include "TileMap.h"
#include "Sound.h"
#include "Timer.h"
/**
	\brief Classe responsável por arrastar e posicionar Objetos.

	O DragAndDrop é um tipo de Componente. O objetivo do DragAndDrop é selecionar um objeto arrastavel por meio de Input e posicioná-lo em uma tile. E assim, atualizar o mapa de Tiles.
*/
class DragAndDrop : public Component{
	public:
		/**
			\brief Construtor.
			\param map Usado para inserir o objeto no tileMap.
			\param draggin Informa se o objeto esta sendo arrastado.
			\param redrag Verdadeiro se for um redrag, falso caso seja um drag inicial.
			\param forceDrag
		
			Instancia o DragAndDrop, informando se o objeto está sendo arrastado.
		*/
		DragAndDrop(TileMap &map, Vec2 associatedInitialPos, GameObject &associated, bool redrag = true);
		/**
			\brief Atualiza o estado do objeto em arrasto.
			
			Nesse método a lógica do drag and drop é implementada. Se o botão do drag estiver sendo segurado o objeto é arrastado mantendo seu centro onde o mouse está. Quando o botão relativo ao drag and drop é liberado chama-se o tileMap para iserir o GameObject no tile onde o mouse está.
		*/
		void Update(float dt);
		/**
			\brief Informa que o tipo deste Compomente é DragAndDrop.
			\param CompomentType A ser comparado se é igual ao Tipo desta classe.
			
			Retorna Verdadeiro se CompomentType também é do tipo DragAndDrop, caso contrário retorna falso.
		*/
		bool Is(ComponentType) const;
		bool firstClick;
		bool redrag;/**<Informa se é um drag inicial ou um redrag*/
	private:
		Vec2 associatedInitialPos;/**<Vec2 no qual o será armazenada a posição do objeto associado no instante do drag.*/
		TileMap &tileMap;/**< TileMap no qual o GameObject será inserido após o drag.*/
		GameObject &associated;
		Timer strobe;
		Timer toAppear;
		Sound dragNDrop;
		bool isDropping;
		bool dropped;
};

#endif // DRAGANDDROP_H
