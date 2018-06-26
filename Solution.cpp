//=============================================================================
// solution.c

#include "Solution.h"
#include "functions.h"
//#include "include/Point2.h"
//#include "include/VoronoiDiagramGenerator.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"


Solution *Solution::sol = NULL;

Solution::Solution() : numFrames(0)
{
}

Solution::~Solution()
{

}


// initializing the opengl functions and windows
int Solution::initOpenGL()
{
	//initialize OpenGL
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(900, 900);
	glutCreateWindow("Placenta");
	glClearColor(0.05,0.05,0.05, 1.0); // 0.58,0.53,0.42

	glEnable(GL_CLIP_DISTANCE0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glutDisplayFunc(Solution::renderCB);
	glutReshapeFunc(Solution::winResizeCB);
	glutKeyboardFunc(Solution::keyboardCB);
	glutMouseFunc(Solution::mouseClickCB);
	glutSpecialFunc(Solution::specialKeyboardCB);
	glutTimerFunc(FRAME_TIME, Solution::timerCB, UPDATE_RENDERRED_OBJECTS);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		printf("Error - %s \n", glewGetErrorString(res));
		return (-1);
	}


	return 0;
}

/************************************************************/

// render callback function.  This is a static funcion


void Solution::renderCB()
{

	sol->render();
	
}


/************************************************************/

// keyboard callback function.  This is a static funcion


void Solution::keyboardCB(unsigned char key, int x, int y)
{
	sol->keyboard(key, x, y);
}

/****************************************************************/
void Solution::mouseClickCB(int button, int state, int x, int y)
{
	sol->mouseClick(button, state, x, y);
}
/************************************************************/

// special keyboard callback function.  This is a static funcion



void Solution::specialKeyboardCB(int key, int x, int y)
{
	sol->specialKeyboard(key, x, y);
}


/************************************************************/

// window resize callback function.  This is a static funcion



void Solution::winResizeCB(int width, int height)
{
	sol->winResize(width, height);
}

/************************************************************/

// timer  callback function.  This is a static funcion


void Solution::timerCB(int operation)
{

	glutTimerFunc(FRAME_TIME, Solution::timerCB, UPDATE_RENDERRED_OBJECTS);	
	sol->timer(operation);

}


/************************************************************/

// timrt  function.  


int Solution::timer(int operation)
{
	numFrames++;
	switch (operation) {
	case UPDATE_RENDERRED_OBJECTS:
		updateObjects(numFrames);
		break;
	default:
		break;
	}
	return(0);
}


bool Solution::findId(Cylinder *vessel, std::vector<Cylinder*> vessels){
	bool flag = false;
	for (int i = 0; i < vessels.size();i++){
		if (vessel->getId() == vessels[i]->getId())
			flag = true;
	}
	return flag;
}

