#define WIN32_LEAN_AND_MEAN
#define BITMAP_ID 0x4D42


#include <GL/glfw.h>
#include <stdlib.h>
#include <SOIL.h>
#include <vector>
#include <cmath>
#include <stdio.h>
#include "alexFunc.h"
#include "functions.h"
#include "Object.h"


/////////


#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "Terrain.h"

void Init(void);
void Shut_Down(int return_code);
void Main_Loop(void);
void Draw_Square(float red, float green, float blue);
void Draw(void);
void LIGHT(void);

using namespace std;

string strArrow = "Data/bigArrow.obj";
string strBallista = "Data/ballista.obj";
string strBorg = "Data/Borg.obj";
char texname[] = "Data/johda2.tga";

Object objArrow(strArrow);
Object objBallista(strBallista);
Object objBorg(strBorg);

int frames = 0;
double t0 = 0.0;
char titlestring[200];
double current_time = 0.0;

GLuint	texture[1];
 
vector<GLfloat> vertices;
vector<GLfloat> texcoords;
vector<GLfloat> normals;
vector<GLuint> indices;


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

float _angle = 60.0f;
Terrain* _terrain;
BITMAPINFOHEADER	landInfo;
unsigned char*       landTexture;
unsigned int		   land;


void cleanup() {
	delete _terrain;
}

int LoadGLTextures()
{
	/* load an image file directly as a new OpenGL texture */
	texture[0] = SOIL_load_OGL_texture
		(
		texname,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);

	if(texture[0] == 0)
		Shut_Down(1);

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	return true;
}

bool LoadTextures()
{
	// load the land texture data
	landTexture = LoadBitmapFile("Terrain2.bmp", &landInfo);
	if (!landTexture)
		return false;

	// generate the land texture as a mipmap
	glGenTextures(1, &land);                  
	glBindTexture(GL_TEXTURE_2D, land);       
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, landInfo.biHeight, landInfo.biWidth, GL_RGB, GL_UNSIGNED_BYTE, landTexture);

	return true;
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	//LoadTextures();
}


int main(void)
{
	Init();
	//LoadGLTextures();
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

	LIGHT();

	initRendering();
	_terrain = loadTerrain("Data/heightmap2.bmp", 20);

  	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);				// Black Background

	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

    //glEnable(GL_CULL_FACE); // Do not draw polygons facing away from us
 
    glLineWidth(2.0f);			// Set a 'chunky' line width

	// Ladda in objekt
	//parseObj(objname, vertices, texcoords, normals, indices);


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


  // this just loops as long as the program runs
  while(1)
  {	
	  double current_time = glfwGetTime();

	  showFPS();

    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw the figure
	Draw();


	// Calculate our camera movement
	calculateCameraMovement();

	calculate_BallistaAngle();
	

	setFireAngle(getFireAngle());

	// Move our camera
	moveCamera();

	calculate_Arrow(current_time);

	// swap back and front buffers
	glfwSwapBuffers();
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

	
	float scale = 200.0f / max(_terrain->width() - 1, _terrain->length() - 1);
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

void DrawScene ()
{

	// Draw the scene
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normals.data());
	glTexCoordPointer(3, GL_FLOAT, 0, texcoords.data());
	glVertexPointer(3, GL_FLOAT, 0, vertices.data());

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
	//glDrawArrays(GL_TRIANGLES, 0, (vertices.size()));

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}


void Draw_3DSquare()
{

	GLfloat vertices[] = {
		-1.0000,	0.0000,	 1.0000,
		-1.0000,	0.0000,	 -1.0000,
		1.0000,	0.0000,	 -1.0000,
		1.0000,	0.0000,	 1.0000,
		-1.0000,	2.0000,	 1.0000,
		1.0000,	2.0000,	 1.0000,
		1.0000,	2.0000,	 -1.0000,
		-1.0000,	2.0000,	 -1.0000};



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
	 // Clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    // Reset the matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 

	Move_Camera();


		
	//Scengraf

	glPushMatrix();

		//Ballista
		glPushMatrix();	
			//Matrismultiplikationer som rör Ballistan här!
			glColor3f(1.0f, 0.0f, 0.0f);
			glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, 0.0f);
			glScalef(0.2f, 0.2f, 0.2f);
			rotateBallista();
			objBallista.DrawObject();
		
			//Arrow
			glPushMatrix();		
				//Matrismultiplikationer som rör Pilen här!
				glRotatef(90.0f,0.0f,1.0f,0.0f);
				MOVE_ARROW();
				glTranslatef(0.0f, 0.0f, 0.0f);		
				objArrow.DrawObject();			
			glPopMatrix();
		glPopMatrix();
		
		//Borg	
		glPushMatrix();
			//Matrismultiplikationer som rör Borgen här!	
			glColor3f(0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, 0.0f, 50.0f);
			glScalef(5.0f, 5.0f, 5.0f);
			glRotatef(0.0f, 0.0f, 1.0f, 0.0f);		
			objBorg.DrawObject();		
		glPopMatrix();


		//Terräng och "markplan"
		//Matrismultiplikationer som rör Heightmap här!	
		glColor3f(0.0f, 1.0f, 0.0f);
		glRotatef(0.0f, 0.0f, 1.0f, 0.0f);	
		DrawMap();
		drawGround();
	glPopMatrix();
 
    // ----- Stop Drawing Stuff! ------
 

}

