#include "GLRobot.h"
#include "Scene.h"
#include "EasingFunction.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "include\irrKlang\irrKlang.h"

bool running = true;
std::vector<Point> pts1,pts2;
GLRobot * robot = NULL;
const double PI = 3.14159265;
irrklang::ISoundEngine* engine;
irrklang::ISound* music;

void CALLBACK errorCallBack(){
	fprintf(stderr,"Quadric Error\n");
}

GLRobot* GLRobot::instance = NULL;

GLRobot::GLRobot(void)
{
	instance = this;
	startList = 0;
	running = true;
	robot = this;

	posX = posY = posZ = 0.0;
	rotateX = rotateY = rotateZ = 0;

	faceIndex = 0;
	mouseIndex = 5;

	engine = irrklang::createIrrKlangDevice();

	leftArmJoint.angleZ = 30;
	rightArmJoint.angleZ = -30;

	current  = 0;
}


GLRobot::~GLRobot(void)
{
	running = false;
	if (music)
		music->drop(); // release music stream.
	if(engine)
		engine->drop(); // delete engine
}

void GLRobot::draw(){
	if(startList == 0){
		buildLists();		
	}
	
	glPushMatrix();
		glRotatef(-90,1,0,0);
		glTranslatef(8,-8,-4.8);
		particle.draw();
		glTranslatef(0,16,0);
		particle.draw();
		glTranslatef(-16,0,0);
		particle.draw();
		glTranslatef(0,-16,0);
		particle.draw();
	glPopMatrix();

	glTranslatef(0,-1.8,0);
	glTranslatef(posX,posY,posZ);
	glRotatef(rotateX,1,0,0);
	glRotatef(rotateY,0,1,0);
	glRotatef(rotateZ,0,0,1);

	//GLfloat leftToLand = 1.25 - 1.0/(1+pow(tan(leftLegJoint.angleX/180*PI),2)+pow(tan(leftLegJoint.angleY/180*PI),2)) * 1.25;
	//GLfloat rightToLand = 1.25 - 1.0/(1+pow(tan(rightLegJoint.angleX/180*PI),2)+pow(tan(rightLegJoint.angleY/180*PI),2)) * 1.25;
	//glTranslatef(0,-min(leftToLand,rightToLand),0);
	//printf("aL:(%f,%f) aR:(%f,%f) dL:%f dR:%f dMin:%f\n",leftLegJoint.angleX,leftLegJoint.angleY,rightLegJoint.angleX,rightLegJoint.angleY,leftToLand, rightToLand, -min(leftToLand,rightToLand));
	
	glPushMatrix();
		glTranslatef(0,-0.35,0);
		glCallList(startList);//trousers
		glCallList(startList+1);//body
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.7,-1.4,0.0);
		glRotatef(leftLegJoint.angleX,1,0,0);
		glRotatef(leftLegJoint.angleY,0,1,0);
		glRotatef(leftLegJoint.angleZ,0,0,1);
		glCallList(startList+2);//leg
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(0.7,-1.4,0.0);
		glRotatef(rightLegJoint.angleX,-1,0,0);
		glRotatef(rightLegJoint.angleY,0,1,0);
		glRotatef(rightLegJoint.angleZ,0,0,1);
		glCallList(startList+2);//leg
	glPopMatrix();

	glPushMatrix();
		glTranslatef(1.4,1.6,0.0);
		glRotatef(leftArmJoint.angleX,1,0,0);
		glRotatef(leftArmJoint.angleY,0,1,0);
		glRotatef(leftArmJoint.angleZ,0,0,1);
		glCallList(startList+2);//arm
		glTranslatef(0.0,-1.25,0.0);
		glRotatef(leftSmallArmJoint.angleX,1,0,0);
		glRotatef(leftSmallArmJoint.angleY,0,1,0);
		glRotatef(leftSmallArmJoint.angleZ,0,0,1);
		glCallList(startList+2);//arm
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.4,1.6,0.0);
		glRotatef(rightArmJoint.angleX,-1,0,0);
		glRotatef(rightArmJoint.angleY,0,1,0);
		glRotatef(rightArmJoint.angleZ,0,0,1);
		glCallList(startList+2);//arm
		glTranslatef(0.0,-1.25,0.0);
		glRotatef(rightSmallArmJoint.angleX,-1,0,0);
		glRotatef(rightSmallArmJoint.angleY,0,1,0);
		glRotatef(rightSmallArmJoint.angleZ,0,0,1);
		glCallList(startList+2);//arm
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0,2.0,0.0);
	glRotatef(headJoint.angleX,1.0,0.0,0.0);
	glRotatef(headJoint.angleY,0.0,1.0,0.0);
	glRotatef(headJoint.angleZ,0.0,0.0,1.0);
	glTranslatef(0.0,-2.0,0.0);

		glPushMatrix();
			glTranslatef(1.7,5.5,0.0);
			glRotatef(-45,0,0,1);
			glRotatef(leftEarJoint.angleX,1,0,0);
			glRotatef(leftEarJoint.angleY,0,1,0);
			glRotatef(leftEarJoint.angleZ,0,0,1);
			glCallList(startList+3);//ear
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-1.7,5.5,0.0);
			glRotatef(45,0,0,1);
			glRotatef(rightEarJoint.angleX,-1,0,0);
			glRotatef(rightEarJoint.angleY,0,1,0);
			glRotatef(rightEarJoint.angleZ,0,0,1);
			glCallList(startList+3);//ear
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.0,6.0,0.0);
			//glCallList(startList+4);//head
			// use real-time render instead
			GLUquadric * qobj = gluNewQuadric();
			gluQuadricCallback(qobj,GLU_ERROR,errorCallBack);
			gluQuadricDrawStyle(qobj,GLU_FILL);
			gluQuadricNormals(qobj,GLU_SMOOTH);
			gluQuadricTexture(qobj, GL_TRUE); 

			GLfloat mat_diffuse_white [] = {0.8, 0.8, 0.8, 1.0};
			GLfloat mat_diffuse_red [] = {0.8, 0.0, 0.0, 1.0};
			GLfloat mat_diffuse_dark [] = {0.1, 0.1, 0.1, 1.0};
			GLfloat mat_diffuse_black [] = {0.0, 0.0, 0.0, 1.0};
			glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse_white);
			glPushMatrix();
				glRotatef(-90,1,0,0);
				glScalef(1.5,1.5,1.5);
				glScalef(1.0,0.75,1.0);
				glEnable(GL_TEXTURE_2D);
				glPushMatrix();
					glScalef(1.0,1.0,0.4);
					glTranslatef(0.0,0.0,-0.3);
					glBindTexture(GL_TEXTURE_2D, commonTextures[0]);
					gluSphere(qobj,1.8,100,40);
					glTranslatef(0.0,0.0,0.3);
					glTranslatef(0.0,0.0,-2.0/0.4);
					glTranslatef(0.0,0.0,-0.3);
					glBindTexture(GL_TEXTURE_2D, mouseTextures[mouseIndex]);
					gluSphere(qobj,2.6,100,40);
					glTranslatef(0.0,0.0,0.3);
				glPopMatrix();
				glTranslatef(0.0,0.0,-2.0);
				glBindTexture(GL_TEXTURE_2D, faceTextures[faceIndex]);
				gluCylinder(qobj,2.6,1.80,2.0,100,30);
				glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		glPopMatrix();

		/*
		glPushMatrix();
			glTranslatef(-1.2,4.2,2.5);
			glCallList(startList+5);//eye
		glPopMatrix();

		glPushMatrix();
			glTranslatef(1.2,4.2,2.5);
			glCallList(startList+5);//eye
		glPopMatrix();
		*/

		glPushMatrix();
			glTranslatef(-0.05,3.5,2.8);
			glScalef(1.0,0.75,1.5);
			glCallList(startList+5);//nose
		glPopMatrix();

	glPopMatrix();
	//glPushMatrix();
	//	glTranslatef(0.0,2.9,2.88);
	//	glRotatef(-10,1,0,0);
	//	glCallList(startList+6);
	//glPopMatrix();

	//glPushMatrix();
	//	glTranslatef(-1.2,5.2,2.0);
	//	glRotatef(10,1,0,0);
	//	glCallList(startList+6);
	//glPopMatrix();

	//glPushMatrix();
	//	glTranslatef(1.2,5.2,2.0);
	//	glRotatef(10,1,0,0);
	//	glCallList(startList+6);
	//glPopMatrix();
}

