#include "ClothSystem.h"

//TODO: Initialize here
ClothSystem::ClothSystem(int n)
{
	_n = n;
	int i,j;

	_restLength = .25;
	float x0 = -_restLength*_n/2;
	float y0 = _restLength*_n/2;
	float z0 =  -_restLength*_n/2;

	Vector3f pos(x0,y0,z0);
	Vector3f dx(_restLength,0,0);
	Vector3f dy(0,_restLength,0);
	Vector3f dz(0,0,_restLength);
	_sphereCenter.Set(0,0,0);
	_sphereRadius = 1.0f;

	m_numParticles = _n*_n;
	
	j = 0;
	for(int particleIdx = 0; particleIdx < m_numParticles; particleIdx++) {
		
		m_vVecState.push_back(pos);
		m_vVecState.push_back(Vector3f(0,0,0));
		pos += dx;
		if(j == n - 1){
			pos.x() = x0;
			pos += dz;
			j = 0;
		}
		else{
			j++;
		}
	}

	// acceleration due to gravity
	_g.Set(0.0f,-9.8f,0.0f);
	//spring constant (N/m)
	_kspring = 50.0f;
	// constant mass for all spheres
	_m = 2.0f;

	//a little underdamped
	_kdrag = 5.0f*(sqrt(4*_m*_kspring));

	_grid = new Grid(_n,_n);
	
	for(int particleIdx = 0; particleIdx < m_numParticles; particleIdx++){
		_grid->getGridIndices(particleIdx,i,j);	
		connectStructuralSprings(i,j);
		connectShearSprings(i,j);
		connectTorsionalSprings(i,j);
	}
	
}

void ClothSystem::connectStructuralSprings(int i, int j){

	_grid->addConnection(i,j,i-1,j,_restLength,_kspring);
	_grid->addConnection(i,j,i+1,j,_restLength,_kspring);
	_grid->addConnection(i,j,i,j+1,_restLength,_kspring);
	_grid->addConnection(i,j,i,j-1,_restLength,_kspring);
}

void ClothSystem::connectShearSprings(int i, int j){

	float length = sqrt(2.0f)*_restLength;

	_grid->addConnection(i,j,i-1,j-1,length,_kspring);
	_grid->addConnection(i,j,i+1,j+1,length,_kspring);
	_grid->addConnection(i,j,i-1,j+1,length,_kspring);
	_grid->addConnection(i,j,i+1,j-1,length,_kspring);
}

void ClothSystem::connectTorsionalSprings(int i, int j){

	float length = 2*_restLength;

	_grid->addConnection(i,j,i-2,j,length,_kspring);
	_grid->addConnection(i,j,i+2,j,length,_kspring);
	_grid->addConnection(i,j,i,j+2,length,_kspring);
	_grid->addConnection(i,j,i,j-2,length,_kspring);
}

bool ClothSystem::checkSphere(int particleIdx){
	Vector3f pos = getPosition(particleIdx);
	Vector3f segment = pos - _sphereCenter;
	float epilson = .18f;

	if(Vector3f::dot(segment,segment) < _sphereRadius*_sphereRadius + epilson){
		return true;
	}
	return false;
}

vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state){
	vector<Vector3f> f;
	Vector3f pos, vel, f_gravity,f_drag,f_spring, fwind, fnet;
	Vector3f c(0,0,0);
	float a = -10.0f;

	for(int particleIdx = 0; particleIdx < m_numParticles; particleIdx++){
	
		vel = getVelocity(particleIdx);	
		pos = getPosition(particleIdx);

		if(checkSphere(particleIdx)){
			f.push_back(Vector3f(0,0,0));
			f.push_back(Vector3f(0,0,0));
		}
		else{

			f_gravity = _m*_g;
			f_drag = -_kdrag*vel; 
			f_spring = _grid->calculateSpringForce(2*particleIdx, m_vVecState);
			
			//Gaussian wind
			fwind = Vector3f(0,0,a*exp(-(pos.x()*pos.x() + pos.y()*pos.y())/2));

			fnet = f_gravity + f_drag + f_spring + fwind;
			checkSphere(particleIdx);

			//if( (particleIdx > 0 && particleIdx < _n/4) || (particleIdx > (3*_n)/4 && particleIdx < _n)) fnet.Set(0,0,0);
			//if(particleIdx == 0 || particleIdx == _n-1) fnet.Set(0,0,0);
			f.push_back(vel);
			f.push_back(fnet/_m);
		}
	}

	return f;
}

void ClothSystem::draw() {

	glDisable(GL_LIGHTING);
	glPushMatrix();
	glColor3f(1.0f,0.0f,0.0f);
	glTranslatef(_sphereCenter[0], _sphereCenter[1], _sphereCenter[2]);
	glutSolidSphere(_sphereRadius,20.0f,20.0f);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	
	for(int i = 0; i < m_numParticles; i++){
		_grid->drawRectangle(i,m_vVecState);
	}
}

