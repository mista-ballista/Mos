#include <gl/glfw.h>
#include <stdlib.h>
#include <stdio.h>
#include "functions.h"
#include "alexFunc.h"

void Init(void);
void Shut_Down(int return_code);
void Main_Loop(void);
void Draw_Square(float red, float green, float blue);
void Draw(void);

float rotate_y = 0,
	rotate_z = 0,
	rotate_x = 0;

const float rotations_per_tick = .2;

const int window_width = 800,
	window_height = 600;

int main(void)
{
	Arrowpos();
	Init();
	Main_Loop();
	Shut_Down(0);
}


void Init(void)
{


	if (!glfwInit())
		Shut_Down(1);
	// 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
	if (!glfwOpenWindow(window_width, window_height, 5, 6, 5, 0, 0, 0, GLFW_WINDOW))
		Shut_Down(1);

	glfwSetWindowTitle("jävla openGL skit lol");

	setProjectionMatrix ();
	setViewMatrix();



	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	//glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
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
		//double current_time = glfwGetTime(),
		//	delta_rotate = (current_time - old_time) * rotations_per_tick * 360;
		//old_time = current_time;

		//Stäng av om esc 
		if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
			break;

		keyboardInput();
		// clear the buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// draw the figure
		Draw();
		// swap back and front buffers
		glfwSwapBuffers();
	}
}



void Draw_3DSquare()
{
	//GLfloat vertices[] = {x+1,y+1,z,
	//	x	,y+1	,z,
	//	x	,y		,z,
	//	x+1	,y		,z,
	//	x+1	,y		,z+1,
	//	x+1	,y+1	,z+1,
	//	x	,y+1	,z+1,
	//	x	,y		,z+1};

	GLfloat vertices[] = {
		-1.0000,	0.0000,	 1.0000,
		-1.0000,	0.0000,	 -1.0000,
		1.0000,	0.0000,	 -1.0000,
		1.0000,	0.0000,	 1.0000,
		-1.0000,	2.0000,	 1.0000,
		1.0000,	2.0000,	 1.0000,
		1.0000,	2.0000,	 -1.0000,
		-1.0000,	2.0000,	 -1.0000};

		//GLubyte indices[] = {
		//	0,1,2,3,
		//	0,3,4,5,
		//	0,5,6,1,
		//	1,6,7,2,
		//	7,4,3,2,
		//	4,7,6,5};

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

				//GLfloat normals[] = {x,y,z+1,
				//	x+1	,y		,z,
				//	x	,y+1	,z,
				//	x-1	,y		,z,
				//	x	,y-1	,z,
				//	x	,y		,z-1};

				//GLfloat texcoord[] = {1,1,	0,1,	0,0,	1,0,
				//	0,1,	0,0,	1,0,	1,1,
				//	1,0,	1,1,	0,1,	0,0,
				//	1,1,	0,1,	0,0,	1,0,
				//	1,1,	0,1,	0,0,	1,0,
				//	0,0,	1,0,	1,1,	0,1};

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

	modViewMatrix();
	modProjectionMatrix();
	Draw_3DSquare();
}