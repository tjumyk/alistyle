#include "Particle.h"
#include <GL/glew.h>
#include <GL/glut.h>

using namespace PAPI;

Particle::Particle(void)
{
	int particle_handle = P.GenParticleGroups(1, 10000);

    P.CurrentGroup(particle_handle);
	action_handle = P.GenActionLists(1);
	numSteps = 1;
	maxParticles = 10000;
	lineHeight = 0.4;
}


Particle::~Particle(void)
{
}

void Particle::draw(){
	ComputeParticles();
    DrawGroupAsPoints();
}

// Draw as points using vertex arrays
// To draw as textured point sprites just call
// glEnable(GL_POINT_SPRITE_ARB) before calling this function.
void Particle::DrawGroupAsPoints(ParticleContext_t &P, const bool const_color)
{
    int cnt = (int)P.GetGroupCount();
    if(cnt < 1) return;

    float *ptr;
    size_t flstride, pos3Ofs, posB3Ofs, size3Ofs, vel3Ofs, velB3Ofs, color3Ofs, alpha1Ofs, age1Ofs, up3Ofs, rvel3Ofs, upB3Ofs, mass1Ofs, data1Ofs;

    cnt = (int)P.GetParticlePointer(ptr, flstride, pos3Ofs, posB3Ofs,
        size3Ofs, vel3Ofs, velB3Ofs, color3Ofs, alpha1Ofs, age1Ofs,
        up3Ofs, rvel3Ofs, upB3Ofs, mass1Ofs, data1Ofs);
    if(cnt < 1) return;

    if(!const_color) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, int(flstride) * sizeof(float), ptr + color3Ofs);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
	
    glVertexPointer(3, GL_FLOAT, int(flstride) * sizeof(float), ptr + pos3Ofs);

	glDrawArrays(GL_POINTS, 0, cnt);
    glDisableClientState(GL_VERTEX_ARRAY);
    if(!const_color)
        glDisableClientState(GL_COLOR_ARRAY);
}

// A fountain spraying up in the middle of the screen
void Particle::ComputeParticles()
{
     // Set up the state.
    P.Velocity(PDCylinder(pVec(0.0f, -0.01f, 0.25f), pVec(0.0f, -0.01f, 0.27f), lineHeight/20, lineHeight/40));
    P.Color(PDLine(pVec(0.8f, 0.9f, 1.0f), pVec(1.0f, 1.0f, 1.0f)));

    // Generate particles along a very small line in the nozzle.
    P.Source(50 * lineHeight, PDLine(pVec(0, 0, 0), pVec(0, 0, lineHeight*4)));

    // Gravity.
    P.Gravity(pVec(0, 0, -0.01f));

    // Bounce particles off a disc of radius 5.
    P.Avoid(-0.05f, 0.35f, 0, PDDisc(pVec(0, 0, 0), pVec(0, 0, 1), 5));

    // Kill particles below Z=-3.
    P.Sink(false, PDPlane(pVec(0,0,0), pVec(0,0,1)));

    // Move particles to their new positions.
    P.Move(true, false);
}

// Draw as points using vertex arrays
// To draw as textured point sprites just call
// glEnable(GL_POINT_SPRITE_ARB) before calling this function.
void Particle::DrawGroupAsPoints()
{
	//GLfloat mat_diffuse_red [] = {1.0, 1.0, 1.0, 1.0};
	//glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse_red);
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_POINT_SPRITE_ARB);
    glPointSize(2);
	//glColor3f(0.7,0.2,0.1);
    DrawGroupAsPoints(P, true);
    //glDisable(GL_POINT_SPRITE_ARB);
    glDisable(GL_TEXTURE_2D);
}


