/*
#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <iostream>


static void glfwError(int id, const char* description)
{
    std::cout << description << std::endl;
}


int main()
{
 
        std::cout << "Hello World!\n";	    
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

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            std::cout << "Failed to initialize GLEW" << std::endl;
            return -1;
        }

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        while (!glfwWindowShouldClose(window))     // play cycle
        {
            // check events
            glfwPollEvents();

            // drow here
            glPointSize(2);
            glBegin(GL_POINTS);
            glColor3d(1, 0, 0);
            glVertex3d(-4.5, 4, 0); // первая точка
            glColor3d(0, 1, 0);
            glVertex3d(-4, 4, 0);   // вторая точка
            glColor3d(0, 0, 1);     // третья
            glVertex3d(-3.5, 4, 0);
            glEnd();
            glPointSize(5);
            glBegin(GL_POINTS);
            glColor3d(1, 0, 0);
            glVertex3d(-2, 4, 0); // первая точка
            glColor3d(0, 1, 0);
            glVertex3d(-1, 4, 0);   // вторая точка
            glColor3d(0, 0, 1);     // третье
            glVertex3d(0, 4, 0);
            glEnd();
            glPointSize(10);
            glEnable(GL_POINT_SMOOTH);
            glBegin(GL_POINTS);
            glColor3d(1, 0, 0);
            glVertex3d(2, 4, 0); // первая точка
            glColor3d(0, 1, 0);
            glVertex3d(3, 4, 0);   // вторая точка
            glColor3d(0, 0, 1);     // третья
            glVertex3d(4, 4, 0);
            glEnd();
            glDisable(GL_POINT_SMOOTH);
            // we have 2 buffers (front back). we see front buffer when back buffer has been drowing
            // then swap buffers
            glfwSwapBuffers(window);
        }

        glfwTerminate();
        return 0;
    
}
*/