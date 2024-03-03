#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	/// <summary>
	/// Shader class that handles shader loading and compiling
	/// </summary>
	/// <param name="vertexPath">Vertex shader file path</param>
	/// <param name="fragmentPath">Fragment shader file path</param>
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// code taken from the file as a string
		std::string vertexCode;			
		std::string fragmentCode;
		// file stream from wich load the code
		std::ifstream vShaderFile;		 
		std::ifstream fShaderFile;

		// check if the ifstram can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		// try open vert and frag files and store the code
		try
		{
			// open file
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			// create a stream buffer
			std::stringstream vShaderStream, fShaderStream; // buffer that contains a sequence of characters
			// read the text from the files and put it into the two streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close the files just opened
			vShaderFile.close();
			fShaderFile.close();
			// convert the stream (sequence o chars) into a string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		// convert string to char*
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// 2. compile shaders
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		// vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		// fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	~Shader()
	{
		// delete the program on destruction
		glDeleteProgram(ID);
	}

	/// <summary>
	/// Returns the shader's program ID
	/// </summary>
	GLuint getID()
	{
		return ID;
	}

	/// <summary>
	/// Enables the shader in the current context
	/// </summary>
	void use()
	{
		glUseProgram(ID);
	}

	/// <summary>
	/// Sets a bool uniform
	/// </summary>
	/// <param name="name">Name of the uniform</param>
	/// <param name="value">New value</param>
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	/// <summary>
	/// Sets an Int uniform
	/// </summary>
	/// <param name="name">Name of the uniform</param>
	/// <param name="value">New value</param>
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	/// <summary>
	/// Sets a Float uniform
	/// </summary>
	/// <param name="name">Name of the uniform</param>
	/// <param name="value">New value</param>
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

private:
	GLuint ID;

	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};

#endif // !SHADER_H

