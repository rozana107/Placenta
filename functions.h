#include "Solution.h"


class Plane{
public:
//	Plane();
	Plane(cv::Vec3f n, cv::Point3f p){
		normal = n;
		point = p;
	};

	cv::Vec3f normal; // normal of the plane
	cv::Point3f point; // a point on the plane
};

std::list<std::pair<node*, float>> ComputeDijkstraPath(node *source, node *endp, std::vector<node*> graph, int level, std::set<node*> visited)
{
	std::set<node*>::iterator it;

	std::list<std::pair<node*, float>> path;

	float dist;
	std::set<node*> explored;
	std::priority_queue<node*, std::vector<node*>, CompareDist > pixel_queue;

	for (unsigned int i = 0; i < graph.size(); ++i)
	{
		if (level == 0){
			if (graph[i] != source)
			{
				graph[i]->isVisited = false;
				graph[i]->distance = FLT_MAX;
			}
		}
		else{
			if (graph[i] != source )
			{
				if (graph[i]->isVisited == false)
					graph[i]->distance = FLT_MAX;
			}
		}
	}

	source->distance = 0.0f;
	source->parent = NULL;
	float s[3];
	float ds = source->distTo(endp); // length of the edge
	s[0] = (endp->x - source->x) / ds;
	s[1] = (endp->y - source->y) / ds;
	s[2] = (endp->z - source->z) / ds;// e = unit vector in the direction of source-endpoint
	if (level == 0)
		source->setGuidVector(0, 1.0, 0);
	else
		source->setGuidVector(s[0], s[1], s[2]);

	pixel_queue.push(source);
	explored.empty();


	while (!pixel_queue.empty()) {
		node *u = pixel_queue.top();
		u->isVisited = true;
		explored.insert(u);		
		dist = u->distance;
		pixel_queue.pop();


		std::vector<int> neighbours; // index of neighbours
		for (unsigned int i = 0; i < u->edges.size(); i++)
			if (u->edges[i] != 0)
				neighbours.push_back(i);
		for (unsigned int i = 0; i < neighbours.size(); ++i)
		{
			// neighbour vertexes
			node *v = u->edges[neighbours[i]]->getTheOtherSide(u);


			for (it = visited.begin(); it != visited.end(); ++it)
			{
				//if ((*it) == v && (*it) != source) {
				//	std::cout << "found endpoint ..." << std::endl;
				//	path.push_back(std::pair<node*, float>(v, v->distance));
				//	v->isVisited = true;
				//	visited.insert(v);
				//	return path;
				//}
				if ((*it) == v && v == endp)
				{

					std::cout << "found endpoint..." << std::endl;
					path.push_back(std::pair<node*, float>(v, v->distance));
					v->isVisited = true;
					visited.insert(v);
					return path;
					//displayPath(path);
				}
				else
					continue;
			}


			//if (v == endp )
			//{
			//	std::cout << "found endpoint..." << std::endl;
			//	path.push_back(std::pair<node*, float>(v, v->distance));
			//	v->isVisited = true;
			//	visited.insert(v);
			//	return path;
			//	//displayPath(path);
			//} 

				float e[3];
				float dE = u->distTo(v); // length of the edge

				e[0] = (v->x - u->x) / dE;
				e[1] = (v->y - u->y) / dE;
				e[2] = (v->z - u->z) / dE;// e = unit vector in the direction of uv

				//===========================================================================
				float alt = dist + u->edges[neighbours[i]]->getWeight() /** (1 - (e[0] * u->guid[0] + e[1] * u->guid[1] + e[2] * u->guid[2]))*/; // without guiding vector

				if (alt < v->distance)
				{
					v->distance = alt;
					v->isVisited = true;
					v->parent = u;
					//================= apply guiding vector========================================== 
					//if (level == 0) {
					//	v->setGuidVector(u->guid[0], u->guid[1], u->guid[2]);
					//}
					//if (level >= 1){
					//	if (endp->y > source->y || endp->distTo(source) > 200)
					//	{ // rotate 5 degree clockwise
					//		v->setGuidVector((float)(cos10 * u->guid[0] + sin10 * u->guid[1]), (float)(-sin10 * u->guid[0] + cos10 * u->guid[1]));
					//	}
					//	else
					//	{ // rotate pi-5
					//		v->setGuidVector((float)(-cos5 * u->guid[0] + sin5 * u->guid[1]), (float)(-sin5 * u->guid[0] - cos5 * u->guid[1]));
					//	}
					//}
					//========================================================================
					path.push_back(std::pair<node*, float>(v, alt));	// track parent and distance for all visited node
					pixel_queue.push(v);
					explored.insert(v);
				}

		} // end for
		neighbours.clear();
	}
	explored.clear();
	return path;
}


