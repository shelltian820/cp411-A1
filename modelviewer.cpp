
//TODO:
/*
ISSUES
-translating z in ortho mode doesn't work?
-translate on object's own axis
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
vector<float> center_point;
static float obj_scale;

static float Xvalue = 0.0, Yvalue = 0.0, Zvalue = -10.0;// values to translate
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // angles to rotate
static float Oleft = -1.0, Oright = 1.0, Obottom = -1.0, Otop = 1.0, Onear = -8.0, Ofar = 100.0; //camera for Ortho mode
static float Pleft = -1.0, Pright = 1.0, Pbottom = -1.0, Ptop = 1.0, Pnear = 9.0, Pfar = 100.0; //camera for Perspective mode

static int view_mode = 0;
//function prototypes
void keyInput(unsigned char key, int x, int y);
void specialKeyInput(int key, int x, int y);
void drawScene();
void resize(int w, int h);
void setup();
void reset();
void translate_obj(float Xvalue, float Yvalue, float Zvalue);
void rotate_obj(float Xangle, float Yangle, float Zangle);

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
  center_point = center(v_vector);
  obj_scale = scale(v_vector);

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
  glutSpecialFunc(specialKeyInput);

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
  glEndList();
  
  
}

void drawScene(){
  glClear (GL_COLOR_BUFFER_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //toggle perspective
  if (view_mode == 0){ //ORTHOGRAPHIC VIEW
    glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
    glOrtho(Oleft, Oright, Obottom, Otop, Onear, Ofar);
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
  }
  if (view_mode == 1){ //PERSECTIVE VIEW
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glFrustum(Pleft, Pright, Pbottom, Ptop, Pnear, Pfar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

  translate_obj(Xvalue, Yvalue, Zvalue);
  rotate_obj(Xangle, Yangle, Zangle);
  
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
      case 'x':
        reset();
        break;
      case 'w':
        write(v_vector,f_vector);
        break;
      case 'v':
        view_mode = 0;
        glutPostRedisplay();
        break;
      case 'V':
        view_mode = 1;
        glutPostRedisplay();
        break;
      case 'n':
        Zvalue -= 0.1;
        glutPostRedisplay();
        break;
      case 'N':
        Zvalue += 0.1;
        glutPostRedisplay();
        break;

      //object rotation
      case 'p':
        Xangle -= 10.0;
        if (Xangle < 0.0) Xangle += 360.0;
        glutPostRedisplay();
        break;
      case 'P':
        Xangle += 10.0;
        if (Xangle > 360.0) Xangle -=360.0;
        glutPostRedisplay();
        break;
      case 'y':
        Yangle -= 10.0;
        if (Yangle < 0.0) Yangle += 360.0;
        glutPostRedisplay();
        break;
      case 'Y':
        Yangle += 10.0;
        if (Yangle > 360.0) Yangle -=360.0;
        glutPostRedisplay();
        break;
      case 'r':
        Zangle -= 10.0;
        if (Zangle < 0.0) Zangle += 360.0;
        glutPostRedisplay();
        break;
      case 'R':
        Zangle += 10.0;
        if (Zangle > 360.0) Zangle -=360.0;
        glutPostRedisplay();
        break;

      //camera translation
      case 'd':
        Oleft -= 0.1; Pleft -= 0.1;
        Oright -= 0.1; Pright -= 0.1;
        glutPostRedisplay();
        break;
      case 'D':
        Oleft += 0.1; Pleft += 0.1;
        Oright += 0.1; Pright += 0.1;
        glutPostRedisplay();
        break;
      case 'c':
        Obottom -= 0.1; Otop -= 0.1;
        Pbottom -= 0.1; Ptop -= 0.1;
        glutPostRedisplay();
        break;
      case 'C':
        Obottom += 0.1; Otop += 0.1;
        Pbottom += 0.1; Ptop += 0.1;
        glutPostRedisplay();
        break;
      case 'z':
        Onear -= 0.1; Ofar -= 0.1;
        Pnear -= 0.1; Pnear -= 0.1;
        glutPostRedisplay();
        break;
      case 'Z':
        Onear += 0.1; Ofar += 0.1;
        Pnear += 0.1; Pnear += 0.1;
        glutPostRedisplay();
        break;


      default:
         break;
   }
}

void specialKeyInput(int key, int x, int y){
  switch(key){
    case GLUT_KEY_UP:
      Yvalue += 0.1;
      break;
    case GLUT_KEY_DOWN:
      Yvalue -= 0.1;
      break;
    case GLUT_KEY_LEFT:
      Xvalue -= 0.1;
      break;
    case GLUT_KEY_RIGHT:
      Xvalue += 0.1;
      break;
    default:
      break;
  }
   glutPostRedisplay();
}

void resize(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
}

void reset(){
  Xvalue = 0.0, Yvalue = 0.0, Zvalue = -10.0;
  Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;
  Oleft = -1.0, Oright = 1.0, Obottom = -1.0, Otop = 1.0, Onear = -8.0, Ofar = 100.0;
  Pleft = -1.0, Pright = 1.0, Pbottom = -1.0, Ptop = 1.0, Pnear = 9.0, Pfar = 100.0;
  glutPostRedisplay();
}

void translate_obj(float Xvalue, float Yvalue, float Zvalue){
  //textbook fig4.8
  glTranslatef(Xvalue, Yvalue, Zvalue);
  //glRotatef(Zangle, 0.0, 0.0, 1.0);
  //glRotatef(Yangle, 0.0, 1.0, 0.0);
  //glRotatef(Xangle, 1.0, 0.0, 0.0);
}



void rotate_obj(float Xangle, float Yangle, float Zangle){
  glRotatef(Zangle, 0.0, 0.0, 1.0);
  glRotatef(Yangle, 0.0, 1.0, 0.0);
  glRotatef(Xangle, 1.0, 0.0, 0.0);
}
