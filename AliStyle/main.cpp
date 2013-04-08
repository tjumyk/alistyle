#include "Scene.h"
#include "GLRobot.h"
#include "GLStage.h"

int main(int argc, char ** argv){
	Scene scene;
	scene.init(argc, argv);
	GLStage stage;
	GLRobot robot;
	scene.addObject(&stage);
	scene.addObject(&robot);
	scene.robot = &robot;
	robot.readScript("Data\\script_mouth.txt");
	robot.readScript("Data\\script_particle.txt");
	robot.readScript("Data\\script_body_1.txt");
	robot.readScript("Data\\script_body_2.txt");
	robot.readScript("Data\\script_body_3.txt");
	robot.readScript("Data\\script_body_4.txt");
	robot.readScript("Data\\script_body_5.txt");
	robot.readScript("Data\\script_body_6.txt");
	robot.readScript("Data\\script_body_7.txt");
	robot.readScript("Data\\script_body_8.txt");
	robot.readScript("Data\\script_body_9.txt");
	//robot.printScript();
	scene.start(1024,640,"Opengl Assignment 3 by tjumyk");
	return 0;
}