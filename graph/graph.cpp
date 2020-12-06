#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "ShaderSmoke.h"

#include <vector>
#include <map>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define WIDTH 800
#define HEIGHT 600

float randFloat() {
    return ((float)rand() / RAND_MAX);
}

//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = false;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

/************* TRIANGLE ***************/
    // vertex data
GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

GLfloat square_vert[] = {
    // Позиции            // Цвета            // Текстурные координаты
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Верхний левый
};

GLuint indices[] = {
    0, 1, 3,   // Первый треугольник
    1, 2, 3    // Второй треугольник
};

GLfloat star_vert[] = {
        0.0f,  0.5f,  0.0f, 
        0.0f,  0.0f,  0.5f,
        0.5f,  0.0f,  0.0f,

        0.0f,  0.5f,  0.0f,
        0.0f,  0.0f,  0.5f,
       -0.5f,  0.0f,  0.0f,

        0.0f,  0.5f,  0.0f,
        0.0f,  0.0f, -0.5f,
        0.5f,  0.0f,  0.0f,

        0.0f,  0.5f,  0.0f,
        0.0f,  0.0f, -0.5f,
       -0.5f,  0.0f,  0.0f,

        0.0f,  -0.5f,  0.0f,
        0.0f,  0.0f,  0.5f,
        0.5f,  0.0f,  0.0f,

        0.0f, -0.5f,  0.0f,
        0.0f,  0.0f,  0.5f,
       -0.5f,  0.0f,  0.0f,

        0.0f, -0.5f,  0.0f,
        0.0f,  0.0f, -0.5f,
        0.5f,  0.0f,  0.0f,

        0.0f, -0.5f,  0.0f,
        0.0f,  0.0f, -0.5f,
       -0.5f,  0.0f,  0.0f,
};

