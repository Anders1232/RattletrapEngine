#include "Text.h"

#include "Game.h"
#include "RectTransform.h"
#include "Resources.h"

namespace RattletrapEngine {
		
    Text::Text( GameObject& associated )
				: Component( associated )
				, texture( nullptr )
				, text( " " )
				, style( TextStyle::BLENDED )
				, fontSize( 12 )
				, color( {255, 255, 255, 255} )
				, bgColor( {0, 0, 0, 0} )
				, fontFile( "" ) {}

	Text::~Text() {
		if( nullptr != texture ) {
			SDL_DestroyTexture( texture );
			texture = nullptr;
		}
	}

	void Text::EarlyUpdate( float dt ) {}

	void Text::Update( float dt ) {}

	void Text::LateUpdate( float dt ) {}

    void Text::Render(){
		if( nullptr == texture ) {
			REPORT_DEBUG( "Texto nao sera renderizado pois nao foi corretamente (ou totalmente) configurado." );
			return;
		}

		if( associated.box.w > 0 && associated.box.h > 0 ) {
			SDL_Rect srcRect;
			srcRect.x = 0;
			srcRect.y = 0;
			srcRect.w = fontDimensions.x;
			srcRect.h = fontDimensions.y;
			SDL_Rect destRect = associated.box;
			if( 0 != SDL_RenderCopy(Game::GetInstance().GetRenderer(), texture, &srcRect, &destRect) ) {
				Error( "Render error: " << SDL_GetError() );
			}
		}
	}

	bool Text::Is( int componentType ) const {
		return ComponentType::TEXT == componentType;
	}

	void Text::SetText( string text ) {
		this->text = text;
		RemakeTexture();
	}

	void Text::SetColor( Color color ) {
		this->color = color;
		RemakeTexture();
	}

	void Text::SetBgColor( Color bgColor ) {
		this->bgColor = color;
		RemakeTexture();
	}

	void Text::SetStyle( TextStyle style ) {
		this->style = style;
		RemakeTexture();
	}

	void Text::SetFontSize( int fontSize ) {
		this->fontSize = fontSize;
		RemakeTexture();
	}

	void Text::SetFont( std::string fontFile ) {
		this->fontFile = fontFile;
		RemakeTexture();
	}

	Vec2 Text::GetSize() const {
		return fontDimensions;
	}

	void Text::RemakeTexture() {
		{
			bool skip = false;
			REPORT_DEBUG2( ( skip |= text.empty(), text.empty() ), "Textura de texto nao sera refeita pois o texto e invalido (vazio)." );
			REPORT_DEBUG2( ( skip |= 0 >= fontSize, fontSize ), "Textura de texto nao sera refeita pois o tamanho da fonte e invalido (menor ou igual a zero)." );
			REPORT_DEBUG2( ( skip |= fontFile.empty(), fontFile.empty() ), "Textura de texto nao sera refeita pois nao possui fonte assimilada." );
			if( skip ) return;
		}

		if( nullptr != texture ) {
			SDL_DestroyTexture( texture );
		}

		font = Resources::GetFont( fontFile, fontSize );
		SDL_Surface *temp = nullptr;
		if( TextStyle::SOLID == style ) {
			temp = TTF_RenderText_Solid( font.get(), text.c_str(), color );
		} else if( TextStyle::SHARED == style ) {
			temp = TTF_RenderText_Shaded( font.get(), text.c_str(), color, bgColor );
		} else if( TextStyle::BLENDED == style ) {
			temp = TTF_RenderText_Blended( font.get(), text.c_str(), color );
		}

		if( nullptr == temp ) {
			REPORT_DEBUG2( true, " " << TTF_GetError() );
		}

		texture = SDL_CreateTextureFromSurface( Game::GetInstance().GetRenderer(), temp );
		if( nullptr == texture ) {
			REPORT_DEBUG( " Cuidado! Nao foi possivel se criar uma textura de texto:\t" << SDL_GetError() << "\n\t\t\t\t\t Se crashar, esse talvez seja o motivo..." );
		}
		
		SDL_FreeSurface( temp );
		int w = 0;
		int h = 0;
		SDL_QueryTexture( texture, nullptr, nullptr, &w, &h );
		fontDimensions = Vec2( w, h );
        dynamic_cast<RectTransform*>( associated.GetComponent( ComponentType::RECT_TRANSFORM ) )->SetKernelSize( fontDimensions );
	}

}