void GLRobot::buildLists(){
	startList = glGenLists(6);
	GLUquadric * qobj = gluNewQuadric();
	gluQuadricCallback(qobj,GLU_ERROR,errorCallBack);
	gluQuadricDrawStyle(qobj,GLU_FILL);
	gluQuadricNormals(qobj,GLU_SMOOTH);
	gluQuadricTexture(qobj, GL_TRUE); 

	GLfloat mat_diffuse_white [] = {0.8, 0.8, 0.8, 1.0};
	GLfloat mat_diffuse_red [] = {0.8, 0.0, 0.0, 1.0};
	GLfloat mat_diffuse_dark [] = {0.1, 0.1, 0.1, 1.0};
	GLfloat mat_diffuse_black [] = {0.0, 0.0, 0.0, 1.0};

	GLdouble eqn [4] = {0.0,-1.0,0.0,0.0};
	GLdouble eqn2 [4] = {0.0,1.0,0.0,0.0};

	commonTextures.push_back(GLObject::loadTexture("Data/0.bmp"));
	commonTextures.push_back(GLObject::loadTexture("Data/1.bmp"));
	commonTextures.push_back(GLObject::loadTexture("Data/2.bmp"));
	commonTextures.push_back(GLObject::loadTexture("Data/3.bmp"));
	commonTextures.push_back(GLObject::loadTexture("Data/4.bmp"));
	commonTextures.push_back(GLObject::loadTexture("Data/5.bmp"));

	faceTextures.push_back(GLObject::loadTexture("Data/f_n.bmp"));

	mouseTextures.push_back(GLObject::loadTexture("Data/m_a.bmp"));
	mouseTextures.push_back(GLObject::loadTexture("Data/m_e.bmp"));
	mouseTextures.push_back(GLObject::loadTexture("Data/m_i.bmp"));
	mouseTextures.push_back(GLObject::loadTexture("Data/m_u.bmp"));
	mouseTextures.push_back(GLObject::loadTexture("Data/m_o.bmp"));
	mouseTextures.push_back(GLObject::loadTexture("Data/m_s.bmp"));

	glNewList(startList,GL_COMPILE);//trousers
		glClipPlane(GL_CLIP_PLANE0,eqn);
		glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse_white);
		glPushMatrix();
			glEnable(GL_CLIP_PLANE0);
			glScalef(1.0,0.8,0.75);			
			gluSphere(qobj,2.0,30,30);
			glDisable(GL_CLIP_PLANE0);
		glRotatef(90,1,0,0);
		glPopMatrix();
	glEndList();

	glNewList(startList+1,GL_COMPILE);//body
		glClipPlane(GL_CLIP_PLANE0,eqn2);
		glEnable(GL_CLIP_PLANE0);
		glEnable(GL_TEXTURE_2D);
		glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse_white);
		glPushMatrix();
			glScalef(1.0,1.5,0.75);
			glBindTexture(GL_TEXTURE_2D, commonTextures[4]);
			gluSphere(qobj,2.0,30,30);
		glPopMatrix();
		glDisable(GL_CLIP_PLANE0);
		glDisable(GL_TEXTURE_2D);
	glEndList();

	glNewList(startList+2,GL_COMPILE);//arm or leg
		glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse_white);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
			glRotatef(-90,1,0,0);
			glBindTexture(GL_TEXTURE_2D, commonTextures[0]);
			gluSphere(qobj,0.5,10,10);
			glTranslatef(0.0,0.0,-1.25);
			gluCylinder(qobj,0.5,0.5,1.25,30,10);
			gluSphere(qobj,0.5,10,10);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	glEndList();

	glNewList(startList+3,GL_COMPILE);//ear
		glClipPlane(GL_CLIP_PLANE0,eqn2);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_CLIP_PLANE0);
		glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse_white);
		glPushMatrix();
			glScalef(0.8,1.25,0.5);
			glBindTexture(GL_TEXTURE_2D, commonTextures[3]);
			gluSphere(qobj,2.0,30,30);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CLIP_PLANE0);
	glEndList();

	glNewList(startList+4,GL_COMPILE);//head
		glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse_white);
		glPushMatrix();
			glRotatef(-90,1,0,0);
			glScalef(1.5,1.5,1.5);
			glScalef(1.0,0.75,1.0);
			glEnable(GL_TEXTURE_2D);
			glPushMatrix();
				glScalef(1.0,1.0,0.4);
				glTranslatef(0.0,0.0,-0.3);
				glBindTexture(GL_TEXTURE_2D, commonTextures[0]);
				gluSphere(qobj,1.8,100,40);
				glTranslatef(0.0,0.0,0.3);
				glTranslatef(0.0,0.0,-2.0/0.4);
				glTranslatef(0.0,0.0,-0.3);
				glBindTexture(GL_TEXTURE_2D, commonTextures[2]);
				gluSphere(qobj,2.6,100,40);
				glTranslatef(0.0,0.0,0.3);
			glPopMatrix();
			glTranslatef(0.0,0.0,-2.0);
			glBindTexture(GL_TEXTURE_2D, commonTextures[1]);
			gluCylinder(qobj,2.6,1.80,2.0,100,30);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	glEndList();

	glNewList(startList+5,GL_COMPILE);//eye or nose
		GLfloat mat_specular [] = {1.0, 1.0, 1.0, 1.0};
		GLfloat mat_shininess []  = {50.0};
		glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse_black);
		glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
		glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
		glPushMatrix();
			glScalef(1.0,1.0,0.4);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, commonTextures[5]);
			gluSphere(qobj,0.4,20,20);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	glEndList();

	//glNewList(startList+6,GL_COMPILE);//mouse or eyebow
	//	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse_dark);
	//	glPushMatrix();
	//		glDisable(GL_LIGHTING);
	//		glScalef(1.0,1.0,0.4);
			//glutSolidTorus(0.04,0.3,20,20);
	//		glEnable(GL_LIGHTING);
	//	glPopMatrix();
	//glEndList();

	gluDeleteQuadric(qobj);
}