GLfloat cube_vert[] = {
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

GLfloat plane[] = {
    -18, 0, -18,   0,   0,  
     18, 0,  18,   1, 1,  
     18, 0, -18,   1,  0,  
     18, 0,  18,   1,  1,  
    -18, 0, -18,   0,   0,  
    -18, 0,  18,   0,  1, 
};

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

GLfloat billboardVertices[] = {
     -1, -1, 0,   0, 1,
      1, -1, 0,   1, 1,
     -1,  1, 0,   0, 0,
      1, -1, 0,   1, 1,
      1,  1, 0,   1, 0,
     -1,  1, 0,   0, 0
};

std::vector<glm::vec3> billboards = {
        glm::vec3(-1.0f, 1.0f, -4.0f),
        //glm::vec3(-1.0f, 0.5f, -4.0f),
        glm::vec3(-2.0f, 1.0f, -4.0f),
        glm::vec3(-3.0f, 1.0f, -4.0f)
};

float transparentVertices[] = {
    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
    1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

std::vector<glm::vec3> windows
{
    glm::vec3(-1.5f, 2.0f, -0.48f),
    glm::vec3(1.5f, 2.0f, 0.51f),
    glm::vec3(0.0f, 2.0f, 0.7f),
    glm::vec3(-0.3f, 2.0f, -2.3f),
    glm::vec3(0.5f, 2.0f, -0.6f)
};


GLuint loadTexture(char const* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_uc* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        switch (nrComponents) {
        case 1: format = GL_RED; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        default: format = 0;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }
    else std::cout << "Failed to load texture:" << std::endl << path << std::endl;

    stbi_image_free(data);

    return textureID;
}

GLuint loadCubemap(std::vector<std::string> faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < 6; i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

static void glfwError(int id, const char* description)
{
    std::cout << description << std::endl;
}

class Skybox {
public:
    GLuint skyboxVAO, skyboxVBO;
    GLuint cubemapTexture;
    Shader shader = Shader("shaders/skybox.vs", "shaders/skybox.fs");

    Skybox() {
        // skybox VAO
        //GLuint skyboxVAO, skyboxVBO;
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
        glBindVertexArray(0);
        //shader = Shader("shaders/skybox.vs", "shaders/skybox.fs");

        std::vector<std::string> faces
        {
            "textures/ArstaBridge/posx.jpg",
            "textures/ArstaBridge/negx.jpg",
            "textures/ArstaBridge/posy.jpg",
            "textures/ArstaBridge/negy.jpg",
            "textures/ArstaBridge/posz.jpg",
            "textures/ArstaBridge/negz.jpg"
        };
        cubemapTexture = loadCubemap(faces);
    }

    void Draw() {
        // draw skybox
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glUniform1i(glGetUniformLocation(shader.Program, "ourTexture"), 0);
        shader.Use();
        glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        // skybox cube
        glBindVertexArray(skyboxVAO);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
    }

    ~Skybox() {
        glDeleteVertexArrays(1, &skyboxVAO);
        glDeleteBuffers(1, &skyboxVBO);
    }
};

int main()
{
    int width = 800;
    int height = 600;
    glfwSetErrorCallback(&glfwError);
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(width, height, "kek", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    //glfwSetKeyCallback(window, key_callback);


    /***********************************************/
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    std::cout << vec.x << vec.y << vec.z << std::endl;
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;

    glm::vec3 cubePos(1.0f, 1.0f, 0.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePos);
    glm::vec3 position(-0.5f, -0.5f, -0.5f);
    glm::vec4 FragPos4 = model * glm::vec4(position, 1.0f);

    std::cout << FragPos4.x << FragPos4.y << FragPos4.z << std::endl;

    //////////////////////////////////////////////////

    Skybox skybox;

    /***************** SQUARE *********************/

    //GLuint textureCoal = loadTexture("textures/coal.jpg");
    GLuint textureSnow = loadTexture("textures/ice.jpg");

    GLuint EBO, starVAO, starVBO, planeVAO, planeVBO;
    glGenVertexArrays(1, &starVAO);
    glGenBuffers(1, &starVBO);
    //glGenBuffers(1, &EBO);
    glBindVertexArray(starVAO);
    //glBindVertexArray(planeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, starVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(star_vert), star_vert, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(1);
    // TexCoord attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0);

    Shader Star = Shader("shaders/star.vs", "shaders/star.fs");

    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0);

    Shader Plane = Shader("shaders/plane.vs", "shaders/plane.fs");

    /****************** LIGHTING **********************/

    GLuint cubeVAO, cubeVBO, lightVAO, lightVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vert), cube_vert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    


    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vert), cube_vert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    Shader Cube = Shader("shaders/square.vs", "shaders/square.fs");
    Shader Light = Shader("shaders/light.vs", "shaders/light.fs");

   

    /**************** BILLBOARD *******************/
    GLuint billboardVAO, billboardVBO;

    glGenVertexArrays(1, &billboardVAO);
    glGenBuffers(1, &billboardVBO);
    glBindVertexArray(billboardVAO);
    glBindBuffer(GL_ARRAY_BUFFER, billboardVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(billboardVertices), &billboardVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0);
    Shader Billboard("shaders/billboard.vs", "shaders/billboard.fs");

    GLuint billboardTexture = loadTexture("textures/amongus_red.png");

    // transparent VAO
    unsigned int transparentVAO, transparentVBO;
    glGenVertexArrays(1, &transparentVAO);
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    Shader Glass("shaders/glass.vs", "shaders/glass.fs");
    unsigned int transparentTexture = loadTexture("textures/blue.png");

    // SMOKE
    
    GLuint posBuf[2], velBuf[2];
    GLuint particleArray[2];
    GLuint feedback[2], initVel, startTime[2];
    GLuint drawBuf = 1, query;
    GLuint renderSub, updateSub;
    
    int nParticles = 500;

    // Generate the buffers
    glGenBuffers(2, posBuf);    // position buffers
    glGenBuffers(2, velBuf);    // velocity buffers
    glGenBuffers(2, startTime); // Start time buffers
    glGenBuffers(1, &initVel);  // Initial velocity buffer (never changes, only need one)

    
     // Allocate space for all buffers
    int size = nParticles * 3 * sizeof(GLfloat);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);
    
    // Fill the first velocity buffer with random velocities
    glm::vec3 v(0.0f);
    float velocity, theta, phi;
    GLfloat* data = new GLfloat[nParticles * 3];
    
    for (unsigned int i = 0; i < nParticles; i++) {
        theta = glm::mix(0.0f, glm::pi<float>() / 3.0f, randFloat());
        phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

        v.x = sinf(theta) * cosf(phi);
        v.y = cosf(theta);
        v.z = sinf(theta) * sinf(phi);

        velocity = glm::mix(0.1f, 0.3f, randFloat());
        v = glm::normalize(v) * velocity;

        data[3 * i] = v.x;
        data[3 * i + 1] = v.y;
        data[3 * i + 2] = v.z;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    
    //delete[] data;
    
    // Fill the first start time buffer
    //delete[] data;
    GLfloat* tdata = new GLfloat[nParticles];
    float ttime = 0.0f;
    float rate = 0.2f;
    for (int i = 0; i < nParticles; i++) {
        tdata[i] = ttime;
        ttime += rate;
    }
    glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //delete[] data;

    // Create vertex arrays for each set of buffers
    glGenVertexArrays(2, particleArray);

    // Set up particle array 0
    glBindVertexArray(particleArray[0]);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    // Set up particle array 1
    glBindVertexArray(particleArray[1]);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    // Setup the feedback objects
    glGenTransformFeedbacks(2, feedback);

    // Transform feedback 0
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[0]);

    // Transform feedback 1
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[1]);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

    GLint value;
    glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS, &value);
    printf("MAX_TRANSFORM_FEEDBACK_BUFFERS = %d\n", value);

    ShaderSmoke Smoke("shaders/smoke.vs", "shaders/smoke.fs");
    const char* outputNames[] = { "Position", "Velocity", "StartTime" };
    glTransformFeedbackVaryings(Smoke.Program, 3, outputNames, GL_SEPARATE_ATTRIBS);
    Smoke.Link();

    unsigned int smokeTexture = loadTexture("textures/smoke.png");
    float T = glfwGetTime();

    /*
    // Shadow
    GLfloat border[] = { 1.0f, 0.0f,0.0f,0.0f };
    GLuint shadowFBO, depthTex;
    int shadowMapWidth = 512, shadowMapHeight = 512;

    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, shadowMapWidth, shadowMapHeight);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

    // Assign the depth buffer texture to texture channel 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTex);

    // Create and set up the FBO
    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, depthTex, 0);

    GLenum drawBuffers[] = { GL_NONE };
    glDrawBuffers(1, drawBuffers);

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is complete.\n");
    }
    else {
        printf("Framebuffer is not complete.\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shader Shadow("shaders/glass.vs", "shaders/glass.fs");
    */
    
    /****************** PLAY CYCLE ********************/

    while (!glfwWindowShouldClose(window))     
    {
        // check events
        glfwPollEvents();

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        Do_Movement();


        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 view = glm::mat4(20.0f);
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        view = camera.GetViewMatrix();

        float screenWidth = 800.0f;
        float screenHeight = 600.0f;
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
        //glActiveTexture(GL_TEXTURE0);
        glm::mat4 model = glm::mat4(1.0f);
        
        
       glBindTexture(GL_TEXTURE_2D, textureSnow);
       glUniform1i(glGetUniformLocation(Plane.Program, "ourTexture"), 0);

        Plane.Use();
       glUniformMatrix4fv(glGetUniformLocation(Plane.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
       glUniformMatrix4fv(glGetUniformLocation(Plane.Program, "model") , 1, GL_FALSE, glm::value_ptr(model));
       glUniformMatrix4fv(glGetUniformLocation(Plane.Program, "projection") , 1, GL_FALSE, glm::value_ptr(projection));
       glBindVertexArray(planeVAO);
       glDrawArrays(GL_TRIANGLES, 0, 6);
       glBindVertexArray(0);
       
        
        Star.Use();
        glm::mat4 transform = glm::mat4(1.0f);
        
        GLint modelLoc = glGetUniformLocation(Star.Program, "model");
       glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        GLint viewLoc = glGetUniformLocation(Star.Program, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        GLint projectionLoc = glGetUniformLocation(Star.Program, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        GLfloat greenValue = (sin(glfwGetTime()) / 2) + 0.5f;
        GLfloat redValue = (sin(glfwGetTime()) / 2) + 0.5f;
        GLint vertexColorLocation = glGetUniformLocation(Star.Program, "ourColor");
        glUniform3f(vertexColorLocation, redValue, greenValue, 1.0f);

        glBindVertexArray(starVAO);

        glm::vec3 cubePositions[] = {
            glm::vec3(0.0f,  10.0f,  0.0f),
            glm::vec3(6.0f,  10.0f, 0.0f),
            glm::vec3(1.0f,  10.0f,  -3.0f),
            glm::vec3(4.7f,  10.0f,  -3.0f),
            glm::vec3(-3.0f,  10.0f,  2.0f),
            glm::vec3(-6.0f,  10.0f, 4.0f),
            glm::vec3(-11.0f,  10.0f,  4.0f),
        };
        for (GLuint i = 0; i < 7; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            //model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 24);
        }
        //glBindVertexArray(0);
        glBindVertexArray(0);

        /*********************** LIGHTING *************************/
        glm::vec3 cubeColor(0.0f, 0.0f, 1.0f);
        glm::vec3 lightColor(1.0f, 0.5f, 0.5f);
        glm::vec3 lightPos(1.0f, 1.0f, 1.0f);

        Cube.Use();
        glm::vec3 cubePos(1.0f, 1.0f, 0.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePos);
        glUniformMatrix4fv(glGetUniformLocation(Cube.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(Cube.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(Cube.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(glGetUniformLocation(Cube.Program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

        glUniform3f(glGetUniformLocation(Cube.Program, "objectColor"), cubeColor.x, cubeColor.y, cubeColor.z);
        glUniform3f(glGetUniformLocation(Cube.Program, "lightColor"), 1.0f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(Cube.Program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);


        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));

        
        Light.Use();
        glUniformMatrix4fv(glGetUniformLocation(Light.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(Light.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(Light.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);


       //draw skybox
        skybox.Draw();


        // billboard
        view = camera.GetViewMatrix();
        glBindTexture(GL_TEXTURE_2D, billboardTexture);
        glUniform1i(glGetUniformLocation(Billboard.Program, "ourTexture"), 1);
        Billboard.Use();
        glUniform1f(glGetUniformLocation(Billboard.Program, "size"), 0.4f);
        
        
        for (int i = 0, n = billboards.size(); i + 1 < n; i++) {
            for (int j = i; j < n; j++) {
                glm::vec3 delta1 = billboards[i] - camera.Position;
                glm::vec3 delta2 = billboards[j] - camera.Position;
                if (dot(delta1, delta1) < dot(delta2, delta2)) {
                    glm::vec3 tmp = billboards[i];
                    billboards[i] = billboards[j];
                    billboards[j] = tmp;
                }
            }
        }
       
        glm::mat4 PV = projection * view;

        Billboard.setMat4("PVM", PV);
        Billboard.setVec3("cameraRight", camera.Right);
        Billboard.setVec3("cameraUp", camera.Up);
        glBindVertexArray(billboardVAO);

        // draw billboards
        for (int i = 0, n = billboards.size(); i < n; i++) {
            Billboard.setVec3("billboardPos", billboards[i]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glBindVertexArray(0);
        
        view = camera.GetViewMatrix();

        // windows
        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < windows.size(); i++)
        {
            float distance = glm::length(camera.Position - windows[i]);
            sorted[distance] = windows[i];
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, transparentTexture);
        Glass.setInt("texture1", 0);
        Glass.Use();
        //glUniform1i(glGetUniformLocation(Billboard.Program, "ourTexture"), 1);
        //glBindTexture(GL_TEXTURE_2D, transparentTexture);
        //Glass.setInt("texture1", 1);
        Glass.setMat4("view", view);
        Glass.setMat4("projection", projection);
        //model = glm::mat4(1.0f);
        Glass.setMat4("model", model);
        glBindVertexArray(transparentVAO);
        //glBindTexture(GL_TEXTURE_2D, transparentTexture);
        
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            Glass.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        
        glBindVertexArray(0);
        //gluSphere()

        // SMOKE 
        
        glDisable(GL_DEPTH_TEST);
        //glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glPointSize(10.0f);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, smokeTexture);
        //prog.setUniform("ParticleTex", 0);
        //prog.setUniform("ParticleLifetime", 3.5f);
        //prog.setUniform("Gravity", vec3(0.0f, -0.2f, 0.0f));
        //glEnable(GL_DEPTH_TEST);
        Smoke.Use();

        renderSub = glGetSubroutineIndex(Smoke.Program, GL_VERTEX_SHADER, "render");
        updateSub = glGetSubroutineIndex(Smoke.Program, GL_VERTEX_SHADER, "update");

        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &updateSub);

        Smoke.setInt("texture1", 0);
        Smoke.setFloat("ParticleLifetime", 7.0f);
        //Smoke.setVec3("Gravity", glm::vec3(0.0f, -0.2f, 0.0f));
        //prog.setUniform("ParticleLifetime", 3.5f);
        //prog.setUniform("Accel", vec3(0.0f, -0.4f, 0.0f));
        glm::vec3 Accel(0.0f, 0.1f, 0.0f);
        Smoke.setVec3("Accel", Accel);
        Smoke.setFloat("MinParticleSize", 10.0f);
        Smoke.setFloat("MaxParticleSize", 200.0f);

        Smoke.setFloat("Time", glfwGetTime());
        Smoke.setFloat("H", glfwGetTime() - T);
        T = glfwGetTime();


        glEnable(GL_PROGRAM_POINT_SIZE);
        glPointSize(10.0);
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_RASTERIZER_DISCARD);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBeginTransformFeedback(GL_POINTS);
        glBindVertexArray(particleArray[1 - drawBuf]);
        glDrawArrays(GL_POINTS, 0, nParticles);
        glEndTransformFeedback();

        glDisable(GL_RASTERIZER_DISCARD);

        // Render pass
        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &renderSub);
        //glClear(GL_COLOR_BUFFER_BIT);
        model = glm::mat4(1.0f);
        glm::vec3 la(2.0f, 0.0f, 2.0f);
        model = glm::translate(model, la);
        //glClear(GL_COLOR_BUFFER_BIT);
        glm::mat4 mv = view * model;
        Smoke.setMat4("MVP", projection* mv);
       // prog.setUniform("Time", time);
       // prog.setUniform("H", deltaT);

        //model = glm::mat4(1.0f);
        //glm::vec3 la(4.0f, 0.0f, 4.0f);
        //model = glm::translate(model, la);
        //glClear(GL_COLOR_BUFFER_BIT);
        //glm::mat4 mv = view * model;
        //Smoke.setMat4("MVP", projection * mv);
        glBindVertexArray(particleArray[drawBuf]);
        glDrawTransformFeedback(GL_POINTS, feedback[drawBuf]);


        // swap buffers
        drawBuf = 1 - drawBuf;

        glBindVertexArray(0);
        
        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_RASTERIZER_DISCARD);
        // we have 2 buffers (front back). we see front buffer when back buffer has been drowing
        // then swap buffers
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1, &starVAO);
    glDeleteBuffers(1, &starVBO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &lightVBO);
    glDeleteVertexArrays(1, &transparentVAO);
    glDeleteBuffers(1, &transparentVBO);
    glDeleteVertexArrays(1, &billboardVAO);
    glDeleteBuffers(1, &billboardVBO);
    delete[] data;
    delete[] tdata;
    glDeleteBuffers(1, &initVel);   
    //glDeleteBuffers(2, &startTime); 
    //glDeleteVertexArrays(1, &particles);
    glfwTerminate();
    return 0;
}

void Do_Movement()
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //cout << key << endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}