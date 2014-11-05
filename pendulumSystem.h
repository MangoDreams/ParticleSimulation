#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include "particleSystem.h"


class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem(int numParticles);
	~PendulumSystem() {delete _grid;}
	
	Vector3f getPosition(int i){return m_vVecState[i*2];}
	Vector3f getVelocity(int i){return m_vVecState[i*2 + 1];}

	vector<Vector3f> evalF(vector<Vector3f> state);
	void draw();
	

private:
	Vector3f _g;
	float _kdrag,_kspring,_m;
	Grid* _grid;
};

#endif
