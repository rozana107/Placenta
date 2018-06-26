#ifndef NODE_H
#define NODE_H

#include "myEdge.h"
#include <vector>


class node {
public:

	float x, y, z;
	float distance;
	node* parent;
	std::vector<node*>  parents;
	bool isVisited;
	std::vector<myEdge*> edges;

	int id;
	float guid[3]; // guiding vector 

	node();
	node(float _x, float _y, float _z);

	void printNode();
	void copyFrom(node *p);
	float distTo(node *p);
	//float distEllipsXTo(node *p);
	//float distEllipsYTo(node *p);
	void setGuidVector(float x1, float y1, float z1);
	float calcGuidVector();
	void reset();
	/*std::vector<node*>  getAllParents();*/
	void  getAllParents();
};

#endif