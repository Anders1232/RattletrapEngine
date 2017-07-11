#ifndef VEC2OP_H
#define VEC2OP_H

#define INCLUDE_SDL 
#include "SDL_include.h"

/**
	\brief Classe que modela um vetor(no sentido físico)

	Vec2 é uma ED que descreve um vetor internamente pelas coordenadas (x,y). Também serve para definir pontos em um plano bidimensional.
*/
class Vec2 {
	public:
		/**
			\brief Construtor padrão
		
			Inicializa seus atributos com zero.
		*/
		Vec2(void);
		/**
			\brief Construtor
		
			Inicializa seus atributos com os argumentos passados.
		*/
		Vec2(float x, float y);
		/**
			\brief Constrói um Vec2 a partir de coordenadas polares.
		
			Cria um Vec2 com a magnetude informada e então rotaciona o ângulo informado.
		*/
		static Vec2 FromPolarCoord(float magentude, float angle);
		/**
			\brief Copia um Vec2
		
			Cria um novo Vec2 com os atributos do primeiro.
		*/
		Vec2(Vec2 const &b);
		/**
			\brief Sobrecarga do operador de soma entre dois Vec2.
			\return Resultado da soma
			
			Retorna um Vec2 cujo x é a soma dos x dos dois Vec2 e o y é a soma do y dos dois Vec2.
			Observação: Nennhum dos Vec2 sobre qual essa operação opera é alterado!
		*/
		Vec2 operator+(Vec2 const &b)const;
		/**
			\brief Sobrecarga do operador de subtração entre dois Vec2.
			\return Resultado da subtração
			
			Retorna um Vec2 cujo x é a diferença dos x dos dois Vec2 e o y é a diferença do y dos dois Vec2.
			Observação: Nennhum dos Vec2 sobre qual essa operação opera é alterado!
		*/
		Vec2 operator-(Vec2 const &b) const;
		/**
			\brief Sobrecarga do operador de multiplicação entre de um Vec2 por um float.
			\return Produto de Vec2 por escalar.
			
			O cálculo do produto é feito de forma na qual a magnetude do vetor é multiplicada pelo argumento sem que sua angulação seja alterada.
			Observação: Nennhum dos Vec2 sobre qual essa operação opera é alterado!
		*/
		Vec2 operator*(float b)const;
		/**
			\brief Copy Assignment.
			
			Atribuição na quao valor de um Vec2 é atribuido à outro. Para mais informações veja http://www.cplusplus.com/doc/tutorial/classes2/.
		*/
		Vec2& operator=(Vec2 const &b);
		/**
			\brief Sobrecarga do operador += entre dois Vec2.
			
			A mesma coisa de x= x+b(dado que x e b são Vec2).
		*/
		Vec2& operator+=(Vec2 const &b);
		/**
			\brief Sobrecarga do operador -= entre dois Vec2.
			
			A mesma coisa de x= x-b(dado que x e b são Vec2).
		*/
		Vec2& operator-=(Vec2 const &b);
		/**
			\brief Verifica de dois Vec2 possuem os mesmos valores.
			\return Verdadeiro se sim, falso caso contrário
			
			Verifica se os componentes x e y dos vetores são iguais entre os dois Vec2.
		*/
		bool operator==(Vec2 const &b) const;
		/**
			\brief Verifica de dois Vec2 possuem valores diferentes.
			\return Verdadeiro se sim, falso caso contrário
			
			Verifica se pelo menos um dos componentes x e y dos vetores é diferentes entre os dois Vec2.
		*/
		bool operator!=(Vec2 const &b) const;
		/**
			\brief Multiplicação memberwise entre dois Vec2
			
			Cria um Vec2 cujo x é o produto do x dos dois operando e y é o produto de y dos dois operandos.
		*/
		Vec2 MemberMult(float const &b)const;
		/**
			\brief Magnetudo do Vec2
			
			Retorna a magnetude do Vec2, obtido graças aos estudos de Pitágoras.
		*/
		float Magnitude(void)const;
		/**
			\brief Normaliza o Vec2
			\todo Discutir se o coloca como const.
			
			Faz com que o vetor tenha a mesma direção-sentido porém a magnetude é unitária.
		*/
		Vec2 Normalize(void);
		/**
			\brief Rotaciona o Vec2.
			\param angle Ângulo de rotação.
			
			Retorna um Vec2 rotacionado em angle.
			Observação: O Vec2 sobre qual essa operação opera não é alterado!
		*/
		Vec2 Rotate(float angle) const;
		/**
			\brief Calcula a distância entre dois Vec2.
			\param b Vec2 com o qual quer se medir a distância.
			
			Retorna a distância entre os dois Vec2. Essa distância é sempre positiva.
		*/
		float DistanceTo(Vec2 const &b) const;
		/**
			\brief Calcula o vetor distância entre dois Vec2.
			\param b Vec2 com o qual quer se medir a distância.
			
			Retorna o vetor distância entre os dois Vec2.
		*/
		Vec2 VecDistance(Vec2 destino);
		/**
			\brief Informa a angulação do vetor.
			
			Retorna a angulação do Vec2.
		*/
		float Inclination(void) const;
		/**
			\brief Informa a angulação do vetor distância entre os dois vetores.
			
			Retorna a angulação do vetor distância entre os dois vetores.
		*/
		float Inclination(Vec2 const &b) const;
		/**
			\brief Verifica se o Vec está no Rect.
			\param rect Rect o qual se deseja verifica e o ponto dado por Vec entá nele.
			
			Retorna verdadeiro se o ponto dado pelo Vec2 estiver no Rect, falso caso contrário.
		*/
		bool IsInRect(SDL_Rect const &rect) const;
		float x;/**< Coordenada x do vetor/ponto.*/
		float y;/**< Coordenada y do vetor/ponto.*/
};

#define MARGEM_ERRO_COMPARACAO (0.1)

#endif // VEC2OP_H
