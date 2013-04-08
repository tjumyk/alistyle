#include "Scene.h"
#include "Shader.h"
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

void Scene::display(double trans[3][4]){
	//printf("Drawing\n");
	
	for(int i = 0 ; i < (int)(objList.size()); i++)
	{
		glPushMatrix();
		objList[i]->draw();
		glPopMatrix();
	}
}

void Scene::keyboard(unsigned char key, int x, int y){
	switch(key){
	case 'v':
		perspective = !perspective;
		glutPostRedisplay();
		break;
	case '0':
		GLboolean state1;
		glGetBooleanv(GL_LIGHT0,&state1);
		if(state1)
			glDisable(GL_LIGHT0);
		else
			glEnable(GL_LIGHT0);
		glutPostRedisplay();
		break;
	case '1':
		GLboolean state2;
		glGetBooleanv(GL_LIGHT1,&state2);
		if(state2)
			glDisable(GL_LIGHT1);
		else
			glEnable(GL_LIGHT1);
		glutPostRedisplay();
		break;
	case '5':
		robot->startScript();
		break;
	case '6':
		robot->abortScript();
		break;

	case '9':
		robot->showDebugInfo();
		break;
	default:break;
	}
}


void Scene::initGL(){
	GLfloat mat_specular [] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_diffuse [] = {0.8, 0, 0, 1.0};
	GLfloat mat_shininess []  = {30.0};
	GLfloat white_light [] = {1.0, 1.0, 1.0, 1.0};
	GLfloat ambient [] = {1.0,1.0,1.0,1.0};
	GLfloat light0_pos [] = {1000.0,-2000.0,1000.0,0.0};
	GLfloat light1_pos [] = {-1000.0,2000.0,1000.0,0.0};

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);

	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,white_light);
	glLightfv(GL_LIGHT0,GL_SPECULAR,white_light);
	glLightfv(GL_LIGHT0,GL_POSITION,light0_pos);
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

	/*Shader shaderVert = Shader::readFromFile("wood.vert",true);
	shaderVert.useProgram();
	Shader shaderFrag = Shader::readFromFile("wood.frag",false);
	shaderFrag.useProgram();
	*/
	
}
