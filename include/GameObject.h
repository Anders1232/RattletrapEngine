#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#include "SDL_include.h"

#include <vector>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

#include "Rect.h"

#define TYPE(x) std::type_index(typeid(x))

class State;
class Component;

/**
	\brief Classe virtual que especifica o funcionamento de um GameObject.

	Especifica quais métodos um gameObject deve ter para conseguir ser manipulado corretamente pela engine.
*/
class GameObject{
	public:
	    bool debug;
		/**
			\brief Construtor

			Não é virtual, sua implementação inicializa a rotação com o valor zero. Pode ser usado pelos herdeiros.
		*/
		GameObject(void);
		/**
			\brief Construtor

			\param tag Inicializa o valor da tag.
			\param context Referência do contexto que GameObject está inserido, permitindo que este instancie outros objetos.
		*/
		GameObject(std::string tag, State* context = nullptr);

		/**
			\brief Adiciona componente a um gameobjet.
			\param component componente a ser adicionado.

		*/
		template<typename T>
		void AddComponent(T* component);
		/**
			\brief Obtém componente
			\param type tipo do componente a ser buscado.
			\todo Verificar viabilidade de lançar exceção ao invés de fechar o jogo.

			Se não existir um componente do tipo informado Error() será chamado
		*/
		//Component& GetComponent(unsigned int type) const;
		/*
		template<typename T>
		T& GetComponent(unsigned int type) const;
		*/
		template<typename T>
		T& GetComponent() const;


		/**
			\brief Obtém componentes
			\param type tipo do componente a ser buscado.
			\todo Verificar viabilidade de lançar exceção ao invés de fechar o jogo.
			\return Lista de componentes do tipo informado sobre posse do GameObject

			Se não existir um componente do tipo informado Error() será chamado. Não se deve deletar os componentes obtidos.
		*/
		std::vector<Component*> GetComponents(unsigned int type) const;
		/**
			\brief Remove componente a um gameobjet.
			\param type Tipo do componente a ser removido.
			Se a não existir um componente desse tipo no GameObject nada será feito.
		*/
		template<typename T>
		void RemoveComponent();
		/**
			\brief Remove componente a um gameobjet.
			\param type Ponteiro do componente a ser removido.
			Se a não existir o componente no GameObject nada será feito.
		*/
		//void RemoveComponent(Component* component);
		/**
			\brief Destrutor

			Basicamente é para obrigar quem implementar de usar um destrutor.
		*/
		virtual ~GameObject(void);
		/**
			\brief Associa um objeto pai.
			\param parent Objeto designado como pai.
			\param parent Objeto designado como pai.
			\param xrelative Distanciamento no eixo x relativo ao pai.
			\param yrelative Distanciamento no eixo y relativo ao pai.

			Com a associação, caso o objeto pai seja movimentado, o objeto filho terá sua posição atualizada de acordo, afim de manter o posicionamento relativo.
		*/
		void SetParent(GameObject& parent, int xrelative = 0, int yrelative = 0);
		/**
			\brief Atualizar estado.
			\param dt tempo transcorrido desde a última chamada ao método.

			No corpo método o GameObject deve atualizar seu estado, utilizando o argumento passado e quaisquer outras funcionalidades que necessitar.
		*/
		virtual void EarlyUpdate(float dt);
		/**
			\brief Atualizar estado.
			\param dt tempo transcorrido desde a última chamada ao método.

			No corpo método o GameObject deve atualizar seu estado, utilizando o argumento passado e quaisquer outras funcionalidades que necessitar.
		*/
		virtual void Update(float dt);
		/**
			\brief Atualiza estado do GameObject.

			É executado todo frame após o Render.
		*/
		virtual void LateUpdate(float dt);
		/**
			\brief Atualiza active.

			Esse método não deve ser sobrescrevido. Também atualiza o status enabled de seus Componenetes.
			Em GameObjects desativados os métodos EarlyUpdate, Update, Render e LateUpdate não são chamados.
		*/
		void UpdateActive(void);
		/**
			\brief Renderiza o GameObject.
			Basicamente percorre os Componentes chamando o Render deles.
		*/
		virtual void Render(void);
		/**
			\brief Informa se esse gameObject não precisa mais existir.
			\return Verdadeiro se esse GameObject não é mais necessário, false caso contrário.

			Deve ser usado no State que o contém para o eliminar da sua estrutura de manipulação de gameObject e destruí-lo.
		*/
		virtual bool IsDead(void);
		/**
			\brief Solicita que o GameObject seja destruído.

			Uma vez que esse método é chamado, futuras chamadas ao IsDead devem retornar verdadeiro.
		*/
		virtual void RequestDelete(void);
		/**
			\brief Notifica Colisão
			\param other Referência para o gameobject com o qual se colidiu.

			Deve realizar as ações para tratamento da colisão.
		*/
		virtual void NotifyCollision(GameObject &other);
		/**
			\brief Informa que tipo de GameObject é.
			\param type Tipo com o qual quer se verificar que esse GameObject é.
			\return Verdadeiro se for do tipo perguntado, falso caso contrário;

			É usado no tratamento de colisão para que se identifique com quem colidiu.
		*/
		virtual bool Is(std::string type);
		/**
			\brief Obtém Rect informando a posição renderizada da animação.

			Obtém Rect informando a posição renderizada, computando zoom, escala e posição da câmera.
		*/
		virtual Rect GetWorldRenderedRect(void) const;
		/**
			\brief Ativa ou desativa o GameObject.

			Essa modificação só passa a valer a partir do frame seguinte.
			Em GameObjects desativados os métodos EarlyUpdate, Update, Render e LateUpdate não são chamados.
		*/
		void SetActive(bool newValue);
		/**
			\brief Informa se o GameObject está ativo.

			Em GameObjects desativados os métodos EarlyUpdate, Update, Render e LateUpdate não são chamados.
		*/
		bool IsActive(void) const;

