#include "alexFunc.h"
#include "functions.h"
#include "parseObj.h"
#include <SOIL.h>


const float TO_RADS = 3.141592654f / 180.0f;

static float zoomFactor=1.0;
GLdouble move_x=5.0,
	move_y=5.0,
	move_z=0;

const int window_width = 800,
	window_height = 600;

GLint midWindowX = window_width  / 2;         // Middle of the window horizontally
GLint midWindowY = window_height / 2;         // Middle of the window vertically

GLfloat translateX = 0.0f;
GLfloat translateY= 0.0f;


GLfloat fieldOfView = 45.0f;                 // Define our field of view (i.e. how quickly foreshortening occurs)
GLfloat near        = 1.0f;                  // The near (Z Axis) point of our viewing frustrum (default 1.0f)
GLfloat far         = 1500.0f;               // The far  (Z Axis) point of our viewing frustrum (default 1500.0f)

GLfloat movementSpeedFactor = 3.0f;

// Camera rotation
GLfloat camXRot = 0.0f;
GLfloat camYRot = 0.0f;
GLfloat camZRot = 0.0f;
 
// Camera position
GLfloat camXPos = 0.0f;
GLfloat camYPos = 0.0f;
GLfloat camZPos = 0.0f;
 
// Camera movement speed
GLfloat camXSpeed = 0.0f;
GLfloat camYSpeed = 0.0f;
GLfloat camZSpeed = 0.0f;

// Hoding any keys down?
bool holdingForward     = false;
bool holdingBackward    = false;
bool holdingLeftStrafe  = false;
bool holdingRightStrafe = false;






void setProjectionMatrix ()
{
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//float aspect_ratio = ((float)window_height) / window_width;
	//glFrustum(.5, -.5, -.5 * aspect_ratio, .5 * aspect_ratio, 1, 50);
	
	
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


void moveArrow(double current_time)
{


	translateX = getArrowposX(current_time);
	translateY = getArrowposY(current_time);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(translateX,translateY,0);
}

void moveCamera()
{
    camXPos += camXSpeed;
    camYPos += camYSpeed;
    camZPos += camZSpeed;
}


// Function to deal with mouse position changes, called whenever the mouse cursorm moves
void handleMouseMove(int mouseX, int mouseY)
{
    GLfloat vertMouseSensitivity  = 10.0f;
    GLfloat horizMouseSensitivity = 10.0f;
 
    //cout << "Mouse cursor is at position (" << mouseX << ", " << mouseY << endl;
 
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

// Function to draw a grid of lines
void drawGround()
{
    GLfloat extent      = 600.0f; // How far on the Z-Axis and X-Axis the ground extends
    GLfloat stepSize    = 20.0f;  // The size of the separation between points
    GLfloat groundLevel = -50.0f;   // Where on the Y-Axis the ground is drawn
 
    // Set colour to white
    glColor3ub(255, 255, 255);
 
    // Draw our ground grid
    glBegin(GL_LINES);
    for (GLint loop = -extent; loop < extent; loop += stepSize)
    {
        // Draw lines along Z-Axis
        glVertex3f(loop, groundLevel,  extent);
        glVertex3f(loop, groundLevel, -extent);
 
        // Draw lines across X-Axis
        glVertex3f(-extent, groundLevel, loop);
        glVertex3f(extent,  groundLevel, loop);
    }
    glEnd();
 
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
 
        default:
            // Do nothing...
            break;
        }
    }
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