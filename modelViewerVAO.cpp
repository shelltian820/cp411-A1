
//figure out VBO

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <regex>
#include <vector>
#include <iomanip>
#include <cmath>
#include "readwrite.h"
#include "transform.h"

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
//static unsigned int anObject; //for display list
static unsigned int buffer[2]; //for VBO
static unsigned int vao[1]; //for VAO
vector<vector<float>> v_vector;
vector<vector<int>> f_vector;
float *vertices_pointer; int *faces_pointer;

static float Xvalue = 0.0, Yvalue = 0.0, Zvalue = -10.0;// total translation values
static float tx = 0.0, ty = 0.0, tz = 0.0; //current translation (tx or ty or tz) = ( -1.0 or 1.0);
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // angles to rotate object (degrees)
float eyeX = 0.0, eyeY = 0.0, eyeZ = 0.0, centerX = 0.0, centerY = 0.0, centerZ = -10.0, upX = 0.0, upY = 0.1, upZ = 0.0;
static float Oleft = -1.0, Oright = 1.0, Obottom = -1.0, Otop = 1.0, Onear = -8.0, Ofar = 100.0; //camera for Ortho mode
static float Pleft = -1.0, Pright = 1.0, Pbottom = -1.0, Ptop = 1.0, Pnear = 8.0, Pfar = 100.0; //camera for Perspective mode
static int view_mode = 0;
static int fog_mode = 0;


//function prototypes
void keyInput(unsigned char key, int x, int y);
void specialKeyInput(int key, int x, int y);
void drawScene();
void resize(int w, int h);
void setup();
void reset();
void translate_obj();
void rotate_obj();
void rotate_camera();
void tilt_camera(int direction);
void pan_camera(int direction);
void roll_camera(int direction);
float deg_to_rad(float degree);


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
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
  center(v_vector);
  scale(v_vector);

  //set up OpenGL
  glutInit(&argc, argv);
  glutInitContextVersion(4, 3);
  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
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
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////




