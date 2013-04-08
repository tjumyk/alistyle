#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include "include\particle\pAPI.h"

class Particle
{
public:
	Particle(void);
	~Particle(void);

	void draw();
	void setHeight(float h){this->lineHeight = h;}
private:
	PAPI::ParticleContext_t P;
	void ComputeParticles();
	void DrawGroupAsPoints();
	void DrawGroupAsPoints(PAPI::ParticleContext_t &P, const bool const_color);
	int maxParticles; // The number of particles the app wants in this demo
    int numSteps; // The number of simulation time steps per rendered frame
    int particle_handle; // The handle of the particle group
    int action_handle; // The handle of the action list being created or used

	float lineHeight;
};

