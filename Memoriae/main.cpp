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

#pragma mark - Textures
/* ------------------------------- Textures --------------------------------- */
const int NUM_TEXTURES = 2;
const int SPHERE_DEFAULT = 0;
const int SPHERE_SELECTED = 1;
GLuint textures[NUM_TEXTURES];

#pragma mark - Global Variables
/* --------------------------- Global Variables ----------------------------- */
// Screen and Frustum values
int screenWidth = 800, screenHeight = 600;
const float X_MAX = 8.0, X_MIN = -8.0;
const float Y_MAX = 6.0, Y_MIN = -6.0;
string fullPath = __FILE__;

// Game logic helpers
int seconds = 0;
bool selected[10][10] = {0};

// Game view matrix sizes (pixels)
float sliceX = screenWidth/4.0f, sliceY = screenHeight/6.0f;
float initialX = sliceX, finalX = sliceX * 3;
float initialY = sliceY, finalY = sliceY * 5;

// Game object
Memoriae game;

#pragma mark - Functions
/* ------------------------------- Functions -------------------------------- */
void timer(int value) {
	glutPostRedisplay();
	seconds++;
	glutTimerFunc(1000, timer, 0);
}

void resetWindowVars(int w, int h){
    screenWidth = w;
    screenHeight = h;
    sliceX = screenWidth/4.0f, sliceY = screenHeight/6.0f;
    initialX = sliceX, finalX = sliceX * 3;
    initialY = sliceY, finalY = sliceY * 5;
}

void reshape(int w, int h) {
    resetWindowVars(w, h);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(X_MIN, X_MAX, Y_MIN, Y_MAX, 0.75, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 1.1, 0, 0, 0, 0, 1, 0);
}

void translateClickToCoordinates(int &x, int &y){
    float ball_num = game.getActualSize();
    float relativeSizeX = finalX - initialX;
    float relativeSizeY = finalY - initialY;
    float relativeSliceX = relativeSizeX / ball_num;
    float relativeSliceY = relativeSizeY / ball_num;
    x -= initialX; x /= relativeSliceX;
    y -= initialY; y /= relativeSliceY;
}

void paintSpheres(int spheresPerRow, int spheresPerColumn, float maxWidth) {
	int maxSpheres = fmax(spheresPerRow, spheresPerColumn);
	float radius = maxWidth/maxSpheres;

	// When the object's vertices are unknown
	// Select texture
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
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
		glTranslatef(radius, i*(2*radius), 0);		// starting position for each row
		for (int j=0; j < spheresPerRow; j++) {
			
			// select texture to paint the required solution
			if (game.isSet(i, j) && seconds >= 2 && seconds <= 4) {
				glBindTexture(GL_TEXTURE_2D, textures[SPHERE_SELECTED]);
			} else {
				glBindTexture(GL_TEXTURE_2D, textures[SPHERE_DEFAULT]);
			}
			
			glPushMatrix();
			glTranslatef(j*(2*radius), radius, 0);	// starting position for each column
			glScalef(0.7, 0.7, 0.7);				// make spheres smaller so they don't touch

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

void mouseClicked(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        translateClickToCoordinates(x, y);
        printf("Clicked -> Row: %d, Col: %d\n", y, x);
    }
}

void init() {
	glClearColor(0, 0, 0, 1.0);
	glColor3f(0, 0, 0);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(NUM_TEXTURES, textures);
	
	// remove "main.cpp" from path
	for (int i = (int)fullPath.length()-1; i>=0 && fullPath[i] != '/'; i--) {
		fullPath.erase(i,1);
	}
	
	// Load textures
	char  path[200];
	sprintf(path,"%s%s", fullPath.c_str() , "concrete.jpg");
	textures[SPHERE_DEFAULT] = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	
	sprintf(path,"%s%s", fullPath.c_str() , "redconcrete.jpg");
	textures[SPHERE_SELECTED] = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Memoriae");
    
    glutPassiveMotionFunc(mouseMoved);
    glutMouseFunc(mouseClicked);
    
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(1000, timer, 0);

	init();
	glutMainLoop();

	return 0;
}