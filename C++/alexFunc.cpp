#include "alexFunc.h"
#include "functions.h"


static float zoomFactor=1.0;
GLdouble move_x=5.0,
	move_y=5.0,
	move_z=0;

const int window_width = 800,
	window_height = 600;

GLfloat translateX = 0.0f;
GLfloat translateY= 0.0f;


void keyboardInput()
{
		if (glfwGetKey(GLFW_KEY_LEFT))
			move_y += 0.2f;
		if (glfwGetKey(GLFW_KEY_RIGHT))
			move_y -= 0.2f;
		if(glfwGetKey(GLFW_KEY_UP))
			move_x+=0.2f;
		if(glfwGetKey(GLFW_KEY_DOWN))
			move_x -= 0.2f;
		if(glfwGetKey(GLFW_KEY_PAGEUP))
			zoomFactor+=0.02f;
		if(glfwGetKey(GLFW_KEY_PAGEDOWN))
			zoomFactor-=0.02f;





}

void setProjectionMatrix ()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect_ratio = ((float)window_height) / window_width;
	glFrustum(.5, -.5, -.5 * aspect_ratio, .5 * aspect_ratio, 1, 50);
}

void setViewMatrix ()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(5.0, 5.0, 30.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
}

void modViewMatrix ()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(move_y, move_x, 30,
		move_y, move_x, 0,
		0.0, 1.0, 0.0);
}

void modProjectionMatrix ()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect_ratio = ((float)window_height) / window_width;
	glFrustum(.5*zoomFactor, -.5*zoomFactor, -.5 * aspect_ratio*zoomFactor, .5 * aspect_ratio*zoomFactor, 1, 50);
}

void moveArrow()
{

	double current_time = glfwGetTime();
	translateX = getArrowposX(current_time);
	translateY = getArrowposY(current_time);
}