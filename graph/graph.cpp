// graph.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

//#include <iostream>
#define GLEW_STATIC
#include<GL/glew.h>

#include<GL/freeglut.h>
#include<glm.hpp>
#include<gtx/transform.hpp>


//int main()
//{
//    std::cout << "Hello World!\n";
//}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.


#include <iostream>

//#include <freeglut.h>

using namespace std;

float WinWid = 400, WinHei = 400;

float angle = 0.0, scale = 1.0;

void timer(int value) {

	glColor3f(0.5, 1.0, 1.0);

	angle++;

	glRotatef(15.0, 10.0, 20.0, 1.0);

	glutPostRedisplay();

	glutTimerFunc(70, timer, 0);

}

void init() {

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glOrtho(-WinWid / 2, WinWid / 2, -WinHei / 2, WinHei / 2, -200.0, 200.0);

	glMatrixMode(GL_MODELVIEW);

}

void draw() {

	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();

	scale = 1.0 - abs(sin(3.14 * angle / 90.0) / sqrt(3.0) / 2);

	glRotatef(angle, 0.0, 0.0, 1.0);

	glScalef(scale, scale, 1.0);

	glBegin(GL_LINES);

	for (float i = -WinWid / 2; i <= WinWid / 2; i += 20.0) {

		glVertex2f(i, -WinHei / 2);

		glVertex2f(i, WinHei / 2);

	}

	for (float i = -WinWid / 2; i <= WinWid / 2; i += 20.0) {

		glVertex2f(-WinWid / 2, i);

		glVertex2f(WinWid / 2, i);

	}

	glEnd();

	glPopMatrix();

	glutSwapBuffers();

}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(WinWid, WinHei);

	glutInitWindowPosition(400, 150);

	glutCreateWindow("WebWindow");

	glutDisplayFunc(draw);

	glutTimerFunc(1000, timer, 0);

	init();

	glutMainLoop();
}