void GLRobot::readScript(const char * fileName)
{
	std::fstream fin = std::fstream(fileName);
	char buffer[1000];
	std::stringstream stream;
	long totalTime = 0;
	long timeSpan = 0;
	std::string action;
	double value = 0.0;
	std::string strValue;
	std::map<std::string ,std::vector<KeyFrame>>::iterator iter;

	while(true){
		fin.getline(buffer,1000);
		if(fin.eof())
			break;
		stream.str("");
		stream.clear();
		stream << buffer;
		if(strlen(buffer) <=0 )
			continue;
		if(strlen(buffer) >= 2 && buffer[0] == '/' && buffer[1] == '/')
			continue;
		stream >> timeSpan;
		totalTime += timeSpan;
		action = "";
		strValue = "";
		value = 0.0;
		while(!stream.eof()){
			stream >> action;
			if(action == "FACE" || action == "MOUTH"){
				stream >> strValue;
				value = strValue[0];
			}else
				stream >> value;
			KeyFrame frame = KeyFrame(totalTime,value,action);
			iter = animationData.find(action);
			if(iter != animationData.end()){// key already exist
				iter->second.push_back(frame);
			}else{
				std::vector<KeyFrame> list;
				list.push_back(frame);
				animationData.insert(std::pair<std::string,std::vector<KeyFrame>>(action,list));
				setValue(action,value);
			}
		}
	}

	printf("Script parsed: \"%s\"\n",fileName);
}

