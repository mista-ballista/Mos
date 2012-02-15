#include <stdio.h> // Needed only for sprintf()
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <glfw.h>

using namespace std;

void parseObj(	string filename,	
				vector<GLfloat> &vertices, 
				vector<GLfloat> &texcoords, 
				vector<GLfloat> &normals, 
				vector<GLubyte> &indices);