static void draw(cv::Mat img, node* p, node* source, std::vector <node*> *parents/*, std::vector<std::vector<node*>> *allParents*/){

	//cv::Vec3b color;
	//color.val[0] = 128;
	//color.val[1] = 0;
	//color.val[2] = 0;
	//img.at<cv::Vec3b>(cv::Point2f(p->y, p->x)) = color;
	if (p == NULL)
	{
		std::cout << "NULL pointer: " << std::endl;
		return;
	}


	if ((p->parent) == NULL)
	{
	//	allParents->push_back(*parents);
	//	parents->clear();
		return ;
	}
	else
	{
		parents->push_back(p);
		cv::circle(img, cv::Point2f(p->y, p->x), 1, cvScalar(0, 0, 255),2,8);
		line(img, cv::Point2f(p->y, p->x), cv::Point2f(p->parent->y, p->parent->x), cvScalar(0, 255, 255), 1, CV_AA, 0);
		draw(img, p->parent, source, parents/*, allParents*/);
		
	}
}


static void drawPath(cv::Mat img, int lev, node* p, std::vector <node*> *parents){
	CvScalar color; 
	if (p == NULL)
	{
		std::cout << "NULL pointer: " << std::endl;
		return;
	}
	if ((p->parent) == NULL)
	{
		return;
	}
	else
	{
		if (lev == 0) color = cvScalar(0, 255, 255);
		if (lev == 1) color = cvScalar(200,  0, 50);
		if (lev == 2) color = cvScalar(255, 255, 0);
		if (lev >= 3) color = cvScalar(50, 0, lev*50);
		parents->push_back(p);
		cv::circle(img, cv::Point2f(p->y, p->x), 1, cvScalar(0, 0, 255), 2, 8);
		line(img, cv::Point2f(p->y, p->x), cv::Point2f(p->parent->y, p->parent->x), color, 1, CV_AA, 0);
		drawPath(img,lev, p->parent, parents);

	}
}

static void drawGraphNodes(cv::Mat img, std::vector<node*> graph){

	for (int i = 0; i < graph.size(); i++){
		float x = graph[i]->x; float y = graph[i]->y; float z = graph[i]->z;
		cv::circle(img, cv::Point2f(y, x), 1, cvScalar(100, 20, 255), 2, 8);
	}

}

void findTargets(node *source, std::vector<node*> *endpoints, std::vector<node*> graph, std::set<node*> visited,float rad){

	for (int i = 0; i < graph.size(); i++){

		// next level endpoints
		if (graph[i]->isVisited == false)
		{
			if (/*graph[i]->distTo(source) > rad/2 &&*/ graph[i]->distTo(source) < rad)
				endpoints->push_back(graph[i]);
		}
	}
}

void findSources(std::vector<node*> *sources, std::vector<node*> currParrent, float rad){
	sources->push_back(currParrent[0]);
	for (int s = 1; s < currParrent.size() ; s+=2){ //ingone first and last
		//node *source = currParrent[s];
		//if ( source->distTo(currParrent[s - 1]) < rad)
		//	sources->push_back(source);
		sources->push_back(currParrent[s]);
	}
}


void ComputeDijkstra(std::vector<node*> sources, std::vector<node*> endPoints, std::vector<node*> graph, int level, std::set<node*> visited)
{
	std::set<node*>::iterator it;

	float dist;
	std::set<node*> explored;
	std::priority_queue<node*, std::vector<node*>, CompareDist > pixel_queue;

	for (unsigned int i = 0; i < graph.size(); ++i)
	{
		graph[i]->isVisited = false;
		graph[i]->distance = FLT_MAX;

	}

	explored.empty();

	for (int s = 0; s < sources.size(); s++){
		sources[s]->distance = 0;
		sources[s]->parent = NULL;
		pixel_queue.push(sources[s]);
	}
	while (!pixel_queue.empty()) {
		node *u = pixel_queue.top();
		u->isVisited = true;
		explored.insert(u);
		dist = u->distance;
		pixel_queue.pop();


		std::vector<int> neighbours; // index of neighbours
		for (unsigned int i = 0; i < u->edges.size(); i++)
			if (u->edges[i] != 0)
				neighbours.push_back(i);
		for (unsigned int i = 0; i < neighbours.size(); ++i)
		{
			// neighbour vertexes
			node *v = u->edges[neighbours[i]]->getTheOtherSide(u);
			if (v->isVisited == true)
				continue;
			else
			{
				//===========================================================================
				float alt = dist + u->edges[neighbours[i]]->getWeight();

				if (alt < v->distance)
				{
					v->distance = alt;
					v->isVisited = true;
					v->parent = u;
					pixel_queue.push(v);
					explored.insert(v);
				}
			}
		} // end for
		neighbours.clear();
	}
	
	explored.clear();

}



