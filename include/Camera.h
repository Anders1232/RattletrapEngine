#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"
#include "Vec2.h"
#include "ActionManager.h"

#define CAMERA_DEFAULT_MIN_LOG_ZOOM (-4.0)
#define CAMERA_DEFAULT_MAX_LOG_ZOOM (0.0)
#define CAMERA_DEFAULT_ZOOMABLE (true)
#define CAMERA_DEFAULT_LOG_ZOOM_SPEED (0.125)
#define CAMERA_DEFAULT_MIN_SPEED (200.)
#define CAMERA_DEFAULT_MAX_SPEED (2000.)
#define CAMERA_DEFAULT_MOVE_SPEED (100.)
#define CAMERA_LOG_ZOOM_BASE 2

/**
	\brief Classe que modela a câmera
	
	A Câmera contém informações sobre qual posição da tela deve ser renderizada, qual objeto deve ficar centralizado(caso necessário).
	Também é responsável por lidar o Zoom, rastreando informações como:
		- Zoom travado
		- Zoom atual
		- Velocidade de zoom
		- Zoom mínimo e máximo

	O zoom é armazenado internamente de forma logarítmica para permitir um comportamento linear nas suas velocidades de zoom e movimentação da câmera,
	que dependem diretamente do nível atual de zoom. A base do zoom é representado em CAMERA_LOG_ZOOM_BASE e indica que, um zoom de 2, representa
	multiplicar as sprites por CAMERA_LOG_ZOOM_BASE^(2). Já um zoom de -2 representa CAMERA_LOG_ZOOM_BASE^(-2).
*/
class Camera {
	public:
		/**
			\brief Coloca a câmera para seguir o GameObject enviado.
			\param newFocus objetos o qual a câmera deve acompanhar.
			
			Basicamente atribui newFocus ao focus.
		*/
		static void Follow(GameObject* newFocus);
		/**
			\brief Caso a câmera esteja seguindo um objeto, a câmera pare de seguir esse objeto.
			
			Basicamente atribui nullptr ao focus.
		*/
		static void Unfollow(void);
		/**
			\brief Atualiza o estado da câmera.
			\param dt Intervalo de tempo transcorrido do frame anterior para o grame atual.
			
			Uma série de operações devem ser feitas:
				- Se o mouse estiver seguindo algum GameObject, deve-se fazer o cálculo para que esse gameobject seja renderizado no centro tela levando em conta o zoom. Isso é calculado pela diferença:
					- Do Vec2 relativo centro do box do GameObject
					- Com a metade das dimensões da tela vezes o inverso do zoom da câmera.
				- Caso contrário, devemos mover a câmera com base no input do usuário. Essa movimentação é feita da seguinte forma:
					- Se a seta para a cima ou o botão "w" for pressionado decrementa-se o y da posição da câmera em Camera::speed * dt.
					- Se a seta para a esquerda ou o botão "a" for pressionado decrementa-se o x da posição da câmera em Camera::speed * dt.
					- Se a seta para a baixo ou o botão "s" for pressionado incrementa-se o y da posição da câmera em Camera::speed * dt.
					- Se a seta para a direita ou o botão "d" for pressionado incrementa-se o x da posição da câmera em Camera::speed * dt.
				- Se a rodinha do mouse(scroll) estiver em movimentação, chamar o método Camera::Zoom enviando o valor y do movimento feito no scroll.
		*/
		static void Update(float dt);
		/**
			\brief Ponto de início do que deve ser exibido na tela
			\todo Verificar viabilidade de tornar privado.
			
			Contém a informação do ponto no jogo que corresponde ao (0,0) da janela do jogo.
		*/
		static Vec2 pos;
		/**
			\brief Força um valor para o zoom.
			\param newZoom novo valor para o Zoom
			
			O valor informado se torna o zoom corrente. O novo valor do zoom pode extrapolar os limites existentes. Esse valor, mesmo que fora dos limites, será convertido para a escala logarítmica e atribuído ao currentLogZoom.
		*/
		static void ForceLinearZoom(float newZoom);
		/**
			\brief Força um valor para o zoom.
			\param newZoom novo valor para o Zoom
			
			O valor informado se torna o zoom corrente. O novo valor do zoom pode extrapolar os limites existentes. Esse valor, mesmo que fora dos limites, será atribuído ao currentLogZoom.
		*/
		static void ForceLogZoom(float newZoom);
		/**
			\brief Trava ou destrava o zoom.
			\param newZoom novo valor para o Zoom

			Caso o argumento da função seja true, as futuras chamadas à Zoom não terão mais efeito. Caso seja false as futuras chamadas à Zoom terão efeito. Esse efeito é obtido atribuindo à variável zoomFixed o oposto desse valor.
		*/
		static void SetZoomable(bool zoomable);
		/**
			\brief Altera o zoom corrente.
			\param deltaZoom Variação no zoom.

			O zoom corrente é alterado logaritmicamente em deltaZoom*logZoomSpeed. Só tem efeito se o valor de zoomFixed for falso.
			Se o novo valor para o zoom extrapolar o limite superior, o valor do limite superior será usado.
			Se o novo valor para o zoom extrapolar o limite inferior, o valor do limite inferior será usado.
			Também ajusta a posição da câmera para que o ponto onde o mouse estava continue no mesmo lugar.
		*/
		static void Zoom(float deltaZoom);
		/**
			\brief Estabelece os limites superior e inferior do zoom.
			\param minZoom Novo limite inferior.
			\param maxZoom Novo limite superior.

			Se o valor de minZoom ou maxZoom não forem fornecidos, o valor default será atribuído no lugar.
		*/
		static void SetZoomLimits(float minZoom=CAMERA_DEFAULT_MIN_LOG_ZOOM, float maxZoom=CAMERA_DEFAULT_MAX_LOG_ZOOM);// No args to set to default
		/**
			\brief Informa o valor do zoom corrente.

			Se o valor for 1.0 significa que nenhum zoom está sendo aplicado.
			Se for maior que 1.0 significa que as imagens devem ser ampliadas.
			Se for menor que 1.0 significa que as imagens devem ser reduzidas.
		*/
		static float GetLinearZoom(void);
		/**
			\brief Informa o valor do zoom corrente na escala logarítmica.

			Se o valor for 0.0 significa que nenhum zoom está sendo aplicado.
			Se for maior que 0.0 significa que as imagens devem ser ampliadas.
			Se for menor que 0.0 significa que as imagens devem ser reduzidas.
		*/
		static float GetLogZoom(void);
		/**
			\brief Estabelece os limites superior e inferior da velocidade da câmera.
			\param minSpeed Novo limite inferior.
			\param maxSpeed Novo limite superior.

			Se o valor de minSpeed ou maxSpeed não forem fornecidos, o valor default será atribuído no lugar.
		*/
		static void SetSpeedLimits(float minSpeed=CAMERA_DEFAULT_MIN_SPEED, float maxSpeed=CAMERA_DEFAULT_MAX_SPEED);
		/**
			\brief Retorna a velocidade mínima da câmera.

			O valor mínimo é a velocidade que a câmera vai se mover quando estiver com o máximo de zoom possível, ou seja, o mais próximo possível.
		*/
		static float GetMinSpeed(void);
		/**
			\brief Retorna a velocidade máxima da câmera.

			O valor máximo é a velocidade que a câmera vai se mover quando estiver com o mínimo de zoom possível, ou seja, o mais afastado possível.
		*/
		static float GetMaxSpeed(void);
		/**
			\brief Altera a sensibilidade do zoom.
			\param newZoomSpeed Novo valor para sensibilidade o zoom.

			Quanto maior o newZoomSpeed, mais sensível será o zoom.
			Quanto menor o newZoomSpeed, menos sensível será o zoom.
			Esse efeito é obtido atribuindo newZoomSpeed à zoomSpeed.
		*/
		static void SetZoomSpeed(float newZoomSpeed);
		/**
			\brief Converte um ponto do espaço de coordenadas do mundo para o espaço de coordenadas da tela
			\param world o ponto no mundo a ser convertido
			\return	o ponto no espaço da tela

			Essa função realiza o cálculo para se retornar onde, no espaço de coordenadas da tela, um ponto no mundo estaria localizado.
			O zoom é considerado nesse cálculo.
		*/
		static Vec2 WorldToScreen(Vec2 world);
		/**
			\brief Converte um retângulo do espaço de coordenadas do mundo para o espaço de coordenadas da tela
			\param world o retângulo no mundo a ser convertido
			\return o retângulo no espaço da tela

			Essa função realiza o cálculo para se retornar onde, no espaço de coordenadas da tela, um retângulo no mundo estaria localizado.
			O zoom é considerado nesse cálculo.
		*/
		static Rect WorldToScreen(Rect world);
		/**
			\brief Converte um ponto do espaço de coordenadas da tela para o espaço de coordenadas do mundo
			\param screen o ponto na tela a ser convertido
			\return	o ponto no espaço do mundo

			Essa função realiza o cálculo para se retornar onde, no espaço de coordenadas do mundo, um ponto na tela estaria localizado.
			O zoom é considerado nesse cálculo.
		*/
		static Vec2 ScreenToWorld(Vec2 screen);
		/**
			\brief Converte um retângulo do espaço de coordenadas da tela para o espaço de coordenadas do mundo
			\param screen o retângulo na tela a ser convertido
			\return	o retângulo no espaço do mundo

			Essa função realiza o cálculo para se retornar onde, no espaço de coordenadas do mundo, um retângulo na tela estaria localizado.
			O zoom é considerado nesse cálculo.
		*/
		static Rect ScreenToWorld(Rect screen);
	private:
		/*! \brief Construtor privado que não deve ser implementado.
		 *
		 *	Essa classe deve ser estática e se alguém tentar de alguma forma instanciar dará erro na compilação/ligação.
		 */
		Camera();
		static GameObject* focus;/**< Gameobject que ficará centralizado na câmera. Caso seja nullptr a câmera se moverá pelas setinhas/WASD.*/
		static float currentLogZoom;/**< Armazena o valor do zoom atual, informando em quantas vezes os objetos devem ser ampliados. Ele deve estar estre o minZoom e o maxZoom, a não ser que o método ForceZoom seja usado. Os métodos Zoom e ForceZoom alteram seu valor.*/
		static float minLogZoom;/**< Armazena o valor mínimo que o zoom pode ter. Esse limite é ignorado pelo método ForceZoom. É alterado pelo SetZoomLimits.*/
		static float maxLogZoom;/**< Armazena o valor mínimo que o zoom pode ter. Esse limite é ignorado pelo método ForceZoom. É alterado pelo SetZoomLimits.*/
		static bool zoomFixed;/**< Se for verdadeiro, o zoom não será alterado pelo método Zoom. Caso contrário o método Zoom pode mudar o valor corrente do zoom. É alterado pelo método SetZoomable.*/
		static float logZoomSpeed;/**< Armazena a velocidade com a qual o zoom deve ocorrer. O argumento do método Zoom é multiplicado por esse valor para depois ser somado ao currentZoom.*/
		static float minSpeed;/**< Armazena o valor mínimo da velocidade da câmera.*/
		static float maxSpeed;/**< Armazena o valor máximo da velocidade da câmera.*/
		static float currentSpeed;/**< Armazena a velocidade atual de movimento da câmera quando não está focalizada em nenhum objeto.*/
};

#endif // CAMERA_H
