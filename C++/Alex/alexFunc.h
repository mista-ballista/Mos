#ifndef alexFunc_h
#define alexFunc_h
#include <gl/glfw.h>

void setProjectionMatrix();
void setViewMatrix();
void moveArrow(double);


void moveCamera();
void handleMouseMove(int mouseX, int mouseY);

void calculateCameraMovement();
void drawGround();

float toRads(const float &theAngleInDegrees);
void calculateCameraMovement();
void handleKeypress(int theKey, int theAction);

GLfloat getCamXpos();
GLfloat getCamYpos();
GLfloat getCamZpos();
GLfloat getCamXRot();
GLfloat getCamYRot();



#endif