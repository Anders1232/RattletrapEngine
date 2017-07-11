#ifndef UIBUTTON_H
#define UIBUTTON_H

#include "Rect.h"
#include "Vec2.h"

#include <string>

/**
	\brief Classe virtual que modela o comportamento de um botão na interface gráfica

	Essa class tem como responsabilidade modelar todo o comportamento esperado de um botão na interface gráfica sem se preocupar com nenhuma parte da renderização.

	Ela possui callbacks que podem ser chamados a cada mudança de estado do botão. Esses callbacks são de ponteiros para função que não retornem nada mas recebem um ponteiro para void*.
	Essas funções podem ser Clojures (funções lambda). Esse ponteiro void* pode ser usado para se manipular o objeto que se desejar dentro da callback, desde que o cast seja feito corretamente.
*/
class UIbutton {
	public:
		/**
			\brief Possíveis estados que um botão pode assumir

			Um botão pode estar em qualquer uma dos seguintes estados:
			- Disabled: O botão está desativado e, portanto, não pode sofrer interação
			- Enabled: O botão está ativado e, portanto, pode sofrer interação. Esse é o estado padrão de um botão.
			- Highlighted: O botão está sendo destacado. Usado para quando o mouse está sobre ele, ou também, para a navegação seja com teclas, esse botão seja o ativo.
			- Pressed: O botão está sendo pressionado. Usado quando o mouse está clicado sobre ele, ou também, para a navegação seja com teclas, esse botão seja pressionado.
		*/
		enum State : int {
				DISABLED = 0,
				ENABLED,
				HIGHLIGHTED,
				PRESSED
		};
		typedef void (*ButtonCallback) (void*); /**< O ponteiro para função do tipo que o callback chamará. */
		/**
			\brief Seta o callback a ser chamado para cada estado.

			\param stateToSet Uma enumeração State que indica qual estado deve ter seu callback alterado.
			\param caller Um ponteiro void* que será passado ao callback para que possa se modificar coisas externas à função. Geralmente é usado this.
			\param callback Um ponteiro para função do tipo void (*)(void*).

			A função passada na variável callback será chamada imediatamente assim que a transição de estado for finalizada.
		*/
		void SetCallback(UIbutton::State stateToSet, void* caller, ButtonCallback callback);
		/**
			\brief Seta o callback a ser chamado ao clicar no botão.

			\param caller Um ponteiro void* que será passado ao callback para que possa se modificar coisas externas à função. Geralmente é usado this.
			\param callback Um ponteiro para função do tipo void (*)(void*).

			A função passada na variável callback será chamada imediatamente assim que o botão for clicado.

			É usado um método diferente para setar esse callback pois ele não se trata de notificação de mudança de estado mas sim de ação com relação ao botão.
		*/
		void SetClickCallback(void* caller, ButtonCallback callback);
		/**
			\brief Solicita ao botão que transite para outro estado

			\param newState Uma enumeração que indica qual é o novo estado desejado para o botão

			Realiza todo o processamento interno para a transição de estados e, por último, chama o callback do novo estado.
		*/
		virtual void SetUIbuttonState(UIbutton::State newState);
		/**
			\brief Retorna o estado atual do botão.

			\return O estado atual do botão

			Retorna qual é o estado que o botão se encontra no momento.
		*/
		UIbutton::State GetUIbuttonState(void) const;
		/**
			\brief Realiza o processo interno de clique.

			Basicamente, chama o callback de clique, caso tenha sido setado.
		*/
		void Click();
		/**
			\brief Checa qual o tipo do objeto

			\return True caso a string seja o nome dessa classe ou de qualquer uma que ela herde.
		*/
		virtual bool Is(std::string UItype) const;
		bool interactOnBoundingBox;
	protected:
		/**
			\brief Construtor protegido para não permitir que seja diretamente instanciado.

			Essa classe NÃO deve ser diretamente instanciada. Somente as classes que herdam dela.
		*/
		UIbutton(UIbutton::State initialState = UIbutton::State::ENABLED, bool interactOnBoundingBox = true);
		void Update(float dt, bool mouseIsInside);
		UIbutton::State actualState; /**< Armazena o estado atual do botão. */
		ButtonCallback disabledCallback; /**< Armazena um ponteiro para função a ser chamada quando o botão for desativado. */
		ButtonCallback enabledCallback; /**< Armazena um ponteiro para função a ser chamada quando o botão for ativado. */
		ButtonCallback highlightedCallback; /**< Armazena um ponteiro para função a ser chamada quando o botão for destacado. */
		ButtonCallback pressedCallback; /**< Armazena um ponteiro para função a ser chamada quando o botão for pressionado. */
		ButtonCallback clickedCallback; /**< Armazena um ponteiro para função a ser chamada quando o botão for clicado. */
		void* disableCaller; /**< Armazena um ponteiro a ser passado para o callback de desativação. Chamado de caller pois geralmente é o que é usado para representar. */
		void* enabledCaller; /**< Armazena um ponteiro a ser passado para o callback de ativação. Chamado de caller pois geralmente é o que é usado para representar. */
		void* highlightedCaller; /**< Armazena um ponteiro a ser passado para o callback de destaque. Chamado de caller pois geralmente é o que é usado para representar. */
		void* pressedCaller; /**< Armazena um ponteiro a ser passado para o callback de pressionamento. Chamado de caller pois geralmente é o que é usado para representar. */
		void* clickCaller; /**< Armazena um ponteiro a ser passado para o callback de clique. Chamado de caller pois geralmente é o que é usado para representar. */
};

#endif // UIBUTTON_H
