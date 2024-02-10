// system includes
#include <iostream>

// opengl includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void input_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// vertices
const float vertices[] = {
	  -0.25f, -0.25f, 0.0f,
	   0.0f,  -0.25f, 0.0f,
	   0.25f, -0.25f, 0.0f,
	  -0.125f, 0.0f,  0.0f,
	   0.125f, 0.0f,  0.0f,
	   0.0f,   0.25f, 0.0f
};
const unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 4,   // second
	3, 4, 5    // third
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "EBO - press W for wireframe and F for fill", NULL, NULL);
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

	// create shader program
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	// link vertex and fragment shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "LOG::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "LOG::SHADER::PROGRAM::LINKING_SUCCESS\n";
	}

	// delete shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// generate buffers
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind VAO
	glBindVertexArray(VAO);
	// setup VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// setup EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// setup vertex attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// set wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// set the shader program
		glUseProgram(shaderProgram);
		// bind current VAO
		glBindVertexArray(VAO);
		// draw the nine indices (three triangles)
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

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