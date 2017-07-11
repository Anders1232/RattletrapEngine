#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#define INCLUDE_SDL 
#define INCLUDE_SDL_IMAGE 
#include "SDL_include.h"

#include "Vec2.h"
#include <unordered_map>
#include <cstdint>

#define INPUT_MANAGER InputManager::GetInstance()

#define LEFT_ARROW_KEY SDLK_LEFT
#define RIGHT_ARROW_KEY SDLK_RIGHT
#define UP_ARROW_KEY SDLK_UP
#define DOWN_ARROW_KEY SDLK_DOWN
#define ESCAPE_KEY SDLK_ESCAPE
#define LEFT_ALT_KEY SDLK_LALT
#define LEFT_CTRL_KEY SDLK_LCTRL
#define LEFT_SHIFT_KEY SDLK_LSHIFT
#define ESPACE_KEY SDLK_SPACE
#define LEFT_MOUSE_BUTTON SDL_BUTTON_LEFT
#define RIGHT_MOUSE_BUTTON SDL_BUTTON_RIGHT
#define MIDDLE_MOUSE_BUTTON SDL_BUTTON_MIDDLE

/**
	\brief Classe que organiza entradas do usuário.

	O InputManager organiza internamente as várias formas om as quais o usuário pode interafir com o programa. Sendo elas:
		- Botões do teclado
		- Posição do mouse
		- Cliques do mouse
		- Scroll do mouse
	Observação: Os métodos que trabalham com o teclado precisam ter seus índices ajustados conforme o documentado em keyState. Para tal a função macro AJUST_KEY foi feita.
*/
class InputManager {
	public:
		/**
			\brief Atualiza as informações internas.
			\todo Verificar se o keyUpdate e o mouseUpdate devem ser atualizados nos casos de SDL_KEYUP e SDL_MOUSEBUTTONUP, respectivamente.

			As seguintes ações são feitas:
				- As coordenas do mouse na tela são atualizadas atraveś da chamada à SDL_GetMouseState passando como argumentos o endereço de mouseX e o endereço mouseY.
				- quitRequested é setado para falso, pois se ele armazenava o valor verdadeiro ele pedido para fechar já foi tratado no frame anterior.
				- o updateCounter é incrementado, o que faz que os inputs de frames anteriores sejam tratados como tal ao se comparar o valor de updateCounter com <input>Update.
				- Faz-se um loop de SDL_Event passando por todos os eventos que ocorram no último frame, nesse loop interno faz-se as seguintes computações:
					- Se o evento for SDL_KEYDOWN e não for repetido de algum frame anterior; atribui-se verdadeiro para o keyState da tecla correspondente e o keyUpdate de tecla em questão recebe o valor de updateCounter.
					- Se o evento for SDL_KEYUP e não for repetido de algum frame anterior; atribui-se false para o keyState da tecla correspondente e o keyUpdate de tecla em questão recebe o valor de updateCounter.
					- Se o evento for SDL_MOUSEBUTTONDOWN e não for repetido de algum frame anterior; atribui-se verdadeiro para o mouseState do botão correspondente e o mouseUpdate do botão em questão recebe o valor de updateCounter.
					- Se o evento for SDL_MOUSEBUTTONUP e não for repetido de algum frame anterior; atribui-se false para o mouseState do botão correspondente e o mouseUpdate do botão em questão recebe o valor de updateCounter.
					- Se o evento for SDL_QUIT o valor verdadeiro é atribuído para quitRequested.
					- Se o evento for SDL_MOUSEWHEEL atribui-se o deslocamente feito no scrool à mouseScroolState e o mouseScroolUpdate de tecla em questão recebe o valor de updateCounter.
		*/
		void Update(void);
		/**
			\brief Informa se uma tecla do teclado foi pressionada no frame corrente
			\param key Tecla do teclado cujo o estado é requierido
			\return Booleano que informa se a tecla foi pressionada ou não no frame corrente.

			Primeiramente, converte-se o argumento a um válido no vetor através da função macro AJUST_KEY. Retorna verdadeiro se o keyState da tecla for verdadeiro E o keyUpdate da tecla for igual ao updateCounter.
			Caso contrário retorna-se o valor falso.
		*/
		bool KeyPress(int key) const;
		/**
			\brief Informa se uma tecla do teclado foi solta no frame corrente
			\param key Tecla do teclado cujo o estado é requierido
			\return Booleano que informa se a tecla foi solta ou não no frame corrente.

			Primeiramente, converte-se o argumento a um válido no vetor através da função macro AJUST_KEY. Retorna verdadeiro se o keyState da tecla for falso E o keyUpdate da tecla for igual ao updateCounter.
			Caso contrário retorna-se o valor falso.
		*/
		bool KeyRelease(int key) const;
		/**
			\brief Informa se uma tecla do teclado está pressionada no momento
			\param key Tecla do teclado cujo o estado é requierido
			\return Booleano que informa se a tecla está pressionada ou não.

			Primeiramente, converte-se o argumento a um válido no vetor através da função macro AJUST_KEY. Retorna verdadeiro se o keyState da tecla for verdadeiro e isso ocorrer nesse frame.
			Caso contrário retorna-se o valor falso.
		*/
		bool IsKeyDown(int key) const;
		/**
			\brief Informa se uma tecla do teclado NÃO está pressionada no momento
			\param key Tecla do teclado cujo o estado é requierido
			\return Booleano que informa se a tecla está pressionada ou não.

			Primeiramente, converte-se o argumento a um válido no vetor através da função macro AJUST_KEY. Retorna verdadeiro se o keyState da tecla for falso.
			Caso contrário retorna-se o valor falso.
		*/
		bool IsKeyUp(int key) const;
		/**
			\brief Informa se um botão do mouse foi pressionado no frame corrente
			\param key Botão do mouse cujo o estado é requierido
			\return Booleano que informa se o botão foi pressionado ou não no frame corrente.

			Retorna verdadeiro se o keyState da tecla for verdadeiro E o mouseUpdate do botão for igual ao updateCounter.
			Caso contrário retorna-se o valor falso.
			Recomenda-se o uso das macros LEFT_MOUSE_BUTTON, RIGHT_MOUSE_BUTTON e MIDDLE_MOUSE_BUTTON.
		*/
		bool MousePress(int button) const;
		/**
			\brief Informa se um botão do mouse foi solto no frame corrente
			\param key Botão do mouse cujo o estado é requierido
			\return Booleano que informa se o botão foi solta ou não no frame corrente.

			Retorna verdadeiro se o keyState da tecla for falso E o mouseUpdate do botão for igual ao updateCounter.
			Caso contrário retorna-se o valor falso.
			Recomenda-se o uso das macros LEFT_MOUSE_BUTTON, RIGHT_MOUSE_BUTTON e MIDDLE_MOUSE_BUTTON.
		*/
		bool MouseRelease(int button) const;
		/**
			\brief Informa se um botão do mouse está pressionada no momento
			\param key Botão do mouse cujo o estado é requierido
			\return Booleano que informa se o botão está pressionado ou não.

			Retorna verdadeiro se o mouseState do botão for verdadeiro.
			Caso contrário retorna-se o valor falso.
			Recomenda-se o uso das macros LEFT_MOUSE_BUTTON, RIGHT_MOUSE_BUTTON e MIDDLE_MOUSE_BUTTON.
		*/
		bool IsMouseDown(int button) const;
		/**
			\brief Informa se um botão do mouse NÃO está pressionada no momento
			\param key Botão do mouse cujo o estado é requierido
			\return Booleano que informa se o botão está pressionado ou não.

			Retorna verdadeiro se o mouseState do botão for falso.
			Caso contrário retorna-se o valor falso.
			Recomenda-se o uso das macros LEFT_MOUSE_BUTTON, RIGHT_MOUSE_BUTTON e MIDDLE_MOUSE_BUTTON.
		*/
		bool IsMouseUp(int button) const;
		/**
			\brief Informa se o scroll dp do mouse foi acionado no frame atual
			\return Booleano que informa se o scroll está em uso ou não.

			Retorna verdadeiro se o mouseScroolUpdate for igual a updateCounter.
			Caso contrário retorna-se o valor falso.
		*/
		bool IsMouseScrolling(void) const;
		/**
			\brief Informa o valor do eixo X do mouse
			\return valor X do mouse na janela

			Esse valor é zero na borda esquerda da janela, cresce para a direita, decresce para a esquerda.
		*/
		int GetMouseX(void) const;
		/**
			\brief Informa o valor do eixo Y do mouse
			\return valor Y do mouse na janela

			Esse valor é zero na borda superior da janela, cresce para a baixo, decresce para a cima.
		*/
		int GetMouseY(void) const;
		/**
			\brief Informa em um Vec2 a posição (x, y) do mouse
			\return Valor (x, y) do mouse

			O eixo horizontal cresce para a direira e o eixo vertical cresce para baixo.
			O eixo horizontal decresce para a esquerda e o eixo vertical decresce para cima.
		*/
		Vec2 GetMousePos(void) const;
		/**
			\brief Informa em um Vec2 o último scrool do mouse feito.
			\return Valor (x, y) do scroll do mouse
			\todo Fazer a correção dos sentidos de acordo com a direções. Essa correção só pode ser feita após a versão 2.0.4 da SDL2. Veja https://wiki.libsdl.org/SDL_MouseWheelEvent

			O eixo horizontal cresce para a direira e o eixo vertical cresce para baixo.
			O eixo horizontal decresce para a esquerda e o eixo vertical decresce para cima.
		*/
		Vec2 MouseScroll(void) const;
		/**
			\brief Informa se o usuário solicitou a saída do programa.
			\return Booleano com a informação solicitada.

			Retorna verdadeiro se o evento SDL_QUIT ocorreu no frame atual.
			Caso contrário retorna falso.
		*/
		bool QuitRequested(void) const;
		/**
			\brief Obtém referência para a instância estática do Inputmanager.
			\return Referência para a instância estática do InputManager

			O InputManager é um singleton, esse método retorna a instância estática do mesmo. Dentro do corpo desse método está a declaração dessa instância que é estática(no sentido de programação em C).
			Essa instância estática é retornada.
		*/
		static InputManager& GetInstance(void);

