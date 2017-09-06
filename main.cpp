#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include "Shader.h"

#include <stdio.h>
#include <math.h>
#define WIREFRAME 0

// All of the information and code is coming from https://learnopengl.com/ (might be paraphrased for author's learning purposes) 

// Function Prototypes
void Framebuffer_size_callback(GLFWwindow* pWindow, int width, int height);
void ProcessInput(GLFWwindow* pWindow);

// Resolution
const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

int main()
{
	// ---------- Initialize and configure GLFW ----------
	glfwInit();																	// Initializes GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);								// Configures GLFW. First parameter tells an option we want to configure and then comes selection
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);								// from enum of possible options prefixed with 'GLFW_'. Second argument is an int that sets the value of our option
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);				// In this case, its specified that GLFW should use 3.3 version of OpenGL and that it should use core-profile

	// ---------- GLFW window creation ----------
	// Create window object that holds all the windowing data and used by other GLFW functions
	GLFWwindow* pWindow = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Test Window", nullptr, nullptr);		// Create window of specified width and height 
	if (pWindow == nullptr)																				// Check if creation was successful
	{
		printf("Failed to create GLFW window \n");														// If not, clear used resources, display error message and stop the program
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(pWindow);																	// Make the context of the window the main context on the current thread

	// ---------- GLAD: Load all OpenGL function pointers ----------
	// Since GLAD manages function pointers for OpenGL, initialize GLAD before calling any OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))											// Pass GLAD the function to load address of the OpenGL function pointers
	{																									// glfwGetProtAddress() defines the correct function based on which OS the program is compiled
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	// ---------- Build and Compile shader program ----------
	
	Shader ourShader("Shaders/VertexShaderSource.vs", "Shaders/FragmentShaderSource.fs");

	// ---------- Set up vertex data (and buffers) and configure vertex attributes ----------
	// Specify three vertices
	float vertices[] = {	// Triangle 1		 // Colors 1
							0.6f,  -0.4f, 0.0f,  0.0f, 0.0f, 1.0f,
							0.3f, 0.2f, 0.0f,  	 0.0f, 1.0f, 0.0f,
							0.0f, -0.4f, 0.0f,	 1.0f,  0.0f, 0.0f,
							// Triangle 2		 // Colors 2
							-0.6f, -0.4f, 0.0f,	 0.0f, 0.0f, 1.0f,
							-0.3f, 0.2f, 0.0f,	 0.0f, 1.0f, 0.0f,
							0.0f, -0.4f, 0.0f,	 1.0f,  0.0f, 0.0f };

	float vertices2[] = {	// Positions		// Colors
							0.3f, 0.2f, 0.0f,	1.0f, 0.0f, 0.0f,
							0.0f, 0.8f, 0.0f,	0.0f, 1.0f, 0.0f,
							-0.3f, 0.2f, 0.0f,	0.0f,  0.0f, 1.0f	};
	
	unsigned int indices[] = {	// First Triangle
								0, 1, 3,
								// Second Triangle
								1, 2, 3	};

	unsigned int VBO[2], VAO[2], EBO;
	glGenVertexArrays(2, VAO);																// Generate Vertex Array Object and assing ID to it
	glGenBuffers(2, VBO);																	// Generate Vertex Buffer Object and assing ID to it
	glGenBuffers(1, &EBO);																	// Generate Element Buffer Object and assign ID to it

	glBindVertexArray(VAO[0]);																// Bind the Vertex Array Object first, then bind and set vertex buffers and then configure vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);													// Bind buffer object to the current buffer type target
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);				// Allocates memory and stores data within the initialized memory in the currently bound buffer object
																							// [Parameters] First: Type of buffer we want to copy data into. Second: size of data (in bytes).
																							// Third: data we want to send; Fourth: specifies how we want the graphics card to manage the given data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);												// Bind EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);		// Copy indices into the buffer via glBufferData(); 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);			// Specifies how OpenGL should interpret the vertex buffer data whenever a drawing call is made.
																							// [Parameters] First: Specify which vertex attribute to configure. Second: Specify size of the vertex attribute
																							// Third: Specify type of data. Fourth: Whether data should be normalized. Fifth: Specify stride (space b/w consecutive
																							// vertex arrtibute sets. Sixth: Specify offset of the begin position data
	glEnableVertexAttribArray(0);															// Enables generic vertex attribute, giving location as its argument (disabled by default)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);					// Update 0 layour (position attribute)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));	// Update 1 layour (color attribute)
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);														// Unbind buffer. Since glVertexAttribPointer call registered VBO as the vertex attribute's bound VBO, its safe to unbind

	//glBindVertexArray(0);																	// Unbind VAO


#if WIREFRAME
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);												// Draws in wireframe polygons
#endif

	// ---------- Render Loop ----------
	while (!glfwWindowShouldClose(pWindow))
	{
		// Input
		ProcessInput(pWindow);

		// Render 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);												// Clear color buffer and set specific color to it at the same time
		glClear(GL_COLOR_BUFFER_BIT);														// Specify which buffer we want to clean

		// Draw 
		ourShader.Use();
		
		// Draw using data from first VAO
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Draw using data from second VAO
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);								// [Parameters] First: specify mode to draw in. Second: count/number of elements to draw. 
																							// Third: type of indices data. Fourth: offset in EBO
		

		// GLFW: Swap buffers and poll IO events (keys pressed/released, mouse movement,	 etc)
		glfwSwapBuffers(pWindow);															// Swaps the color buffer (large buffer that contains color values for each pixel in GLFW's
																							// window) that has been used to draw in during this iteration and outputs to screen
		glfwPollEvents();																	// Checks if any events are triggered (i.e keyboard input or mouse movement), 
																							// updates window state and calls appropriate callback methods
	}
	
	// ---------- Clean up ----------
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);

	glfwTerminate();

	return 0;
}


void ProcessInput(GLFWwindow* pWindow)
{
	// Returns whether the key is currently being pressed
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)									// If ESC key is pressed, close the window
	{
		glfwSetWindowShouldClose(pWindow, true);
	}
}

void Framebuffer_size_callback(GLFWwindow* pWindow, int width, int height)
{
	// Ensure that viewport matches the new window dimenstions.
	glViewport(0, 0, width, height);
}



