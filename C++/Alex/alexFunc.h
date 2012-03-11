#ifndef alexFunc_h
#define alexFunc_h
#include <gl/glfw.h>
#include "Terrain.h"
#include "Object.h"


void setProjectionMatrix();
void setViewMatrix();
void moveArrow();
GLfloat getArrowXpos();
GLfloat getArrowYpos();
void MOVE_ARROW();
void BACKA_ARROW();

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
void CheckCollision(Terrain* _terrain,double current_time, float scale, Object o);
bool getCollision();
void tempScale(float scale);
float getScale();
double getArrowSpeed();
GLfloat getCamXpos();
GLfloat getCamYpos();
GLfloat getCamZpos();
GLfloat getCamXRot();
GLfloat getCamYRot();
GLfloat getArrowBack();
void	setArrowBack();
void	resetArrow();

Object moveBow(Object objStuff);
Object deformBowfunc(Object objStuff, int speed);
bool getDeform();
bool getReform();
void setDoink();



#endif