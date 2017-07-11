#ifndef COLOR_H
#define COLOR_H

#define INCLUDE_SDL
#include "SDL_include.h"

/**
	\brief Struct para armazenar informações de cor

	Cor é uma struct cujo único propósito é permitir o armazenamento de informações de cor em uma variável só.
*/
struct Color {
	public:
		unsigned char r;/**< Canal R */
		unsigned char g;/**< Canal G */
		unsigned char b;/**< Canal B */
		unsigned char a;/**< Canal A */
		/**
			\brief Cria um tipo para armazenar a cor [R,G,B,A]
			\param Intensidade do Canal Vermelho (Red) da cor
			\param Intensidade do Canal Verde (Green) da cor
			\param Intensidade do Canal Azul (Blue) da cor
			\param Intensidade do Canal Alfa (Alpha) da cor

			Essa struct representa uma cor de 8 bits. Ou seja, cada canal pode receber um número entre 0 (inclusive) e 255 (inclusive).
		*/
		Color( unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255 );
		operator SDL_Color();
};

#endif // COLOR_H
