#include "Shader.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

Shader::Shader(const char * source, bool vertOrFrag)
{
	this->program = glCreateProgram();
	if(source != NULL){
		this->shader = glCreateShader(vertOrFrag ? GL_VERTEX_SHADER:GL_FRAGMENT_SHADER);
		glShaderSource(this->shader,1,&source,NULL);
		glCompileShader(shader);
		glGetShaderiv(this->shader,GL_COMPILE_STATUS,&this->completed);

		if(!completed){
			GLint len;
			GLsizei len2;
			char * msg;

			glGetShaderiv(this->shader,GL_INFO_LOG_LENGTH,&len);
			len2 = (int)len;
			msg = (char*)malloc(len2);
			glGetShaderInfoLog(this->shader,len2,&len2,msg);
			fprintf(stderr, "Vertex shader compilation "
				"failure:\n%s \n",msg);
		    free(msg);

			glDeleteProgram(this->program);

			this->program = 0;
		}

		glAttachShader(program,shader);
	}

	glLinkProgram(this->program);
	glGetProgramiv(this->program, GL_LINK_STATUS, &this->completed);

	if(!completed){
		GLint len;
		GLsizei len2;
		char * msg;

		glGetProgramiv(this->program,GL_INFO_LOG_LENGTH,&len);
		len2 = (int)len;
		msg = (char*)malloc(len2);
		glGetProgramInfoLog(this->program,len2,&len2,msg);
		fprintf(stderr, "Program link "
			"failure:\n%s \n",msg);
		free(msg);

		glDeleteProgram(this->program);

		this->program = 0;
	}
}

Shader Shader::readFromFile(const char * fileName, bool vertOrFrag){
	char szBuf[102400];
	memset(szBuf, 0, sizeof(char) * 102400);
 
	std::string strMessage;
 
	FILE * fp = NULL;
	fp = fopen(fileName, "rb");
	if (fp != NULL)
	{
		// UTF-8 file should offset 3 byte from start position.
		//fseek(fp, sizeof(char) * 3, 0);
		while(fread(szBuf, sizeof(char),102400, fp) > 0)
		{
			strMessage += szBuf;
			memset(szBuf, 0, sizeof(char) * 102400);
		}
	}
 
	std::cout << strMessage.c_str() << std::endl;
 
	fclose(fp);
	return Shader(strMessage.c_str(),vertOrFrag);
}


Shader::~Shader(void)
{
}