void GLRobot::printScript(){
	std::cout << "[Script Timeline]\n";
	std::map<std::string ,std::vector<KeyFrame>>::iterator iter;
	for(iter = animationData.begin(), iter++; iter != animationData.end() ; iter++){
		std::cout << iter->first << ": ";
		std::vector<KeyFrame> * list = &(iter->second);
		for(int i = 0, size = list->size(); i < size; i ++ ){
			KeyFrame * frame = &((*list)[i]);
			if(iter->first == "FACE" || iter->first == "MOUTH")
				std::cout << "[" << frame->time << "," << (char)(frame->keyValue) << "] ";
			else
				std::cout << "[" << frame->time << "," << frame->keyValue << "] ";
		}
		std::cout << std::endl;
	}
}

void scriptRunner(int value){
	if(robot->aborted || (music != NULL && music->isFinished()))
		return;
	GLRobot::instance->runScript();
}

void GLRobot::startScript(){
	aborted = false;
	start = timeGetTime();
	runScript();
	playMusic("Data\\style.mp3");
}

void GLRobot::showDebugInfo(){
	std::cout << "Time: " << timeGetTime() - start << "\n";
	std::cout << "HX " << headJoint.angleX << " HY " << headJoint.angleY << " HZ " << headJoint.angleZ;
	std::cout << " LAX " << leftArmJoint.angleX << " LAY " << leftArmJoint.angleY << " LAZ " << leftArmJoint.angleZ;
	std::cout << " RAX " << rightArmJoint.angleX << " RAY " << rightArmJoint.angleY << " RAZ " << rightArmJoint.angleZ;
	std::cout << " LSAX " << leftSmallArmJoint.angleX << " LSAY " << leftSmallArmJoint.angleY << " LSAZ " << leftSmallArmJoint.angleZ;
	std::cout << " RSAX " << rightSmallArmJoint.angleX << " RSAY " << rightSmallArmJoint.angleY << " RSAZ " << rightSmallArmJoint.angleZ;
	std::cout << " LLX " << leftLegJoint.angleX << " LLY " << leftLegJoint.angleY << " LLZ " << leftLegJoint.angleZ;
	std::cout << " RLX " << rightLegJoint.angleX << " RLY " << rightLegJoint.angleY << " RLZ " << rightLegJoint.angleZ;
	std::cout << " LEX " << leftEarJoint.angleX << " LEY " << leftEarJoint.angleY << " LEZ " << leftEarJoint.angleZ;
	std::cout << " REX " << rightEarJoint.angleX << " REY " << rightEarJoint.angleY << " REZ " << rightEarJoint.angleZ;
	std::cout << " POSX " << posX << " POSY " << posY << " POSZ " << posZ;
	std::cout << " ROTX " << rotateX << " ROTY " << rotateY << " ROTZ " << rotateZ;
	std::cout << std::endl;
}

