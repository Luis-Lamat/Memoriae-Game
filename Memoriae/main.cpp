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
const int NUM_TEXTURES = 4;
const int MAIN_MENU = 0;
const int SPHERE_DEFAULT = 1;
const int SPHERE_SELECTED = 2;
const int SPHERE_WRONG = 3;
GLuint textures[NUM_TEXTURES];

#pragma mark - Global Variables
/* --------------------------- Global Variables ----------------------------- */
// Screen and Frustum values
int screenWidth = 800, screenHeight = 600;
const float X_MAX = 8.0, X_MIN = -8.0;
const float Y_MAX = 6.0, Y_MIN = -6.0;
string fullPath = __FILE__;

// Game logic helpers
int seconds = 0, currentLevel = 0;
bool selected[10][10] = {0};

// Game view matrix sizes (pixels)
float sliceX = screenWidth/4.0f, sliceY = screenHeight/6.0f;
float initialX = sliceX, finalX = sliceX * 3;
float initialY = sliceY, finalY = sliceY * 5;

// Game object
Memoriae game;

#pragma mark - Functions
/* ------------------------------- Functions -------------------------------- */
void cleanSelectedMatrix() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			selected[i][j] = false;
		}
	}
}

void timer(int value) {
	glutPostRedisplay();
    if (currentLevel != game.getLevel()) {
        currentLevel = game.getLevel();
        seconds = 0;
        cleanSelectedMatrix();
    }
	seconds++;
	glutTimerFunc(10, timer, 0);
}

void draw3dString (void *font, char *str, float x, float y, float scale) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScaled(scale, scale, scale);
    glColor3f(1, 1, 1);
    for (int i = 0; str[i] != '\0'; i++) {
        glutStrokeCharacter(font, str[i]);
    }
    glPopMatrix();
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
	if (x > finalX || x < initialX) {
		x = -1;
		return;
	} else if (y > finalY || y < initialY) {
		y = -1;
		return;
	}
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
			if ((game.isSet(i, j) && seconds >= 100 && seconds <= 300) || (selected[i][j] && game.isSet(i, j))) {
				glBindTexture(GL_TEXTURE_2D, textures[SPHERE_SELECTED]);
			} else if (selected[i][j]) {
				glBindTexture(GL_TEXTURE_2D, textures[SPHERE_WRONG]);
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

void drawLevelAndScore() {
    char score[100] = "";
    sprintf(score, "Nivel: %d", game.getLevel() + 1);
    draw3dString(GLUT_STROKE_ROMAN, score, -10.8, 7, 0.008);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	switch (game.getState()) {
	case STATE_PLAYING:{
		drawLevelAndScore();
		int spheres = game.getActualSize();
		paintSpheres(spheres, spheres, fmin(X_MAX, Y_MAX));
		}break;
		
	case STATE_PAUSED:{
		glBindTexture(GL_TEXTURE_2D, textures[MAIN_MENU]);
		glColor3f(1, 1, 1);
		float z = 0.35;
		glBegin(GL_QUADS);
		//Asignar la coordenada de textura 0,0 al vertice
		glTexCoord2f(0, 1);
		glVertex3f(-8, -6, z);
		//Asignar la coordenada de textura 1,0 al vertice
		glTexCoord2f(1, 1);
		glVertex3f(8, -6, z);
		//Asignar la coordenada de textura 1,1 al vertice
		glTexCoord2f(1, 0);
		glVertex3f(8, 6, z);
		//Asignar la coordenada de textura 0,1 al vertice
		glTexCoord2f(0, 0);
		glVertex3f(-8, 6, z);
		glEnd();
		}break;
	
	case STATE_WON:{
		printf("");
		}break;
	
	case STATE_GAMEOVER:{
		printf("");
		}break;
	}
	
	glutSwapBuffers();
}

void mouseMoved(int x, int y){
	// printf("Moved -> X: %d, Y: %d\n", x, y);
}

void mouseClicked(int button, int state, int x, int y){
	if (game.getState() == STATE_PLAYING) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && seconds > 300){
			translateClickToCoordinates(x, y);
			if (x == -1 || y == -1) {
				return;
			}
			printf("Clicked -> Row: %d, Col: %d\n", y, x);
			game.selectSphereAt(y,x);
			selected[y][x] = true;
		}
	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (game.getState() == STATE_PAUSED) {
			game.unpause();
			glutTimerFunc(10, timer, 0);
		} else {
			
		}
	}
}

void init() {
	game.pause();
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
	sprintf(path,"%s%s", fullPath.c_str() , "bg.png");
	textures[MAIN_MENU] = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	
	sprintf(path,"%s%s", fullPath.c_str() , "purple.png");
	textures[SPHERE_DEFAULT] = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	
	sprintf(path,"%s%s", fullPath.c_str() , "green.png");
	textures[SPHERE_SELECTED] = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	
	sprintf(path,"%s%s", fullPath.c_str() , "red.png");
	textures[SPHERE_WRONG] = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
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

	init();
	glutMainLoop();

	return 0;
}