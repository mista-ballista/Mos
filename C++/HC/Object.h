#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <GL/glfw.h>

using namespace std;

class Object
{
public:
	Object();
	Object(string filename);
	void DrawObject();

private:
	vector<GLfloat> vertices,
					texcoords, 
					normals;
	vector<GLuint> indices;
};