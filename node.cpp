#include "node.h"
#include <iostream>

node::node(){
//	isVisited = false;
}
node::node(float _x, float _y, float _z){
	this->x = _x;
	this->y = _y;
	this->z = _z;
	this->distance = 0.0;
	this->parent = NULL;
	this->guid[0] = 0; // x
	this->guid[1] = 0; //y
	this->guid[2] = 0; //z
	for (unsigned int i = 0; i < edges.size(); i++){
		edges[i]->weight = 0;
	}
	isVisited = false;
}


void node::printNode(){
	std::cout << this->x << "," << this->y << "," << this->z << std::endl;
}

void node::copyFrom(node *p){
	this->x = p->x;
	this->y = p->y;
	this->z = p->z;
	this->distance = p->distance;
	this->isVisited = p->isVisited;
	this->parent = p->parent;
	this->guid[0] = p->guid[0]; // x,y,z
	this->guid[1] = p->guid[1];
	this->guid[2] = p->guid[2];
	this->edges.clear();
	this->edges.reserve(p->edges.size());
	this->id = p->id;
	for (unsigned int i = 0; i <p->edges.size(); i++){
		this->edges.push_back(p->edges[i]);
		//this->edges[i] = new edge(p, p->edges[i]->getTheOtherSide(p));
		//this->edges[i] = p->edges[i];
	}
}
float node::distTo(node *p)
{
	return sqrt((x - p->x) * (x - p->x) + (y - p->y) * (y - p->y) + (z - p->z) * (z - p->z));
}


void node::setGuidVector(float x1, float y1, float z1){
	float gv = sqrt((x1 - this->x)*(x1 - this->x) + (y1 - this->y)*(y1 - this->y) + (z1 - this->z)*(z1 - this->z));
	this->guid[0] = (x1 - this->x) / gv; // unit guid vector
	this->guid[1] = (y1 - this->y) / gv;
	this->guid[2] = (z1 - this->z) / gv;
}
float node::calcGuidVector(){
	return guid[3];
}

void node::reset(){
	isVisited = false;
	distance = 0.0;
	guid[0] = 0.0; // x
	guid[1] = 0.0; //y
	guid[2] = 0.0; //z
	for (unsigned int i = 0; i < edges.size(); i++){
		edges[i]->weight = 0.0f;
	}
}

void node::getAllParents(){
//	parents.push_back(this);
	node *p = this->parent;
	while (p != NULL)
	{
		parents.push_back(p);
		p = p->parent;
	}
}


