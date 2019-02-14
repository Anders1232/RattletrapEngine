#ifndef TEXT_H
#define TEXT_H

#define INCLUDE_SDL 
#define INCLUDE_SDL_TTF 
#include "SDL_include.h"

#include <string>

#include "Color.h"
#include "Component.h"
#include "ComponentType.h"
#include "Error.h"
#include "GameObject.h"
#include "Vec2.h"

namespace RattletrapEngine {

	class Text : public Component {
		public:
			enum TextStyle {
				SOLID,/**< O texto é renderizado diretamente, sem nenhum tipo de tratamento visual.*/
				SHARED,/**< o texto é renderizado em um retângulo da cor dada em um argumento Color no hora de renderizar.*/
				BLENDED/**< Cria um texto com as bordas suavizadas (usando o canal alpha), para que ele se adapte melhor à região da tela onde for renderizado. É mais custoso de se renderizar que os outros dois, mas faz
			uma diferença perceptível.*/
			};
		
			Text(GameObject& associated);
			~Text();
		
			void EarlyUpdate( float dt );
			void Update( float dt );
			void LateUpdate( float dt );
			bool Is( int componentType ) const;
			void Render();
		
			/**
				\brief Altera o texto.
		
				Altera a posição do texto.
			*/
			void SetText(std::string text);
			/**
				\brief Altera a cor.
		
				Altera a cor do texto.
			*/
			void SetColor(Color color);
			/**
				\brief Altera a cor de fundo.
		
				Altera a cor de fundo do texto. Válido somente para quando se usa TextStyle::SHARED.
			*/
			void SetBgColor(Color color);
			/**
				\brief Altera o estilo.
		
				Altera o estilo do texto.
			*/
			void SetStyle(TextStyle style);
			/**
				\brief Altera o tamanho da fonte.
		
				Altera o estilo do texto.
			*/
			void SetFontSize(int fontSize);
			/**
				\brief Altera a fonte do test.
		
				Altera a fonte do texto.
			*/
			void SetFont( std::string fontFile );
			/**
				\brief Obtém o tamanho do texto.
		
				Obtém o tamanho do texto em pixels.
			*/
			Vec2 GetSize(void) const;
		
		private:
			void RemakeTexture(void);
			std::shared_ptr<TTF_Font> font;
			std::shared_ptr<SDL_Texture> texture;
			std::string text;
			TextStyle style;
			int fontSize;
			Color color;
			Color bgColor;
			std::string fontFile;
			Vec2 fontDimensions;
	};

}

#endif // TEXT_H