void GLRobot::runScript(){
	current = timeGetTime() - start;
	//std::cout << "[" << current << "] ";

	std::map<std::string ,std::vector<KeyFrame>>::iterator iter;
	for(iter = animationData.begin(), iter++; iter != animationData.end() ; iter++){
		std::string action = iter->first;
		std::vector<KeyFrame> * list = &(iter->second);
		double targetValue = 0.0;
		KeyFrame * frame1,*frame2;
		for(int i = 0, size = list->size(); i < size - 1; i ++ ){
			frame1 = &((*list)[i]);
			frame2 = &((*list)[i+1]);

			if(current >= frame1->time && current < frame2->time){
				if(action == "FACE" || action == "MOUTH"){
					targetValue = frame1->keyValue;
				}else{
					targetValue = frame1->keyValue + (frame2->keyValue - frame1->keyValue)/(frame2->time - frame1->time)*(current - frame1->time);
				}
				break;
			}
			if(i == size - 2)
				targetValue = frame2->keyValue;
		}

		//if(action == "FACE" || action == "MOUTH")
		//	std::cout << action << "=" << (char)targetValue<< " "; 
		//else
		//	std::cout << action << "=" << targetValue << " "; 
		setValue(action,targetValue);
	}
	//std::cout << std::endl;
	glutPostRedisplay();
	glutTimerFunc(20,scriptRunner,0);
}

