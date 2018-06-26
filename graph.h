
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <list>
#include <map>
#include <queue>
#include <random>


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Delaunay_triangulation_cell_base_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/IO/Color.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_3<CGAL::Color, K> Vb;
typedef CGAL::Triangulation_data_structure_3<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_3<K, Tds, CGAL::Default>       Delaunay;
typedef Delaunay::Point   Point;
//typedef Delaunay::Edge    Edge;

#include "cylinder.h"
#include "node.h"

#ifndef GRAPH_H
#define GRAPH_H




//class edge;
//class node {
//public:
//
//	float x, y, z;
//	float distance;
//	node* parent;
//	bool isVisited;
//	std::vector<edge*> edges;
//	
//	int id;
//	float guid[3]; // guiding vector 
//
//	node();
//	node(float _x, float _y, float _z);
//
//	void printNode();
//	void copyFrom(node *p);
//	float distTo(node *p);
//	//float distEllipsXTo(node *p);
//	//float distEllipsYTo(node *p);
//	void setGuidVector(float x1, float y1, float z1);
//	float calcGuidVector();
//	void reset();
//	std::vector<node*>  getAllParents();
//};

//class edge {
//	
//	node *first;
//	node *second;
//	bool vertical;
//	float e[3]; // edge direction
//
//public:
//	
//	edge();
//	edge(node *p1, node *p2);
//	edge(node *arg_target, float arg_weight)
//		: second(arg_target), weight(arg_weight) { }
//
//	node *getTheOtherSide(node *p) {
//		return p == first ? second : first;
//	}
//	bool isVertical();
//	void calcWeight(); // spacial distance: P
//	float getWeight() const;
//
//	void getEdgeDir();
//	//	bool isEqual();
//	float weight;
//	void printEdge();
//};


class Sphere;

class Graph{
public:

	int num;  double interval; int nend;
	std::vector<node*> Allnodes;
	node *source;
	std::vector<node*> endpoints;
	std::vector<std::vector<node*>> parentsOfthePath;


	int max_lev;
	Graph();
	Graph( int numnode, int numend, double interval);
	void makeGraphforRandNodes();
//	void generateVessles(node* endp, Shader shader, Vertices vtx, Indices ind, Sphere testSphere, std::vector<Sphere> testSpheres);
	void setMaxLev(int lev);
//	std::list<std::pair<node*, float>> ComputeDijkstraPath(node *source, node *endp, /*std::vector<node*> graph,*/ int level, std::set<node*> visited);
	void drawGraph();
};

//struct Comparator {   // => moved to edge class
//
//	bool operator()(const edge* e1, const edge* e2)
//	{
//		return (e1->getWeight() > e2->getWeight());
//	}
//};

struct CompareDist
{
	bool operator()(node *n1, node *n2)
	{
		return n1->distance > n2->distance;
	}
};

#endif