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
static float tx = 0.0, ty = 0.0, tz = 0.0;
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // angles to rotate
static float Oleft = -1.0, Oright = 1.0, Obottom = -1.0, Otop = 1.0, Onear = -8.0, Ofar = 100.0; //camera for Ortho mode
static float Pleft = -1.0, Pright = 1.0, Pbottom = -1.0, Ptop = 1.0, Pnear = 8.0, Pfar = 100.0; //camera for Perspective mode
static int view_mode = 0;

GLfloat rotation_maatrix[16];
GLfloat translation_matrix[16];



//function prototypes
void keyInput(unsigned char key, int x, int y);
void specialKeyInput(int key, int x, int y);
void drawScene();
void resize(int w, int h);
void setup();
void reset();
void translate_obj();
void rotate_obj();

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

  setup(); //called once

  glutDisplayFunc(drawScene); //called every time something changes
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyInput);
  glutSpecialFunc(specialKeyInput);

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

  //////////////////////////////////////////
  //////////////////////////////////////////
  //translate then rotate is better
  //translate_obj();
  rotate_obj();
  translate_obj();
  //////////////////////////////////////////
  //////////////////////////////////////////


  glColor3f(1.0, 1.0, 1.0); //make object white
  glPushMatrix();
  glCallList(anObject);
  glPopMatrix();

  glFlush();

}

void translate_obj(){

  GLfloat matrixf[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, matrixf);

  //rotate current translation (e.g. +0.1 in x-axis) then add to total X,Y,Z values
  Xvalue += matrixf[0]*tx + matrixf[4]*ty + matrixf[8]*tz;
  Yvalue += matrixf[1]*tx + matrixf[5]*ty + matrixf[9]*tz;
  Zvalue += matrixf[2]*tx + matrixf[6]*ty + matrixf[10]*tz;
  matrixf[12] = Xvalue;
  matrixf[13] = Yvalue;
  matrixf[14] = Zvalue;
  tx = 0.0, ty = 0.0, tz=0.0; //reset movements to zero so the next move is unaffected

  glLoadMatrixf(matrixf);

  //for debugging
  // cout << "translate: ";
  // for (float f: matrixf) cout << f << " ";
  // cout << endl;

}

void rotate_obj(){
  //must rotate around local axis
  glRotatef(Zangle, 0.0, 0.0, 1.0);
  glRotatef(Yangle, 0.0, 1.0, 0.0);
  glRotatef(Xangle, 1.0, 0.0, 0.0);

  //for debugging
  //cout << "Xangle: " << Xangle << "   Yangle: " << Yangle << "   Zangle: " << Zangle << endl;
  // GLfloat matrixf[16];
  // glGetFloatv(GL_MODELVIEW_MATRIX, matrixf);
  // cout << "rotate: ";
  // for (float f: matrixf) cout << f << " ";
  // cout << endl;
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

      //view mode
      case 'v':
        view_mode = 0;
        break;
      case 'V':
        view_mode = 1;
        break;

      //object translation
      case 'n':
        //Zvalue -= 0.1;
        tx = 0.0; ty = 0.0; tz = -0.1;
        break;
      case 'N':
        //Zvalue += 0.1;
        tx = 0.0; ty = 0.0; tz = 0.1;
        break;

      //object rotation
      case 'p':
        Xangle -= 10.0;
        if (Xangle < 0.0) Xangle += 360.0;
        break;
      case 'P':
        Xangle += 10.0;
        if (Xangle > 360.0) Xangle -=360.0;
        break;
      case 'y':
        Yangle -= 10.0;
        if (Yangle < 0.0) Yangle += 360.0;
        break;
      case 'Y':
        Yangle += 10.0;
        if (Yangle > 360.0) Yangle -=360.0;
        break;
      case 'r':
        Zangle -= 10.0;
        if (Zangle < 0.0) Zangle += 360.0;
        break;
      case 'R':
        Zangle += 10.0;
        if (Zangle > 360.0) Zangle -=360.0;
        break;

      //camera translation
      case 'd':
        Oleft -= 0.1; Pleft -= 0.1;
        Oright -= 0.1; Pright -= 0.1;
        break;
      case 'D':
        Oleft += 0.1; Pleft += 0.1;
        Oright += 0.1; Pright += 0.1;
        break;
      case 'c':
        Obottom -= 0.1; Otop -= 0.1;
        Pbottom -= 0.1; Ptop -= 0.1;
        break;
      case 'C':
        Obottom += 0.1; Otop += 0.1;
        Pbottom += 0.1; Ptop += 0.1;
        break;
      case 'z':
        Onear -= 0.1; Ofar -= 0.1;
        Pnear -= 0.1; Pnear -= 0.1;
        break;
      case 'Z':
        Onear += 0.1; Ofar += 0.1;
        Pnear += 0.1; Pnear += 0.1;
        break;

      default:
         break;
   }
   glutPostRedisplay();
}

void specialKeyInput(int key, int x, int y){
  switch(key){
    case GLUT_KEY_UP:
      //Yvalue += 0.1;
      tx = 0.0; ty = 0.1; tz = 0.0;
      break;
    case GLUT_KEY_DOWN:
      //Yvalue -= 0.1;
      tx = 0.0; ty = -0.1; tz = 0.0;
      break;
    case GLUT_KEY_LEFT:
      //Xvalue -= 0.1;
      tx = 0.1; ty = 0.0; tz = 0.0;
      break;
    case GLUT_KEY_RIGHT:
      //Xvalue += 0.1;
      tx = -0.1; ty = 0.0; tz = 0.0;
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
  tx = 0.0, ty = 0.0, tz=0.0;
  Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;
  Oleft = -1.0, Oright = 1.0, Obottom = -1.0, Otop = 1.0, Onear = -8.0, Ofar = 100.0;
  Pleft = -1.0, Pright = 1.0, Pbottom = -1.0, Ptop = 1.0, Pnear = 9.0, Pfar = 100.0;
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glutPostRedisplay();
}
