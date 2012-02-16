#include <GL/glfw.h>
#include <stdlib.h>
#include <SOIL.h>
#include <vector>
#include "parseObj.h"
#include <cmath>
#include <stdio.h>
#include "alexFunc.h"
#include "functions.h"

void Init(void);
void Shut_Down(int return_code);
void Main_Loop(void);
void Draw_Square(float red, float green, float blue);
void Draw(void);

using namespace std;

string objname = "Data/arrow.obj";
char texname[] = "Data/johda2.tga";


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
	Arrowpos();
	Init();
	LoadGLTextures();
	Main_Loop();
	Shut_Down(0);
}
 
void Init(void)
{
	const int window_width = 800,
	          window_height = 600;
	
	if (!glfwInit())
	  Shut_Down(1);

	// 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
	if (!glfwOpenWindow(window_width, window_height, 5, 6, 5, 0, 0, 0, GLFW_WINDOW))
	  Shut_Down(1);
	
	glfwDisable(GLFW_MOUSE_CURSOR); // Hide the mouse cursor
	glfwSetWindowTitle("TEST LOL");
	
	setProjectionMatrix ();
	setViewMatrix();

  	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background

	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    glEnable(GL_CULL_FACE); // Do not draw polygons facing away from us
 
    glLineWidth(2.0f);			// Set a 'chunky' line width

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


	// Specify the function which should execute when a key is pressed or released
	glfwSetKeyCallback(handleKeypress);

	// Specify the function which should execute when the mouse is moved
	glfwSetMousePosCallback(handleMouseMove);


  // this just loops as long as the program runs
  while(1)
  {
	/*  double current_time = glfwGetTime();
	  keyboardInput();

	  if(current_time < getTimeOfFlight())
	  moveArrow(current_time);
	*/

	  
	//St�ng av om esc 
	if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
		break;
    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw the figure
	Draw();


	// Calculate our camera movement
	calculateCameraMovement();

	// Move our camera
	moveCamera();


	// swap back and front buffers
	glfwSwapBuffers();
  }
}


void DrawScene ()
{
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


void Draw_3DSquare()
{

	GLfloat vertices[] = {
		-1.0000,	0.0000,	 1.0000,
		-1.0000,	0.0000,	 -1.0000,
		1.0000,	0.0000,	 -1.0000,
		1.0000,	0.0000,	 1.0000,
		-1.0000,	2.0000,	 1.0000,
		1.0000,	2.0000,	 1.0000,
		1.0000,	2.0000,	 -1.0000,
		-1.0000,	2.0000,	 -1.0000};



		GLubyte indices[] = {
			0, 1, 2, 3, 
			4, 5, 6, 7, 
			0, 3, 5, 4,
			3, 2, 6, 5,
			2, 1, 7, 6, 
			1, 0, 4, 7}; 

			GLfloat normals[] = {
				0.0000, -1.0000, -0.0000,
				0.0000, 1.0000, -0.0000,
				0.0000, 0.0000, 1.0000,
				1.0000, 0.0000, -0.0000,
				0.0000, 0.0000, -1.0000,
				-1.0000, 0.0000, -0.0000};


				GLfloat texcoord[] = {
					1,0,	1,1,	0,1,	0,0,
					0,0,	1,0,	1,1,	0,1,
					0,0,	1,0,	1,1,	0,1,
					0,0,	1,0,	1,1,	0,1,
					0,0,	1,0,	1,1,	0,1,
					0,0,	1,0,	1,1,	0,1,};

					glEnableClientState(GL_NORMAL_ARRAY);
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glEnableClientState(GL_VERTEX_ARRAY);
					glNormalPointer(GL_FLOAT,0,normals);
					glTexCoordPointer(2, GL_FLOAT, 0, texcoord);
					glVertexPointer(3, GL_FLOAT, 0, vertices);

					glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices);

					glDisableClientState(GL_VERTEX_ARRAY);
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					glDisableClientState(GL_NORMAL_ARRAY);


}

 
void Draw(void)
{

	/*DrawScene();*/

	 // Clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    // Reset the matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
    // Move the camera to our location in space
    glRotatef(getCamXRot(), 1.0f, 0.0f, 0.0f);        // Rotate our camera on the x-axis (looking up and down)
    glRotatef(getCamYRot(), 0.0f, 1.0f, 0.0f);        // Rotate our camera on the  y-axis (looking left and right)
    glTranslatef(-getCamXpos(),-getCamYpos(),-getCamZpos());    // Translate the modelviewm matrix to the position of our camera
 
    // Draw the lower ground-grid
    drawGround();
 
    // Draw the upper ground-grid, keeping a copy of our current matrix on the stack before we translate it
    glPushMatrix();
 
    glTranslatef(0.0f, 200.0f, 0.0f);
 
    drawGround();
 
    glPopMatrix();
	glPushMatrix();
	Draw_3DSquare();
	glPopMatrix();
		
	


     
 
    // ----- Stop Drawing Stuff! ------
 
    glfwSwapBuffers(); // Swap the buffers to display the scene (so we don't have to watch it being drawn!)

}
