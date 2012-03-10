#ifndef functions_h
#define functions_h

#include <iostream>

float rsum(float a, float b, int n);
float force(float _drawLength);
void Arrowpos();
float getArrowposX(float time);
float getArrowposY(float time);
double getTimeOfFlight();
void setFireAngle(float _tempVinkel);
float getarrowAngle(float time);
//double getFireAngle();
void ResetIndex();
float getArrowposHeightmapY(float time, double theta);
float getArrowposHeightmapX(float time, double theta);
void Reload();
float getArrowposWorldX(float time, double theta);
float getArrowposWorldY(float time, double theta);

#endif