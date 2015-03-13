
#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "world.h"

#include "surface.h"
#include "vehicle.h"
#include "image.h"


int window;
World world;

float tf=0, rf=0;
void keyPressed(unsigned char key, int x, int y)
{
  switch(key){
  case 27:
    glutDestroyWindow(window);
    exit(0);
  case ' ':
    tf = 0;
    rf = 0;
    break;
  case '+':
    world.zoom *= 1.1f;
    break;
  case '-':
    world.zoom /= 1.1f;
    break;
  case '1':
    world.view_type = LOCAL_OUT_VEHICLE;
    break;
  case '2':
    world.view_type = LOCAL_IN_VEHICLE;
    break;
  case '3':
    world.view_type = GLOBAL;
    break;
  }
}


void specialInput(int key, int x, int y) {
  switch(key){
  case GLUT_KEY_UP:
    tf += 0.1;
    break;
  case GLUT_KEY_DOWN:
    tf -= 0.1;
    break;
  case GLUT_KEY_LEFT:
    rf += 0.1;
    break;
  case GLUT_KEY_RIGHT:
    rf -= 0.1;
    break;
  case GLUT_KEY_PAGE_UP:
    world.camera_z+=0.1;
    break;
  case GLUT_KEY_PAGE_DOWN:
    world.camera_z-=0.1;
    break;
  }
}


void display(void) {
  displayWorld(&world);
}


void reshape(int width, int height) {
  reshapeWorldViewport(&world, width, height);
}

void idle() {
  updateWorld(&world, tf, rf);
  glutPostRedisplay();
  usleep(10000);
  
  // decay the commands
  tf *= 0.999;
  rf *= 0.7;
}

int main(int argc, char **argv) {
  // load the images
  Image* surface_elevation = loadImage(argv[1]);
  Image* surface_texture = loadImage(argv[2]);
  Image* vehicle_texture = loadImage(argv[3]);
  
  // initialize GL
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("main");

  // set the callbacks
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutSpecialFunc(specialInput);
  glutKeyboardFunc(keyPressed);
  glutReshapeFunc(reshape);
  
  // construct the world
  initWorld(&world, surface_elevation, surface_texture, vehicle_texture, 0.5, 0.5, 0.5);
  
  // check out the images not needed anymore
  freeImage(vehicle_texture);
  freeImage(surface_texture);
  freeImage(surface_elevation);

  // run the main GL loop
  glutMainLoop();

  // cleanup
  destroyWorld(&world);
  return 0;             
}