		/**
			\brief Modifica posição do GameObject.
			\param x Posição no eixo x que será atribuida.
			\param y Posição no eixo y que será atribuida.
		*/
		void SetPosition(int x, int y);

		/**
			\brief Modifica posição do GameObject.
			\param x Posição no eixo x que será atribuida.
			\param y Posição no eixo y que será atribuida.

			Caso o objeto tenha um objeto pai, x e y são relativos a posição do objeto pai.
		*/
		void SetRelativePosition(int x, int y);

		/**
			\brief Modifica posição do GameObject.
			\param x Posição no eixo x que será atribuida.
			\param y Posição no eixo y que será atribuida.

			Caso o objeto tenha um objeto pai, x e y são relativos a posição do objeto pai.
		*/
		void SetRelativePosition(Vec2 v);


		/**
			\brief Modifica posição do GameObject.
			\param v Vetor contendo posição x, y que serão atribuidas.
		*/
		void SetPosition(Vec2 v);
		/**
            \brief Retorna true caso haja um click dentro da área do rect.
        */
        bool Clicked(){return clicked;};
        bool Released(){return released;};

		Rect box;/**< Posição do GameObject na tela.*/
		float rotation;/**< Rotação do GameObject.*/
		GameObject* parent;
		bool showOnScreen;

		void SetTag(std::string tag);
		std::string GetTag();
		bool IsTag(std::string tag);
		GameObject* GetChildWithTag(std::string tag);
		void CreateNewObject(GameObject* gameObject);
		State* GetContext();
		Vec2 GetPosition();

	protected:
	    std::unordered_map<std::string, GameObject*> child;
	    std::string tag;
	    State* context;
	    bool clicked;
	    bool released;
	    Vec2 parentRelative;
		std::unordered_map< std::type_index,  std::vector< std::shared_ptr<Component> > > components;/**< Vetor de componentes, que provêem funcionalidades adicionais.*/
		bool dead;/**<Booleano informado se o GameObject deve ser destruído. Faz-se necessário para que a mecânia de RequestDelete e IsDead funcione num GameObject. */
		bool active;/**<Informa Se o gameObject está ativo ou não*/
		bool newActive;/**< Informa se esse GO estará ativo no próximo frame. Feito para que o GO não mude de ativo para inativo no decorrer de um frame*/
};

#include "Component.h"
#include "Error.h"
/*
template<typename T>
T& GameObject::GetComponent(unsigned int type) const{

    for(unsigned int i = 0; i < components.size();i++){
        if(typeid(*components[i]) == typeid(T)){//if(components[i]->Is(type)){
            return dynamic_cast<T&> (*(components[i])) ;
        }
    }
}
*/
template<typename T>
T& GameObject::GetComponent() const{
    static_assert(std::is_base_of<Component, T>::value, "Given type must be Component." );
    if(components.find(TYPE(T)) != components.end()){//typeid(*components[i]) == typeid(T)){//if(components[i]->Is(type)){
        return dynamic_cast<T&> (*(components.at(TYPE(T)) )[0]) ;
    }else{
        Error("No " << typeid(T).name() << " in GameObject!");
    }
}

template<typename T>
void GameObject::AddComponent(T* component){
	static_assert(std::is_base_of<Component, T>::value, "Given object must be component (or extend it)." );
	DEBUG_PRINT("Adding to GameObject [ " << this << " ]" << ": " << typeid(T).name());
	auto it = components.find(TYPE(T));
	if(it == components.end()){
        DEBUG_PRINT("First component of this type. Creating vector..");
        components[TYPE(T)] =  *new std::vector<std::shared_ptr<Component>>();
	}
	components[TYPE(T)].emplace_back(component);
	DEBUG_PRINT(typeid(T).name() << " added to object.");
}

template<typename T>
void GameObject::RemoveComponent(){
    auto it = components.find(TYPE(T));
    if(it != components.end()){
        if(it->second.size() > 0){
            it->second.erase( it->second.begin() );
        }else if(it->second.size() == 1){
            components.erase(it);
        }
    }else{
        Error("GameObject has no " << typeid(T).name());
    }
}
/*
void GameObject::RemoveComponent(Component* component){
	for(uint i = 0; i < components.size();i++){

		if(components[i] == component){
			delete components[i];
			components.erase(components.begin() + i);
			return;
		}
	}
}
*/


#endif // GAMEOBJECT_H

