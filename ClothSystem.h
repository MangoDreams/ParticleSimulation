#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vecmath.h>
#include <vector>

#include "particleSystem.h"

class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem(int n);
	~ClothSystem() {delete _grid;}

	void connectStructuralSprings(int i, int j);
	void connectShearSprings(int i, int j);
	void connectTorsionalSprings(int i, int j);
	
	Vector3f getPosition(int i){return m_vVecState[i*2];}
	Vector3f getVelocity(int i){return m_vVecState[i*2 + 1];}
	void setPosition(int i, Vector3f pos){m_vVecState[i*2] = pos;}
	void setVelocity(int i, Vector3f v){m_vVecState[i*2 + 1] = v;} 
	
	bool checkSphere(int particleIdx);

	vector<Vector3f> evalF(vector<Vector3f> state);
	void draw();
	
private:
	Vector3f _g,_sphereCenter;
	float _kdrag,_kspring,_m,_sphereRadius;
	Grid* _grid;
	int _n;
	float _restLength;
};


#endif
