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
#include "Load.h"

#include <vector>

#define WIDTH 1600
#define HEIGHT 1200

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = false;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat T = 0.0f;

GLfloat starVertices[] = {
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

        0.0f, -0.5f,  0.0f,
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

GLfloat planeVertices[] = {
    // positions            // normals         // texture
     10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   5.0f,  0.0f,
    -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
    -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  5.0f,

     10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   5.0f,  0.0f,
    -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  5.0f,
     10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   5.0f,  5.0f
};

GLfloat cubeVertices[] = {
    // positions            // normals             // texture
    -1.0f, -1.0f, -1.0f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f, 
     1.0f,  1.0f, -1.0f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f, 
     1.0f, -1.0f, -1.0f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,         
     1.0f,  1.0f, -1.0f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f, 
    -1.0f, -1.0f, -1.0f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f, 
    -1.0f,  1.0f, -1.0f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f, 

    -1.0f, -1.0f,  1.0f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f, 
     1.0f, -1.0f,  1.0f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f, 
     1.0f,  1.0f,  1.0f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f, 
     1.0f,  1.0f,  1.0f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f, 
    -1.0f,  1.0f,  1.0f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f, 
    -1.0f, -1.0f,  1.0f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f, 
 
    -1.0f,  1.0f,  1.0f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f, 
    -1.0f,  1.0f, -1.0f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f, 
    -1.0f, -1.0f, -1.0f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f, 
    -1.0f, -1.0f, -1.0f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f, 
    -1.0f, -1.0f,  1.0f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f, 
    -1.0f,  1.0f,  1.0f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f, 
  
     1.0f,  1.0f,  1.0f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f, 
     1.0f, -1.0f, -1.0f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
     1.0f,  1.0f, -1.0f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,        
     1.0f, -1.0f, -1.0f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f, 
     1.0f,  1.0f,  1.0f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f, 
     1.0f, -1.0f,  1.0f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,     
  
    -1.0f, -1.0f, -1.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f, 
     1.0f, -1.0f, -1.0f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f, 
     1.0f, -1.0f,  1.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f, 
     1.0f, -1.0f,  1.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f, 
    -1.0f, -1.0f,  1.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f, 
    -1.0f, -1.0f, -1.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f, 
   
    -1.0f,  1.0f, -1.0f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f, 
     1.0f,  1.0f , 1.0f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,  
     1.0f,  1.0f, -1.0f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,       
     1.0f,  1.0f,  1.0f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,  
    -1.0f,  1.0f, -1.0f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,  
    -1.0f,  1.0f,  1.0f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f           
};

GLfloat skyboxVertices[] = {
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
      // position   // texture
     -1, -1, 0,     0, 1,
      1, -1, 0,     1, 1,
     -1,  1, 0,     0, 0,
      1, -1, 0,     1, 1,
      1,  1, 0,     1, 0,
     -1,  1, 0,     0, 0
};

GLfloat transparentVertices[] = {
    // positions         // texture 
    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
    1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };

static void glfwError(int id, const char* description)
{
    std::cout << description << std::endl;
}

class Skybox {

    GLuint skyboxVAO, skyboxVBO;
    GLuint cubemapTexture;
    Shader shader = Shader("shaders/skybox.vs", "shaders/skybox.fs");

public:

    Skybox() {
        // skybox VAO
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0);

        std::vector<std::string> faces
        {
            "textures/IceLake/posx.jpg",
            "textures/IceLake/negx.jpg",
            "textures/IceLake/posy.jpg",
            "textures/IceLake/negy.jpg",
            "textures/IceLake/posz.jpg",
            "textures/IceLake/negz.jpg"
        };
        cubemapTexture = loadCubemap(faces);
    }

    void Draw() {
        // depth. we need background
        glDepthFunc(GL_LEQUAL);  

        // texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glUniform1i(glGetUniformLocation(shader.Program, "ourTexture"), 0);
        shader.Use();
        glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // background
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        // cube
        glBindVertexArray(skyboxVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
    }

    ~Skybox() {
        glDeleteVertexArrays(1, &skyboxVAO);
        glDeleteBuffers(1, &skyboxVBO);
    }
};

class Billboard {
    GLuint billboardVAO, billboardVBO;
    GLuint billboardTexture;
    Shader shader = Shader("shaders/billboard.vs", "shaders/billboard.fs");

    std::vector<glm::vec3> billboards = {
        glm::vec3(-1.0f, 0.0f, 0.0f),
        //glm::vec3(-1.0f, 0.5f, -4.0f),
        glm::vec3(-2.0f, 0.0f, 2.0f),
        glm::vec3(-3.0f, 0.0f, 0.0f)
    };
public:
    Billboard() {
        glGenVertexArrays(1, &billboardVAO);
        glGenBuffers(1, &billboardVBO);
        glBindVertexArray(billboardVAO);
        glBindBuffer(GL_ARRAY_BUFFER, billboardVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(billboardVertices), &billboardVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0);

        billboardTexture = loadTexture("textures/amongus_red.png");
    }

    void Draw() {
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

        glBindTexture(GL_TEXTURE_2D, billboardTexture);
        glUniform1i(glGetUniformLocation(shader.Program, "ourTexture"), 1);
        shader.Use();
        glUniform1f(glGetUniformLocation(shader.Program, "size"), 0.4f);

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

        shader.setMat4("PVM", PV);
        shader.setVec3("cameraRight", camera.Right);
        shader.setVec3("cameraUp", camera.Up);
        glBindVertexArray(billboardVAO);

        // draw billboards
        for (int i = 0, n = billboards.size(); i < n; i++) {
            shader.setVec3("billboardPos", billboards[i]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glBindVertexArray(0);
    }

    ~Billboard() {
        glDeleteVertexArrays(1, &billboardVAO);
        glDeleteBuffers(1, &billboardVBO);
    }
};

class Transparent {
    GLuint transparentVAO, transparentVBO;
    GLuint transparentTexture;
    Shader shader = Shader("shaders/glass.vs", "shaders/glass.fs");

    std::vector<glm::vec3> windows
    {
        glm::vec3(-1.5f, 2.0f, -0.48f),
        glm::vec3(-0.3f, 2.0f, -2.3f),
        glm::vec3(0.5f, 2.0f, -0.6f),
        glm::vec3(0.5f, 2.0f, 0.0f),
        glm::vec3(-1.0f, 2.0f, -2.0f),
        glm::vec3(-2.5f, 2.0f, -1.3f),
        glm::vec3(-2.1f, 2.0f, 2.3f),
        glm::vec3(-3.5f, 2.0f, 0.6f),
    };

public:
    Transparent() {
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

        transparentTexture = loadTexture("textures/blue.png");
    }

    void Draw() {
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
        glm::mat4 model(1.0f);

        for (int i = 0, n = windows.size(); i + 1 < n; i++) {
            for (int j = i; j < n; j++) {
                glm::vec3 delta1 = windows[i] - camera.Position;
                glm::vec3 delta2 = windows[j] - camera.Position;
                if (dot(delta1, delta1) < dot(delta2, delta2)) {
                    glm::vec3 tmp = windows[i];
                    windows[i] = windows[j];
                    windows[j] = tmp;
                }
            }
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, transparentTexture);
        shader.setInt("texture1", 0);

        shader.Use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        glBindVertexArray(transparentVAO);
        
        for (int i = 0, n = windows.size(); i < n; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, windows[i]);
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glBindVertexArray(0);
    }

    ~Transparent() {
        glDeleteVertexArrays(1, &transparentVAO);
        glDeleteBuffers(1, &transparentVBO);
    }
};

class Star {
    GLuint starVAO, starVBO;
    Shader shader = Shader("shaders/star.vs", "shaders/star.fs");

public:
    Star() {
        glGenVertexArrays(1, &starVAO);
        glGenBuffers(1, &starVBO);
        glBindVertexArray(starVAO);
        glBindBuffer(GL_ARRAY_BUFFER, starVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(starVertices), starVertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0);
    }

    void Draw() {
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
        glm::mat4 model(1.0f);
        shader.Use();
        
        shader.setMat4("model", model);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        GLfloat greenValue = (sin(glfwGetTime()) / 2) + 0.5f;
        GLfloat redValue = (sin(glfwGetTime()) / 2) + 0.5f;
        GLint vertexColorLocation = glGetUniformLocation(shader.Program, "ourColor");
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
            glm::mat4 model(1.0f);
            model = glm::translate(model, cubePositions[i]);
            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 24);
        }
        glBindVertexArray(0);
    }

    ~Star() {
        glDeleteVertexArrays(1, &starVAO);
        glDeleteBuffers(1, &starVBO);
    }

};

class Smoke {
    GLuint posBuf[2], velBuf[2];
    GLuint particleArray[2];
    GLuint feedback[2], initVel, startTime[2];
    GLuint drawBuf = 1;
    GLuint renderSub, updateSub;

    int nParticles = 500;

    GLuint smokeTexture;

    ShaderSmoke shader = ShaderSmoke("shaders/smoke.vs", "shaders/smoke.fs");
public:
    Smoke() {
        glGenBuffers(2, posBuf);    
        glGenBuffers(2, velBuf);    
        glGenBuffers(2, startTime); 
        glGenBuffers(1, &initVel);  

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

        // first velocity buffer with random velocities
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

        // first start time 
        GLfloat* tdata = new GLfloat[nParticles];
        float ttime = 0.0f;
        float rate = 0.1f;
        for (int i = 0; i < nParticles; i++) {
            tdata[i] = ttime;
            ttime += rate;
        }
        glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), tdata);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // create vertex arrays for each set of buffers
        glGenVertexArrays(2, particleArray);

        // set up particle array 0
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

        // set up particle array 1
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

        // setup the feedback objects
        glGenTransformFeedbacks(2, feedback);

        // feedback 0
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[0]);

        // feedback 1
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[1]);

        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

        const char* outputNames[] = { "Position", "Velocity", "StartTime" };
        glTransformFeedbackVaryings(shader.Program, 3, outputNames, GL_SEPARATE_ATTRIBS);
        shader.Link();  // before link we need bind feedback var's name

        smokeTexture = loadTexture("textures/smoke.png");
        //float T = glfwGetTime();

        delete[] data;
        delete[] tdata;
    }

    void Draw() {
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
        glm::mat4 model(1.0f);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, smokeTexture);
        
        shader.Use();

        // we call in shader needed function in shader
        renderSub = glGetSubroutineIndex(shader.Program, GL_VERTEX_SHADER, "render");
        updateSub = glGetSubroutineIndex(shader.Program, GL_VERTEX_SHADER, "update");
        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &updateSub);       // update

        // set uniform
        shader.setInt("texture1", 0);
        shader.setFloat("ParticleLifetime", 10.0f);
        glm::vec3 Accel(0.0f, 0.1f, 0.0f);
        shader.setVec3("Accel", Accel);
        shader.setFloat("MinParticleSize", 20.0f);
        shader.setFloat("MaxParticleSize", 500.0f);
        shader.setFloat("Time", glfwGetTime());
        shader.setFloat("H", glfwGetTime() - T);
        T = glfwGetTime();

        // render normal
        glEnable(GL_PROGRAM_POINT_SIZE);
        glPointSize(10.0);
        
        glEnable(GL_RASTERIZER_DISCARD);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
        glBeginTransformFeedback(GL_POINTS);
        glBindVertexArray(particleArray[1 - drawBuf]);
        glDrawArrays(GL_POINTS, 0, nParticles);
        glEndTransformFeedback();

        glDisable(GL_RASTERIZER_DISCARD);

        // render 
        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &renderSub);

        // matrices
        model = glm::mat4(1.0f);
        glm::vec3 pos(3.0f, -0.5f, 2.0f);
        model = glm::translate(model, pos);
        glm::mat4 mv = view * model;
        shader.setMat4("MVP", projection * mv);
        
        glBindVertexArray(particleArray[drawBuf]);
        glDrawTransformFeedback(GL_POINTS, feedback[drawBuf]);

        // swap buffers
        drawBuf = 1 - drawBuf;

        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);
    }

    ~Smoke() {
        glDeleteBuffers(1, &initVel);
        glDeleteBuffers(2, posBuf);
        glDeleteBuffers(2, velBuf);    
        glDeleteBuffers(2, startTime);
    }
};

