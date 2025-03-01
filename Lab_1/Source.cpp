#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

using namespace std;

// ������� � ���� �����
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

// ��������� ������ ��� ��������
float vertices[] = {
    // �������
     0.5f,  0.5f, 0.0f,  // ������� ������
     0.5f, -0.5f, 0.0f,  // ������ ������
    -0.5f, -0.5f, 0.0f,  // ������ �����
    -0.5f,  0.5f, 0.0f   // ������� �����
};
unsigned int indices[] = {  // ������� ��������
    0, 1, 3,  // ������ �����������
    1, 2, 3   // ������ �����������
};

int main()
{
    // ������������� GLFW
    if (!glfwInit())
    {
        cerr << "Failed to initialize GLFW" << endl;
        return -1;
    }

    // ��������� GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // �������� ����
    GLFWwindow* window = glfwCreateWindow(512, 512, "Gray Square on Red Background", NULL, NULL);
    if (window == NULL)
    {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // ������������� GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        cerr << "Failed to initialize GLEW" << endl;
        return -1;
    }

    // ��������� viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);


    // ��������� VAO, VBO, EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // �������� VAO
    glBindVertexArray(VAO);

    // �������� � ���������� VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // �������� � ���������� EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // ��������� ��������� ���������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // ���������� ���������� �������
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // �������� ������ ����������
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    // ���������� ������������ �������
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // �������� ������ ����������
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }

    // �������� �������� � ���������
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // �������� ������ ��������
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    // �������� ��������, ��� ��� ��� ��� ������� � ���������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ���� ����������
    while (!glfwWindowShouldClose(window))
    {
        // ��������� �������
        glfwPollEvents();

        // ������� ������
        glClearColor(0.9f, 0.1f, 0.2f, 1.0f); // ������� ���
        glClear(GL_COLOR_BUFFER_BIT);

        // ������������� ��������� ���������
        glUseProgram(shaderProgram);

        // ��������� ���������� ����� (0.1, 0.2, 0.8)
        glUniform4f(glGetUniformLocation(shaderProgram, "ourColor"), 0.1f, 0.2f, 0.8f, 1.0f);

        // ���������� uniform-���������� ��� �������� �����
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.5f, greenValue, 0.5f, 1.0f); // ����� ���� � ��������� ������� ����������

        // �������� VAO
        glBindVertexArray(VAO);

        // ��������� ��������
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // ����� �������
        glfwSwapBuffers(window);
    }

    // ������������ ��������
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // ���������� ������ GLFW
    glfwTerminate();
    return 0;
}