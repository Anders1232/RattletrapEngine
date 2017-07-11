#ifndef STATEDATA_H
#define STATEDATA_H

/**
	\brief Tipo cujo objetivo é identificar qual filho de stateDataa instância de StateData é.

	A ideia é a mesmo do método Is de GameObject. Porém aqui, por motivos de performance escolheu-se usar inteiros ao invés de strings para a verificação.
	Para manter o isolamento do código da engine ao jogo recomenda-se que o jogo que o utilizar crie o arquivo Defines.h, no qual os tipos de StateData podem ficar encapsulados em defines, ao invés de números mágicos.
*/
typedef int StateDataType;

/**
	\brief Interface de comunicação entre States.

	O objetivo dessa classe é conter informações para um state a ser construído. Para um State conseguir saber que informações o StateData tem, existe o método Is, que irá identificar o StateData.
	Isso permite então o cast do StateData para o tipo correto.
*/
class StateData {
	public:
		/**
			\brief Informa o tipo do StateData

			Iidentifica o tipo de StateData da instância. Isso permite então o cast correto do StateData para o tipo desejado.
		*/
		virtual bool Is(StateDataType typeToCheck) const=0;
		int waveCount;
		int lifes;
};

#endif // STATEDATA_H
