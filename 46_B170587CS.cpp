#include <iostream>
#include "./stl_reader/stl_reader.h"
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <vector>
#include <string>
#include <fstream>

/*
README

Source code to display 3D bunny and perform the following operations:
1. Rotation : Mouse left-click and drag
2. Scale Up and Down: Mouse scroll (or 2 finger movement on trackpad)

The stl_reader library is used to read in stl input.


*/
struct Point
{
  std::vector <GLfloat> coords;
};

struct  Triangle{
  std::vector<struct Point> points;
};

int origin_x;
int origin_y;
int active;
int dx,dy;
float org_x,org_y;
struct Triangle trivec;
struct Point pointvec;
std::vector <struct Triangle> triangles;
std::vector<float> coords, normals;
std::vector<unsigned int> tris, solids;

void draw(){

  for(auto triangle:triangles){

    glBegin(GL_LINE_LOOP);
    for(auto point: triangle.points){

      glVertex3f(point.coords[0],point.coords[1],point.coords[2]);
      
    }

    glEnd();
  }

}

void renderFunction(){
  
  glClearColor(1.0f,1.0f,1.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearDepth(1.0f);
  glColor3f(0,0,1);
  glPointSize(5);
  glEnable(GL_BLEND);

  //Plot Bunny
  draw();
  glFlush();
  
}

void ClickHandler(int button, int state, int x, int y)
{
    //Checking for Left Click
    if(button==GLUT_LEFT && state== GLUT_DOWN && active==0){

        origin_x=x;
        origin_y=y;
        active=1;
      
    }
    else 
        active=0;

    //Checking for Mouse scroll
    if(button==3 || button==4)
    {
        float scale_mag=0.75;
        if(button==4)      
            scale_mag=1+scale_mag;
        else
            scale_mag=1-scale_mag;
        

        glScalef(scale_mag,scale_mag,scale_mag);
        glutPostRedisplay();

    }



}

void MouseMoveHandler(int x,int y)//Mapping mouse movements to rotations
{

    if(active==1)
    {
        int dx=x-origin_x;
        int dy=y-origin_y;

        glRotatef(0.6,dy,dx,0.0);
        glutPostRedisplay();
        
        origin_x=x;
        origin_y=y;
    }

}

void readInput(int *argc, char **argv){ 

  //Read input through stl_reader library

  try {
    stl_reader::ReadStlFile ("input.txt", coords, normals, tris, solids);
    const size_t numTris = tris.size() / 3;
    for(size_t itri = 0; itri < numTris; ++itri) {
        for(size_t icorner = 0; icorner < 3; ++icorner) {
            GLfloat* c = &coords[3 * tris [3 * itri + icorner]];
            pointvec.coords.push_back(c[0]);
            pointvec.coords.push_back(c[1]);
            pointvec.coords.push_back(c[2]);
            //Pushing coordinates to points vector
            trivec.points.push_back(pointvec);
            //Storing points in triangle
            pointvec.coords.clear();
        }
        
        triangles.push_back(trivec);
        //Pusing triangle into triangle vector
        trivec.points.clear();
        
    }
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}



int main(int argc, char **argv){

  glutInit( &argc, argv);
  glutInitDisplayMode(GLUT_DEPTH);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(500, 500);
  readInput(&argc,argv);
  glutCreateWindow("B170587CS-demo1");
  glOrtho(-100.0,100.0,-100.0,100.0,-100.0,100.0);
  glutDisplayFunc(renderFunction);
  glutMouseFunc(ClickHandler);
  glutMotionFunc(MouseMoveHandler);
  glutMainLoop();

  return 0;
}