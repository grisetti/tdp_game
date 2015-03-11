
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

void keyPressed(unsigned char key, int x, int y)
{
  switch(key){
  case 27:
    glutDestroyWindow(window);
    exit(0);
  case ' ':
    world.translational_force *= 0.1;
    world.rotational_force *= 0.1;
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
    world.translational_force += world.translational_force_increment;
    if(world.translational_force > world.max_translational_force)
      world.translational_force = world.max_translational_force;
    break;
  case GLUT_KEY_DOWN:
    world.translational_force -= world.translational_force_increment;
    if(world.translational_force < -world.max_translational_force)
      world.translational_force = -world.max_translational_force;
    break;
  case GLUT_KEY_LEFT:
    world.rotational_force += world.rotational_force_increment;
    if(world.rotational_force > world.max_rotational_force)
      world.rotational_force = world.max_rotational_force;
    break;
   case GLUT_KEY_RIGHT:
    world.rotational_force -= world.rotational_force_increment;
    if(world.rotational_force < -world.max_rotational_force)
      world.rotational_force = -world.max_rotational_force;
    break;
  case GLUT_KEY_PAGE_UP:
    printf("camera_z: %lf\n", world.camera_z);
    world.camera_z+=0.1;
    break;
  case GLUT_KEY_PAGE_DOWN:
    world.camera_z-=0.1;
    printf("camera_z: %lf\n", world.camera_z);
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
  updateWorld(&world);
  glutPostRedisplay();
  usleep(10000);
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
