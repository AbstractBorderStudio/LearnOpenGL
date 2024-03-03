// system includes
#include <iostream>

// opengl includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void RenderLoop(GLFWwindow* window);
void Draw(GLFWwindow* window, GLuint shaderProgram, GLuint VAO);
void input_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// data
float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
};
const unsigned int indices[] = {  // note that we start from 0!
    0, 1, 2,   // first triangle
};

// shaders
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aCol;\n"
    "out vec3 vCol;\n"
    "void main()\n"
    "{\n"
    "   vCol = aCol;\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "in vec3 vCol;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(vCol, 1.0);\n"
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
    glfwSetKeyCallback(window, input_keyCallback);

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
}

void RenderLoop(GLFWwindow* window)
{
    // create Vertex Array Object
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    // create Vertex Buffer in the GPU
    glGenBuffers(1, &VBO);
    // create Element Object Buffer
    glGenBuffers(1, &EBO);


    // bind VAO :: everything after that is related to this VAO ::
    glBindVertexArray(VAO);
    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // copy data inside the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // setup EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // setup Vertex Attrib pointers to pass POSITION DATA to the GPU
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // setup Vertex Attrib pointers to pass COLOR DATA to the GPU
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // render
        Draw(window, shaderProgram, VAO);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void Draw(GLFWwindow* window, GLuint shaderProgram, GLuint VAO)
{
    // clear frame buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // set frame buffer color
    glClearColor(0.1, 0.4, 0.5, 1.0);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    // swap buffer
    glfwSwapBuffers(window);
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