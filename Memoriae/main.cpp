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

using namespace std;

#pragma mark - Global Variables
/* --------------------------- Global Variables ----------------------------- */
// Screen and Frustum values
int screenWidth = 500, screenHeight = 500;
const float X_MAX = 4.0, X_MIN = -4.0;
const float Y_MAX = 4.0, Y_MIN = -4.0;
string fullPath = __FILE__;
GLuint texture;

#pragma mark - Functions
/* ------------------------------- Functions -------------------------------- */
void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(500, timer, 0);
}

void reshape(int w, int h) {
	screenWidth = w;
	screenHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(X_MIN, X_MAX, Y_MIN, Y_MAX, 1, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 1.1, 0, 0, 0, 0, 1, 0);
}

void paintSpheres(int spheresPerRow, int spheresPerColumn, float maxWidth) {
	int maxSpheres = fmax(spheresPerRow, spheresPerColumn);
	float radius = maxWidth/maxSpheres;
	
//	Cuando se desconocen los vertices del objeto
//	Selecciona la textura
	glEnable(GL_TEXTURE_2D);
//	Cambiamos el color para que no se pinte la textura
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, texture);
//	Como se van a generar las coordenadas?
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
//	Activar la generación de coordenadas
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	
	glPushMatrix();
	glTranslatef(-maxWidth*((float)spheresPerRow/maxSpheres), -maxWidth*((float)spheresPerColumn/maxSpheres), 0);//center all spheres
	glScalef(1, 1, 0.1);//scale spheres on Z so they don't distort
	for (int i=0; i < spheresPerColumn; i++) {
		glPushMatrix();
		glTranslatef(radius, i*(2*radius), 0);//starting position for each row
		
		for (int j=0; j < spheresPerRow; j++) {
			glPushMatrix();
			glTranslatef(j*(2*radius), radius, 0);//starting position for each column
			glRotated(180, 1, 0, 0);//turn texture upside down
			glScalef(0.8, 0.8, 0.8);//make spheres smaller so they don't touch
			glutSolidSphere(radius, 30, 30);
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
	
//	Debes desabilitar la generacion automatica Filmina 42
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	paintSpheres(4, 3, fmin(X_MAX, Y_MAX));
	
	glutSwapBuffers();
}

void init() {
	glClearColor(0.2039, 0.6588, 0.3254, 1.0);
	glColor3f(0, 0, 0);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	
//	remove "main.cpp" from path
	for (int i = (int)fullPath.length()-1; i>=0 && fullPath[i] != '/'; i--) {
		fullPath.erase(i,1);
	}
	
//	Load texture
	char  path[200];
	sprintf(path,"%s%s", fullPath.c_str() , "img_test.png");
	texture = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Memoriae");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc(100, timer, 0);
	init();
	glutMainLoop();
	
	return 0;
}