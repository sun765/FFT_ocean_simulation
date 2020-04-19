#ifndef __LOADTEXTURE_H__
#define __LOADTEXTURE_H__

#include <windows.h>
#include "GL/glew.h"
#include "GL/gl.h"
#include <string>

GLuint LoadTexture(const char* fname);
GLuint LoadCube(const std::string& fname);
#endif