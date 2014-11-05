#include "pendulumSystem.h"

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles){
	m_numParticles = numParticles;
	float restLength = .25;
	
	Vector3f x(0,.75,0);
	Vector3f v(0,0,0);
	Vector3f dx(restLength,0,0);

	// fill in code for initializing the state based on the number of particles
	for (int particleIdx = 0; particleIdx < m_numParticles; particleIdx++) {
		// for this system, we care about the position and the velocity
		
		m_vVecState.push_back(x);
		m_vVecState.push_back(Vector3f(0,0,0));
		x += dx;
	}
	
	// acceleration due to gravity
	_g.Set(0,-9.8f,0);
	//spring constant (N/m)
	_kspring = 50.0f;
	// constant mass for all spheres
	_m = 3.0f;

	//a little underdamped
	_kdrag = (.3f*sqrt(4*_m*_kspring));
	
	_grid = new Grid(1,numParticles);

	for(int i = 0; i < numParticles; i++){
		_grid->addConnection(0,i,0,i+1,restLength,_kspring);
		_grid->addConnection(0,i,0,i-1,restLength,_kspring);
	}	
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state){
	vector<Vector3f> f;
	Vector3f pos, vel, f_gravity,f_drag,f_spring, fnet;
	
	for(int particleIdx = 0; particleIdx < m_numParticles; particleIdx++){
		vel = getVelocity(particleIdx);	
		pos = getPosition(particleIdx);
	 
		f_gravity = _m*_g;
		f_drag = -_kdrag*vel; 
		
		f_spring = _grid->calculateSpringForce(2*particleIdx, m_vVecState);
		fnet = f_gravity + f_drag + f_spring;
		
		if(particleIdx == 0) fnet.Set(0,0,0);

		f.push_back(vel);
		f.push_back(fnet/_m);
	}

	return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	for (int i = 0; i < m_numParticles; i++) {
		Vector3f pos = getPosition(i);
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
}
