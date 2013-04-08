#pragma once
#include <GL\glut.h>
#include <vector>
#include "GLObject.h"
#include "GLRobot.h"

class Scene
{
public:
	Scene();
	~Scene(void);

	static void init(int & argc, char ** argv);
	static int start(int w, int h, const char * title);

	static bool light_1_on;
	static bool light_2_on;

	static GLRobot * robot;

	static void addObject(GLObject * obj){objList.push_back(obj);}

private:
	static void init();
	static void display();
	static void reshape(int w, int h);
	static void keyboard(unsigned char key, int x, int y);
	static void mouse(int button, int state, int x, int y);
	static void motion(int x, int y);

	static bool perspective;
	static int width,height;

	static float camera_angle_h,camera_angle_v;
	static int mouse_anchor_x,mouse_anchor_y;

	static std::vector<GLObject *> objList;
};

