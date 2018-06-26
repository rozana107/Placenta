#ifndef _MYEDGE_H_
#define _MYEDGE_H_

#include <math.h>
class node;

class myEdge {

	node *first;
	node *second;
	bool vertical;
	float e[3]; // edge direction

public:

	myEdge();
	myEdge(node *p1, node *p2);
	myEdge(node *arg_target, float arg_weight)
		: second(arg_target), weight(arg_weight) { }

	node *getTheOtherSide(node *p) {
		return p == first ? second : first;
	}
	bool isVertical();
	void calcWeight(); // spacial distance: P
	float getWeight() const;

	void getEdgeDir();
	//	bool isEqual();
	float weight;
	void printEdge();
};

struct Comparator {

	bool operator()(const myEdge* e1, const myEdge* e2)
	{
		return (e1->getWeight() > e2->getWeight());
	}
};

#endif