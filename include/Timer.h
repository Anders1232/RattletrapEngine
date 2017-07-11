#ifndef TIMER_H
#define TIMER_H

/**
	\brief Temporizador

	Implementa um cronômetro básico, com funções de resetar o contador e obter tempo.
	O contador conta o tempo desde o momento em que é iniciado.
*/
class Timer {
	public:
		/**
			\brief Construtor

			Instancia o contador com tempo zero.
		*/
		Timer(void);
		/**
			\brief Atualiza o contador.
			\param dt Tempo transcorrido desde o últimno frame em segundos.

			Incrementa o tempo contabilidado em dt.
		*/
		void Update(float dt);
		/**
			\brief Reinicia o contador

			O tempo contabilizado volta para zero.
		*/
		void Restart(void);
		/**
			\brief Informa termpo cronometrado.

			Obtém o tempo cronometrado até o momento em segundos.
		*/
		float Get(void) const;
	private:
		float time;/**< Variável que conta o tempo.*/
};

#endif // TIMER_H
