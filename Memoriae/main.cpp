//
//  main.cpp
//  Memoriae
//
//  Created by Luis Alberto Lamadrid on 3/15/16.
//  Copyright © 2016 Luis Alberto Lamadrid. All rights reserved.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>//glut.h includes gl.h and glu.h
#endif

//http://www.lonesock.net/soil.html
#include "SOIL/SOIL.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <math.h>
#include "Memoriae.hpp"

using namespace std;

#pragma mark - Global Variables
/* --------------------------- Global Variables ----------------------------- */
// Screen and Frustum values
int screenWidth = 800, screenHeight = 600;
const float X_MAX = 8.0, X_MIN = -8.0;
const float Y_MAX = 6.0, Y_MIN = -6.0;
string fullPath = __FILE__;
GLuint texture;

int seconds = 0;
bool selected[7][7];

// Game object
Memoriae game;

#pragma mark - Functions
/* ------------------------------- Functions -------------------------------- */
void timer(int value) {
	glutPostRedisplay();
    seconds++;
	glutTimerFunc(1000, timer, 0);
}

void reshape(int w, int h) {
	screenWidth = w;
	screenHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(X_MIN, X_MAX, Y_MIN, Y_MAX, 0.75, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 1.1, 0, 0, 0, 0, 1, 0);
}

void paintSpheres(int spheresPerRow, int spheresPerColumn, float maxWidth) {
	int maxSpheres = fmax(spheresPerRow, spheresPerColumn);
	float radius = maxWidth/maxSpheres;
    
    // Cuando se desconocen los vertices del objeto
    // Selecciona la textura
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	
	glPushMatrix();

    // Rotate board to match solution...
    glRotated(180, 0, 0, 0);
    // center all spheres
	glTranslatef(-maxWidth*((float)spheresPerRow/maxSpheres),
                 -maxWidth*((float)spheresPerColumn/maxSpheres),
                 0);
    // scale spheres on Z so they don't distort
	glScalef(1, 1, 0.1);
    
	for (int i=0; i < spheresPerColumn; i++) {
		glPushMatrix();
		glTranslatef(radius, i*(2*radius), 0);//starting position for each row
		for (int j=0; j < spheresPerRow; j++) {
            glPushMatrix();
            glTranslatef(j*(2*radius), radius, 0);// starting position for each column
            glScalef(0.7, 0.7, 0.7);              // make spheres smaller so they don't touch
            
            // paint the required solution
            if (game.isSet(i, j) && seconds >= 2 && seconds <= 4) {
                glColor3f(0, 0, 0);
            } else {
                glColor3f(1, 1, 1);
            }
            
            glutSolidSphere(radius, 30, 30);
            glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
	
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	
    int spheres = game.getActualSize();
	paintSpheres(spheres, spheres, fmin(X_MAX, Y_MAX));
	
	glutSwapBuffers();
}

void mouseMoved(int x, int y){
    // printf("Moved -> X: %d, Y: %d\n", x, y);
}

void init() {
	glClearColor(0.2039, 0.6588, 0.3254, 1.0);
	glColor3f(0, 0, 0);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	
    // remove "main.cpp" from path
	for (int i = (int)fullPath.length()-1; i>=0 && fullPath[i] != '/'; i--) {
		fullPath.erase(i,1);
	}
	
    // Load texture
	char  path[200];
	sprintf(path,"%s%s", fullPath.c_str() , "img_test.png");
	texture = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Memoriae");
    
    glutPassiveMotionFunc(mouseMoved);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(1000, timer, 0);
    
	init();
	glutMainLoop();
	
	return 0;
}