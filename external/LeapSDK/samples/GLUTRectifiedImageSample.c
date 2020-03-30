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
#include "LeapC.h"
#include "ExampleConnection.h"
#include "GLutils.h"
#include <math.h>
#include <string.h>

//The pixel size of the textures we write the undistorted images to
#define TEX_WIDTH 400
#define TEX_HEIGHT 400
//Max field of view varies by device, use 8 for the peripheral, 22 for Rigel. I.e. tan(FOV/2)
#define MAX_FOV 22

LEAP_CONNECTION *connection = NULL;

float image_width;
float image_height;
uint64_t image_size = 0;
unsigned char* image_buffer = NULL;
unsigned char* undistorted_image_left = NULL;
unsigned char* undistorted_image_right = NULL;
bool imageReady = false;
bool textureChanged = false;
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

//display the textured quad
static void display(void)
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(-32, -24, -50); //"Camera" viewpoint
  glClear(GL_COLOR_BUFFER_BIT);
  if(imageReady){
    //Left image
    for( float row = 0; row < TEX_HEIGHT; row++ ) {
      for( float col = 0; col < TEX_WIDTH; col++ ) {

            //Normalize from pixel xy to range [0..1]
            LEAP_VECTOR input;
            input.x = col/TEX_WIDTH;
            input.y = row/TEX_HEIGHT;

            //Convert from normalized [0..1] to ray slopes
            input.x = (input.x - .5) * MAX_FOV;
            input.y = (input.y - .5) * MAX_FOV;

            LEAP_VECTOR pixel = LeapRectilinearToPixel(*connection,
                                                       eLeapPerspectiveType_stereo_left,
                                                       input);
            int dindex = (int)floor(row * TEX_WIDTH + col);
            int pindex = (int)roundf(pixel.y) * image_width + (int)roundf(pixel.x);
            if(pixel.x >= 0 && pixel.x < image_width && pixel.y >=0 && pixel.y < image_height){
              undistorted_image_left[dindex] = ((char*)image_buffer)[pindex];
            } else {
              undistorted_image_left[dindex] = 128;
            }
        }
    }
    // right image
    for( float row = 0; row < TEX_HEIGHT; row++ ) {
        for( float col = 0; col < TEX_WIDTH; col++ ) {

            //Normalize from pixel xy to range [0..1]
            LEAP_VECTOR input;
            input.x = col/TEX_WIDTH;
            input.y = row/TEX_HEIGHT;

            //Convert from normalized [0..1] to ray slopes
            input.x = (input.x - .5) * MAX_FOV;
            input.y = (input.y - .5) * MAX_FOV;

            LEAP_VECTOR pixel = LeapRectilinearToPixel(*connection,
                                                       eLeapPerspectiveType_stereo_right,
                                                       input);
            int dindex = (int)floor(row * TEX_WIDTH + col);
            int pindex = (int)roundf(pixel.y + image_height) * image_width + (int)roundf(pixel.x);

            if(pixel.x >= 0 && pixel.x < image_width && pixel.y >=0 && pixel.y < image_height){
              undistorted_image_right[dindex] = ((char*)image_buffer)[pindex];
            } else {
              undistorted_image_right[dindex] = 200;
            }
        }
    }

    DrawImageQuad(0, 0, 64, 24, TEX_WIDTH, TEX_HEIGHT, undistorted_image_left);
    DrawImageQuad(0, 26, 64, 50, TEX_WIDTH, TEX_HEIGHT, undistorted_image_right);
    imageReady = false;
  }
  glFlush();
  glPopMatrix();
  glutSwapBuffers();
}
//end of display

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

  connection = OpenConnection();
  LeapSetPolicyFlags(*connection, eLeapPolicyFlag_Images, 0);

  undistorted_image_left = malloc(TEX_WIDTH * TEX_HEIGHT);
  undistorted_image_right = malloc(TEX_WIDTH * TEX_HEIGHT);
  while(!IsConnected){
    millisleep(250);
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(640, 480);
  window = glutCreateWindow("LeapC Undistorted Image Example");

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
  free(undistorted_image_left);
  free(undistorted_image_right);
  return 0;
}
//End-of-Sample