		/**
			\brief Informa se um botão do controle foi pressionado no frame corrente
			\param button Botão do controle cujo o estado é requerido
			\return Booleano que informa se o botão foi pressionado ou não no frame corrente.

			Retorna verdadeiro se o controllerState do botão for falso E o mouseUpdate do botão for igual ao updateCounter.
			Caso contrário retorna-se o valor falso.
		*/

		bool ButtonPress(int button) const;
		/**
			\brief Informa se um botão do controle foi solto no frame corrente
			\param button Botão do controle cujo o estado é requierido
			\return Booleano que informa se o botão foi solto ou não no frame corrente.

			Retorna verdadeiro se o controllerState do botao for falso E o controllerUpdate do botão for igual ao updateCounter.
			Caso contrário retorna-se o valor falso.
		*/
		bool ButtonRelease(int button) const;
		/**
			\brief Informa se um botão do controle está pressionada no momento
			\param button Botão do mouse cujo o estado é requierido
			\return Booleano que informa se o botão está pressionado ou não.

			Retorna verdadeiro se o controllerState do botão for verdadeiro.
			Caso contrário retorna-se o valor falso.
			
		*/
		bool IsButtonDown(int button) const;
		/**
			\brief Informa se um botão do controle está NÃO pressionada no momento
			\param button Botão do mouse cujo o estado é requierido
			\return Booleano que informa se o botão está pressionado ou não.

			Retorna verdadeiro se o controllerState do botão for falso.
			Caso contrário retorna-se o valor falso.
			
		*/
		bool IsButtonUp(int button) const;
		/**
			\brief Calcula o deslocamento do sticker esquerdo de um controle
			\return Vetor bidimensional que informa a posição (x,y) do analógico esquerdo.
			
		*/
		Vec2 GetControllerLeftStickState() const;
		/**
			\brief Calcula o deslocamento do sticker direito de um controle
			\return Vetor bidimensional que informa a posição (x,y) do analógico direito.
			
		*/
		Vec2 GetControllerRightStickState() const;
		/**\brief Informa se algum sticker do controle está sendo deslocado nesse frame
			\return Booleano que informa se algum sticker está em uso ou não.

			Retorna verdadeiro se o mouseScroolUpdate for igual a updateCounter.
			Caso contrário retorna-se o valor falso.
		*/
		bool IsControllerSticking(void) const;

