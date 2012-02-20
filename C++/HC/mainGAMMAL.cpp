#include <glfw.h>
#include <stdlib.h>
#include <SOIL.h>
#include <vector>
#include "parseObj.h"
#include <cmath>
#include <stdio.h>

void Init(void);
void Shut_Down(int return_code);
void Main_Loop(void);
void Draw_Square(float red, float green, float blue);
void Draw(void);

using namespace std;

string objname = "Data/arrow.obj";
char texname[] = "Data/johda2.tga";

float rotateC = 0;
const float rotations_per_tick = .2;
GLuint	texture[1];
 
vector<GLfloat> vertices;
vector<GLfloat> texcoords;
vector<GLfloat> normals;
vector<GLubyte> indices;

int LoadGLTextures()
{
	/* load an image file directly as a new OpenGL texture */
	texture[0] = SOIL_load_OGL_texture
		(
		texname,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);

	if(texture[0] == 0)
		Shut_Down(1);

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	return true;
}

int main(void)
{
	
	Init();
	LoadGLTextures();
	Main_Loop();
	Shut_Down(0);
}
 
void Init(void)
{
	const int window_width = 1500,
	          window_height = 800;
	
	if (!glfwInit())
	  Shut_Down(1);

	// 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
	if (!glfwOpenWindow(window_width, window_height, 5, 6, 5, 0, 0, 0, GLFW_WINDOW))
	  Shut_Down(1);
	
	glfwSetWindowTitle("TEST LOL");
	
	// set the projection matrix to a normal frustum with a max depth of 50
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect_ratio = ((float)window_height) / window_width;
	glFrustum(.5, -.5, -.5 * aspect_ratio, .5 * aspect_ratio, 1, 50);
	glMatrixMode(GL_MODELVIEW);

  	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 1.0f, 1.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// Ladda in objekt
	parseObj(objname, vertices, texcoords, normals, indices);


}
 
void Shut_Down(int return_code)
{
  glfwTerminate();
  exit(return_code);
}
 
void Main_Loop(void)
{
  // the time of the previous frame
  double old_time = glfwGetTime();
  // this just loops as long as the program runs
  while(1)
  {
    // calculate time elapsed, and the amount by which stuff rotates
    double current_time = glfwGetTime(),
           delta_rotate = (current_time - old_time) * rotations_per_tick * 360;
    old_time = current_time;

    rotateC += delta_rotate;

    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw the figure
    Draw();
    // swap back and front buffers
    glfwSwapBuffers();
  }
}
 
 
void Draw(void)
{
	// reset view matrix
	glLoadIdentity();
	// move view back a bit
	glTranslatef(0, -1, -5);

	glRotatef(rotateC, 0, 1, 0);	// Rotation around Y
	glRotatef(rotateC, 0, 0, 1);	// Rotation around Z
	glRotatef(rotateC, 1, 0, 0);	// Rotation around X

	// Draw the scene
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normals.data());
	glTexCoordPointer(3, GL_FLOAT, 0, texcoords.data());
	glVertexPointer(3, GL_FLOAT, 0, vertices.data());

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_BYTE, indices.data());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

}