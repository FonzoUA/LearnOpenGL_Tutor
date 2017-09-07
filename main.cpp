#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include "my_stb_image.h"

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
	
	Shader ourShader("Shaders/TextureVertexShaderSource.vs", "Shaders/FragmentShaderSource.fs");

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

	float vertices2[] = {	// Positions			// Colors				// Texture coords
							 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,			// Top right
							 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,			// Bottom right
							-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,			// Botoom left
							-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,		0.0f, 1.0f	};		// Top left
	
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
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);												// Bind EBO
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);		// Copy indices into the buffer via glBufferData(); 
	
																							// First Buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);			// Specifies how OpenGL should interpret the vertex buffer data whenever a drawing call is made.
																							// [Parameters] First: Specify which vertex attribute to configure. Second: Specify size of the vertex attribute
																							// Third: Specify type of data. Fourth: Whether data should be normalized. Fifth: Specify stride (space b/w consecutive
																							// vertex arrtibute sets. Sixth: Specify offset of the begin position data
	glEnableVertexAttribArray(0);															// Enables generic vertex attribute, giving location as its argument (disabled by default)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// Second buffer
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);												// EBO binds to a CURRENTLY ACRIVE ARRAY BUFFER 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);					// Update 0 layout (position attribute)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));	// Update 1 layout (color attribute)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));	// Update 2 layout (texture coords attribute)
	glEnableVertexAttribArray(2);

	

	//glBindBuffer(GL_ARRAY_BUFFER, 0);														// Unbind buffer. Since glVertexAttribPointer call registered VBO as the vertex attribute's bound VBO, its safe to unbind

	//glBindVertexArray(0);																	// Unbind VAO


#if WIREFRAME
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);												// Draws in wireframe polygons
#endif

	// ---------- Set up and load Textures ----------
	// Instanciate a texture ID
	unsigned int texture[1];
	glGenTextures(2, texture);																// [Parameters] First: how many textures to generate. Second: Where to store those generated textures 
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Set texture wrap option																// Coordinate axis' for textures are 's, t, r' (equivalent to 'x, y, z')
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);					// [Parameters] First: Specify the texture target (since 2D texture is used in this case the target is GL_TEXTURE2D)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);					// Second: Option to set for which texture axis. Third: Texture wrapping mode
	
	// GL_CLAMP_TO_BORDER option setup
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Setup texture filtering for magnifying and minifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);						// Use nearest neighbor filtering for minifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);						// Use linear filtering for magnifyig

																								// Load in and create textures (using stb_image.h library)
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);														// Flips any loaded image vertically
	unsigned char* data = stbi_load("Textures/w33d.jpg", &width, &height, &nrChannels, 0);	// [Parameters] First: location of an image file. Second+Third: image's width and height. 
																								// Fourth: number of color channels
	if (!data)
	{
		printf("TEXTURE::LOAD_FAIL\n");
	}
	

	// Generate texture using previously loaded image data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	// [Parameters] First: Specify the texture target: setting it to GL_TEXTURE_2D will generate a texture
																								// on the currently bound texture object at the same target (so any textues bound to targets GL_TEXTURE_1D / 3D 
																								// will not be affected). 
																								// Second: Specify the mipmap level for which to create a texture (if want to set up each mipmap
																								// level manually) or 0 for base level.
																								// Third: Specify in what kind of format to store the texture (since in this case image has only RGB 
																								// values, store the texture with RGB values as well
																								// Fourth + Fifth: set width and heigth
																								// Sixth: should always be 0 (b/c legacy)
																								// Seventh and Eight: Specify the format and datatype of the source image (since image was loaded with 
																								// RGB values and stored as char (bytes), pass in corresponding values)
																								// Ninth: actual image data
	glGenerateMipmap(GL_TEXTURE_2D);															// Generates texture mipmaps

	// Free the image memory before new load
	stbi_image_free(data);

	// Load another texture
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	
	// Setup texture filtering for magnifying and minifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("Textures/SlepoyEvrei.png", &width, &height, &nrChannels, 4);
	if (!data)
	{
		printf("TEXTURE::LOAD_FAIL\n");
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free the image memory
	stbi_image_free(data);																		


	// Set uniforms
	ourShader.Use();															// Activate shader before setting uniforms
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);				// Setting it manually
	ourShader.setInt("texture2", 1);											// SEtiing it with shader class

	// ---------- Render Loop ----------
	while (!glfwWindowShouldClose(pWindow))
	{
		// Input
		ProcessInput(pWindow);

		// Render 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);												// Clear color buffer and set specific color to it at the same time
		glClear(GL_COLOR_BUFFER_BIT);														// Specify which buffer we want to clean

		// Bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		// Draw 
		ourShader.Use();
		
		// Draw using data from first VAO
#if 0
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
#endif
		// Draw using data from second VAO
		
		glBindVertexArray(VAO[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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
	glDeleteBuffers(1, &EBO);
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