/******************************************************************************/
// initialization of the solution
int Solution::initSolution(Graph *g, cv::Mat img, cv::Mat intersection)
{
	//std::cout << "Size of cylinder class: " << sizeof(Cylinder) << std::endl;
	//std::cout << "Size of shader class: " << sizeof(Shader) << std::endl;
	std::string path = "E:\\myApp\\Placenta Project\\code\\Placenta_v2\\Placenta_v2\\";
	g->endpoints.reserve(g->nend);
	int rc;
	Vertices vtx;
	Indices ind;

	// create the shader objects
	rc = shader.createShaderProgram("vertSahder.vert", "fragShader.frag");
	if (rc != 0) {
		fprintf(stderr, "Error in generating shader (solution)\n");
		rc = -1; 
		goto err;
	}
	

	std::cout << "num of end points: " << g->nend << std::endl;
	std::cout << "max level: " << g->max_lev << std::endl;
	maximum_lev = g->max_lev;
	//g->source = new node();

	int lev = 0; float rad, radi;
	Vector4f color = Vector4f(1.0, 0.0, 0.0, 1.0);

	/*********draw graph nodes*********/

//	drawGraphNodes(img,g->Allnodes);
//	cv::imwrite(path + "graphNodes.png", img);

	/**********************draw 3D nodes*************************/
	//for (unsigned int i = 0; i< g->num; i++){
	//	float x = g->Allnodes[i]->x; float y = g->Allnodes[i]->y; float z = g->Allnodes[i]->z;
	//	Sphere::createSphere(10, 10, vtx, ind, Vector4f(1.0, 1.0, 1.0, 1.0));
	//	testSphere.createVAO(shader, vtx, ind);
	//	testSphere.setInitialPosition(x, y, z);
	//	testSphere.setScale(1, 1, 1);
	//	Spheres.push_back(testSphere);
	//}

	/*************************************************************/

	do{

		if (lev == 0)
		{
			for (unsigned int i = 0; i < g->num; ++i)
			{
				float x = g->Allnodes[i]->x; float y = g->Allnodes[i]->y; float z = g->Allnodes[i]->z;

				if (((g->interval / 8) > y) && ((g->interval / 2) - (g->interval / 6) < x) &&
					(x < (g->interval / 2) + (g->interval / 6)) && ((g->interval / 2) - (g->interval / 6) < z) &&
					(z < (g->interval / 2) + (g->interval / 6))) {

					if (g->Allnodes[i] != NULL) {
						selecteds.push_back(g->Allnodes[i]);
					}
				}

				if (((4 * g->interval / 5) < y) && (y <  g->interval) && ((x - g->interval / 2)*(x - g->interval / 2) +
					(z - g->interval / 2)*(z - g->interval / 2) < (4 * g->interval / 5)*(4 * g->interval / 5))
					&& ((x - g->interval / 2)*(x - g->interval / 2) + (z - g->interval / 2)*(z - g->interval / 2) > (g->interval / 5)*(g->interval / 5)))
				{
					if (g->Allnodes[i] != NULL)
						selectedt.push_back(g->Allnodes[i]);
				}
			}

			sources.push_back(selecteds[rand() % selecteds.size()]); // 1 source for lev 0

			for (int i = 0; i < g->nend; i++) {
				targetPoints.push_back(selectedt[rand() % selectedt.size()]);
			}

			selectedt.clear();
			selecteds.clear();
		}


		if (lev >= 1)
		{
			float sumS = 0;
			std::cout << "size of the possible sources: " << selecteds.size() << std::endl; // selected along the path from the prev level
			for (int i = 0; i <selecteds.size(); i++) {
				sources.push_back(selecteds[i]);
			}
			rad = g->interval / ((lev)* 5.0f);

			std::cout << "rad  : " << rad << " , lev :" << lev << std::endl;

			//===============possible endpoints for every source point=====================================
			for (int k = 0; k < sources.size(); k++)
			{
				for (int i = 0; i < g->num; i++)
				{
					if (g->Allnodes[i]->distTo(sources[k]) < rad && g->Allnodes[i]->y >= sources[k]->y) {
						selectedt.push_back(g->Allnodes[i]);
					}
				}
				//=============== random endpoint for every source point=====================================

				if (lev == 1) {
					//for (int j = 0; j < 1; j++){
					targetPoints.push_back(selectedt[rand() % selectedt.size()]); // 1endpoint per source node
																				  //}
				}
				else {
					for (int j = 0; j < lev * 2 /*selectedt.size() / (lev * 4)*/; j++) { // if level 2 or higher
						targetPoints.push_back(selectedt[rand() % selectedt.size()]);
					}
				}


				selectedt.clear();
			}
			selecteds.clear();
		}

		std::cout << " number of sources:  " << sources.size() << std::endl;
		std::cout << " number of target Points:  " << targetPoints.size() << std::endl;
		//std::vector<node*> temp;
		//// resize the sources to smaller number compare to the target points
		//for (int i = 0; i <targetPoints.size() / ((lev+1) *10); i++){
		//	temp.push_back(sources[rand() % sources.size()]);
		//}
		//
		//std::cout << " number of sources after pruning:  " << temp.size() << std::endl;


		ComputeDijkstra(sources, targetPoints, g->Allnodes, lev, visited);

		for (int p = 0; p < targetPoints.size(); p++) {
			std::vector<node*> parents;
			drawPath(img, lev, targetPoints[p], &parents);
			for (unsigned int i = 0; i< parents.size(); i++) {

				// get the sourses for next level---the last node not add as source to next level
				if (i < parents.size() - 1)
					selecteds.push_back(parents[i]);
				//======================draw 3D cylinder=========================================
				Vector3f dir = Vector3f(parents[i]->x - parents[i]->parent->x, parents[i]->y - parents[i]->parent->y,
					parents[i]->z - parents[i]->parent->z);

				Cylinder *vessel = new Cylinder(parents[i], parents[i]->parent);
				vessel->setLev(lev);
				vessel->createVessels(4, vtx, ind, dir, color, lev);
				vessel->createVAO(shader, vtx, ind);
				Vector4f curPos = Vector4f((parents[i]->x + parents[i]->parent->x) / 2.0, (parents[i]->y + parents[i]->parent->y) / 2.0,
					(parents[i]->z + parents[i]->parent->z) / 2.0, 1.0);
				vessel->setId(parents[i]->id, parents[i]->parent->id);
				vessel->setInitialPosition(curPos.x, curPos.y, curPos.z);
				vessel->setScale(1.0, 1.0, 1.0);
				// if not already added to the vector
				if (findId(vessel, vessels) == false) {
					vessels.push_back(vessel);
				}
			}
			parents.clear();
		}


		sources.clear();
		targetPoints.clear();

		std::cout << "level " << lev << " is done -----------" << std::endl;
		lev++;
	} while (lev < maximum_lev);


	cv::imwrite(path + "path.png", img);

	//************Plane information******************
	
	intersection = cv::Mat::zeros(cvSize(g->interval,g->interval), img.type());
	for (int r = 0; r < intersection.rows;r++){
		for (int c = 0; c < intersection.cols;c++){
			intersection.at<cv::Vec3b>(r, c) = cv::Vec3b(120, 154, 165); //bgr
		}
	}
//******************find intersections******************

	getInterSections(vessels,intersection, maximum_lev, intersectedPoints,sites, shapes);
	cv::imwrite("intersections.png", intersection);

	drawShapes(intersection, intersectedPoints, shapes, maximum_lev);
	////AvaliableDistanceFromsites(vessels, intersectedPoints, shapes);

	cv::imwrite( "shapes.png", intersection);
    cv::namedWindow("vessles", 1);
//	cv::createTrackbar("Hight", "vessles", &threshval, 35,  , (void*)in_img);
	imshow("vessles", intersection);
	intersectedPoints.clear();
	shapes.clear();

	// set the camera default
	cam.setCamera(Vector3f(0, 550, 600), Vector3f(100, yView, 150), Vector3f(0, 1, 0)); // yView =0

	err:
	return 0;
}