	private:
		/**
			\brief Inicializa os atributos

			Todos os atributos internos não iniciados com zero ou falso, dependendo do tipo.
		*/
		InputManager();
		/**
			\brief Destrutor

			Como todos os atributos dessa classe são alocados estaticamente, nada precisa ser feito.
		*/
		~InputManager();
		bool mouseState[6];/**< Armazena a informação dos estados botões do mouse(pressionado ou não pressionado).*/
		int mouseUpdate[6];/**< Armazena a informação de quando houve alguma modificação em cada posição do mouseState.*/
		bool quitRequested;/**< Armazena a informação se botão de sair ou Alt+F4 foi pressionado.*/
		int updateCounter;/**< Armazena a informação de quantas vezes o update foi chamada. É usado nos <input>Update para saber se a modificação no estado foi detectada no frame corrente.*/
		int mouseX;/**< Armazena a posição X do mouse na janela.*/
		int mouseY;/**< Armazena a posição Y do mouse na janela.*/
		std::unordered_map<int,bool> keyState;/**< Armazena as informações de quais botões do teclado estão pressionados. As 127 primeiras posições são relativas às suas correspondentes ASCII. As outras posições rastreiam o estado das teclas não-ASCII, como ctrl, shift, caps lock, F1...F12, etc.*/
		std::unordered_map<int,int>keyUpdate;/**< Armazena as informações de quando os estados dos botões no respectivo keyState foram modificadas. É usado junto com o updateCounter para saber quão recente a informação é.*/
		std::unordered_map<int,SDL_GameController*>padToController;/**< Armazena a relação entre o indice de um controle e o seu referente ponteiro do tipo SDL_GameController**/
		
		std::unordered_map<int,bool> controllerState;/**< Armazena as informações de quais botões do controle estão pressionados.*/
		std::unordered_map<int,int>	controllerUpdate;/**< Armazena as informações de quando os estados dos botões do controle no respectivo keyState foram modificadas. É usado junto com o updateCounter para saber quão recente a informação é.*/
		Vec2 controllerLeftStickState;/**< Armazena a informação de em qual timestamp(updateCounter) o controllerLeftStickState foi alterado.*/
		int controllerStickUpdate;/**< Armazena a informação do último stick feito no controle.*/
		Vec2 controllerRightStickState;/**< Armazena a informação de em qual timestamp(updateCounter) o controllerStickState foi alterado.*/
		int mouseScroolUpdate;/**< Armazena a informação de em qual timestamp(updateCounter) o mouseScrollState foi alterado.*/
		Vec2 mouseScroolState;/**< Armazena a informação do último scroll feito no mouse.*/
};

#endif // INPUTMANAGER_H
