#include "alexFunc.h"
#include "functions.h"
//#include "Terrain.h"
#include <iostream>

using namespace std;
const float TO_RADS = 3.141592654f / 180.0f;

static float zoomFactor=1.0;
GLdouble move_x=5.0,
	move_y=5.0,
	move_z=0,
	max_angle = 50.0,
	min_angle = 0;

const int window_width = 800,
	window_height = 600;




int doink				= 0;
int zoink				= 0;
int steps				= 20;
int deSpeed				= 1;
int reSpeed				= -1;
GLfloat back			= -7.0;

GLint midWindowX = window_width  / 2;         // Middle of the window horizontally
GLint midWindowY = window_height / 2;         // Middle of the window vertically

GLfloat translateX = 0.0f;
GLfloat translateY= 0.0f;
GLfloat heightmapX = 150.0f;
GLfloat heightmapY = 150.0f;
GLfloat arrowAngle = 0.0f;


GLfloat fieldOfView = 45.0f;                 // Define our field of view (i.e. how quickly foreshortening occurs)
GLfloat near        = 1.0f;                  // The near (Z Axis) point of our viewing frustrum (default 1.0f)
GLfloat far         = 7500.0f;				// The far  (Z Axis) point of our viewing frustrum (default 1500.0f)

GLfloat movementSpeedFactor = 5.0f;

// Camera rotation
GLfloat camXRot = 0.0f;
GLfloat camYRot = 0.0f;
GLfloat camZRot = 0.0f;
 
// Camera position
GLfloat camXPos = 0.0f;
GLfloat camYPos = 0.0f;
GLfloat camZPos = 0.0f;

//Arrow pos
GLfloat arrowXpos = 0.0f;
GLfloat arrowYpos = 0.0f;
 
// Camera movement speed
GLfloat camXSpeed = 0.0f;
GLfloat camYSpeed = 0.0f;
GLfloat camZSpeed = 0.0f;

GLfloat fireAngle = 0.0f;
GLfloat ballistaAngle = 0.0f;
GLfloat temp =0.0f;
double ArrowSpeed = 1.0;
float snurr =0.0f;

// Hoding any keys down?
bool holdingForward     = false;
bool holdingBackward    = false;
bool holdingLeftStrafe  = false;
bool holdingRightStrafe = false;
bool firetheballista	= false;
bool holdingUPARROW		= false;
bool holdingDOWNARROW	= false;
bool holdingLEFTARROW	= false;
bool holdingRIGHTARROW	= false;
bool collision			= false;
bool deformBow			= false;
bool reformBow			= false;
double pushtime			= 0;
double old_time			= 0;
double arrowHeight		= 0;
double _terrainHeight	= 0;


double 	worldarrowX =0,
		worldarrowY=0;


float temp1 = 0.0f;



void setProjectionMatrix ()
{

	
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // The following code is a fancy bit of math that is eqivilant to calling:
    // gluPerspective(fieldOfView/2.0f, width/height , near, far);
    // We do it this way simply to avoid requiring glu.h
    GLfloat aspectRatio = (window_width > window_height)? float(window_width)/float(window_height) : float(window_height)/float(window_width);
    GLfloat fH = tan( float(fieldOfView / 360.0f * 3.14159f) ) * near;
    GLfloat fW = fH * aspectRatio;
    glFrustum(-fW, fW, -fH, fH, near, far);


}

void setViewMatrix ()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(3.0,4.0, 5.0,
	//	0.0, 0.0, 0.0,
	//	0.0, 1.0, 0.0);
}

void calculate_Arrow(double current_time)
{
	double time = current_time - pushtime;
	double timeOfFlight = getTimeOfFlight();
	
	//cout << " tid " << time << endl;
	//cout << "timeofflight" << timeOfFlight << endl;
	//cout << "Kollision ? " << getCollision() << endl;

	if(time < timeOfFlight && !getCollision())
	{
		//cout << "HERP" << endl;
		arrowAngle = getarrowAngle(time);
		translateX = getArrowposX(time);
		translateY = getArrowposY(time);
		heightmapX = getArrowposHeightmapX(time, getBallistaAngle());
		heightmapY = getArrowposHeightmapY(time,getBallistaAngle());
		worldarrowX= getArrowposWorldX(time, getBallistaAngle());
		worldarrowY=getArrowposWorldY(time, getBallistaAngle());
		old_time = time;
	}
	else if(getCollision())
	{
		//cout << "DERP DERP DERP" << endl;
		translateX = getArrowposX(old_time);
		translateY = getArrowposY(old_time);
		heightmapX = heightmapX;
		heightmapY = heightmapY;
	}
	
}

