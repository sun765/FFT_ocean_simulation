#ifndef __QUERY_H__
#define __QUERY_H__

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <iostream>
class query
{
public:
	query();
	query(int type);
	void start();

	void end();

	bool isResultReady();

	bool isInUse();

	unsigned int getResult();

	void test();

	~query();

private:
	 GLuint id;
	 int type;
	 GLint ready = 0;
	 bool inUse = false;
	 unsigned int samples;


};

#endif

