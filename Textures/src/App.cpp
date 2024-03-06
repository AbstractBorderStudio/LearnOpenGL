// system includes
#include <iostream>

// opengl includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void RenderLoop(GLFWwindow* window);
void Draw(GLFWwindow* window, Shader shader, GLuint VAO);
void input_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// data
float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};
const unsigned int indices[] = {  // note that we start from 0!
    0, 1, 2,   // first triangle
    0, 2, 3
};

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
    // generating a texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // loading texture source
    int width, height, nChannels;
    const char* path = "./resources/textures/container.jpg";
    unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);
    if (data)
    {
        // creating the texture from the source
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // creating the mipmap for the texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load Texture" << std::endl;
    }


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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // setup Vertex Attrib pointers to pass COLOR DATA to the GPU
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // set attrib pointer per location 2 for texPosition
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // status check variables
    GLint success;
    char infoLog[512];


    // create shader
    Shader shader(
        "./resources/shaders/vert.vs",
        "./resources/shaders/frag.fs"
    );


    // render loop

    while (!glfwWindowShouldClose(window))
    {
        // render
        Draw(window, shader, VAO);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwPollEvents();
    }

    // cleanup
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

static float val = 0.0;

void Draw(GLFWwindow* window, Shader shader, GLuint VAO)
{
    // clear frame buffer
    glClear(GL_COLOR_BUFFER_BIT);
    // set frame buffer color
    glClearColor(0.1, 0.4, 0.5, 1.0);

    shader.use();
    shader.setFloat("uTheta", val);
    val = glfwGetTime();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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