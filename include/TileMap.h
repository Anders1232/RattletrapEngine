#ifndef TILEMAP_H
#define TILEMAP_H

#include <string>
#include <vector>
#include <list>
#include <queue>
#include <limits>
#include <map>
#include <vector>
#include <functional>
#include <algorithm>
#include "Tileset.h"
#include "Vec2.h"
#include "GameObject.h"
#include "AStarHeuristic.h"
#include "Resources.h"
#include "TileMapObserver.h"
#include "NearestGOFinder.h"
#define TILE_VAZIO (-1)
#define SPAWN_POINT (75)
#define COLLISION_LAYER (1)
#define WALKABLE_LAYER (0)
#define END_POINT (74)


using std::string;
using std::vector;
/**
	\brief Classe que modela o TileMap

	Gerencia um tileMap,internamente possui um tileSet que é usado para renderizar o mapa. Internamente possui três mapas: um para o tileSet, outro para colisão e um terceiro de GameObjects.
*/
class TileMap: public NearestGOFinder{
	public:
		/**
			\brief Construtor.
			\param tileSet TileSet que será usado pelo tileMap.
			\param file Arquivo com as informações do tileMap.
			\param collisionTileMapFile Arquivo com as informações do TileMap de colisão.
		
			Instancia o tileMap com o tileSet enviado e criar o tileMap lendo o arquivo enviado.
		*/
		TileMap(string file, TileSet *tileSet);
		/**
			\brief Obtém o índice do tileMap na posição informada
			\param x Posição X do tileMap
			\param y posição Y do tileMap.
			\param z posição z do tileMap.
			\return índice existente na posição (x, y, z do tileMap).
		
			O valor de retorno é um referência para a posição para que se possa permitir alteração externa.
			PS: Com grandes poderes vêm grandes responsabilidades.
		*/
		int& At(int x, int y, int z=0) const;
		/**
			\brief Obtém o índice do tileMap na posição informada
			\param index2D índice xy do tileMap,.tal como retornado pelo GetTileMousePos.
			\param layer Layer da qual queremos obter a informação existente em index2D.
			\return índice existente na posição (x, y, z do tileMap).
		
			O valor de retorno é um referência para a posição para que se possa permitir alteração externa.
			PS: Com grandes poderes vêm grandes responsabilidades.
		*/
		int& AtLayer(int index2D, int layer) const;
		/**
			\brief Renderiza o TileMap a partir da posição dada.
			\param pos Valor x,y da câmera, será usado como coordenada de início do tileMap.
			\param parallax Flag para se ativar ou desativar o parallax das camadas
			\param mouse Posição do mouse para checar qual tile deve sofre highlight.

			Renderiza-se todas as layers começando pelas de menor valor numérico de profundidade até as de maior valor numérico.
			Verifica em qual tile o mouse está para renderizá-lo highlighted.
		*/
		void Render(Vec2 pos = Vec2(0,0), bool parallax = false, Vec2 mouse= Vec2(-1, -1)) const;
		/**
			\brief Renderiza a layer informada.
			\param layer Layer a ser renderizada.
			\param pos Valor x,y da câmera, será usado como coordenada de início do tileMap.
			\param mouse Posição do mouse para checar qual tile deve sofre highlight.

			Renderiza-se todas as posições da matriz na layer indicada, com suas alterações sendo feitas por parallaxe.
			Observação: pode exibir o tileMap de colisão se assim for pedido em ShowCollisionInfo.
		*/
		void RenderLayer(int layer, Vec2 pos = Vec2(0,0), bool parallax = false, Vec2 mouse= Vec2(-1, -1)) const;
		/**
			\brief Informa a largura do tileMap.

			Retorna a largura do tileMap em posições.
		*/
		int GetWidth(void) const;
		/**
			\brief Informa a altura do tileMap.

			Retorna a altura do tileMap em posições.
		*/
		int GetHeight(void) const;
		/**
			\brief Informa o número de layers do tileMap.

			Retorna o número de layers(profundidade) do tileMap.
		*/
		int GetDepth(void) const;
		/**
			\brief Sobre qual tile o mouse está.
			\param mousePos posição do mouse, é passado como referência por motivos de performance
			\param affecteedByZoom verdadeiro se o tileMap está sofrendo zoom, falso caso contrário.
			\param layer layer no tileMap do qual se quer obter a posição.
			\todo Verificar se deve lançar exceção ao invés de retornar um valor negativo.

			Retorna o número de layers(profundidade) do tileMap.Retorna o índice da posição do tileMap relativo à posição do mouse, esse valor pode ser enviado ao método TileMap::At para saber o conteúdo da posição.
			O valor retornado será negativo e inválido se alguma coordenada do mouse tiver fora dos limites do tileMap.
			Internamente usa-se busca binária para achar o tile correspondente.
		*/
		int GetCoordTilePos(Vec2 const &mousePos, bool affecteedByZoom, int layer)const;
		/**
			\brief Insere GameObjct no tileMap
			\param obj GameObject a ser inserido no tileMap de GameObjects.

			Utiliza a posição do mouse no momento(que deve ser a mesma do centro do GameObject) para identificar onde o GameObject deve ser colocado no tileMap de GameObjects. Então obj é colocado nessa posição, sua posição é alterada para se encaixar exatamente com o início da posição.
			Atualiza-se o tileMao de colisão para adicionar a informação que tem um GameObject na posição respectiva.
		*/
		bool InsertGO(GameObject* obj, bool checkCollision = true);
		/**
			\brief Insere GameObjct no tileMap ou o posiciona na posição passada caso não seja possivel
			\param obj GameObject a ser inserido no tileMap de GameObjects.

			Utiliza a posição do mouse no momento(que deve ser a mesma do centro do GameObject) para identificar onde o GameObject deve ser colocado no tileMap de GameObjects. Então obj é colocado nessa posição, sua posição é alterada para se encaixar exatamente com o início da posição.
			Atualiza-se o tileMao de colisão para adicionar a informação que tem um GameObject na posição respectiva.
		*/
		bool InsertGO(GameObject* obj,Vec2 initialPos);
		/**
			\brief Remove GameObjct no tileMap
			\param position Posição que contém o Gameobject a ser retirado.

			Atualiza-se o tileMap de colisão para adicionar a informação que tem um GameObject na posição respectiva.
		*/
		void RemoveGO(int position);
		/**
			\brief Remove GameObjct no tileMap

			Variação do RemoveGO que internamente faz a busca o GameObject que está debaixo do mouse para remover.
			Atualiza-se o tileMap de colisão para adicionar a informação que tem um GameObject na posição respectiva.
		*/
		void RemoveGO(void);
		/**
			\brief Obtém GameObject de uma determinada posição do tileMap

			Faz-se checagem de limites para saber se a posição desejada existe.
		*/
		GameObject* GetGO(int index);
		/**
			\brief Ativa ou desativa a exibição na tela das informações do tileMap de colisão

			Se show for verdadeiro, futuras chamadas à Render fará com que seja exibido o tileMap de colisão.
			Se shor for falso, futuras chamadas à Render não exibirão o tileMap de colisão.
		*/
		void ShowCollisionInfo(bool show);
		/**
			\brief Informa estado a exibição do tilemap de colisão.

			Retorna verdadeiro se o tileMap de colisão estiver sendo exibido, falso caso contrário.
		*/
		bool IsShowingCollisionInfo();
		/**
			\brief Obtém os groups de tiles.
			\param tileType Tipo de tile do qual se quer obter posições
			\todo Resolver bug na detecção de adjacências.

			Retorna grupos de tiles adjacentes do tipo informado.
		*/
		vector<vector<int>>* GetTileGroups(int tileType) const;
		/**
			\brief Obtém o tamanho de um tile

			O tamanho retornado não leva em consideração zoom, mas leva em consideração escala.
		*/
		Vec2 GetTileSize(void) const;
		/**
			\brief Altera o tilemap mostrando o caminho contido em list

		*/