void drawScene(){
  glClear (GL_COLOR_BUFFER_BIT);

  // float* bufferData = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
  // glUnmapBuffer(GL_ARRAY_BUFFER);
  // glDrawElements(GL_TRIANGLES, f_vector.size(), GL_UNSIGNED_INT, 0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
  float fogColor[4] = {0.0, 0.0, 0.0, 1.0};
  float fogStart = 10.0;
  float fogEnd = 11.0;
  if (fog_mode) glEnable(GL_FOG); else glDisable(GL_FOG);
  glFogfv(GL_FOG_COLOR, fogColor);
  glFogf(GL_FOG_MODE, GL_LINEAR);
  glFogf(GL_FOG_START, fogStart);
  glFogf(GL_FOG_END, fogEnd);


  //toggle perspective
  if (view_mode == 0){ //ORTHOGRAPHIC VIEW
    glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
    glOrtho(Oleft, Oright, Obottom, Otop, Onear, Ofar);
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
  }
  if (view_mode == 1){ //PERSECTIVE VIEW
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glFrustum(Pleft, Pright, Pbottom, Ptop, Pnear, Pfar);
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

  //////////////////
  rotate_obj();
  translate_obj();
  //////////////////

  glColor3f(1.0, 1.0, 1.0); //make object white

  glBindVertexArray(vao[0]);
  glDrawElements(GL_TRIANGLE_STRIP, f_vector.size(), GL_UNSIGNED_INT, faces_pointer);
  //glDrawArrays(GL_TRIANGLES, 0, 3);
  glutSwapBuffers();
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

void tilt_camera(int direction){
  float theta = deg_to_rad(1);
  if (direction == 0){
    centerY = cos(theta)*centerY - sin(theta)*centerZ;
    centerZ = sin(theta)*centerY + cos(theta)*centerZ;
  }
  else{
    centerY = cos(theta)*centerY + sin(theta)*centerZ;
    centerZ = -1*sin(theta)*centerY + cos(theta)*centerZ;
  }
  //cout << centerX << " " << centerY << " " << centerZ << endl ;
}

void pan_camera(int direction){
  float theta = deg_to_rad(1);
  if (direction == 0){
    centerX = cos(theta)*centerX + sin(theta)*centerZ;
    centerZ = -1*sin(theta)*centerX + cos(theta)*centerZ;
  }
  else {
    centerX = cos(theta)*centerX - sin(theta)*centerZ;
    centerZ = sin(theta)*centerX + cos(theta)*centerZ;
  }
  // cout << centerX << " " << centerY << " " << centerZ << endl ;
}

void roll_camera(int direction){
  float theta = deg_to_rad(10);
  if (direction == 0){
    upX = cos(theta)*upX - sin(theta)*upY;
    upY = sin(theta)*upX + cos(theta)*upY;
  }
  else{
    upX = cos(theta)*upX + sin(theta)*upY;
    upY = -1*sin(theta)*upX + cos(theta)*upY;
  }
}

float deg_to_rad(float degree){
  float radian = 3.141592/180.0 * degree;
  return radian;
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
      case 'f':
        fog_mode = 0;
        break;
      case 'F':
        fog_mode = 1;
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
        //eyeX -= 0.1;
        break;
      case 'D':
        Oleft += 0.1; Pleft += 0.1;
        Oright += 0.1; Pright += 0.1;
        //eyeX += 0.1;
        break;
      case 'c':
        Obottom -= 0.1; Otop -= 0.1;
        Pbottom -= 0.1; Ptop -= 0.1;
        //eyeY -= 0.1;
        break;
      case 'C':
        Obottom += 0.1; Otop += 0.1;
        Pbottom += 0.1; Ptop += 0.1;
        // eyeY += 0.1;
        break;
      case 'z':
        Onear -= 0.1; Ofar -= 0.1;
        Pnear -= 0.1; Pnear -= 0.1;
        // eyeZ -= 0.1;
        break;
      case 'Z':
        Onear += 0.1; Ofar += 0.1;
        Pnear += 0.1; Pnear += 0.1;
        // eyeZ += 0.1;
        break;

      //camera rotation
      case 't':
        tilt_camera(1);
        break;
      case 'T':
        tilt_camera(0);
        break;
      case 'a':
        pan_camera(1);
        break;
      case 'A':
        pan_camera(0);
        break;
      case 'l':
        roll_camera(1);
        break;
      case 'L':
        roll_camera(0);
        break;


      default:
         break;
   }
   glutPostRedisplay();
}

void specialKeyInput(int key, int x, int y){
  switch(key){
    case GLUT_KEY_UP:
      tx = 0.0; ty = 0.1; tz = 0.0;
      break;
    case GLUT_KEY_DOWN:
      tx = 0.0; ty = -0.1; tz = 0.0;
      break;
    case GLUT_KEY_LEFT:
      tx = 0.1; ty = 0.0; tz = 0.0;
      break;
    case GLUT_KEY_RIGHT:
      tx = -0.1; ty = 0.0; tz = 0.0;
      break;
    default:
      break;
  }
   glutPostRedisplay();
}



void reset(){
  Xvalue = 0.0, Yvalue = 0.0, Zvalue = -10.0;
  tx = 0.0, ty = 0.0, tz=0.0;
  Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;
  //camXangle = 0.0, camYangle = 0.0, camZangle = 0.0;
  eyeX = 0.0, eyeY = 0.0, eyeZ = 0.0, centerX = 0.0, centerY = 0.0, centerZ = -10.0, upX = 0.0, upY = 1.0, upZ = 0.0;
  Oleft = -1.0, Oright = 1.0, Obottom = -1.0, Otop = 1.0, Onear = -8.0, Ofar = 100.0;
  Pleft = -1.0, Pright = 1.0, Pbottom = -1.0, Ptop = 1.0, Pnear = 9.0, Pfar = 100.0;
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glutPostRedisplay();
}



void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
  glewExperimental = GL_TRUE;
  glewInit();

  float vertices[v_vector.size()*3];
  for(int i=0; i < v_vector.size(); i++){
    vector<float> p = v_vector[i];
    vertices[i*3] = p[0];
    vertices[i*3+1] = p[1];
    vertices[i*3+2] = p[2];
  }
  vertices_pointer = vertices;
  int faces[f_vector.size()*3];
  for(int i=0; i < f_vector.size(); i++){
    vector<int> f = f_vector[i];
    faces[i*3] = f[0];
    faces[i*3+1] = f[1];
    faces[i*3+2] = f[2];
  }
  faces_pointer = faces;
  glGenVertexArrays(1, vao);
  glBindVertexArray(vao[0]);

  glGenBuffers(2, buffer);
  glEnableClientState(GL_VERTEX_ARRAY);

  glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices_pointer, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces_pointer, GL_STATIC_DRAW);

  glVertexPointer(3, GL_FLOAT, 0, vertices_pointer);
}



void resize(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

}
