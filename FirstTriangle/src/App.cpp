// system includes
#include <iostream>

// opengl includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void RenderLoop(GLFWwindow* window);
void Draw(GLFWwindow* window);
void inptu_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// data
float vertices[] = {
//    x,     y     z    // in NDC
    -0.5f, -0.5f, 0.0f, // vertex 1#
     0.5f, -0.5f, 0.0f, // vertex 2#
     0.0f,  0.5f, 0.0f  // vertex 3#
};

// shaders
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

int main(void)
{
    // init glfw
    if (!glfwInit())
    {
        std::cerr << "Could not load GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    // glfw settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window and context
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    glfwSetKeyCallback(window, inptu_keyCallback);

    // load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    // set OpenGL Viewport size
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // main loop
    RenderLoop(window);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();

    // app closed successfully
    exit(EXIT_SUCCESS);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // update 
    glViewport(0, 0, width, height);
    Draw(window);
}

void RenderLoop(GLFWwindow* window)
{
    // create Vertex Buffer in the GPU
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // create and compile shaders
    GLint success;
    char infoLog[512];
    // vertex
    unsigned int vertexShader;
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
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_VERTEX_SHADER);
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

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // render
        Draw(window);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwPollEvents();
    }
}

void Draw(GLFWwindow* window)
{
    // clear frame buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // set frame buffer color
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // swap buffer
    glfwSwapBuffers(window);
}

void inptu_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // input
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}