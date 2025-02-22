#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>

int main() {
    // Инициализация GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Создание окна
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(512, 512, "OpenGL Example", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Инициализация GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Основной цикл
    while (!glfwWindowShouldClose(window)) {
        // Очистка экрана
        glClearColor(0.9f, 0.1f, 0.2f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Рисование квадрата
        glBegin(GL_QUADS);
        glColor3f(0.1f, 0.2f, 0.8f); // Цвет заливки
        glVertex2f(-0.5f, 0.5f);     // Верхний левый угол
        glVertex2f(-0.5f, -0.5f);    // Нижний левый угол
        glVertex2f(0.5f, -0.5f);     // Нижний правый угол
        glVertex2f(0.5f, 0.5f);      // Верхний правый угол
        glEnd();

        // Обмен буферов
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Завершение работы
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}