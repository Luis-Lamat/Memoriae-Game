//
//  main.cpp
//  Memoriae
//
//  Created by Luis Alberto Lamadrid on 3/15/16.
//  Copyright © 2016 Luis Alberto Lamadrid. All rights reserved.
//

#include <iostream>
#include <GLFW/glfw3.h>

#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#pragma mark - Global Variables
/* --------------------------- Global Variables ----------------------------- */

// Screen and Frustum values
int screenWidth = 500, screenHeight = 500;
const float X_MAX = 4.0, X_MIN = -4.0;
const float Y_MAX = 4.0, Y_MIN = -4.0;



#pragma mark - Functions
/* ------------------------------- Functions -------------------------------- */

static void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void reshape(GLFWwindow* window, int w, int h) {
	screenWidth = w;
	screenHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(X_MIN, X_MAX, Y_MIN, Y_MAX, 1, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
//	ignore only this deprecated function warning
	gluLookAt(0, 0, 1.1, 0, 0, 0, 0, 1, 0);
#pragma clang diagnostic pop
}

void display() {
//	ommit glClear & swapBuffer, they're inside while loop on main()
	
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void init() {
	glClearColor (0.2039, 0.6588, 0.3254, 1);
	glColor3f(0, 0, 0);
}

int main(int argc, char** argv) {
	glfwSetErrorCallback(error_callback);
	GLFWwindow *window;
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	
	window = glfwCreateWindow(screenWidth, screenHeight, "Memoriae", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, reshape);
	
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); don't think this is used in GLFW
	
	init();
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
	return 0;
}