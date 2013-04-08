#include "GLStage.h"


GLStage::GLStage(void)
{
}


GLStage::~GLStage(void)
{
}

void GLStage::draw(){
	GLfloat mat_diffuse [] = {0.15, 0.15, 0.15, 1.0};
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glTranslatef(0,-5,0);
	glScalef(1.0,0.01,1.0);
	glColor3f(0.3,0.3,0.3);
	glutSolidCube(20);
}
