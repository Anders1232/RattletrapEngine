#ifndef PREFABFACTORY_H
#define PREFABFACTORY_H

#include "GameObject.h"

#include <memory>

struct PrefabFactory {
	public:
		static std::unique_ptr<GameObject> GridGroup();
	private:
		PrefabFactory();
		~PrefabFactory();
};

#endif // PREFABFACTORY_H