// intersect3D_SegmentPlane(): find the 3D intersection of a segment and a plane
//    Input:  S = a segment, and Pn = a plane = {Point V0;  Vector n;}
//    Output: *I0 = the intersect point (when it exists)
//    Return: 0 = disjoint (no intersection)
//            1 =  intersection in the unique point *I0
//            2 = the  segment lies in the plane
int intersect3D_SegmentPlane(Cylinder *S, Plane Pn, cv::Point3f &I)
{
	// Vector u = S.P1 - S.P0; // dir vector
//	Vector3f   u = Vector3f(S->nod1->x - S->nod2->x, S->nod1->y - S->nod2->y, S->nod1->z - S->nod2->z); // dir vector
	Vector3f   u = Vector3f(S->nod1->x - S->nod2->x, S->nod1->y - S->nod2->y, S->nod1->z - S->nod2->z); // dir vector


	//Vector    w = S.P0 - Pn.V0;
	Vector3f w = Vector3f(S->nod2->x - Pn.point.x, S->nod2->y - Pn.point.y, S->nod2->z - Pn.point.z);

	//float     D = dot(Pn.n, u);
	float D = (Pn.normal).val[0] * u.x + (Pn.normal).val[1] * u.y + (Pn.normal).val[2] * u.z;// dot(Pn.normal, u);
	//float     N = -dot(Pn.n, w);
	float N = -((Pn.normal).val[0] * w.x + (Pn.normal).val[1] * w.y + (Pn.normal).val[2] * w.z);// dot(Pn.normal, u);

	//if (fabs(D) < SMALL_NUM) {           // segment is parallel to plane
	//	if (N == 0)                      // segment lies in plane
	//		return 2;
	//	else
	//		return 0;                    // no intersection
	//}
	if (fabs(D)< 0.00000001){ // segment is parallel to plane
		if (N == 0)
			return 2;        // segment lies in plane
		else
			return 0;        // no intersection
	}

	//// they are not parallel
	//// compute intersect param
	//float sI = N / D;
	float sI = N / D;
	//if (sI < 0 || sI > 1)
	//	return 0;                        // no intersection

	if (sI < 0 || sI > 1.0f)
		return 0;

	//*I = S.P0 + sI * u;                  // compute segment intersect point
	I = cv::Point3f(S->nod2->x, S->nod2->y, S->nod2->z) + sI * cv::Point3f(u.x,u.y,u.z);
	return 1;
}
//===================================================================

//******************this function draws intersection of vessels and the plan *******************************
void getInterSections(std::vector<Cylinder*> &vessels, cv::Mat &intersection, int maximum_lev,
	std::vector<std::pair <cv::Point3f, int >> &intersectedPoints, std::vector<cv::Point3f> &sites, std::vector<Shape*> &shapes) {

	Plane pln(cv::Vec3f(0, 1.0, 0), cv::Point3f(450, 3* intersection.size().height/4, 450)); // y = 3/4 height 

	for (int i = 0; i < vessels.size(); i++) {
		std::pair< cv::Point3f, int >intersect; // point and level

		int colide = intersect3D_SegmentPlane(vessels[i], pln, intersect.first);
		if (colide == 1) {
			intersect.second = vessels[i]->getLev();
			intersectedPoints.push_back(intersect);
			float x = intersect.first.x; float y = intersect.first.y; float z = intersect.first.z;
			
			cv::circle(intersection, cv::Point2f(x, z), 5.0f / (vessels[i]->getLev() + 1.0f), cvScalar(0, 0, 0)/* cvScalar(95, 55, 168)*/, CV_FILLED, 8);
			//if (vessels[i]->getLev() == maximum_lev - 1) {
			//	sites.push_back(cv::Point3f(x, y, z));
			//	cv::circle(intersection, cv::Point2f(x, z), 10.0f / (vessels[i]->getLev() + 1.0f), cvScalar(0, 0, 0), CV_FILLED, 8);
			//}
			
		}
		if (colide == 2) {
			cv::line(intersection, cv::Point2f(vessels[i]->nod1->x, vessels[i]->nod1->z),
				cv::Point2f(vessels[i]->nod2->x, vessels[i]->nod2->z), cvScalar(95, 55, 168), 5.0f / (vessels[i]->getLev() + 1), 8);
		}
	}
}

