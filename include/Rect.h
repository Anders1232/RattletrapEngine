#ifndef RECT_H
#define RECT_H

#define INCLUDE_SDL 
#define INCLUDE_SDL_IMAGE 
#include "SDL_include.h"

#include "Vec2.h"

/**
	\brief Classe que modela um rect
	
	Rect(como implementado) é um retângulo existente num plano cartesiano.
	É usado para várias coisas, como por exemplo as posições dos GameObjects na tela, corte de sprites em spriteSheets e tileSets.
	A partir da coordenada de início o rect cresce para direita e para baixo.
*/
class Rect {
	public:
		/**
			\brief Construtor
			
			Construtor padrão que inicia todos os membros com zero.
		*/
		Rect(void);
		/**
			\brief Construtor
			
			Construtor padrão que inicializa todos os membros com os valores informados.
		*/
		Rect(float x, float y, float w, float h);
		/**
			\brief Sobrecarga do operador de typecast para SDL_Rect.
			
			Retorna um SDL_Rect a partir do Rect.
		*/
		operator SDL_Rect() const;
		/**
			\brief Sobrecarga do operador de typecast para Vec2.
			
			Retorna um Vec2 a partir do membros x e y do Rect.
		*/
		operator Vec2 () const;
		/**
			\brief Sobrecarga do operador de soma entre um Rect e um Vec2.
			\return Resultado da soma
			
			Retorna um Rect cujo x é a soma dos x e y do rect e do vect e w e h do rect usado no cálculo.
			Observação: O rect usado no argumento não é alterado!
		*/
		Rect operator+(Vec2 const &a) const;
		/**
			\brief Sobrecarga do operador de subtração entre um Rect e um Vec2.
			\return Resultado da diferença
			
			Retorna um Rect cujo x é a diferança dos x e y do rect e do vect e w e h do rect usado no cálculo.
			Observação: O rect usado no argumento não é alterado!
		*/
		Rect operator-(Vec2 const &a) const;
//		SDL_Rect operator-(SDL_Rect const &b) const;
		/**
			\brief Sobrecarga do operador de atribuição de um vec2 para um Rect.
			
			O x e y do Vec2 são atribuídos aos x e y do Rect.
		*/
		Rect operator=(Vec2 const &a);
		/**
			\brief Sobrecarga do operador de multiplicação entre um Rect e um float.
			\return Produto do Rect pelo float.
			
			Retorna um Rect cujo todos os membros são multiplicados pelo float informado.
			Observação: O rect usado no argumento não é alterado!
		*/
		Rect operator*(float const zoom)const;
		/**
			\brief Obtém o centro do Rect.
			\return Vec2 do centro do Rect.
			
			Retorna um Vec2 com as coordenadas do centro do Rect.
		*/
		Vec2 Center(void) const;
		/**
			\brief Atribui a largura e a altura do Rect a partir do Vec2.
			\todo Transformar isso em const.
			
			Atribui ao w do rect o x do Vec2 e atribui ao h o y do Vec2.
		*/
		void SetWidthAndHeight(Vec2 const &vec);
		float x;/**< Coordenada x de início do rect.*/
		float y;/**< Corrdenada y de início do rect.*/
		float w;/**< Largura do Rect.*/
		float h;/**< Altura do Rect.*/
};

#include <cmath>

#endif // RECT_H