void tempScale(float scale)
{
	temp1 = scale;
}
float getScale()
{
	return temp1;
}

GLfloat getArrowRotation()
{

		return getArrowAngle();

}

void MOVE_ARROW()
{	

		glTranslatef(getArrowXpos(),getArrowYpos(),0);  
		glRotatef(getArrowRotation(),0,0,1);
		glRotatef(snurr,1,0,0);

		if(Fired() && !getCollision())
		snurr +=0.5f;
	
}
void BACKA_ARROW()
{
	for(int i=0; i<5; i++)
	{
		float t =-0.1*i;
		glTranslatef(t,1.5f,0.4f);
	}
	
}


GLfloat getArrowAngle()
{
	return arrowAngle;
}

GLfloat getArrowXpos()
{
	//cout << translateX << endl;
	return translateX*ArrowSpeed;

}
GLfloat getArrowYpos()
{
	return translateY*ArrowSpeed;
}


void moveCamera()
{
    camXPos += camXSpeed;
    camYPos += camYSpeed;
    camZPos += camZSpeed;
}

void setDoink()
{
	doink = steps/deSpeed;
}


Object moveBow(Object objStuff)
{
	//if(deformBow)
	//{
	//	doink = steps/deSpeed;
	//}

	if(reformBow)
	{
		zoink = abs(steps/reSpeed) * (getdrag() - 3) / 0.5;
		cout << "zoink " << zoink << endl;
					Resetbow();
	}

	if(doink != 0)
	{
		doink--;
		objStuff = deformBowfunc(objStuff, deSpeed);
	}

	while(zoink != 0)
	{
		zoink--;
		objStuff = deformBowfunc(objStuff, reSpeed);
	}

	return objStuff;
}

Object deformBowfunc(Object objStuff,int speed)
{
	vector<GLfloat> temp = objStuff.getVertices();

	for (int i = 0; i < temp.size(); i++)
	{
		if (i % 3 == 2)
		{
			temp[i] =  temp[i] - pow((double)temp[i - 2] , 2)/(10000/speed);
		}
	}
	objStuff.setVertices(temp);
	return objStuff;
}

// Function to deal with mouse position changes, called whenever the mouse cursorm moves
void handleMouseMove(int mouseX, int mouseY)
{
    GLfloat vertMouseSensitivity  = 10.0f;
    GLfloat horizMouseSensitivity = 10.0f;
 
 
    int horizMovement = mouseX - midWindowX;
    int vertMovement  = mouseY - midWindowY;
 
    camXRot += vertMovement / vertMouseSensitivity;
    camYRot += horizMovement / horizMouseSensitivity;
 
    // Control looking up and down with the mouse forward/back movement
    // Limit loking up to vertically up
    if (camXRot < -90.0f)
    {
        camXRot = -90.0f;
    }
 
    // Limit looking down to vertically down
    if (camXRot > 90.0f)
    {
        camXRot = 90.0f;
    }
 
    // Looking left and right. Keep the angles in the range -180.0f (anticlockwise turn looking behind) to 180.0f (clockwise turn looking behind)
    if (camYRot < -180.0f)
    {
        camYRot += 360.0f;
    }
 
    if (camYRot > 180.0f)
    {
        camYRot -= 360.0f;
    }
 
    // Reset the mouse position to the centre of the window each frame
    glfwSetMousePos(midWindowX, midWindowY);
}


