#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
// Defining shaders in GLSL (OpenGL Shading Language) store as a C string
/*	In Modetn OpenGL, it is required to define at least vertex and
fragment shaders (there are no default ones on the GPU). */
/*	Each shader begins with a declaration of its version. In this case, its also specified that we are
using core profile functionality.
Next all input vertex attributes in the vertex shader are declared with the 'in' keyword.
There is a vector datatype in GLSL that contains 1 to 4 floats based on postfix digit.
vec3 'aPos' input variable would hold a 3D coordinate of a vertex. Location of the input variable
is also explicidly specified via 'layout (location = 0)' command*/
/*	Fragment shader calculates the color output of pixels. Note that colors in OpenGL/GLSL
are set b/w 0.0f and 1.0f RGBA.
Fragment shader only requires one output variable: a vector of size 4 that defines the final color
output ('out' keyword declares output values). In this case output value is name 'FragColor' which is
assigned an orange color with alpha value of 1.0f*/

class Shader
{
public:
	// The program ID
	unsigned int ID;

	// Constructor reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)			// Requires the filepath of the source code of vertex and fragment shader respectively
	{
		//	------ Retrieve the vertex/fragment source cpde from filepath	------
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// Ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer content into steams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// Close files
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();


		//	------ Compile shaders	------
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];
		
		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, nullptr);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
			printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
		};

		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, nullptr);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
			printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
		};

		// Shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, nullptr, infoLog);
			printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
		}

		// ------ Delete the shaders as they are no longer necessery ------
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}
	// Use/Activate the shader
	void Use()
	{
		glUseProgram(ID);
	}
	// Utility uniform functions
	void setBool(const std::string &name, bool val) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)val);
	}
	void setInt(const std::string &name, int val) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
	}
	void setFloat(const std::string &name, float val) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
	}

};



#endif // !SHADER_H
