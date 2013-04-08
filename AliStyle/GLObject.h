#pragma once
#include <gl/glut.h>
#include "glaux.h"

class GLObject
{
public:

	GLObject(void)
	{
	}

	virtual ~GLObject(void)
	{
	}

	virtual void draw() = 0;

	GLuint static loadTexture(const char * fName);
};

