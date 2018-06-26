#include "node.h"
#include <iostream>


myEdge::myEdge() {

}

myEdge::myEdge(node *p1, node *p2)
{
	first = p1;
	second = p2;
	calcWeight();
};

void myEdge::calcWeight(){ // spacial distance 
	weight = first->distTo(second); // 
}

float myEdge::getWeight() const {
	return weight;
}

bool myEdge::isVertical(){
	/*if (first->edges[DOWN] || first->edges[TOP])
	vertical = true;*/
	return (first->x == second->x);
}

//bool Edge::isEqual() {
//	if (first == second && second == first) return true;
//}

void myEdge::getEdgeDir(){
	e[0] = first->x - second->x;
	e[1] = first->y - second->y;
	e[2] = first->z - second->z;
}

void myEdge::printEdge(){
	std::cout << "first: [" << first->x << "," << first->y << "," << first->z 
		<< "] [second :" << second->x << "," << second->y << "," << second->z << "]" << std::endl;
}