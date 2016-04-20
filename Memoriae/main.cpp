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
const int NUM_TEXTURES = 8;
const int MAIN_MENU = 0;
const int SPHERE_DEFAULT = 1;
const int SPHERE_SELECTED = 2;
const int SPHERE_WRONG = 3;
const int GAME_OVER = 4;
const int CHECKMARK = 5;
const int INSTRUCTIONS = 6;
const int CREDITS = 7;
GLuint textures[NUM_TEXTURES];

#pragma mark - Global Variables
/* --------------------------- Global Variables ----------------------------- */
// Screen and Frustum values
int screenWidth = 800, screenHeight = 600;
const float X_MAX = 8.0, X_MIN = -8.0;
const float Y_MAX = 6.0, Y_MIN = -6.0;
string fullPath = __FILE__;

// Game logic helpers
int seconds = 0, currentLevel = 0, currentSubLevel = 0;
bool selected[10][10] = {0}, changingLevel, changingSubLevel;
bool showInstructions, showCredits;

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

void selectCorrectAnswer() {
	for (int i = 0; i < game.getActualSize(); i++) {
		for (int j = 0; j < game.getActualSize(); j++) {
			if (!selected[i][j]) {
				selected[i][j] = game.isSet(i, j);
			}
		}
	}
}

void validateLevelChange(int &last, int current, bool &cond){
    if (last != current) {
        if (cond) {
            if (seconds > -300) {
                last = current;
                cleanSelectedMatrix();
                seconds = 0;
                cond = false;
            }
        } else {
            seconds = -400;
            cond = true;
        }
    }
}

void timer(int value) {
	glutPostRedisplay();
    validateLevelChange(currentLevel, game.getLevel(), changingLevel);
    validateLevelChange(currentSubLevel, game.getSubLevel(), changingSubLevel);
	seconds++;
	if (game.getState() == STATE_PLAYING || game.getState() == STATE_GAMEOVER) {
		glutTimerFunc(10, timer, 0);
	}
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

void translateClickToCoordinates(int &x, int &y) {
    float ball_num = game.getActualSize(), delta = 10;
	if (x > finalX-delta || x < initialX+delta) {
		x = -1;
		return;
	} else if (y > finalY-delta || y < initialY+delta) {
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

void addLights() {
	float mat[4];
	mat[0] = 0030;
	mat[1] = 0030;
	mat[2] = 0030;
	mat[3] = 1;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
	mat[0] = 0.1;
	mat[1] = 0.1;
	mat[2] = 0.1;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
	mat[0] = 0.4;
	mat[1] = 0.4;
	mat[2] = 0.4;
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
	glMaterialf(GL_FRONT, GL_SHININESS, .078125 * 128.0);
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
			if (changingLevel || changingSubLevel) {
				glBindTexture(GL_TEXTURE_2D, textures[SPHERE_DEFAULT]);
			} else {
				if ((game.isSet(i, j) && seconds >= 100 && seconds <= 250) || (selected[i][j] && game.isSet(i, j))) {
					glBindTexture(GL_TEXTURE_2D, textures[SPHERE_SELECTED]);
				} else if (selected[i][j]) {
					glBindTexture(GL_TEXTURE_2D, textures[SPHERE_WRONG]);
				} else {
					glBindTexture(GL_TEXTURE_2D, textures[SPHERE_DEFAULT]);
				}
			}
			
			glPushMatrix();
			glTranslatef(j*(2*radius), radius, 0);	// starting position for each column
			glScalef(0.7, 0.7, 0.7);				// make spheres smaller so they don't touch
			
			
			addLights();
			
			glutSolidSphere(radius, 30, 30);
			
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
	
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
}

void drawLevelAndScore() {
    char level[100] = "";
    sprintf(level, "Level: %d (%d/%d)", game.getLevel() + 1,
            game.getSubLevel() + 1, maxSubLevel);
    draw3dString(GLUT_STROKE_ROMAN, level, -10.8, 7, 0.008);
    
    char score[100] = "";
    sprintf(score, "Score: %d", game.getScore());
    draw3dString(GLUT_STROKE_ROMAN, score, 5, 7, 0.008);
}

void drawSpheresAndText() {
	drawLevelAndScore();
	int spheres = game.getActualSize();
	if (changingLevel) {
		spheres -= 1;
	}
	paintSpheres(spheres, spheres, fmin(X_MAX, Y_MAX));
}

void drawFullScreenTexture(int texture) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[texture]);
	glColor3f(1, 1, 1);
	float z = 0.35;
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(X_MIN, Y_MIN, z);
	glTexCoord2f(1, 1);
	glVertex3f(X_MAX, Y_MIN, z);
	glTexCoord2f(1, 0);
	glVertex3f(X_MAX, Y_MAX, z);
	glTexCoord2f(0, 0);
	glVertex3f(X_MIN, Y_MAX, z);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	switch (game.getState()) {
	case STATE_PLAYING:
		drawSpheresAndText();
		if ((changingLevel || changingSubLevel) &&
            (game.getLevel() + game.getSubLevel() > 0)) {
			drawFullScreenTexture(CHECKMARK);
		}
		break;
		
	case STATE_PAUSED:
			if (showInstructions) {
				drawFullScreenTexture(INSTRUCTIONS);
			} else if (showCredits) {
				drawFullScreenTexture(CREDITS);
			} else {
				drawFullScreenTexture(MAIN_MENU);
			}
		break;
	
	case STATE_WON:{
		printf("");
		}break;
	
	case STATE_GAMEOVER:
		changingLevel = false;
        changingSubLevel = false;
		if (seconds < 400) {
			selectCorrectAnswer();
			drawSpheresAndText();
		} else if (seconds < 480) {
			cleanSelectedMatrix();
			drawFullScreenTexture(GAME_OVER);
		} else {
			game.restart();// Just one restart call doesn't reset the level?
			game.pause();
			seconds = 0;
            currentLevel = 0;
            currentSubLevel = 0;
		}
		break;
	}
	
	glutSwapBuffers();
}

void keyboardFunc(unsigned char key, int x, int y) {
	if (showInstructions || showCredits) {
		return;
	}
	switch (key) {
	case 'i':
	case 'I':
		showInstructions = true;
		break;
	case 'c':
	case 'C':
		showCredits = true;
		break;
	case 27:
		if (game.getState() == STATE_PAUSED) {
			exit(0);
		}
	}
	glutPostRedisplay();
}

void keyboardUpFunc(unsigned char key, int x, int y) {
	switch (key) {
		case 'i':
		case 'I':
			showInstructions = false;
			break;
		case 'c':
		case 'C':
			showCredits = false;
			break;
	}
	glutPostRedisplay();
}

void mouseMoved(int x, int y) {
	// printf("Moved -> X: %d, Y: %d\n", x, y);
}

void mouseClicked(int button, int state, int x, int y) {
	if (game.getState() == STATE_PLAYING && !showInstructions && !showCredits) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && seconds > 250){
			translateClickToCoordinates(x, y);
			if (x == -1 || y == -1) {
				return;
			}
			printf("Clicked -> Row: %d, Col: %d\n", y, x);
            if (!selected[y][x]){
                game.selectSphereAt(y,x);
                selected[y][x] = true;
            }
			if (game.getState() == STATE_GAMEOVER) {
				seconds = 301;
			}
		}
	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (game.getState() == STATE_PAUSED) {
			game.unpause();
			glutTimerFunc(10, timer, 0);
		} else {
			
		}
	}
}

