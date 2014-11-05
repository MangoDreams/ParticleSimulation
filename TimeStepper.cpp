#include "TimeStepper.hpp"

///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{

	vector<Vector3f> X0, X1,f;
	Vector3f temp;

	X0 = particleSystem->getState();
	f = particleSystem->evalF(X0);

	for(int i = 0; i < X0.size(); i++){
		temp = X0[i] + stepSize*f[i];
		X1.push_back(temp);
	}

	particleSystem->setState(X1);
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> X0, X1, f0, f1;
	Vector3f temp;

	X0 = particleSystem->getState();
	f0 = particleSystem->evalF(X0);

	for(int i = 0; i < X0.size(); i++){
		temp = X0[i] + stepSize*f0[i];
		X1.push_back(temp);
	}

	f1 = particleSystem->evalF(X1);
	X1.clear();

	for(int i = 0; i < X0.size(); i++){
		temp = X0[i] + (stepSize/2)*(f0[i] + f1[i]);
		X1.push_back(temp);
	}

	particleSystem->setState(X1);
}