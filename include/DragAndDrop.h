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
template <class T>
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
		DragAndDrop(TileMap<T> &map, Vec2 associatedInitialPos, GameObject &associated, bool redrag = true);
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
		GameObject &associated;
		Timer strobe;
		Timer toAppear;
		TileMap<T> &tileMap;/**< TileMap no qual o GameObject será inserido após o drag.*/
		Sound dragNDrop;
		bool isDropping;
		bool dropped;
};

//implementação:

#include "Camera.h"
#include "Error.h"
#include "InputManager.h"
#include "TileMap.h"
#include "Game.h"
#include "Animation.h"

#define STROBBING_TIME 0.5
#define TIME_TO_APPEAR 1

template <class T>
DragAndDrop<T>::DragAndDrop(TileMap<T> &map,Vec2 associatedInitialPos, GameObject &associated, bool redrag)
			: Component(associated)
			, firstClick(false)
			, redrag(redrag)
			, associatedInitialPos(associatedInitialPos)
			, tileMap(map)
			, dragNDrop("audio/Acoes/Consertando1.wav")
			, strobe()
			, toAppear()
			, dragNDrop("audio/Acoes/Consertando1.wav")
			, isDropping(false)
			, dropped(false) {
}

template <class T>
void DragAndDrop<T>::Update(float dt) {
	InputManager &inputManager= InputManager::GetInstance();
	
	if(!redrag){
		if(inputManager.MousePress(LEFT_MOUSE_BUTTON)){
			if(tileMap.InsertGO(&associated)){
				redrag = true;
			}
		}
	}

	else{
		if(firstClick){
			strobe.Update(dt);
			if(strobe.Get() > STROBBING_TIME){
				strobe.Restart();
			}
			// faça a componente sprite piscar

			if(isDropping){
				Game::GetInstance().GetCurrentState().AddObject(new Animation(associatedInitialPos.x, associatedInitialPos.y, associated.rotation, "./img/SpriteSheets/Fumaca_spritesheet.png", 8, 0.1, true));
				toAppear.Update(dt);
				if(toAppear.Get() > TIME_TO_APPEAR){
					Game::GetInstance().GetCurrentState().AddObject(new Animation(associated.box.Center().x, associated.box.Center().y, associated.rotation, "./img/SpriteSheets/Fumaca_spritesheet.png", 8, 0.1, true));
					dropped = true;
					toAppear.Restart();
				}
			}

			if(inputManager.MouseRelease(RIGHT_MOUSE_BUTTON)) {
				isDropping = tileMap.InsertGO(&associated, associatedInitialPos);
				if(isDropping) {
					dragNDrop.Play(1);
					toAppear.Restart();
				}
			}
			
			
			if(dropped){ firstClick = false; }
		} 
		
		if(!firstClick){
			if(inputManager.MouseRelease(RIGHT_MOUSE_BUTTON)) {
				bool taken = false;
				taken = tileMap.InsertGO(&associated, associatedInitialPos);
				if(taken) {
					firstClick = true;
				}
			}
		}
	}
}

template <class T>
bool DragAndDrop<T>::Is(ComponentType type) const {
	return ComponentType::DRAG_AND_DROP == type;
}



#endif // DRAGANDDROP_H