		void ShowPath(std::shared_ptr<std::vector<int>> path);
		/**
			\brief Calcula o caminho menos custoso entre dois pontos baseado em uma heuristica utilizando o algoritmo A*.
			\param originTile Tile de origem
			\param destTile Tile de destino
			\param heuristic Heuristica a ser usada
			\param weightMap Dicionário com os pesos relacionados a cada tipo de tile do mapa

			Retorna uma lista com a sequencia dos indices dos tiles que formam o caminho
		*/
		std::list<int>* AStar(int originTile,int destTile,AStarHeuristic* heuristic,std::map<int, double> weightMap);
		void ObserveMapChanges(TileMapObserver *);
		void RemoveObserver(TileMapObserver *);
		GameObject* FindNearestGO(Vec2 origin,std::string objectDestType, float range= std::numeric_limits<float>::max());
		std::vector<GameObject*>* FindNearestGOs(Vec2 origin, std::string targetType, float range= std::numeric_limits<float>::max());
		/**
			\brief Vê se um dado objeto está no tileMap
			\param obj Objeto o qual se quer sabe se está no tileMap

			Se o objeto existir no tileMap de gameObjects do TileMap, sua posição na matriz será informada, caso contrário um valor negativo será retornado.
		*/
		int64_t Have(GameObject *obj);
	protected:
		/**
			\brief Carrega um arquivo das informações do timeMap.
			\param target Tilemap onde as informações do arquvio deve ser carregadas.
			\param setOfficialSize Verdadeiro se a largura, altura e profundiade lidas devem se tornar as oficiais do tilemap.

			O arquivo deve começar com 3 números: largura, altura e profundidade.
			Depois disso dever existir um números inteiros igual ao produto dos três números lidos organizados em layers, linhas e colunas.
			O argumento setOfficialSize deve ser verdadeiro se estiver lendo o arquivo de tileMap relativo com informações do tileSet.
		*/
		void Load(string file, std::vector<int> &target, bool setOfficialSize= false);
		/**
			\brief Altera o tileSet usado pelo tileMap

			O tileSet informado passa a ser usado pelo tileMap.
		*/
		void SetTileSet(TileSet *tileSet);
		/**
			\brief Calcula a paralaxe
			\param num coordenada a ser modificada.
			\param pos posição de começo da tilemap.
			\param layer Layer da coordenada que ser modificada
			\return Valor da coordenada alterado pela parallaxe.

			Ele subtrai da coordenada o produto da câmera pelo incremento da layer.
		*/
		Vec2 CalculateParallaxScrolling(Vec2 num, Vec2 pos, int layer) const;
		std::vector<int> tileMatrix;/**< Matriz tridimentsional de índices linearizados em um vetor.*/
		TileSet *tileSet;/**< TileSet utilizado para renderização.*/
		int mapWidth;/**< Largura do TileMap.*/
		int mapHeight;/**< Altura do TileMap.*/
		int mapDepth;/**< Número de camadas do TileMap.*/
		std::vector<GameObject*> gameObjectMatrix;/**< TileMap linearizado de GameObjects*/	//bidimensional??
		bool displayCollisionInfo;/**<Verdadeiro se as informações de colisão devem ser exibidas no TileMap::Render, falso caso contrário.*/
		/**
			\Verifica se um determinado tile está livre na camada de colisão

			\return true se o tile está livre na camada de colisão
		*/
		bool Traversable(int index) const;
		/**
			\brief Obtém todos os vizinhos de um determinado tile

			\return vetor com o indice dos tiles dos vizinhos.
		*/
		std::vector<int>* GetNeighbors(int tile) const;
		void ReportChanges(int tileChanged);
		vector<TileMapObserver*> observers;

