#pragma once
#include <GL\glew.h>

class Shader
{
public:
	Shader(const char * source, bool vertOrFrag);
	~Shader(void);

	static Shader readFromFile(const char * fileName, bool vertOrFrag);

	void useProgram(){if(program != 0)glUseProgram(this->program);}

private :
	GLuint shader,program;
	GLint completed;
};

