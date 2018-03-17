 #include "window.h"
#include "GLshader.h"
#include "Model.h"

const char* window_title = "GLFW Starter Project";
Cube * cube;
GLint shaderProgram, skyShaderProgram;

//OBJ Loader Declarations:
Shader* objShader;
Model* testDino;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
#define SKY_VERTEX_SHADER_PATH "../skyShader.vert"
#define SKY_FRAGMENT_SHADER_PATH "../skyShader.frag"
// OBJ Shader
#define myOBJ_VERT_PATH "../myOBJShader.vert"
#define myOBJ_FRAG_PATH "../myOBJShader.frag"

Skybox *Window::skybox;

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

GLfloat move_speed = 0.5f;                  // movement per input tick
GLfloat cam_sensitivity = 0.0005f;          // Speed of camera rotation

int Window::width;
int Window::height;


glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyShaderProgram = LoadShaders(SKY_VERTEX_SHADER_PATH, SKY_FRAGMENT_SHADER_PATH);

	cube = new Cube();
	char * images[] = {
		"../imgs/rainforest_lf.ppm",
		"../imgs/rainforest_rt.ppm",
		"../imgs/rainforest_up.ppm",
		"../imgs/rainforest_dn.ppm",
		"../imgs/rainforest_ft.ppm",
		"../imgs/rainforest_bk.ppm"
	};
	skybox = new Skybox(images);
   objShader = new Shader(myOBJ_VERT_PATH, myOBJ_FRAG_PATH);
   testDino = new Model("../raptor/raptor.obj");
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	cube->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw Skybox
	glUseProgram(skyShaderProgram);
	skybox->draw(skyShaderProgram);
	
	// Update Camera Position
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);

	// Render the cube
	glUseProgram(shaderProgram);
	cube->draw(shaderProgram);

   // Set ObjShader / Render Objs:
   objShader->use();
   objShader->setMat4("projection", Window::P);
   objShader->setMat4("view", Window::V);

   glm::mat4 model;
   model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); //adjust starting location
   model = glm::scale(model, glm::vec3(4.2f, 4.2f, 4.2f)); //adjust size
   objShader->setMat4("model", model);
   testDino->Draw(*objShader);



	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
	// Unit vector pointing forward
	glm::vec3 move_dir = glm::normalize(cam_look_at - cam_pos);
	move_dir.y = 0.0f;
	// Unit vector pointing right
	glm::vec3 right    = glm::cross(move_dir, cam_up);

	// WASD movement/strafing
	if (key == GLFW_KEY_W) {
		cam_pos += move_speed * move_dir;
		cam_look_at += move_speed * move_dir;
	}
	else if (key == GLFW_KEY_S) {
		cam_pos -= move_speed * move_dir;
		cam_look_at -= move_speed * move_dir;
	}
	else if (key == GLFW_KEY_D) {
		cam_pos += move_speed * right;
		cam_look_at += move_speed * right;
	}
	else if (key == GLFW_KEY_A) {
		cam_pos -= move_speed * right;
		cam_look_at -= move_speed * right;
	}

}
void Window::mouse_pos_callback(GLFWwindow *window, double xPos, double yPos) {
	// Unit vector in view direction
	glm::vec3 look_dir = glm::normalize(cam_look_at - cam_pos);

	// Rotation vectors for up/down and left/right
	// (Normal to plane of rotation)
	glm::vec3 ud_axis = glm::cross(cam_up, look_dir);
	glm::vec3 lr_axis = -cam_up;

	// Mouse movement
	glm::vec2 delta = glm::vec2(xPos - Window::width/2.0f, yPos - Window::height/2.0f);
	
	// Update view direction
	GLfloat rotScale = cam_sensitivity * glm::pi<float>();
	look_dir = glm::mat3(glm::rotate(glm::mat4(1.0f), rotScale * delta.x, lr_axis)) * look_dir;
	look_dir = glm::mat3(glm::rotate(glm::mat4(1.0f), rotScale * delta.y, ud_axis)) * look_dir;
	cam_look_at = cam_pos + (20.0f * look_dir);
	
	// Reset Cursor Position
	glfwSetCursorPos(window, Window::width/2.0f, Window::height/2.0f);
}
