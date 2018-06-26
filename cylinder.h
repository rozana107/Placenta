#pragma once

#ifndef CYLINDER_HEADER
#define CYLINDER_HEADER
//=============================================================================
// cylinder.h
//
//Author: Doron Nussbaum (C) 2015 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. creates a cylinder object. 
//
//
// Description:
//--------------
//  Creates a cylinder geometry.  Currently it is used only to create a geometry of a sphere. It can be converted to a stand alone function. 
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================




#include "graphicsObject.h"
#include "node.h"
class Cylinder : public GraphicsObject
{
public:
	Cylinder();
	Cylinder(node *n1,node *n2);
	~Cylinder();
	// Return the number of array elements that form the cylinder
	//static int Cylinder::createCylinder(int numLong, Vertices &vtx, Indices &ind, Vector4f color);
	static int Cylinder::createVessels(int numLong, Vertices &vtx, Indices &ind, Vector3f direction, Vector4f color, int lev);
	//static int Cylinder::CreateCylinder(float cylinder_radius = 0.5, float cylinder_height = 1.0, int num_cylinder_samples = 90);
	int cId1,cId2;// cylinder ids (keep the id of the endpoint nodes)
	void setId(int id1, int id2);
	void setLev(int lev);
	int getLev();
	node *nod1, *nod2; // cylinder contain 2 nodes
	int level; 
	int getId(); // return id1
};


#endif