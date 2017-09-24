
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

  //center & scale object
  center_and_scale(v_vector);


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
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  anObject = glGenLists(1);
  glNewList(anObject, GL_COMPILE);
  //add triangles
  glBegin(GL_TRIANGLES);
    for(int i=0; i < f_vector.size(); i++){
      vector<float> p1 = v_vector[f_vector[i][0]-1];
      vector<float> p2 = v_vector[f_vector[i][1]-1];
      vector<float> p3 = v_vector[f_vector[i][2]-1];
      glVertex3f(p1[0], p1[1], p1[2]);
      glVertex3f(p2[0], p2[1], p2[2]);
      glVertex3f(p3[0], p3[1], p3[2]);
    }
  glEnd();


  // for(int i=0; i < f_vector.size(); i++){
  //   glBegin(GL_TRIANGLES);
  //       vector<float> p1 = v_vector[f_vector[i][0]-1];
  //       vector<float> p2 = v_vector[f_vector[i][1]-1];
  //       vector<float> p3 = v_vector[f_vector[i][2]-1];
  //       glVertex3f(p1[0], p1[1], p1[2]);
  //       glVertex3f(p2[0], p2[1], p2[2]);
  //       glVertex3f(p3[0], p3[1], p3[2]);
  //   glEnd();
  // }

  glEndList();

  glColor3f(1.0, 1.0, 1.0); //make object white
  glPushMatrix();
  glCallList(anObject);
  glPopMatrix();

  glFlush();

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
	//glOrtho(-5.0, 5.0, -5.0, 5.0, -8.0, 100.0);
  glOrtho(-1.0, 1.0, -1.0, 1.0, -8.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
