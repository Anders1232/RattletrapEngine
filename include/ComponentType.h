#ifndef COMPONENTTYPE_H
#define COMPONENTTYPE_H

namespace RattletrapEngine {

	/**
		\brief Tipos de Componentes

		Enumeração com os tipos de componentes possíveis, usado no método Is que todo componente deve implementar.
		Foi colocado em um arquivo separado por problemas de inclusão circular entre GameObject e Component.
	*/
	enum ComponentType{
		SPRITE=0,
		TILEMAP,
		TILESET,
		RECT_TRANSFORM,
		BUTTON,
		GROUPER,
		TEXT,
		COMPONENT_TYPE_SIZE
	};
	
	namespace Components {
		void ShowComponentsSize(void);
	}

}

#endif // COMPONENTTYPE_H
