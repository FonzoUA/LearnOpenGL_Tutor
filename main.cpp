#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include <stdio.h>

#define WIREFRAME 0

// All of the information and code is coming from https://learnopengl.com/ (might be paraphrased for author's learning purposes) 

// Function Prototypes
void Framebuffer_size_callback(GLFWwindow* pWindow, int width, int height);
void ProcessInput(GLFWwindow* pWindow);

// Resolution
const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

// Defining shaders in GLSL (OpenGL Shading Language) store as a C string
																						/*	In Modetn OpenGL, it is required to define at least vertex and 
																							fragment shaders (there are no default ones on the GPU). */
// Vertex Shader definition
const GLchar* VertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
																						/*	Each shader begins with a declaration of its version. In this case, its also specified that we are
																							using core profile functionality. 
																							Next all input vertex attributes in the vertex shader are declared with the 'in' keyword. 
																							There is a vector datatype in GLSL that contains 1 to 4 floats based on postfix digit. 
																							vec3 'aPos' input variable would hold a 3D coordinate of a vertex. Location of the input variable
																							is also explicidly specified via 'layout (location = 0)' command*/

// Fragment Shader definition
const GLchar* FragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

																						/*	Fragment shader calculates the color output of pixels. Note that colors in OpenGL/GLSL
																							are set b/w 0.0f and 1.0f RGBA.
																							Fragment shader only requires one output variable: a vector of size 4 that defines the final color
																							output ('out' keyword declares output values). In this case output value is name 'FragColor' which is
																							assigned an orange color with alpha value of 1.0f*/

int main()
{
	// ---------- Initialize and configure GLFW ----------
	glfwInit();																	// Initializes GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);												// Configures GLFW. First parameter tells an option we want to configure and then comes selection
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);												// from enum of possible options prefixed with 'GLFW_'. Second argument is an int that sets the value of our option
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);				// In this case, its specified that GLFW should use 3.3 version of OpenGL and that it should use core-profile

	// ---------- GLFW window creation ----------
	// Create window object that holds all the windowing data and used by other GLFW functions
	GLFWwindow* pWindow = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Test Window", nullptr, nullptr);		// Create window of specified width and height 
	if (pWindow == nullptr)																									// Check if creation was successful
	{
		printf("Failed to create GLFW window \n");														// If not, clear used resources, display error message and stop the program
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(pWindow);																				// Make the context of the window the main context on the current thread

	// ---------- GLAD: Load all OpenGL function pointers ----------
	// Since GLAD manages function pointers for OpenGL, initialize GLAD before calling any OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))											// Pass GLAD the function to load address of the OpenGL function pointers
		{																																						// glfwGetProtAddress() defines the correct function based on which OS the program is compiled
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	// ---------- Build and Compile shader program ----------
	// Vertex shader 
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);											// Create a shader object
	glShaderSource(vertexShader, 1, &VertexShaderSource, nullptr);										// Attach shader source code to the shader object 
																																										// [Parameters] First: shader to compile. Second: Specify how many strings are being passed
																																										// as a source code. Third: actual source code reference. 
	glCompileShader(vertexShader);																		// Compile the shader
																										/*  First create an empty shader object and assign its ID to unsigned int (done with 'glCreateShader()'
																											Since vertex shader is being created, pass in GL_VERTEX_SHADER as parameter. Next attach the
																											shader source code to the shader object and compile the shader*/
	
	// Check for compilation errors
	int success;																																	// Indicates success status
	char infoLog[512];																														// Stores error messages
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);											// Checks if compilation was successful
	if (!success)																						
	{	
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);										// Retrieve error message
		printf("Error::Shader::Vertex::Compilation_failed\n");
	}

	// Fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// Check for compilation errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);										
		printf("Error::Shader::Fragment::Compilation_failed\n");
	}

	// Link Shaders
	int shaderProgram = glCreateProgram();																// Creates a program object
																										/*	A shader program object is the final linked version of multiple shaders combined. In order
																											for compiled shaders to be used, they have to be linked to a shader program object which then
																											is activated when objects are rendered. Activated shader program's shaders will be used when
																											render calls are issued.
																											When linking the shaders into a program it links the output of each shader to the input of the
																											next shader. This would give linkining errors if outputs and inputs do not match*/
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		printf("Error::Shader::Program::Linking_failed\n");
	}

	// Delete the shader objects once they've been linked into the program object
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	// ---------- Set up vertex data (and buffers) and configure vertex attributes ----------
	// Specify three vertices
	float vertices[] = {	-0.5f, -0.5f, 0.0f,
							0.5f, -0.5f, 0.0f,
							0.0f,  0.5f, 0.0f };

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);														// Generate Vertex Array Object and assing ID to it
	glGenBuffers(1, &VBO);																// Generate Vertex Buffer Object and assing ID to it

	// Bind the Vertex Array Object first, then bind and set vertex buffers and then configure vertex attributes
	glBindVertexArray(VAO);									
	glBindBuffer(GL_ARRAY_BUFFER, VBO);																						// Bind buffer object to the current buffer type target
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// Allocates memory and stores data within the initialized memory in the currently bound buffer object
																																								// [Parameters] First: Type of buffer we want to copy data into. Second: size of data (in bytes).
																																								// Third: data we want to send; Fourth: specifies how we want the graphics card to manage the given data

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Specifies how OpenGL should interpret the vertex buffer data whenever a drawing call is made.
																																								// [Parameters] First: Specify which vertex attribute to configure. Second: Specify size of the vertex attribute
																																								// Third: Specify type of data. Fourth: Whether data should be normalized. Fifth: Specify stride (space b/w consecutive
																																								// vertex arrtibute sets. Sixth: Specify offset of the begin position data
	glEnableVertexAttribArray(0);																									// Enables generic vertex attribute, giving location as its argument (disabled by default)
								
	glBindBuffer(GL_ARRAY_BUFFER, 0);																							// Unbind buffer. Since glVertexAttribPointer call registered VBO as the vertex attribute's bound VBO, its safe to unbind
								
	glBindVertexArray(0);																													// Unbind VAO
								
#if WIREFRAME								
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);																		// Draws in wireframe polygons
#endif

	// ---------- Render Loop ----------
	while (!glfwWindowShouldClose(pWindow))
	{
		// Input
		ProcessInput(pWindow);

		// Render 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);											// Clear color buffer and set specific color to it at the same time
		glClear(GL_COLOR_BUFFER_BIT);															// Specify which buffer we want to clean

		// Draw Triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// GLFW: Swap buffers and poll IO events (keys pressed/released, mouse movement, etc)
		glfwSwapBuffers(pWindow);																		// Swaps the color buffer (large buffer that contains color values for each pixel in GLFW's
																																// window) that has been used to draw in during this iteration and outputs to screen
		glfwPollEvents();																						// Checks if any events are triggered (i.e keyboard input or mouse movement), 
																																// updates window state and calls appropriate callback methods
	}
	
	// ---------- Clean up ----------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}


void ProcessInput(GLFWwindow* pWindow)
{
	// Returns whether the key is currently being pressed
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)			// If ESC key is pressed, close the window
	{
		glfwSetWindowShouldClose(pWindow, true);
	}
}

void Framebuffer_size_callback(GLFWwindow* pWindow, int width, int height)
{
	// Ensure that viewport matches the new window dimenstions.
	glViewport(0, 0, width, height);
}



