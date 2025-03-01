#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

using namespace std;

// Шейдеры в виде строк
const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform float time;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)glsl";

const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;

uniform vec4 ourColor;

void main()
{
    FragColor = ourColor;
}
)glsl";

// Вершинные данные для квадрата
float vertices[] = {
    // Позиции
     0.5f,  0.5f, 0.0f,  // Верхний правый
     0.5f, -0.5f, 0.0f,  // Нижний правый
    -0.5f, -0.5f, 0.0f,  // Нижний левый
    -0.5f,  0.5f, 0.0f   // Верхний левый
};
unsigned int indices[] = {  // Позиции квадрата
    0, 1, 3,  // Первый треугольник
    1, 2, 3   // Второй треугольник
};

int main()
{
    // Инициализация GLFW
    if (!glfwInit())
    {
        cerr << "Failed to initialize GLFW" << endl;
        return -1;
    }

    // Настройка GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(512, 512, "Gray Square on Red Background", NULL, NULL);
    if (window == NULL)
    {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Инициализация GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cerr << "Failed to initialize GLEW" << endl;
        return -1;
    }

    // Настройка viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);


    // Генерация VAO, VBO, EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Привязка VAO
    glBindVertexArray(VAO);

    // Привязка и заполнение VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Привязка и заполнение EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Установка вершинных атрибутов
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Компиляция вершинного шейдера
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Проверка ошибок компиляции
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    // Компиляция фрагментного шейдера
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Проверка ошибок компиляции
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }

    // Линковка шейдеров в программу
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Проверка ошибок линковки
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    // Удаление шейдеров, так как они уже связаны в программу
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Цикл рендеринга
    while (!glfwWindowShouldClose(window))
    {
        // Обработка событий
        glfwPollEvents();

        // Очистка экрана
        glClearColor(0.9f, 0.1f, 0.2f, 1.0f); // Красный фон
        glClear(GL_COLOR_BUFFER_BIT);

        // Использование шейдерной программы
        glUseProgram(shaderProgram);

        // Установка начального цвета (0.1, 0.2, 0.8)
        glUniform4f(glGetUniformLocation(shaderProgram, "ourColor"), 0.1f, 0.2f, 0.8f, 1.0f);

        // Обновление uniform-переменной для анимации цвета
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.5f, greenValue, 0.5f, 1.0f); // Серый цвет с анимацией зелёного компонента

        // Привязка VAO
        glBindVertexArray(VAO);

        // Отрисовка квадрата
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Обмен буферов
        glfwSwapBuffers(window);
    }

    // Освобождение ресурсов
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Завершение работы GLFW
    glfwTerminate();
    return 0;
}