void calculateCameraMovement()
{
    // Break up our movement into components along the X, Y and Z axis
    float camMovementXComponent = 0.0f;
    float camMovementYComponent = 0.0f;
    float camMovementZComponent = 0.0f;
 
    if (holdingForward == true)
    {
        // Control X-Axis movement
        float pitchFactor = cos(toRads(camXRot));
        camMovementXComponent += ( movementSpeedFactor * float(sin(toRads(camYRot))) ) * pitchFactor;
 
        // Control Y-Axis movement
        camMovementYComponent += movementSpeedFactor * float(sin(toRads(camXRot))) * -1.0f;
 
        // Control Z-Axis movement
        float yawFactor = float(cos(toRads(camXRot)));
        camMovementZComponent += ( movementSpeedFactor * float(cos(toRads(camYRot))) * -1.0f ) * yawFactor;
    }
 
    if (holdingBackward == true)
    {
        // Control X-Axis movement
        float pitchFactor = cos(toRads(camXRot));
        camMovementXComponent += ( movementSpeedFactor * float(sin(toRads(camYRot))) * -1.0f) * pitchFactor;
 
        // Control Y-Axis movement
        camMovementYComponent += movementSpeedFactor * float(sin(toRads(camXRot)));
 
        // Control Z-Axis movement
        float yawFactor = float(cos(toRads(camXRot)));
        camMovementZComponent += ( movementSpeedFactor * float(cos(toRads(camYRot))) ) * yawFactor;
    }
 
    if (holdingLeftStrafe == true)
    {
        // Calculate our Y-Axis rotation in radians once here because we use it twice
        float yRotRad = toRads(camYRot);
 
        camMovementXComponent += -movementSpeedFactor * float(cos(yRotRad));
        camMovementZComponent += -movementSpeedFactor * float(sin(yRotRad));
    }
 
    if (holdingRightStrafe == true)
    {
        // Calculate our Y-Axis rotation in radians once here because we use it twice
        float yRotRad = toRads(camYRot);
 
        camMovementXComponent += movementSpeedFactor * float(cos(yRotRad));
        camMovementZComponent += movementSpeedFactor * float(sin(yRotRad));
    }
 
    // After combining our movements for any & all keys pressed, assign them to our camera speed along the given axis
    camXSpeed = camMovementXComponent;
    camYSpeed = camMovementYComponent;
    camZSpeed = camMovementZComponent;
 
    // Cap the speeds to our movementSpeedFactor (otherwise going forward and strafing at an angle is twice as fast as just going forward!)
    // X Speed cap
    if (camXSpeed > movementSpeedFactor)
    {
        //cout << "high capping X speed to: " << movementSpeedFactor << endl;
        camXSpeed = movementSpeedFactor;
    }
    if (camXSpeed < -movementSpeedFactor)
    {
        //cout << "low capping X speed to: " << movementSpeedFactor << endl;
        camXSpeed = -movementSpeedFactor;
    }
 
    // Y Speed cap
    if (camYSpeed > movementSpeedFactor)
    {
        //cout << "low capping Y speed to: " << movementSpeedFactor << endl;
        camYSpeed = movementSpeedFactor;
    }
    if (camYSpeed < -movementSpeedFactor)
    {
        //cout << "high capping Y speed to: " << movementSpeedFactor << endl;
        camYSpeed = -movementSpeedFactor;
    }
 
    // Z Speed cap
    if (camZSpeed > movementSpeedFactor)
    {
        //cout << "high capping Z speed to: " << movementSpeedFactor << endl;
        camZSpeed = movementSpeedFactor;
    }
    if (camZSpeed < -movementSpeedFactor)
    {
        //cout << "low capping Z speed to: " << movementSpeedFactor << endl;
        camZSpeed = -movementSpeedFactor;
    }
}



double getArrowSpeed()
{
	return ArrowSpeed;
}

//Flytta kameran
void Move_Camera()
{
	// Move the camera to our location in space
    glRotatef(getCamXRot(), 1.0f, 0.0f, 0.0f);        // Rotate our camera on the x-axis (looking up and down)
    glRotatef(getCamYRot(), 0.0f, 1.0f, 0.0f);        // Rotate our camera on the  y-axis (looking left and right)
    glTranslatef(-getCamXpos(),-getCamYpos(),-getCamZpos());    // Translate the modelviewm matrix to the position of our camera
}

void calculate_BallistaAngle()
{
	if(holdingDOWNARROW == true && fireAngle > min_angle)
	{
		fireAngle -= 1.0;
		cout << fireAngle<<endl;

	}
	if(holdingUPARROW == true && fireAngle < max_angle)
	{
		fireAngle += 1.0;
		cout << fireAngle<<endl;
	}
	if(holdingLEFTARROW == true && firetheballista == false)
	{
		ballistaAngle -= 1.0;
		cout << "ballista angle " << ballistaAngle << endl;
	}
	if(holdingRIGHTARROW == true && firetheballista == false)
	{
		ballistaAngle += 1.0;
		cout << "ballista angle " << ballistaAngle << endl;
	}


}

void rotateBallista()
{
	glRotatef(getBallistaAngle(), 0,1,0);
}

GLfloat getFireAngle()
{
	return fireAngle;
}

