
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem()
{
	m_vVecState.push_back(Vector3f(1,1,0));
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	Vector3f temp;

	for(int i = 0; i < state.size(); i++){
		 temp.x() = -1.0f*state[i].y();
		 temp.y() = 1.0f*state[i].x();
		 f.push_back(temp);
	}

	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
		Vector3f pos = m_vVecState[0];//YOUR PARTICLE POSITION
		
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
		
}
