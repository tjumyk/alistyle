#pragma once
#include "globject.h"
#include "Particle.h"
#include <map>
#include <vector>
#include <string>

class Joint{
public:
	Joint(){
		angleX = angleY = angleZ = 0;
	}
	GLdouble angleX,angleY,angleZ;
};

class KeyFrame{
public:
	long time;
	double keyValue;
	std::string type;

	KeyFrame(){
		time = 0;
		keyValue = 0.0;
	}

	KeyFrame(long time, double keyValue, std::string type){
		this->time = time;
		this->keyValue = keyValue;
		this->type = type;
	}
};

class GLRobot :
	public GLObject
{
public:
	GLRobot(void);
	~GLRobot(void);

	void draw();
	Joint headJoint,leftArmJoint,rightArmJoint,leftSmallArmJoint,rightSmallArmJoint,leftLegJoint,rightLegJoint,leftEarJoint,rightEarJoint;
	GLfloat posX,posY,posZ,rotateX,rotateY,rotateZ;
	
	void readScript(const char * fileName);
	void printScript();
	void startScript();
	void abortScript();
	void playMusic(const char * fileName);

	void showDebugInfo();

	static GLRobot * instance;
	friend void scriptRunner(int value);
private:
	GLuint startList;
	std::vector<GLuint> commonTextures, faceTextures, mouseTextures;
	int faceIndex, mouseIndex;
	void buildLists();
	void runScript();
	void setValue(std::string action, double targetValue);
	DWORD start,current;
	bool aborted;
	std::map<std::string ,std::vector<KeyFrame>> animationData;
	Particle particle;
};

