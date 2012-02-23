#include <Windows.h>

#include <string>
#include <sstream>

#include <stdio.h>
#include <glut.h>
#include <GL/gl.h>

using namespace std;


/* Windows parameters */
int WIDTH = 600;
int HEIGHT = 400;
#define TITLE  "Christoffers Testskit"
int winIdMain;
int winIdSub;


/* Animation State Vriables */
double TIME_STEP   = 0.1;
double time = 0.0;

//Skriver ut en sträng
void drawString (string s)
{
  for (int i = 0; i < s.size(); i++)
    glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, s[i]);
};

//Skriver ut en sträng med större font
void drawStringBig (string s)
{
  for (int i = 0; i < s.size(); i++)
    glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, s[i]);
};


/* Here is a display fucntion that updates
   the main graphic window */
void mainDisplay (void)
{
  glutSetWindow (winIdMain);

  //Set background color
  glClearColor(0.0, 0.3, 0.3, 0.4);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Write Footnote */
  glColor3f (1.0, 0.7, 0.0);
  glRasterPos2f (-0.7, -0.7);
  drawString ("(c)Christoffer Dahl for Mistah Ballista");
};


/* Another display function, this one will be 
used to update the graphic subwindow */
void subDisplay ()
{
	glutSetWindow (winIdSub);

	//Set background color
	glClearColor(1.0, 0.5, 0.0, 0.7);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Konvertera tiden till en sträng
	ostringstream os;
	os << time;
	string str = os.str();

	/* Type Time */
	glRasterPos2f (-1.0 , 0.7);
	drawStringBig ("Time ="+str);
};


/* Callback function for reshaping the main window */
void mainReshape (int w, int h)
{
};


/* Callback function for reshaping the subwindow */
void subReshape (int w, int h)
{
};

/* There can be only one idle() callback function. In an
   animation, this idle() function must update not only the
   main window but also all derived subwindows */
void idle (void)
{
  /* Update  state variables */
  time += TIME_STEP;

  /* Update main and sub window */
  glutSetWindow (winIdMain);
  glutPostRedisplay ();
  glutSetWindow (winIdSub);
  glutPostRedisplay ();
};

int main ()
{
  /* Glut initializations */
  glutInitWindowSize (WIDTH, HEIGHT);

  /* Main window creation and setup */
  winIdMain = glutCreateWindow (TITLE);
  glutDisplayFunc (mainDisplay);
  glutIdleFunc (idle);

  /* Sub window creation and setup */
  winIdSub = glutCreateSubWindow (winIdMain, 0, 0, WIDTH, HEIGHT / 4);
  glutDisplayFunc (subDisplay);

  glutMainLoop ();

  return 0;
};