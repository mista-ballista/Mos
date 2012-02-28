#ifndef alexFunc_h
#define alexFunc_h
#include <gl/glfw.h>

void setProjectionMatrix();
void setViewMatrix();
void moveArrow();
GLfloat getArrowXpos();
GLfloat getArrowYpos();
void MOVE_ARROW();

void moveCamera();
void handleMouseMove(int mouseX, int mouseY);

void calculateCameraMovement();
void drawGround();

float toRads(const float &theAngleInDegrees);
void calculateCameraMovement();
void handleKeypress(int theKey, int theAction);
void Move_Camera();
void calculate_Arrow(double current_time);
void calculate_BallistaAngle();
void rotateBallista();
GLfloat getBallistaAngle();
GLfloat getFireAngle();
GLfloat getArrowRotation();
GLfloat getArrowAngle();
bool Fired();

GLfloat getCamXpos();
GLfloat getCamYpos();
GLfloat getCamZpos();
GLfloat getCamXRot();
GLfloat getCamYRot();



#endif