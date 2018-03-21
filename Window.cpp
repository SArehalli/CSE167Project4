 #include "window.h"
#include "Model.h"
#include "Translation.h"

const char* window_title = "GLFW Starter Project";
GLint shaderProgram, skyShaderProgram, objShaderProgram, rimShaderProgram, boxShaderProgram;

//OBJ Loader Declarations:
Transformation *sceneRoot;
Translation *moveControl;

// Demo Toggles:
int Window::shaderToggle = 0; // toggle Rim Shading
int Window::boxToggle = 0; // toggles bounding boxes

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
#define SKY_VERTEX_SHADER_PATH "../skyShader.vert"
#define SKY_FRAGMENT_SHADER_PATH "../skyShader.frag"
// OBJ Shader
#define myOBJ_VERT_PATH "../myOBJShader.vert"
#define myOBJ_FRAG_PATH "../myOBJShader.frag"
// Rim Shader
#define myRIM_VERT_PATH "../myRimShader.vert"
#define myRIM_FRAG_PATH "../myRimShader.frag"
// Box Shader
#define myBOX_VERT_PATH "../boxShader.vert"
#define myBOX_FRAG_PATH "../boxShader.frag"

Skybox *Window::skybox;

// Default camera parameters
glm::vec3 cam_pos(0.0f, 1.5f, 0.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 20.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

GLfloat move_speed = 0.5f;                  // movement per input tick
GLfloat cam_sensitivity = 0.0005f;          // Speed of camera rotation

int Window::width;
int Window::height;


glm::mat4 Window::P;
glm::mat4 Window::V;
Model* dino;

void Window::initialize_objects()
{
	// Load the shader program. Make sure you have the correct filepath up top
	skyShaderProgram = LoadShaders(SKY_VERTEX_SHADER_PATH, SKY_FRAGMENT_SHADER_PATH);
   objShaderProgram = LoadShaders(myOBJ_VERT_PATH, myOBJ_FRAG_PATH);
   rimShaderProgram = LoadShaders(myRIM_VERT_PATH, myRIM_FRAG_PATH);
   boxShaderProgram = LoadShaders(myBOX_VERT_PATH, myBOX_FRAG_PATH);

	char * images[] = {
		"../imgs/rainforest_lf.ppm",
		"../imgs/rainforest_rt.ppm",
		"../imgs/rainforest_up.ppm",
		"../imgs/rainforest_dn.ppm",
		"../imgs/rainforest_ft.ppm",
		"../imgs/rainforest_bk.ppm"
	};
	skybox = new Skybox(images);

   //objShader = new Shader(myOBJ_VERT_PATH, myOBJ_FRAG_PATH);
   //Shader *pinkShader = new Shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
   // Set up scene

   sceneRoot = new Transformation(glm::mat4(1.0f));
   moveControl = new Translation();
   Transformation *dinoRelPos = new Transformation(glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0.0f, 1.5f)));
   dino = new Model("../raptor/raptor.obj", objShaderProgram);
   
   //Model *dino = new Model("../raptor/raptor.obj", objShaderProgram);
   Tree *tree = new Tree(5, objShaderProgram);
   Maze *maze = new Maze(10, objShaderProgram);

   sceneRoot->addChild(dinoRelPos);
   sceneRoot->addChild(maze);
   dinoRelPos->addChild(moveControl);
   moveControl->addChild(dino);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
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
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //Toggle Shaders: 0 = no rim shading, 1 = Rim Shading
   if (shaderToggle == 0){
      dino->shader = objShaderProgram;
   }
   else if (shaderToggle == 1) {
      dino->shader = rimShaderProgram;

   }
   //dino->shader = objShaderProgram;


	// Draw Skybox
	glUseProgram(skyShaderProgram);
	skybox->draw(skyShaderProgram);
	
	// Update Camera Position
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);

	// Render the cube

   // Set ObjShader / Render Objs:
   glUseProgram(rimShaderProgram);
   glUniformMatrix4fv(glGetUniformLocation(rimShaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
   glUniformMatrix4fv(glGetUniformLocation(rimShaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
   glUniform3f(glGetUniformLocation(rimShaderProgram, "lightPosition"), 0.0f, 50.0f, -50.0f); // #1 50,0,50, #2 0,50,50 moonlight
   glUniform3f(glGetUniformLocation(rimShaderProgram, "eyePosition"), cam_pos.x, cam_pos.y, cam_pos.z);
   sceneRoot->draw(glm::mat4(1.0f));
   //sceneRoot->Draw(glm::mat4(1.0f), rimShaderProgram);

   glUseProgram(boxShaderProgram);
   glUniformMatrix4fv(glGetUniformLocation(boxShaderProgram, "view"), 1, GL_FALSE, &Window::V[0][0]);
   glUniformMatrix4fv(glGetUniformLocation(boxShaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);

   //Toggle BBoxes: 0 = no BBs, 1 = yes BBs
   if (boxToggle == 0) {
   }
   else if (boxToggle == 1) {
      sceneRoot->DrawWBB(rimShaderProgram, boxShaderProgram, glm::mat4(1.0f));
   }
   //sceneRoot->DrawWBB(rimShaderProgram, boxShaderProgram, glm::mat4(1.0f));

   //Collision Flag:
   glUniform1i(glGetUniformLocation(boxShaderProgram, "collisionFlag"), 1);
   glUniform1i(glGetUniformLocation(boxShaderProgram, "collisionFlag"), 0);


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
		if (key == GLFW_KEY_R) {
			srand(time(NULL));
			sceneRoot->regenerate();
		}
      if (key == GLFW_KEY_P) {//"P" to toggle Rim Shading
         if (shaderToggle == 0) {
            shaderToggle = 1;
            std::cout << "shaderToggle = " << shaderToggle << "\n";
         }
         else {
            shaderToggle = 0;
            std::cout << "shaderToggle = " << shaderToggle << "\n";
         }
      }
      if (key == GLFW_KEY_B) {//"B" to toggle visible Bounding Boxes
         if (boxToggle == 0) {
            boxToggle = 1;
            std::cout << "boxToggle = " << boxToggle << "\n";
         }
         else {
            boxToggle = 0;
            std::cout << "boxToggle = " << boxToggle << "\n";
         }
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
		moveControl->moveBy(move_speed * move_dir);
	}
	if (key == GLFW_KEY_S) {
		cam_pos -= move_speed * move_dir;
		cam_look_at -= move_speed * move_dir;
		moveControl->moveBy(-move_speed * move_dir);
	}
	if (key == GLFW_KEY_D) {
		cam_pos += move_speed * right;
		cam_look_at += move_speed * right;
		moveControl->moveBy(move_speed * right);
	}
	if (key == GLFW_KEY_A) {
		cam_pos -= move_speed * right;
		cam_look_at -= move_speed * right;
		moveControl->moveBy(-move_speed * right);
	}


}
void Window::mouse_pos_callback(GLFWwindow *window, double xPos, double yPos) {
	// Unit vector in view direction
	glm::vec3 look_dir = glm::normalize(cam_look_at - cam_pos);
	glm::vec3 center = cam_pos + 1.5f * look_dir;

	// Rotation vectors for up/down and left/right
	// (Normal to plane of rotation)
	glm::vec3 ud_axis = glm::cross(cam_up, look_dir);
	glm::vec3 lr_axis = -cam_up;

	// Mouse movement
	glm::vec2 delta = glm::vec2(xPos - Window::width/2.0f, yPos - Window::height/2.0f);
	
	// Update view direction
	GLfloat rotScale = cam_sensitivity * glm::pi<float>();
	glm::mat4 rotateLR = glm::rotate(glm::mat4(1.0f), rotScale * delta.x, lr_axis);
	glm::mat4 rotateUD = glm::rotate(glm::mat4(1.0f), rotScale * delta.y, ud_axis);
	look_dir = glm::mat3(rotateLR * rotateUD) * look_dir;
	moveControl->rotateBy(rotateLR);

	cam_look_at = center + 1.5f * look_dir;
	cam_pos = center - 1.5f * look_dir;
	
	// Reset Cursor Position
	glfwSetCursorPos(window, Window::width/2.0f, Window::height/2.0f);
}


