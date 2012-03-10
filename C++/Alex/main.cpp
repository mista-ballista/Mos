#define WIN32_LEAN_AND_MEAN
#define BITMAP_ID 0x4D42

#include <GL/glfw.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <stdio.h>
#include "alexFunc.h"
#include "functions.h"
//#include "Object.h"
#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
//#include "Terrain.h"
#include "skybox.h"

void Init(void);
void Shut_Down(int return_code);
void Main_Loop(void);
void Draw(void);
void LIGHT(void);
void FOG(void);
float getScale();
float scaleH=0.0f;

using namespace std;

string strArrow = "Data/Arrow_correct.obj";
string strBallista = "Data/ballista_woBow.obj";
string strBow = "Data/ballista_onlyBow.obj";
string strBorg = "Data/old_castle.obj";
string strMan = "Data/swordman.obj";

Object objArrow(strArrow);
Object objBallista(strBallista);
Object objBorg(strBorg);
Object objBow(strBow);
Object objSwordman(strMan);


int frames = 0;
float roll;
double t0 = 0.0;
char titlestring[200];
double current_time = 0.0;
float _heightTranslate = 0.0;

GLuint textureTest[6];
 
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
			float h = height * ((color / 255.0f) /*- 0.5f*/);
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
	const int window_width = 1200,
	          window_height = 800;
	
	if (!glfwInit())
	  Shut_Down(1);

	// 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
	if (!glfwOpenWindow(window_width, window_height, 5, 6, 5, 0, 0, 0, GLFW_WINDOW))
	  Shut_Down(1);
	


	objBorg.createBoundingBox();




	glfwDisable(GLFW_MOUSE_CURSOR); // Hide the mouse cursor
	//glfwSetWindowTitle("TEST LOL");
	
	setProjectionMatrix ();
	setViewMatrix();
	myQuad = gluNewQuadric ();	

	_terrain = loadTerrain("Data/heightmap2.bmp", 20);
	LIGHT(); 
	glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
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
	glGenTextures(6, textureTest);
	glBindTexture(GL_TEXTURE_2D, textureTest[0]); // Activate first texture
    glfwLoadTexture2D("Data/UV_2.tga", GLFW_BUILD_MIPMAPS_BIT); // Load image
    glBindTexture(GL_TEXTURE_2D, textureTest[1]); // Activate second texture
    glfwLoadTexture2D("Data/UV_old_castle_TAG.tga", GLFW_BUILD_MIPMAPS_BIT);
	glBindTexture(GL_TEXTURE_2D, textureTest[2]); // Activate second texture
    glfwLoadTexture2D("Data/grass.tga", GLFW_BUILD_MIPMAPS_BIT);
	glBindTexture(GL_TEXTURE_2D, textureTest[3]);
	glfwLoadTexture2D("Data/UV_pil.tga", GLFW_BUILD_MIPMAPS_BIT);	
	glBindTexture(GL_TEXTURE_2D, textureTest[4]);
	glfwLoadTexture2D("Data/sky.tga", GLFW_BUILD_MIPMAPS_BIT);	
	glBindTexture(GL_TEXTURE_2D, textureTest[5]);
	glfwLoadTexture2D("Data/swordman.tga", GLFW_BUILD_MIPMAPS_BIT);	

	

	//FOG();



}

