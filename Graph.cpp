#include "graph.h"
#include <math.h>
#include "cylinder.h"
#include "sphere.h"
#include <random>

#define cos5  0.99
#define sin5  0.08
#define cos10  0.98
#define sin10  0.17
#define cos15  0.96
#define sin15  0.25

#define cos30  0.86
#define sin30  0.5

std::map<Point, node*> nodeMap;

class Solution;
Graph::Graph(){
	
}
Graph::Graph(const int numnode,const int numend,const double intval){

	num = numnode;
	interval = intval;
	nend = numend;
	makeGraphforRandNodes();
}

void Graph::makeGraphforRandNodes(){
	
	//std::default_random_engine generator;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 generator(rd());
	std::uniform_real_distribution<double> distribution(0, interval); // range of values

	myEdge* temp;

	std::list<Point> Lr;

	for (int i = 0; i < num; ++i){

		Point p(distribution(generator), distribution(generator), distribution(generator));
	//	std::cout << "Point " << i << " :" << p.x() << "," << p.y() << "," << p.z() << std::endl;
		node *pp = new node((float)p.x(), (float)p.y(), (float)p.z());
		pp->id = i;
		nodeMap[p] = pp;
		Allnodes.push_back(pp);
		Lr.push_back(p);
	}
	

	// building Delaunay triangulation.
	Delaunay T(Lr.begin(), Lr.end());

	//CGAL::make_conforming_Delaunay_2(T); (try to find a way for 3D triangulation!) 

	CGAL::Triangulation_3<K, Tds, CGAL::Default>::Finite_edges_iterator start = T.finite_edges_begin();
	while (start != T.finite_edges_end())
	{
		const Point& p1 = T.segment(*start).point(0);
		const Point& p2 = T.segment(*start).point(1);
		temp = new myEdge(nodeMap[p1], nodeMap[p2]);
		nodeMap[p1]->edges.push_back(temp);
		nodeMap[p2]->edges.push_back(temp);
		start++;
	}

	std::cout << "number of vertexes :" << T.number_of_vertices() << std::endl;
	std::cout << "number of edges :" << T.number_of_edges() << std::endl;

	for (int i = 0; i < num; ++i){
		int sizeedge = (Allnodes[i]->edges).size();
//		std::cout << "number of edges of point (" << i << " ) is :" << sizeedge << std::endl;
	}

	std::cout << "done make graph----" << std::endl;

}

void Graph::setMaxLev(int lev){
	max_lev = lev;
}


void Graph::drawGraph(){

}