GLfloat getBallistaAngle()
{
	return ballistaAngle;
}
// Function to convert degrees to radians
float toRads(const float &theAngleInDegrees)
{
    return theAngleInDegrees * TO_RADS;
}

// Function to set flags according to which keys are pressed or released
void handleKeypress(int theKey, int theAction)
{
    // If a key is pressed, toggle the relevant key-press flag
    if (theAction == GLFW_PRESS)
    {
 
        switch(theKey)
        {
        case 'W':
            holdingForward = true;
            break;
 
        case 'S':
            holdingBackward = true;
            break;
 
        case 'A':
            holdingLeftStrafe = true;
            break;
 
        case 'D':
            holdingRightStrafe = true;
			break;

		case 'F':
			firetheballista = true;
			pushtime = glfwGetTime();
			
			Arrowpos();
			reformBow = true;
			resetArrow();
			//Resetbow();
			break;

		case 'R':
			if(getdrag() < 5)
			{
				setdrag();
				setArrowBack();
				/*deformBow = true;*/
				setDoink();
				collision = false;
				firetheballista= false;
				ResetIndex();
				pushtime = 0;
				translateX = 0.0f;
				translateY = 0.0f;
				arrowHeight = 0.0f;
				_terrainHeight = 99999.0f;
			}

			break;

		case 'O':
			deformBow = true;
			break;


		case 'P':
			reformBow = true;
			break;

		case GLFW_KEY_UP:
			holdingUPARROW = true;
			break;

		case GLFW_KEY_DOWN:
			holdingDOWNARROW = true;
			break;

		case GLFW_KEY_LEFT:
			holdingLEFTARROW = true;
			break;

		case GLFW_KEY_RIGHT:
			holdingRIGHTARROW = true;
			break;
		case GLFW_KEY_ESC:
			exit(0);
			break;
 
        default:
            // Do nothing...
            break;
        }
    }
    else // If a key is released, toggle the relevant key-release flag
    {
        switch(theKey)
        {
        case 'W':
            holdingForward = false;
            break;
 
        case 'S':
            holdingBackward = false;
            break;
 
        case 'A':
            holdingLeftStrafe = false;
            break;
 
        case 'D':
            holdingRightStrafe = false;
            break;


		case 'R':
			deformBow = false;
			break;

		case 'F':
			reformBow = false;
			break;


		case 'O':
			deformBow = false;
			break;
			
		case 'P':
			reformBow = false;
			break;
			

			
		case GLFW_KEY_UP:
			holdingUPARROW = false;
			break;

		case GLFW_KEY_DOWN:
			holdingDOWNARROW = false;
			break;

		case GLFW_KEY_LEFT:
			holdingLEFTARROW = false;
			break;

		case GLFW_KEY_RIGHT:
			holdingRIGHTARROW = false;
			break;

 
        default:
            // Do nothing...
            break;
        }
    }
}
bool Fired()
{
	return firetheballista;
}
GLfloat getCamXpos()
{
	return camXPos;
}
GLfloat getCamYpos()
{
	return camYPos;
}
GLfloat getCamZpos()
{
	return camZPos;
}

GLfloat getCamXRot()
{
	return camXRot;
}
					
GLfloat getCamYRot()
{
	return camYRot;
}

void CheckCollision(Terrain* _terrain,double current_time, float scale, Object o)
{
	_terrainHeight = _terrain->getHeight(heightmapX,heightmapY)*scale;
	arrowHeight = (getArrowYpos())/**scale*/;

	//cout <<" X   Y   " << heightmapX<<" ,  "<<heightmapY<< endl;
	

	double time = current_time - pushtime;

	if(_terrainHeight>arrowHeight && pushtime != current_time)
	{		
		collision = true;
		//cout << "NEIN" << endl;
	}
	//cout << worldarrowX<<"     "<< o.max.getX() <<"     "<< o.min.getX() << endl;
	//cout << worldarrowX << "    " << worldarrowY << endl;


	if(worldarrowX<-o.max.getX() && worldarrowX>-o.min.getX() &&  worldarrowY<o.max.getZ() && worldarrowY>o.min.getZ() && getArrowYpos()<o.max.getY() && getArrowYpos()>o.min.getY())
		collision = true;

		
}
bool getCollision(){
	return collision;
}



bool getDeform()
{
	return deformBow;
}

bool getReform()
{
	return reformBow;
}

GLfloat getArrowBack()
{
	return back;
}

void setArrowBack()
{

	 back-=0.5;
}
void resetArrow()
{
	back= -7;
}