void GLRobot::setValue(std::string action, double targetValue){
	if(action == "HX") headJoint.angleX = targetValue;
		else if(action == "HY") headJoint.angleY = targetValue;
		else if(action == "HZ") headJoint.angleZ = targetValue;
		
		else if(action == "LAX")leftArmJoint.angleX = targetValue;
		else if(action == "LAY")leftArmJoint.angleY = targetValue;
		else if(action == "LAZ")leftArmJoint.angleZ = targetValue;
		else if(action == "RAX")rightArmJoint.angleX = targetValue;
		else if(action == "RAY")rightArmJoint.angleY = targetValue;
		else if(action == "RAZ")rightArmJoint.angleZ = targetValue;
		
		else if(action == "LSAX")leftSmallArmJoint.angleX = targetValue;
		else if(action == "LSAY")leftSmallArmJoint.angleY = targetValue;
		else if(action == "LSAZ")leftSmallArmJoint.angleZ = targetValue;
		else if(action == "RSAX")rightSmallArmJoint.angleX = targetValue;
		else if(action == "RSAY")rightSmallArmJoint.angleY = targetValue;
		else if(action == "RSAZ")rightSmallArmJoint.angleZ = targetValue;
		
		else if(action == "LLX")leftLegJoint.angleX = targetValue;
		else if(action == "LLY")leftLegJoint.angleY = targetValue;
		else if(action == "LLZ")leftLegJoint.angleZ = targetValue;
		else if(action == "RLX")rightLegJoint.angleX = targetValue;
		else if(action == "RLY")rightLegJoint.angleY = targetValue;
		else if(action == "RLZ")rightLegJoint.angleZ = targetValue;

		else if(action == "LEX")leftEarJoint.angleX = targetValue;
		else if(action == "LEY")leftEarJoint.angleY = targetValue;
		else if(action == "LEZ")leftEarJoint.angleZ = targetValue;
		else if(action == "REX")rightEarJoint.angleX = targetValue;
		else if(action == "REY")rightEarJoint.angleY = targetValue;
		else if(action == "REZ")rightEarJoint.angleZ = targetValue;

		else if(action == "POSX")posX = targetValue;
		else if(action == "POSY")posY = targetValue;
		else if(action == "POSZ")posZ = targetValue;

		else if(action == "ROTX")rotateX = targetValue;
		else if(action == "ROTY")rotateY = targetValue;
		else if(action == "ROTZ")rotateZ = targetValue;

		else if(action == "MOUTH"){
			switch ((int)targetValue)
			{
			case 'a':mouseIndex = 0; break;
			case 'e':mouseIndex = 1; break;
			case 'i':mouseIndex = 2; break;
			case 'u':mouseIndex = 3; break;
			case 'o':mouseIndex = 4; break;
			case 's':mouseIndex = 5; break;
			default:
				break;
			}
		}

		else if(action == "FACE"){
			switch ((int)targetValue)
			{
			case 'n':mouseIndex = 0; break;
			default:
				break;
			}
		}

		else if(action == "PARTICLE")
			particle.setHeight(targetValue);
}

void GLRobot::abortScript(){
	if(music != NULL)
		music->stop();
	aborted = true;
}


void GLRobot::playMusic(const char * fileName){
	if(music != NULL)
		music->stop();
	engine->setListenerPosition(irrklang::vec3df(0,0,0),
                                  irrklang::vec3df(0,0,1));
	music = engine->play3D(fileName,irrklang::vec3df(0,0,0), false, false, true);
	if (music)
      music->setMinDistance(5.0f);
}