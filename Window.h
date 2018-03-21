#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <time.h>
#include "shader.h"
#include "Skybox.h"
#include "Tree.h"
#include "Maze.h"

class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_pos_callback(GLFWwindow* window, double xPos, double yPos);

   //Debug Toggle
   static int shaderToggle;
   static int boxToggle;

   // Collision Lists:
   static vector<glm::vec3> dinoMin;
   static vector<glm::vec3> dinoMax;

   static vector<glm::vec3> otherMin;
   static vector<glm::vec3> otherMax;

   static int boxID;


private:
	static Skybox *skybox;
};

#endif