void FOG()
{	
	glFogi(GL_FOG_MODE, GL_LINEAR);			// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);		// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.005f);			// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_NICEST);			// Fog Hint Value
	glFogf(GL_FOG_START, 100.0f);			// Fog Start Depth
	glFogf(GL_FOG_END, 1500.0f);				// Fog End Depth
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
	
	scaleH = 5.0f; /*2000.0f / max(_terrain->width() - 1, _terrain->length() - 1);*/
	_heightTranslate = _terrain->getHeight(150,150);
	_heightTranslate =_heightTranslate* scaleH;

	tempScale(scaleH);



  while(1)  // This just loops as long as the program runs
  {	
	  double current_time = glfwGetTime();
	  showFPS();

    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	

		Draw(); // Draw everything

	glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
		//Kolla pilen mot heightmap
	if( Fired())
	{
		calculate_Arrow(current_time);
		CheckCollision(_terrain, current_time, scaleH);
		
	}
	
	// Calculate our camera movement
	calculateCameraMovement();
	calculate_BallistaAngle();
	setFireAngle(getFireAngle());

	objBow = moveBow(objBow);//Kollar om bågen spänns
	moveCamera(); // Move our camera

	
	glfwSwapBuffers(); // swap back and front buffers
  }
}

void LIGHT()
{
	//GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f};
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	//GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 0.0f};
	//GLfloat lightPos0[] = {-0.0f, 50.0f, 0.0f, 0.0f};
	////glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	//glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, lightColor0);
	 GLfloat lightpos[4]={0.0f, 0.0f, 0.0f, 5.0f};
	glLightfv(GL_LIGHT0, GL_POSITION,lightpos);
}

void DrawMap()
{	
	float scale = 5.0f; /*/ max(_terrain->width() - 1, _terrain->length() - 1);*/
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
	//glColor4f(0,0,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureTest[4]);
	glTranslatef(0,0,0);
	glRotatef(90,1,0,1);
	gluSphere(myQuad,3500,50,50);
	//glRotatef(roll/10, 0.2, 1, 0);
	//glColor4f(1, 1, 1, cos(roll/12));
	//gluSphere(myQuad, 55000-0.8, 50, 50);
	glEnable(GL_DEPTH);
	glDisable(GL_BLEND);
	roll+=0.002f;
	glEnable(GL_LIGHTING);
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
	glColor3f(0.0f,0.0f,0.0f);
		SkyBox();
		glPopMatrix();

		//Ballista
		glPushMatrix();	

			//Matrismultiplikationer som rör Ballistan här!
			rotateBallista();
			glTranslatef(0.0f, _heightTranslate, 0.0f);
			glBindTexture(GL_TEXTURE_2D, textureTest[0]);
			//glScalef(0.25f,0.25f,0.25f);
			objBallista.DrawObject();
			glPushMatrix();
			objBow.DrawObject();
			glPopMatrix();
			//Arrow
			glPushMatrix();		
				//Matrismultiplikationer som rör Pilen här!
			glRotatef(270.0f,0,1,0);
			if(!getCollision()&& !Fired())
			{
				glTranslatef(-9.0f,1.5f,0.4f);
				glRotatef(6.0f,0,0,1);
			}
			else
			{
				//glTranslatef(0.0f,20.0f,0.0f);
				MOVE_ARROW();
			}
				glBindTexture(GL_TEXTURE_2D, textureTest[3]);
				objArrow.DrawObject();			
			glPopMatrix();
		glPopMatrix();
		
		//Borg	
		glPushMatrix();
			//Matrismultiplikationer som rör Borgen här!	
			glTranslatef(-100.0f, _heightTranslate+15, 400.0f);
			glRotatef(45.0f,0,1,0);
			glScalef(15.0f,15.0f, 15.0f);
			glBindTexture(GL_TEXTURE_2D, textureTest[1]);
			objBorg.DrawObject();		
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(5,0,5);
			glRotatef(35,0,1,0);
			glScalef(0.015f,0.015f,0.015f);
			glBindTexture(GL_TEXTURE_2D, textureTest[5]);
			objSwordman.DrawObject();
		glPopMatrix();

		//Terräng och "markplan"
		//Matrismultiplikationer som rör Heightmap här!	
		glBindTexture(GL_TEXTURE_2D, textureTest[2]);
		glColor3f(0.0f,1.0f,0.0);
		DrawMap();
	glPopMatrix();

 

    // ----- Stop Drawing Stuff! ------
}