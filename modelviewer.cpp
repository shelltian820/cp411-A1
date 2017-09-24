
//TODO:
/*
- add verties to display list
- change all faces to triangles, ...or use gl triangle fan?
- make v_vector and f_vector global :(
-convert all shapes to triangles
*/


#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <regex>
#include <vector>
#include <iomanip>
#include <cmath>
#include "readwrite.cpp"
#include "transform.cpp"

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif

using namespace std;

//Globals
static unsigned int anObject; //for display list
vector<vector<float>> v_vector;
vector<vector<int>> f_vector;
//function prototypes
void keyInput(unsigned char key, int x, int y);
void drawScene();
void resize(int w, int h);
void setup();

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int main(int argc, char *argv[]){
  //check file format
  if (argc != 2){
    cout << "Invalid. Please input one file.\n";
    exit(1);
  }
  string input = argv[1];
  string obj = ".obj";
  string input_end = input.substr(input.size()-4);
  if ( input_end.compare(obj) ){
    cout << "Invalid. Please input \".obj\" file.\n";
    exit(1);
  }

  //read file
  read(input, v_vector, f_vector);

  //set up OpenGL
  glutInit(&argc, argv);
  glutInitContextVersion(4, 3);
  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Object");

  glutDisplayFunc(drawScene);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyInput);

  setup();

  glewExperimental = GL_TRUE;
  glewInit();
  glutMainLoop();



  return 0;
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////




void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

  //setup camera
}

void drawScene(){
  glClear (GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0); //make object white
  anObject = glGenLists(1);
  glNewList(anObject, GL_COMPILE);



}

void keyInput(unsigned char key, int x, int y){
   switch(key)
   {
      case 'q':
        exit(0);
        break;
      case 'w':
        write(v_vector,f_vector);
        break;
      default:
         break;
   }
}

void resize(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-100.0, 100.0, -100.0, 100.0, -1.0, 1.0);
  glOrtho(-1.0, 1.0, -1.0, 1.0, -8, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