/**********************************************************************/

void Solution::setSolution(Solution * _sol)
{
	Solution::sol = _sol;
}


/************************************************************/

// render function.  


void Solution::render()
{

	Vector3f viewerPosition;
	Vector3f lookAtPoint;
	Vector3f upVector;
	Matrix4f viewMat, projMat;


	viewerPosition =   Vector3f(0, 0, 100);
	lookAtPoint =  Vector3f(0, 0, 0);
	upVector = Vector3f(0, 1, 0);

	// use the created shader
	shader.useProgram(1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);
	
	// set the camera matrix

	//viewMat = Matrix4f::cameraMatrix(viewerPosition, lookAtPoint, upVector);
	viewMat = cam.getViewMatrix(NULL);
	// move matrix to shader
	shader.copyMatrixToShader(viewMat, "view");
	

	// set the projection matrix
	//projMat = Matrix4f::symmetricPerspectiveProjectionMatrix(30, 1, .1f, 1000);
	projMat = cam.getProjectionMatrix(NULL);
	// move matrix to shader
	shader.copyMatrixToShader(projMat, "projection");

	
	//for (auto spheres : AllLevelSpheres) {
	//	std::cout << " Shperes " << spheres.size() << "\n";
	//	if (spheres.size() != 0){
	//		for (auto sphere : spheres) {
	//			sphere.render(shader);
	//		}
	//	}
	//}

/*********************Render Vessels***************************************/
	for (int k = 0; k <vessels.size(); k++) {
		vessels[k]->render(shader);
	}

	glutSwapBuffers();
}
/************************************************************/

// keyboard handling function. 


void Solution::keyboard(unsigned char key, int x, int y)
{
	static int nc = 0;
	nc++;
	switch (key) {
	case 033:
	case 'Q':
	case 'q':
		this->~Solution();
		exit(1);
		break;
	case 'a':
		cam.roll(1);
		break;
	case 'd':
		cam.roll(-1);
		break;
	case 'w':
		cam.moveForward(1);
		break;
	case 's':
		cam.moveBackward(1);
		break;
	case 'x':
		cam.yaw(1);
		break;
	case 'z':
		cam.yaw(-1);
		break;
	case '+':
		shininess += 2;
		break;
	case '-':
		shininess -= 2;
		if (shininess < 0) shininess = 0;
		break;
	case 'M':
		useAmbient = 0;
		break;
	case 'm':
		useAmbient = 1;
		break;
	case 'N':
		useDiffuse = 0;
		break;
	case 'n':
		useDiffuse = 1;
		break;
	case 'B':
		useSpecular = 0;
		break;
	case 'b':
		useSpecular = 1;
		break;
	//case 'i':

	//	break;

	default:
		break;
	}
}


/************************************************************/

// special keyboard handling  function.  



void Solution::specialKeyboard(int key, int x, int y)
{
	static int nc = 0;
	nc++;
	switch (key) {
	case 033:
	case 'Q':
	case 'q':
		this->~Solution();
		exit(1);
		break;
	case GLUT_KEY_UP:
		cam.pitch(1);
		break;
	case GLUT_KEY_DOWN:
		cam.pitch(-1);
		break;
	case GLUT_KEY_LEFT:
		cam.yaw(1);
		break;
	case GLUT_KEY_RIGHT:
		cam.yaw(-1);
		break;
	default:
		break;
	}
}

/*********************************************************/
void Solution::mouseClick(int button, int state, int x, int y){
	// Respond to mouse button presses.
	// If button1 pressed, mark this state so we know in motion function.
	if (button == GLUT_LEFT_BUTTON)
	{
		//g_bButton1Down = (state == GLUT_DOWN) ? TRUE : FALSE;
		yView = y ;
	}

}

/************************************************************/

// window resize handling function.  



void Solution::winResize(int width, int height)
{
	glViewport(0,  0, width, height);
}

/***********************************************************/
// update the state of the objects

int Solution::updateObjects(int numFrames)
{

	// recall that this will be carried out in the model space
	//testCylinder.incrementRotations(0, 0, -0.5);

	glutPostRedisplay();
	return 0;
}