//void trimIntersectionDistance(std::vector<std::pair <cv::Point3f, int >> &intersectedPoints) {
//
//}


//******************* check nearby space for the sites****************************
//   locate shapes around each site
//***************************************************************************
void drawShapes(cv::Mat &intersection, std::vector<std::pair <cv::Point3f, int >> &intersectedPoints, std::vector<Shape*> &shapes, int max_level) {
	
	//    read shapes
	char curfile[80];
	char filenames[9][20] = {"shape1","shape2","shape3","shape4","shape5","shape6","shape7","shape8","shape9"};

	cv::Mat dst(intersection.size(), CV_32FC1); // output from distanceTransform
	cv::Mat input = cv::imread("intersections.png", 0);// cv::Mat::zeros(intersection.size(), CV_8UC1);
	cv::Mat label = cv::Mat::zeros(intersection.size(), CV_32F); //CV_32SC1
	for (int k = 0; k< 9;k++) {
		sprintf_s(curfile, "E:/myApp/Placenta Project/Papers/%s.jpg", filenames[k]); // shapes   . jpg
		std::string str(filenames[k]);
		cv::Mat img = cv::imread(curfile,1);
		Shape *shape = new Shape(img);
		shapes.push_back(shape);		
	}

	cv::distanceTransform(input,dst,label, CV_DIST_L2,3);
	cv::imwrite("distanceTransform.png",dst);
	cv::imwrite("lable.png", label);

	//std::cout << (float) lable.at<float>(0,0) << std::endl;
	//std::cout << (float) lable.at<float>(lable.rows-1, lable.cols-1) << std::endl;

	//trimIntersectionDistance(intersectedPoints);

	for (int k = 0; k < intersectedPoints.size();k++) {
		    Shape *curShape;
		//if (intersectedPoints[k].second == 0 || intersectedPoints[k].second ==1) {
			cv::Point3f center = intersectedPoints[k].first;
			if (intersectedPoints[k].second == 0)
				curShape = shapes[0];
			else
				curShape = shapes[rand() % shapes.size()]; // shapes[ rand() % shapes.size()];
			cv::Mat dst1(curShape->getShape().size(), CV_8UC3);
			dst1 = curShape->getShape().clone();

			float ratio =  1.0f / (intersectedPoints[k].second + 2.0f);
			curShape->reScaleShape(dst1,ratio,1); // 
			
			//float lab = curShape->getLabel(intersectedPoints[k].first,label);
			//float err = abs(curShape->cellMax - curShape->maxLen);
			//std::cout << "err befor: " << err << std::endl;
			//float thresh = err/4.0f;
			//while (err >= thresh) {
			//	if (err < thresh) break;
			//	else {
			//		//curShape->rotateShape(dst1, 1, 1);
			//		curShape->reScaleShape(dst1, 0.8f, 1); // 
			//		err = abs(curShape->cellMax - curShape->maxLen);
			//	}
			//}
			//std::cout << "err after: " << err << std::endl;

			int width = dst1.cols;
			int height = dst1.rows;

			for (int i = - floor(height/2) ; i < floor(height/2) ;i++) {
				for (int j = - floor(width / 2); j < floor(width / 2); j++) {
					if (dst1.at<cv::Vec3b>(i + height / 2, j + width / 2) != cv::Vec3b(119, 154, 164) 
						&& center.z + i > 0 && center.z + i < intersection.rows && center.x + j > 0 && center.x + j < intersection.cols)
					{
						intersection.at<cv::Vec3b>(center.z + i, center.x + j) = dst1.at<cv::Vec3b>(i + height / 2, j + width / 2);
					}

				}
			}
		//}
	}

	//shapes.clear();
	//intersectedPoints.clear();

}