#include "Scene.h"
#include <stdio.h>
#include <cmath>

bool Scene::perspective = true;
int Scene::width = 0, Scene::height = 0, Scene::mouse_anchor_x = 0, Scene::mouse_anchor_y = 0;
float Scene::camera_angle_h = 0.0,Scene::camera_angle_v = 30.0;
std::vector<GLObject *> Scene::objList;
bool Scene::light_1_on = true, Scene::light_2_on = true;
GLRobot * Scene::robot = NULL;

const double PI = 3.14159265;

Scene::Scene()
{
}


Scene::~Scene(void)
{
}

void Scene::display(){
	//printf("Drawing\n");
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	for(int i = 0 ; i < (int)(objList.size()); i++)
	{
		glPushMatrix();
		objList[i]->draw();
		glPopMatrix();
	}
	glPopMatrix();
	glutSwapBuffers();
}

void Scene::reshape(int w, int h){
	//printf("Reshaping(%d,%d)", w, h);
	width = w;
	height = h;
	glViewport(0,0,(GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w * h > 0){
		//printf("[%d]\n",perspective);
		if(perspective)
			gluPerspective(60.0,(GLfloat)w/h,1.0,200.0);
		else{
			glOrtho(-3.0*2/h * w,3.0 * 2/h * w, -3.0 * 2,3.0*2,1,200);
		}
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GLfloat cosv = cos(camera_angle_v/180.0*PI);
	GLfloat sinv = sin(camera_angle_v/180.0*PI);
	GLfloat cosh = cos(camera_angle_h/180.0*PI);
	GLfloat sinh = sin(camera_angle_h/180.0*PI);
	gluLookAt(-15.0*cosv*sinh,15.0*sinv,15.0*cosv*cosh,0.0,0.0,0,0.0,1.0,0.0);
	
	GLfloat light_pos [] = {1.0, 1.0, 1.0, 0.0};
	glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
}

void Scene::keyboard(unsigned char key, int x, int y){
	switch(key){
	case '3':
		perspective = !perspective;
		reshape(width,height);
		break;
	case '1':
		GLboolean state1;
		glGetBooleanv(GL_LIGHT0,&state1);
		if(state1)
			glDisable(GL_LIGHT0);
		else
			glEnable(GL_LIGHT0);
		break;
	case '2':
		GLboolean state2;
		glGetBooleanv(GL_LIGHT1,&state2);
		if(state2)
			glDisable(GL_LIGHT1);
		else
			glEnable(GL_LIGHT1);
		break;

	case '5':
		robot->startScript();
		break;
	case '6':
		robot->abortScript();
		break;

	case '0':
		robot->showDebugInfo();
		break;
	
	case 'A' : robot->posX -- ; break;
	case 'D' : robot->posX ++ ;break;
	case 'W': robot->posY ++; break;
	case 'S': robot->posY --; break;

	case 'q':robot->headJoint.angleX --;  break;
	case 'w':robot->headJoint.angleX = 0;  break;
	case 'e':robot->headJoint.angleX ++;  break;
	case 'a':robot->headJoint.angleY --;  break;
	case 's':robot->headJoint.angleY = 0;  break;
	case 'd':robot->headJoint.angleY ++;  break;
	case 'z':robot->headJoint.angleZ --;  break;
	case 'x':robot->headJoint.angleZ = 0;   break;
	case 'c':robot->headJoint.angleZ ++;  break;

	case 't':robot->leftLegJoint.angleX --; robot->rightLegJoint.angleX ++;  break;
	case 'y':robot->leftLegJoint.angleX = 0;robot->rightLegJoint.angleX = 0;   break;
	case 'u':robot->leftLegJoint.angleX ++;robot->rightLegJoint.angleX --;   break;
	case 'g':robot->leftLegJoint.angleY --; robot->rightLegJoint.angleY ++;  break;
	case 'h':robot->leftLegJoint.angleY = 0;robot->rightLegJoint.angleY = 0;   break;
	case 'j':robot->leftLegJoint.angleY ++;robot->rightLegJoint.angleY --;   break;
	case 'b':robot->leftLegJoint.angleZ --; robot->rightLegJoint.angleZ ++;  break;
	case 'n':robot->leftLegJoint.angleZ = 0;robot->rightLegJoint.angleZ = 0;   break;
	case 'm':robot->leftLegJoint.angleZ ++;robot->rightLegJoint.angleZ --;   break;
	
	case 'T':robot->leftEarJoint.angleX --; robot->rightEarJoint.angleX ++;  break;
	case 'Y':robot->leftEarJoint.angleX = 0;robot->rightEarJoint.angleX = 0;   break;
	case 'U':robot->leftEarJoint.angleX ++;robot->rightEarJoint.angleX --;   break;
	case 'G':robot->leftEarJoint.angleY --; robot->rightEarJoint.angleY ++;  break;
	case 'H':robot->leftEarJoint.angleY = 0;robot->rightEarJoint.angleY = 0;   break;
	case 'J':robot->leftEarJoint.angleY ++;robot->rightEarJoint.angleY --;   break;
	case 'B':robot->leftEarJoint.angleZ --; robot->rightEarJoint.angleZ ++;  break;
	case 'N':robot->leftEarJoint.angleZ = 0;robot->rightEarJoint.angleZ = 0;   break;
	case 'M':robot->leftEarJoint.angleZ ++;robot->rightEarJoint.angleZ --;   break;

	case 'i':robot->leftArmJoint.angleX --; robot->rightArmJoint.angleX ++;  break;
	case 'o':robot->leftArmJoint.angleX = 0;robot->rightArmJoint.angleX = 0;   break;
	case 'p':robot->leftArmJoint.angleX ++;robot->rightArmJoint.angleX --;   break;
	case 'k':robot->leftArmJoint.angleY --; robot->rightArmJoint.angleY ++;  break;
	case 'l':robot->leftArmJoint.angleY = 0;robot->rightArmJoint.angleY = 0;   break;
	case ';':robot->leftArmJoint.angleY ++;robot->rightArmJoint.angleY --;   break;
	case ',':robot->leftArmJoint.angleZ --; robot->rightArmJoint.angleZ ++;  break;
	case '.':robot->leftArmJoint.angleZ = 0;robot->rightArmJoint.angleZ = 0;   break;
	case '/':robot->leftArmJoint.angleZ ++;robot->rightArmJoint.angleZ --;   break;
	
	case 'I':robot->leftSmallArmJoint.angleX --; robot->rightSmallArmJoint.angleX ++;  break;
	case 'O':robot->leftSmallArmJoint.angleX = 0;robot->rightSmallArmJoint.angleX = 0;   break;
	case 'P':robot->leftSmallArmJoint.angleX ++;robot->rightSmallArmJoint.angleX --;   break;
	case 'K':robot->leftSmallArmJoint.angleY --; robot->rightSmallArmJoint.angleY ++;  break;
	case 'L':robot->leftSmallArmJoint.angleY = 0;robot->rightSmallArmJoint.angleY = 0;   break;
	case ':':robot->leftSmallArmJoint.angleY ++;robot->rightSmallArmJoint.angleY --;   break;
	case '<':robot->leftSmallArmJoint.angleZ --; robot->rightSmallArmJoint.angleZ ++;  break;
	case '>':robot->leftSmallArmJoint.angleZ = 0;robot->rightSmallArmJoint.angleZ = 0;   break;
	case '?':robot->leftSmallArmJoint.angleZ ++;robot->rightSmallArmJoint.angleZ --;  break;

	default:break;
	}
	glutPostRedisplay();
}

void Scene::mouse(int button, int state, int x, int y){
	//printf("mouse<%d,%d>\n",x,y);
	mouse_anchor_x = x;
	mouse_anchor_y = y;
}

void Scene::motion(int x, int y){
	//printf("motion<%d,%d>\n",x,y);
	camera_angle_h += x - mouse_anchor_x;
	camera_angle_v += y - mouse_anchor_y;
	mouse_anchor_x = x;
	mouse_anchor_y = y;
	reshape(width,height);
	glutPostRedisplay();
}

void Scene::init(int & argc, char ** argv){
	glutInit(&argc, argv);
}

void Scene::init(){
	GLfloat mat_specular [] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_diffuse [] = {0.8, 0, 0, 1.0};
	GLfloat mat_shininess []  = {30.0};
	GLfloat white_light [] = {0.6, 0.6, 0.6, 1.0};
	GLfloat ambient [] = {0.8,0.8,0.8,0.8};
	//GLfloat light0_pos [] = {2.0, 2.0, -10.0, 1.0};
	GLfloat light1_pos [] = {-2.0, 2.0, -10.0, 1.0};

	glClearColor(189.0/255,1.0,1.0,1.0);
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,white_light);
	glLightfv(GL_LIGHT0,GL_SPECULAR,white_light);
	//glLightfv(GL_LIGHT0,GL_POSITION,light0_pos);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,white_light);
	glLightfv(GL_LIGHT1,GL_SPECULAR,white_light);
	glLightfv(GL_LIGHT1,GL_POSITION,light1_pos);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient);

	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_NORMALIZE);

	//glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

int Scene::start(int w, int h, const char * title){
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(w,h);

	int sh = glutGet(GLUT_SCREEN_HEIGHT);
	int sw = glutGet(GLUT_SCREEN_WIDTH);
	glutInitWindowPosition((sw-w)/2,(sh-h)/2);
	glutCreateWindow(title);
	
	init();

	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