		class LessThanByHeuristic{
			public:
				LessThanByHeuristic(int dest,AStarHeuristic* heuristic,int mapWidth,bool reverse):
				destTile(dest),heuristic(heuristic),tileMapWidth(mapWidth),reverse(reverse){}
				bool operator()(const std::pair<double,int> lhs,const std::pair<double,int> rhs) const{
					if(reverse){
						return lhs.first + (*heuristic)(Vec2(lhs.second / tileMapWidth,lhs.second % tileMapWidth),
													Vec2(destTile / tileMapWidth,destTile % tileMapWidth)) > 
								rhs.first + (*heuristic)(Vec2(rhs.second / tileMapWidth,rhs.second % tileMapWidth),
													Vec2(destTile / tileMapWidth,destTile % tileMapWidth));
					}
					else{
						return lhs.first + (*heuristic)(Vec2(lhs.second / tileMapWidth,lhs.second % tileMapWidth),
													Vec2(destTile / tileMapWidth,destTile % tileMapWidth)) < 
								rhs.first + (*heuristic)(Vec2(rhs.second / tileMapWidth,rhs.second % tileMapWidth),
													Vec2(destTile / tileMapWidth,destTile % tileMapWidth));
					}
				}
				
			private:
				int destTile;
				AStarHeuristic* heuristic;
				int tileMapWidth;
				bool reverse;
		};

		template<typename T>
		class AStarPryorityQueue : public std::priority_queue<T, std::vector<T>>{	
  			public:
		  			bool find(const T& value) {
							auto it = std::find(this->c.begin(), this->c.end(), value);
							if (it != this->c.end()){return true;}
		   				else {return false;}
 				}
		};

};

#endif // TILEMAP_H
