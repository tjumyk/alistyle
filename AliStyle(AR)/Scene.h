#pragma once
#include <GL\glew.h>
#include <GL\glut.h>
#include <vector>
#include "GLObject.h"
#include "GLRobot.h"

class Scene
{
public:
	Scene();
	~Scene(void);

	static bool light_1_on;
	static bool light_2_on;

	static GLRobot * robot;

	static void addObject(GLObject * obj){objList.push_back(obj);}
	static void initGL();
	static void display(double trans[3][4]);
	static void keyboard(unsigned char key, int x, int y);

private:

	static bool perspective;
	static int width,height;

	static float camera_angle_h,camera_angle_v;
	static int mouse_anchor_x,mouse_anchor_y;

	static std::vector<GLObject *> objList;
};

