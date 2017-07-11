#ifndef SOUND_H
#define SOUND_H

#define INCLUDE_SDL 
#define INCLUDE_SDL_IMAGE 
#define INCLUDE_SDL_MIXER 
#include "SDL_include.h"

#include <memory>

#include <string>

using std::string;

/**
	\brief Classe que modela sons.

	Modela sons e permite operações básicas como: abiri um arquivo de som, tocar som e para som(stop).
	É permitido até a reprodução de 32 sons simultâneos(além da música). Cada som é tocado num canal diferente.
*/
class Sound {
	public:
		/**
			\brief Construtor padrão.
			\todo Verificar a necessidade da existência desse contrutor.

			Cria uma instância não associada a nenhum som. O uso da instância nesse estado pode causar problemas.
		*/
		Sound();
		/**
			\brief Construtor.
			\param file Arquivo com som a ser utilizado.

			Cria uma instância associando-a um som. Todas as funcionalidades tornam-se disponíveis.
		*/
		Sound(string file);
		/**
			\brief Toca o som.
			\param times Número de vezes que a música deve repetir.

			Toca o som, num canal arbritário. Caso essa instância de Sound já esteja com um som sendo tocado, esse som é interrompido.
			Caso times seja 0, o sem será tocado indefinidamente.
		*/
		void Play(int times);
		/**
			\brief Interrompe o som.

			Interrompe a eecução do som associado à instância.
		*/
		void Stop(void);
		/**
			\brief Carrega um som.
			\param file Arquivo com o som a ser utilizado.

			Associa a instância ao som contido no arquivo. Caso essa instância de Sound já esteja com um som sendo tocado, esse som é interrompido para que depois o novo som seja carregado.
		*/
		void Open(string file);
		/**
			\brief Verifica se tem um arquivo aberto
			\param return verdadeiro se essa instância está associada a um som, falso caso contrário.

			Verifica se a instância de Sound está associada ao som de algum arquivo.
		*/
		bool IsOpen(void) const;
	private:
		std::shared_ptr<Mix_Chunk> sound; /**< Ponteiro para o Mix_Chunk com o som.*/
		int channel;/**< Canal no qual o som está sendo tocado. -1 caso o som não esteja sendo tocado. O valor tem que ser -1 pois esse valor indica na hora da reprodução do som que p sistema pode escolher o canal no qual o som deve ser executado.*/
};

#endif // SOUND_H