unsigned int loadTextureFromPath(char *path) {
	return SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
}

void init() {
	game.pause();
	glClearColor(0, 0, 0, 1);
	glColor3f(0, 0, 0);
	
	GLfloat ambient[] = {1, 1, 1, 1};
	GLfloat diffuse[] = {1, 1, 1, 1};
	GLfloat position[] = {0, 3, 3, 0};
	
	GLfloat lmodel_ambient[] = {10, 10, 10, 1};
	GLfloat local_view[] = {0};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
	
	glFrontFace(GL_CW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LESS);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(NUM_TEXTURES, textures);
	
	// remove "main.cpp" from path
	for (int i = (int)fullPath.length()-1; i>=0 && fullPath[i] != '/'; i--) {
		fullPath.erase(i,1);
	}
	
	// Load textures
	char path[200];
	sprintf(path,"%s%s", fullPath.c_str(), "main menu.png");
	textures[MAIN_MENU] = loadTextureFromPath(path);
	
	sprintf(path,"%s%s", fullPath.c_str(), "purple.png");
	textures[SPHERE_DEFAULT] = loadTextureFromPath(path);
	
	sprintf(path,"%s%s", fullPath.c_str(), "green.png");
	textures[SPHERE_SELECTED] = loadTextureFromPath(path);
	
	sprintf(path,"%s%s", fullPath.c_str(), "red.png");
	textures[SPHERE_WRONG] = loadTextureFromPath(path);
	
	sprintf(path,"%s%s", fullPath.c_str(), "game over.png");
	textures[GAME_OVER] = loadTextureFromPath(path);
	
	sprintf(path,"%s%s", fullPath.c_str(), "checkmark.png");
	textures[CHECKMARK] = loadTextureFromPath(path);
	
	sprintf(path,"%s%s", fullPath.c_str(), "instructions.png");
	textures[INSTRUCTIONS] = loadTextureFromPath(path);
	
	sprintf(path,"%s%s", fullPath.c_str(), "credits.png");
	textures[CREDITS] = loadTextureFromPath(path);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Memoriae");

	glutKeyboardFunc(keyboardFunc);
	glutKeyboardUpFunc(keyboardUpFunc);
	glutPassiveMotionFunc(mouseMoved);
	glutMouseFunc(mouseClicked);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	init();
	glutMainLoop();

	return 0;
}