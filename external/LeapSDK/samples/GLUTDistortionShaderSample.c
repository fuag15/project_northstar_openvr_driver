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

static const char* GLSL_VERT = STRINGIFY(
    \n#version 120\n

    void main()
    {
        gl_Position = ftransform();
        gl_TexCoord[0]=gl_MultiTexCoord0;
    }
);
static const char* GLSL_FRAG = STRINGIFY(
    \n#version 120\n
    uniform sampler2D rawTexture;
    uniform sampler2D distortionTexture;

    void main()
    {
        vec2 distortionIndex = texture2D(distortionTexture, gl_TexCoord[0].st).xy;
        float hIndex = distortionIndex.r;
        float vIndex = distortionIndex.g;

        if(vIndex > 0.0 && vIndex < 1.0 && hIndex > 0.0 && hIndex < 1.0)
        {
            gl_FragColor = vec4(texture2D(rawTexture, distortionIndex).rrr, 1.0);
        }
        else
        {
            gl_FragColor = vec4(0.2, 0.0, 0.0, 1.0);
        }
    }
);

GLsizei image_width;
GLsizei image_height;
void* image_buffer = NULL;
float* distortion_buffer_left = NULL;
float* distortion_buffer_right = NULL;
uint64_t image_size = 0;
bool imageReady = false;
uint64_t currentDistortionId = 0;

int window; // GLUT window handle
int windowWidth = 800;
int windowHeight = 400;

GLuint vertShader, fragShader, program;
GLuint rawTextureLeft = 0;
GLuint rawTextureRight = 0;
GLuint distortionTextureLeft = 0;
GLuint distortionTextureRight = 0;

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
  }

  memcpy(image_buffer, (char*)imageEvent->image[0].data + imageEvent->image[0].offset, image_size);
  memcpy((char*)image_buffer + image_size, (char*)imageEvent->image[1].data + imageEvent->image[1].offset, image_size);

  //Save the distortion data if it's version id changes
  if(currentDistortionId != imageEvent->image[0].matrix_version){
    size_t distortion_size = 64 * 64 * 2;
    distortion_buffer_left = malloc(sizeof(float) * distortion_size);
    distortion_buffer_right = malloc(sizeof(float) * distortion_size);
    memcpy(distortion_buffer_left, imageEvent->image[0].distortion_matrix, sizeof(float) * distortion_size);
    memcpy(distortion_buffer_right, imageEvent->image[1].distortion_matrix, sizeof(float) * distortion_size);
    currentDistortionId = imageEvent->image[0].matrix_version;
  }
  imageReady = true;
}

static void display(void)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPushMatrix();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(imageReady){
    //Setup perspective and view matrices
    glMatrixMode(GL_PROJECTION);
    checkGLError("set matrix mode");
    glLoadIdentity();
    setPerspectiveFrustrum(96, windowWidth/windowHeight, 1, 20);
    checkGLError("set frustrum");

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(1.05f, 0.0, -1.01f);

    glEnable(GL_TEXTURE_2D);
    glUseProgram(program);

    // Left camera image
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rawTextureLeft);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image_width, image_height, 0, GL_RED, GL_UNSIGNED_BYTE, image_buffer);
    checkGLError("Initializing raw texture");

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, distortionTextureLeft);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, 64, 64, 0, GL_RG, GL_FLOAT, distortion_buffer_left);
    checkGLError("Initializing distortion texture");

    glBegin(GL_QUADS); // Draw A Quad for camera image
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f); // Top Left
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f); // Top Right
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, 0.0f); // Bottom Right
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f); // Bottom Left
    glEnd();

    // Right camera image
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rawTextureRight);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, image_width, image_height, 0, GL_RED, GL_UNSIGNED_BYTE,
      (GLvoid*)((size_t)(image_buffer) + (image_width * image_height)));
    checkGLError("Updating raw texture with right image");

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, distortionTextureRight);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, 64, 64, 0, GL_RG, GL_FLOAT, distortion_buffer_right);
    checkGLError("Updating distortion texture with right distortion map");

    glTranslatef(-2.01f, 0.0, 0.0);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f); // Top Left
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f); // Top Right
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f); // Bottom Right
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f); // Bottom Left
    glEnd();

    glPopMatrix();

    imageReady = false;
  }
  glFlush();
  glPopMatrix();
  glutSwapBuffers();
}

static void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
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
  ConnectionCallbacks.on_image        = OnImage;

  LEAP_CONNECTION *connection = OpenConnection();
  LeapSetPolicyFlags(*connection, eLeapPolicyFlag_Images, 0);

  while(!IsConnected){
    millisleep(250);
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(windowWidth, windowHeight);
  window = glutCreateWindow("LeapC Distortion Shader Example");

#if defined(GLEW_VERSION)
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    /* Problem: glewInit failed, something is seriously wrong. */
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    return 1;
  }
#endif

  // GLUT callbacks
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutDisplayFunc(display);

  // init GL
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glColor3f(1.0, 1.0, 1.0);

  // init shader and textures

  //Create the shader program
  program = createProgram(GLSL_VERT, GLSL_FRAG);
  glUseProgram(program);
  GLuint rawSampler = glGetUniformLocation(program, "rawTexture");
  GLuint distortionSampler  = glGetUniformLocation(program, "distortionTexture");
  glUniform1i(rawSampler, 0);
  glUniform1i(distortionSampler,  1);

  //Create textures
  rawTextureLeft = createTextureReference();
  distortionTextureLeft = createTextureReference();
  rawTextureRight = createTextureReference();
  distortionTextureRight = createTextureReference();

  // Start GLUT loop
  glutMainLoop();

  CloseConnection();
  free(distortion_buffer_left);
  free(distortion_buffer_right);
  return 0;
}
//End-of-Sample
