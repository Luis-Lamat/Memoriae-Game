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

#include <stdlib.h>
#include <stdio.h>
#include <iostream>


#pragma mark - Global Variables
/* --------------------------- Global Variables ----------------------------- */
// Screen and Frustum values
int screenWidth = 500, screenHeight = 500;
const float X_MAX = 4.0, X_MIN = -4.0;
const float Y_MAX = 4.0, Y_MIN = -4.0;


#pragma mark - Functions
/* ------------------------------- Functions -------------------------------- */
void reshape(int w, int h) {
	screenWidth = w;
	screenHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(X_MIN, X_MAX, Y_MIN, Y_MAX, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 1.1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	
	glutSwapBuffers();
}

void init() {
	glClearColor (0.2039, 0.6588, 0.3254, 1.0);
	glColor3f(0.0, 0.0, 0.0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Memoriae");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	init();
	glutMainLoop();
}