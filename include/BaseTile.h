#ifndef BASETILE_H
#define BASETILE_H

#include "Error.h"

namespace RattletrapEngine {

	/**
		\brief Classe que especifica a interface mínima que um tile da classe TileMap deve ter.

		Não é obrigatório que o a classe que será o Tile do jogo herde desta, mas é recomendado.
	*/
	class BaseTile{
		public:
			/**
				\brief Construtor
			
				É necessário que o tile tenha possua um contrutor cujo argumento é um inteiro, esse construtor é utilizado durante a leitura do arquivo para construir o tileMap
			*/
			BaseTile(int tileSetIndex);
			/**
				\brief Obtém o índice da imagem do tileset
			
				É utilizado dentro do  Método render do tileMap.
			*/
			int GetTileSetIndex(void) const;
		protected:
			int tileSetIndex;/**< Índice de uma posição do TileSet, preenchido no construtor */
	};

}

#endif // BASETILE_H
