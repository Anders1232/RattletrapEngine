#ifndef NEARESTFINDER_H
#define NEARESTFINDER_H

template<class T>
class Finder
{
	public:
		virtual float operator()(T)=0;
}

template<class T>
class NearestFinder{
	public:
		virtual T* FindNearest(Vec2 origin, Finder<T> finder, float range= std::numeric_limits<float>::max()) = 0;
		virtual std::vector<T*>* FindNearests(Vec2 origin, Finder<T> finder, float range= std::numeric_limits<float>::max()) = 0;
		//lembrando que é responsabilidade do chamador a desalocação
		virtual ~NearestFinder(){};
};

#endif // NEARESTFINDER_H
