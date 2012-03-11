#ifndef Object_h
#define Object_h

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <GL/glfw.h>
#include "vec3f.h"

using namespace std;



class Object
{
public:
	Object();
	Object(string filename);
	void DrawObject();
	Vec3f min;
	Vec3f max;

	vector<GLfloat> getVertices()
	{
		return vertices;
	}

	void setVertices(vector<GLfloat> v)
	{
		vertices = v;
	}

	void createBoundingBox();

private:

	vector<GLfloat> vertices,
		texcoords, 
		normals;
	vector<GLuint> indices;	

};
#endif Object_h