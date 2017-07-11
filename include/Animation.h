#ifndef ANIMATION_H
#define ANIMATION_H

#include "GameObject.h"
#include "Sprite.h"
#include "Timer.h"

/**
	\brief Classe que modela animações.

	As animações são um tipo de GameObject que não se colidem e cujo objetivo é basicamente rodar uma animação e depois sumir, a não ser no caso dela ter que ser executada indefinidamente em loop.
*/
class Animation: public GameObject {
	public:
		/**
			\brief Construtor
			\param x Coordenada horizontal a partir da qual a animação deve ser renderizada.
			\param y Coordenada vertical a partir da qual a animação deve ser renderizada.
			\param rotation Ângulo de rotação da animação.
			\param sprite Arquivo com spriteSheet da animação.
			\param frameCount quantidade de sub-imagens na animação.
			\param frameTime duração para cada frame da animação.
			\param ends Verdadeiro se a animação deve rodar apenas uma vez, falso se a animação deve ser rodar indefinidamente.

			Instancia uma animação com os argumentos dados. E inicializa um timer para contar por quanto tempo a animação deve rodar antes de ser destruída, no caso sela não ter que ser rodada enternamente.
			Caso a animação não deva rodar eternamente o tempo limite para a existência dessa animação é o produto de frameCount por frameTime.
		*/
		Animation
		(
			float x,
			float y,
			float rotation,
			string sprite,
			int frameCount,
			float frameTime,
			bool ends = true
		);
		/**
			\brief Atualiza estado.

			Atualiza o estado da animação e a contagem de tempo do timer.
		*/
		void Update(float dt);
		/**
			\brief Renderiza animação.

			Renderiza a animação na tela.
		*/
		void Render(void);
		/**
			\brief Informa se o objeto deve ser destruído.
			\return Verdadeiro se o objeto deve ser destruído, falso caso contrário.

			Isso é feito verificando se a animação NÃO deve rodar eternamente, se sim, usamos o timer para verificar se já estourou o tempo para a animação rodar.
		*/
		bool IsDead(void);
		/**
			\brief Solicita que o Animation seja destruído.
			
			Uma vez que esse método é chamado, futuras chamadas ao IsDead devem retornar verdadeiro.
		*/
		void RequestDelete(void);
		/**
			\brief Notifica colisão.

			Como a animação a princípio não interage com os outros gameObjects, o corpo desse método está vazio.
		*/
		void NotifyCollision(GameObject &other);
		/**
			\brief informa o tipo de GameObject
			\return Verdadeiro se o argumento for "Animation", falso caso contrário.

			Usado pelos objetos que colidem com a animação para saber que tipo de GameObject é.
		*/
		bool Is(string type);
		/**
			\brief Obtém Rect informando a posição renderizada da animação.

			Obtém Rect informando a posição renderizada, computando zoom, escala e posição da câmera.
		*/
		Rect GetWorldRenderedRect(void) const;
	private:
		Timer endTimer;/**< Temporizador cronomenta o tempo de existência da animação. Usado para verificar se a animação deve ser destruída.*/
		float timeLimit;/**< Armazena o tempo que animação leva para rodar complemante uma vez. No caso em que a animação não deve rodar em loop esse é o tempo de a animação deve durar.*/
		bool oneTimeOnly;/**< Verdadeiro se essa animação deve ser rodada apenas uma vez, falso se deve ser rodada em loop indefinidamente.*/
		Sprite sp;/**< Sprite sheet da animação.*/
};

#endif // ANIMATION_H
