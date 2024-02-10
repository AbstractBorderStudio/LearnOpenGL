// EBO EXERCISE 1
/*Try to draw 2 triangles next to each other using 
glDrawArrays by adding more vertices to your data.*/


// system includes
#include <iostream>

// opengl includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void input_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// vertices
const float firstTriangle[] = {
	// TRIANGLE 1
   -0.5f, -0.25f, 0.0f,
	0.0f, -0.25f, 0.0f,
   -0.25f, 0.25f, 0.0f
};
const float secondTriangle[] = {
	 // TRIANGLE 1
	 0.5f, -0.25f, 0.0f,
	 0.0f, -0.25f, 0.0f,
	 0.25f, 0.25f, 0.0f	
};

// shaders sources
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

const char* altFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.3f, 1.0f, 0.8f, 1.0f);\n"
"}\0";

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	// glfw :: initialization
	if (!glfwInit())
	{
		std::cerr << "ERROR::GLFW::INITIALIZATION_FAILED\n";
	}

	// glfw :: settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window and context
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "EBO - press W for wireframe and F for fill. ESC to close.", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	// set window resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// kayboard input callback
	glfwSetKeyCallback(window, input_keyCallback);

	// load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}

	// set OpenGL Viewport size
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// status check variables
	GLint success;
	char infoLog[512];

	// create and compile shaders
	// vertex
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "LOG::SHADER::VERTEX::COMPILATION_SUCCESS\n";
	}
	// fragment
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "LOG::SHADER::FRAGMENT::COMPILATION_SUCCESS\n";
	}

	// second fragment
	GLuint fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &altFragmentShaderSource, NULL);
	glCompileShader(fragmentShader2);

	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "LOG::SHADER::FRAGMENT::COMPILATION_SUCCESS\n";
	}

	// create shader program
	GLuint shaderProgram1, shaderProgram2;
	shaderProgram1 = glCreateProgram();
	// link vertex and fragment shaders
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShader);
	glLinkProgram(shaderProgram1);

	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
		std::cout << "LOG::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "LOG::SHADER::PROGRAM::LINKING_SUCCESS\n";
	}

	shaderProgram2 = glCreateProgram();
	// link vertex and fragment shaders
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "LOG::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "LOG::SHADER::PROGRAM::LINKING_SUCCESS\n";
	}

	// delete shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);

	// SOLUTION =========================================================================

	// generate buffers
	GLuint VAOs[2], VBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	// bind first VAO
	glBindVertexArray(VAOs[0]);
	// bind first VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	// passa data
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	// setup vertex attrib for the first VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// redo for the second VAO e VBO
	// bind second VAO
	glBindVertexArray(VAOs[1]);
	// bind second VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	// passa data
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	// setup vertex attrib for the first VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// SOLUTION =========================================================================

	// set wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// set the shader program
		glUseProgram(shaderProgram1);
		// SOLUTION =========================================================================
		
		// bind the first vao and draw the dirst triangle
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram2);

		// bind second vao
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// SOLUTION =========================================================================

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, VAOs);
	glDeleteProgram(shaderProgram1);
	glDeleteProgram(shaderProgram2);

	// glfw :: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();

	std::cout << "LOG::APP::CLOSED_SUCCESS\n";
	exit(EXIT_SUCCESS); // app closed successfully
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// update 
	glViewport(0, 0, width, height);
}

void input_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// input
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// switch between wireframe and fill
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}