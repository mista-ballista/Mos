#define WIN32_LEAN_AND_MEAN
#define BITMAP_ID 0x4D42

#include <GL/glfw.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <stdio.h>
#include "alexFunc.h"
#include "functions.h"
#include "Object.h"
#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Terrain.h"

void Init(void);
void Shut_Down(int return_code);
void Main_Loop(void);
void Draw(void);
void LIGHT(void);
void FOG(void);

using namespace std;

string strArrow = "Data/bigArrow.obj";
string strBallista = "Data/ballista.obj";
string strBorg = "Data/Borg.obj";
char texname[] = "Data/johda2.tga";

Object objArrow(strArrow);
Object objBallista(strBallista);
Object objBorg(strBorg);

int frames = 0;
float roll;
double t0 = 0.0;
char titlestring[200];
double current_time = 0.0;

GLuint textureTest[4];
 
GLfloat	fogColor[4] = {0.5f,0.5f,0.5f,1.0f};

vector<GLfloat> vertices;
vector<GLfloat> texcoords;
vector<GLfloat> normals;
vector<GLuint> indices;

Terrain* _terrain;
GLUquadric *myQuad;


//Loads a terrain from a heightmap.  The heights of the terrain range from
//-height / 2 to height / 2.
Terrain* loadTerrain(const char* filename, float height) {
	Image* image = loadBMP(filename);
	Terrain* t = new Terrain(image->width, image->height);
	for(int y = 0; y < image->height; y++) {
		for(int x = 0; x < image->width; x++) {
			unsigned char color =
				(unsigned char)image->pixels[3 * (y * image->width + x)];
			float h = height * ((color / 255.0f) - 0.5f);
			t->setHeight(x, y, h);
		}
	}
	
	delete image;
	t->computeNormals();
	return t;
}

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;							      // the file pointer
	BITMAPFILEHEADER	bitmapFileHeader;		// bitmap file header
	unsigned char		*bitmapImage;			// bitmap image data
	int					imageIdx = 0;		   // image index counter
	unsigned char		tempRGB;				   // swap variable

	// open filename in "read binary" mode
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	// verify that this is a bitmap by checking for the universal bitmap id
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// read the bitmap information header
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// move file pointer to beginning of bitmap data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// allocate enough memory for the bitmap image data
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// verify memory allocation
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// make sure bitmap image data was read
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// close the file and return the bitmap image data
	fclose(filePtr);
	return bitmapImage;
}

void cleanup() {
	delete _terrain;
}

int main(void)
{
	Init();
	Main_Loop();
	Shut_Down(0);
}

void showFPS() {

    double t, fps;    
    // Get current time
    t = glfwGetTime();  // Gets number of seconds since glfwInit()
    // If one second has passed, or if this is the very first frame
    if( (t-t0) > 1.0 || frames == 0 )
    {
        fps = (double)frames / (t-t0);
        sprintf(titlestring, "Mista Ballista (%.1f FPS)", fps);
        glfwSetWindowTitle(titlestring);
        t0 = t;
        frames = 0;
    }
    frames ++;
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
	myQuad = gluNewQuadric ();	

	_terrain = loadTerrain("Data/heightmap2.bmp", 20);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
  	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glEnable(GL_CULL_FACE);								// Do not draw polygons facing away from us

	//Skapa texturer
	glGenTextures(3, textureTest);
	glBindTexture(GL_TEXTURE_2D, textureTest[0]); // Activate first texture
    glfwLoadTexture2D("Data/moon.tga", GLFW_BUILD_MIPMAPS_BIT); // Load image
    glBindTexture(GL_TEXTURE_2D, textureTest[1]); // Activate second texture
    glfwLoadTexture2D("Data/earth.tga", GLFW_BUILD_MIPMAPS_BIT);
	glBindTexture(GL_TEXTURE_2D, textureTest[2]); // Activate second texture
    glfwLoadTexture2D("Data/sun.tga", GLFW_BUILD_MIPMAPS_BIT);
	glBindTexture(GL_TEXTURE_2D, textureTest[3]);
	glfwLoadTexture2D("Data/test.tga", GLFW_BUILD_MIPMAPS_BIT);	
	
	LIGHT(); 
	FOG();
}

