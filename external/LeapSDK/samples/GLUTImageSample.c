/******************************************************************************\
* Copyright (C) 2012-2017 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#undef __cplusplus

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <time.h>
#include <string.h>
#include "LeapC.h"
#include "ExampleConnection.h"
#include "GLutils.h"

void* image_buffer = NULL;
uint64_t image_size = 0;
bool imageReady = false;
bool textureChanged = false;
uint32_t image_width = 0;
uint32_t image_height = 0;
GLuint texture = 0;
int window; // GLUT window handle

/** Callback for when an image is available. */
static void OnImage(const LEAP_IMAGE_EVENT *imageEvent){
  const LEAP_IMAGE_PROPERTIES* properties = &imageEvent->image[0].properties;
  if (properties->bpp != 1)
    return;

  if (properties->width*properties->height != image_size) {
    void* prev_image_buffer = image_buffer;
    image_width = properties->width;
    image_height = properties->height;
    image_size = image_width*image_height;
    image_buffer = malloc(2 * image_size);
    if (prev_image_buffer)
      free(prev_image_buffer);
    textureChanged = true;
  }

  memcpy(image_buffer, (char*)imageEvent->image[0].data + imageEvent->image[0].offset, image_size);
  memcpy((char*)image_buffer + image_size, (char*)imageEvent->image[1].data + imageEvent->image[1].offset, image_size);
  imageReady = true;
}

// Draw a textured quad displaying the image data
static void DrawImageQuad(float p1X, float p1Y, float p2X, float p2Y, int width, int height, void* imagedata){
  glEnable(GL_TEXTURE_2D);
  if(textureChanged){
    textureChanged = false;
    glDeleteTextures(1, &texture);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imagedata);
    checkGLError("Initializing texture.");
  } else { //update existing texture
    glBindTexture ( GL_TEXTURE_2D, texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, imagedata);
    checkGLError("Updating texture.");
  }
  //Draw a texture-mapped quad
  glBegin(GL_QUADS);
    glTexCoord2f(1, 1); glVertex2f((GLfloat)p1X, (GLfloat)p1Y);
    glTexCoord2f(0, 1); glVertex2f((GLfloat)p2X, (GLfloat)p1Y);
    glTexCoord2f(0, 0); glVertex2f((GLfloat)p2X, (GLfloat)p2Y);
    glTexCoord2f(1, 0); glVertex2f((GLfloat)p1X, (GLfloat)p2Y);
  glEnd();
  checkGLError("Drawing quad.");
  glDisable(GL_TEXTURE_2D);
}
// Done drawing quad

static void display(void)
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(-32, -24, -50); //"Camera" viewpoint
  glClear(GL_COLOR_BUFFER_BIT);
  if(imageReady){
    DrawImageQuad(0, 0, 64, 48, image_width, image_height * 2, image_buffer);
    imageReady = false;
  }
  glFlush();
  glPopMatrix();
  glutSwapBuffers();
}

static void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 640/240, 1.0, 1000);
}

static void keyboard(unsigned char key, int x, int y)
{
  switch((char)key) {
  case 'q':
  case 27:  // ESC
    glutDestroyWindow(window);
    CloseConnection();
    if(image_buffer) free(image_buffer);
    exit(0);
  default:
    break;
  }
}

static void idle(void){
  if(imageReady)
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
  ConnectionCallbacks.on_image = OnImage;

  LEAP_CONNECTION *connection = OpenConnection();
  LeapSetPolicyFlags(*connection, eLeapPolicyFlag_Images, 0);

  while(!IsConnected){
    millisleep(250);
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(640, 480);
  window = glutCreateWindow("LeapC Image Example");

  // GLUT callbacks
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutDisplayFunc(display);

  // init GL
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glColor3f(1.0, 1.0, 1.0);

  // Start GLUT loop
  glutMainLoop();

  CloseConnection();
  return 0;
}
//End-of-Sample
