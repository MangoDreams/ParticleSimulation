#ifndef _SPRING_H
#define _SPRING_H

#include <vecmath.h>
#include <vector>
#include <iostream>

#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

using namespace std;

class Spring{
	public:
		int k,l;
		float rest;
		float constant;
};

class SpringList {
	public:
		void addSpring(int k, int l, float restLength,float springConstant){
			Spring temp;
			temp.k = k;
			temp.l = l;
			temp.rest = restLength;
			temp.constant = springConstant;
			data.push_back(temp);
		}
		vector<Spring> data;

};
/// (i,j) : (k, p, Spring1), (r,k,Spring3), (r,a, Spring4)

class Grid {
	public:
		//m by n Grid system
		Grid(int m, int n);
		~Grid();
		
		void getGridIndices(int particleIdx, int &k, int &l);
		int getLinearIndex(int i, int j);
		
		SpringList& getConnections(int i, int j); 
		SpringList& getConnections(int linearIdx);
		void addConnection(int i, int j,int k, int l, float restLength, float springConstant);
		
		Vector3f calculateSpringForce(int i, vector<Vector3f> &pos);
		void drawRectangle(int particleIdx,vector<Vector3f> &pos);

	private:
		int _m;
		int _n;
		SpringList** connections; 
};



#endif