void FOG()
{	
	glFogi(GL_FOG_MODE, GL_LINEAR);			// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);		// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.005f);			// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_NICEST);			// Fog Hint Value
	glFogf(GL_FOG_START, 400.0f);			// Fog Start Depth
	glFogf(GL_FOG_END, 800.0f);				// Fog End Depth
	glEnable(GL_FOG);						// Enables GL_FOG
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

  while(1)  // This just loops as long as the program runs
  {	
	  double current_time = glfwGetTime();
	  showFPS();

    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Draw(); // Draw everything

	// Calculate our camera movement
	calculateCameraMovement();
	calculate_BallistaAngle();
	setFireAngle(getFireAngle());
	
	moveCamera(); // Move our camera

	calculate_Arrow(current_time);
	
	glfwSwapBuffers(); // swap back and front buffers
  }
}

void LIGHT()
{
	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);	
	GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat lightPos0[] = {-0.0f, 20.0f, 0.1f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
}

void DrawMap()
{	
	float scale = 2000.0f / max(_terrain->width() - 1, _terrain->length() - 1);
	glScalef(scale, scale, scale);
	glTranslatef(-(float)(_terrain->width() - 1) / 2,
				 0.0f,
				 -(float)(_terrain->length() - 1) / 2);
	
	
	for(int z = 0; z < _terrain->length() - 1; z++) {
		//Makes OpenGL draw a triangle at every three consecutive vertices
		glBegin(GL_TRIANGLE_STRIP);
		for(int x = 0; x < _terrain->width(); x++) {
			Vec3f normal = _terrain->getNormal(x, z);			
			glNormal3f(normal[0], normal[1], normal[2]);
			//glTexCoord2f(z/_terrain->width(),x/_terrain->length());
			glTexCoord2f(z,x);
			glVertex3f(x, _terrain->getHeight(x, z), z);

			normal = _terrain->getNormal(x, z + 1);			
			glNormal3f(normal[0], normal[1], normal[2]);
			//glTexCoord2f((z/_terrain->width())+1,(x/_terrain->length())+1);
			glTexCoord2f(z+1,x+1);
			glVertex3f(x, _terrain->getHeight(x, z + 1), z + 1);
		}
		glEnd();
	}
}

void SkyBox()
{
	glDisable(GL_DEPTH);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureTest[3]);
	glTranslatef(0,0,0);
	glRotatef(90,1,0,1);
	gluSphere(myQuad,500,200,200);
	glRotatef(roll/10, 0.2, 1, 0);
	glColor4f(1, 1, 1, cos(roll/12));
	gluSphere(myQuad, 500-0.8, 200, 200);
	glEnable(GL_DEPTH);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	roll+=0.002f;
}
 
void Draw(void)
{
	 // Clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
    // Reset the matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 

	Move_Camera();

	// ----------- SCENGRAF -----------
	glPushMatrix();
	glPushMatrix();

	//glBindTexture(GL_TEXTURE_2D, textureTest[3]);
	glScalef(-1.0f,-1.0f,-1.0f);
		SkyBox();
		glPopMatrix();

		//Ballista
		glPushMatrix();	

			//Matrismultiplikationer som rör Ballistan här!
			//glColor3f(1.0f, 0.0f, 0.0f);
			rotateBallista();
			glTranslatef(0.0f, 0.0f, 0.0f);
			glScalef(0.2f, 0.2f, 0.2f);
			glBindTexture(GL_TEXTURE_2D, textureTest[0]);
			objBallista.DrawObject();
		
			//Arrow
			glPushMatrix();		
				//Matrismultiplikationer som rör Pilen här!
				glRotatef(90.0 , 0.0f, 1.0f, 0.0f);
				MOVE_ARROW();
				objArrow.DrawObject();			
			glPopMatrix();
		glPopMatrix();
		
		//Borg	
		glPushMatrix();
			//Matrismultiplikationer som rör Borgen här!	
			//glColor3f(0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, 0.0f, 50.0f);
			glScalef(1.0f, 1.0f, 1.0f);
			glRotatef(0.0f, 0.0f, 1.0f, 0.0f);	
			glBindTexture(GL_TEXTURE_2D, textureTest[1]);
			objBorg.DrawObject();		
		glPopMatrix();

		//Terräng och "markplan"
		//Matrismultiplikationer som rör Heightmap här!	
		//glColor3f(0.0f, 1.0f, 0.0f);
		glRotatef(0.0f, 0.0f, 1.0f, 0.0f);	
		glBindTexture(GL_TEXTURE_2D, textureTest[2]);
		glTranslatef(0.0f,335.0f,0.0f);
		DrawMap();
		//drawGround();
	glPopMatrix();
 
    // ----- Stop Drawing Stuff! ------
}