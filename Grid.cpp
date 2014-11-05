#include "Grid.h"

Grid::Grid(int m, int n){
	_n = n;
	_m = m;
	connections = new SpringList*[m];
	SpringList* data = new SpringList[m*n];
	for(int i = 0; i < m; i++){
		connections[i] = data + n*i;
	}	
}

Grid::~Grid(){
	delete[] connections[0];
	delete[] connections;
}

void Grid::getGridIndices(int particleIdx, int &i, int &j){

	i = particleIdx/_m;
	j = particleIdx%_n;

}

int Grid::getLinearIndex(int i, int j){
	
	if(i < _m && j < _n){
		return 2*(i*_n + j);
	}
	return -1;
}

SpringList& Grid::getConnections(int i, int j){

	return connections[i][j];
}

SpringList& Grid::getConnections(int linearIdx){
	int particleIdx = linearIdx/2;

	int k = particleIdx/_n;
	int l = particleIdx%_n;

	return getConnections(k,l);
}

void Grid::addConnection(int i, int j,int k, int l, float restLength, float springConstant){
	
	if(k >= 0 && k < _m && l >= 0 && l < _n){
		connections[i][j].addSpring(k,l,restLength,springConstant);
	}
}

Vector3f Grid::calculateSpringForce(int linearIdx, vector<Vector3f> &pos){
	
	int springLinearIdx;
	Vector3f Xnear,Xfar,disp,f_spring;
	Spring spring;

	Xnear = pos[linearIdx];
	SpringList springs = getConnections(linearIdx);

	for(int k = 0; k < springs.data.size(); k++){
		spring = springs.data[k];		
		springLinearIdx = getLinearIndex(spring.k,spring.l);
		Xfar = pos[springLinearIdx];
		disp = Xnear - Xfar;
		f_spring += -spring.constant*(disp.abs() - spring.rest)*(disp/disp.abs());
		
	}
	return f_spring;
}

void Grid::drawRectangle(int particleIdx,vector<Vector3f> &pos){
	int i, j;
	getGridIndices(particleIdx,i,j);
	int k = getLinearIndex(i+1,j);
	int l = getLinearIndex(i,j+1);
	int m = getLinearIndex(i+1,j+1);
	
	if(k != -1 && l != -1 && m != -1){
		Vector3f pos0 = pos[2*particleIdx];
		Vector3f pos1 = pos[k];
		Vector3f pos2 = pos[l];
		Vector3f pos3 = pos[m];
		
		Vector3f n0 = Vector3f::cross((pos1 - pos0).normalized(),(pos2 - pos0).normalized());
		Vector3f n1 = Vector3f::cross((pos0 - pos1).normalized(),(pos3 - pos1).normalized());
		Vector3f n2 = Vector3f::cross((pos3 - pos2).normalized(),(pos0 - pos2).normalized());
		Vector3f n3 = Vector3f::cross((pos2 - pos3).normalized(),(pos1 - pos3).normalized());

		glDisable(GL_LIGHTING);
		glPushMatrix();
		glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 1.0f, 1.0f);
			glNormal3f(n2[0],n2[1],n2[2]);
			glVertex3f(pos2[0],pos2[1],pos2[2]);
			glNormal3f(n0[0],n0[1],n0[2]);
			glVertex3f(pos0[0],pos0[1],pos0[2]);
			glNormal3f(n1[0],n1[1],n1[2]);
			glVertex3f(pos1[0],pos1[1],pos1[2]);
		glEnd();

		glBegin(GL_TRIANGLES);
			glColor3f(0.0f, 0.2196f, .7216f);
			glNormal3f(n2[0],n2[1],n2[2]);
			glVertex3f(pos2[0],pos2[1],pos2[2]);
			glNormal3f(n1[0],n1[1],n1[2]);
			glVertex3f(pos1[0],pos1[1],pos1[2]);
			glNormal3f(n3[0],n3[1],n3[2]);
			glVertex3f(pos3[0],pos3[1],pos3[2]);
		glEnd();

		glPopMatrix();
		glEnable(GL_LIGHTING);
	}

}