class ShadowScene {
    Shader object = Shader("shaders/object.vs", "shaders/object.fs");
    Shader shadow = Shader("shaders/shadow.vs", "shaders/shadow.fs");
    GLuint planeVAO, planeVBO, cubeVAO, cubeVBO;
    GLuint iceTexture;

    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    GLuint shadowFBO;
    GLuint shadowMap;
    glm::vec3 lightPos = glm::vec3(-2.0f, 5.0f, -2.0f);

    glm::vec3 cubePositions[6] = {
         glm::vec3(0.0f, 1.5f, 0.0),
         glm::vec3(1.3f, 0.0f, 1.0),
         glm::vec3(-1.0f, 0.0f, 2.0),
         glm::vec3(-2.0f, 0.0f, -1.0),
         glm::vec3(1.0f, 0.0f, -2.0),
         glm::vec3(-1.5f, 1.5f, 1.0),
    };

    glm::vec3 cubeSizes[6] = {
        glm::vec3(0.5f),
        glm::vec3(0.3f),
        glm::vec3(0.25),
        glm::vec3(0.25),
        glm::vec3(0.25),
        glm::vec3(0.25),
    };

    GLfloat cubeAngle[6] = {
        30.0f,
        0.0f,
        60.0f,
        30.0f,
        0.0f,
        45.0f,
    };
    
public:
    ShadowScene() {
        // plane VAO
        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindVertexArray(0);

        // cube VAO
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        iceTexture = loadTexture("textures/show.jpg");

        // configure map
        glGenTextures(1, &shadowMap);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        // configure fbo bind frame buffer to texture
        glGenFramebuffers(1, &shadowFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // shader configuration
        object.Use();
        object.setInt("diffuseTexture", 0);
        object.setInt("shadowMap", 1);
    }
    
    void Draw() {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // at first we draw shadow map to texture. then we use this texture to draw normal scene 
        shadow.Use();

        // matrices. we draw shadow on texture from light's perspective
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightPV;
        float near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightPV = lightProjection * lightView;
        shadow.setMat4("lightPV", lightPV);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, iceTexture);

        // ----- render ----- shadow shader
        // plane
        glm::mat4 model = glm::mat4(1.0f);
        shadow.setMat4("model", model);
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // cubes
        for (int i = 0; i < 6; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::scale(model, cubeSizes[i]);
            model = glm::rotate(model, glm::radians(cubeAngle[i]), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));

            shadow.setMat4("model", model);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        // now we draw normal scene and use our texture with shadow (texture map)
        glViewport(0, 0, WIDTH, HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        object.Use();

        // matrices
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        object.setMat4("projection", projection);
        object.setMat4("view", view);

        // light uniforms
        object.setVec3("viewPos", camera.Position);
        object.setVec3("lightPos", lightPos);
        object.setMat4("lightPV", lightPV);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, iceTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadowMap);

        // ----- render ----- object shader
        // plane
        model = glm::mat4(1.0f);
        object.setMat4("model", model);
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // cubes
        for (int i = 0; i < 6; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::scale(model, cubeSizes[i]);
            model = glm::rotate(model, glm::radians(cubeAngle[i]), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));

            object.setMat4("model", model);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }
    }
    
    ~ShadowScene() {
        glDeleteVertexArrays(1, &planeVAO);
        glDeleteBuffers(1, &planeVBO);
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteBuffers(1, &cubeVBO);
        glDeleteTextures(1, &shadowMap);
        glDeleteFramebuffers(1, &shadowFBO);
    }
};


int main()
{
    int width = 1600;
    int height = 1200;
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
   
    Skybox skybox;
    Star bear;
    Billboard AmongUs;
    Transparent Window;
    Smoke cloud;
    ShadowScene planeAndCubes;
    
    // play cycle   
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

        // draw here

        planeAndCubes.Draw();
        bear.Draw();
        skybox.Draw();
        AmongUs.Draw();
        Window.Draw();
        cloud.Draw();
        
        // we have 2 buffers (front back). we see front buffer when back buffer has been drowing
        // then swap buffers
        glfwSwapBuffers(window);
    }
    
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
