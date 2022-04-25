#include "Display.h"
#include "Logger.h"
#include <string>

GLuint shader;
unsigned int texture, VAO;

Display::Display()
{
    glHelper::getInstance().glInit();
    window = glHelper::getInstance().window;

    texture = setupTexture();
    VAO = setupBuffers();
    shader = createShader();

    lasttime = glfwGetTime();
}

void Display::update(uint8_t* displayData1) {
    while (glfwGetTime() < lasttime + (1.0 / 60)) {
        //wait
    }
    lasttime += (1.0 / 60);

    Overlay::getInstance().update();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 240, 0, GL_RGB, GL_UNSIGNED_BYTE, displayData1);
    glGenerateMipmap(GL_TEXTURE_2D);

    glUseProgram(shader);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //setWindowFPS(window);
}

void Display::setWindowFPS(GLFWwindow* win)
{
    frames++;

    if (glfwGetTime() - lasttimeCounter >= 1.0) {
        char title[256];
        title[255] = '\0';

        snprintf(title, 255, "%s - [FPS: %3.2f]", "NESem", (float)frames);

        glfwSetWindowTitle(win, title);

        frames = 0;
        lasttimeCounter += 1.0;
    }
}

GLuint Display::createShader() {
    const char* fragment_shader =
        "#version 330 core\n"
        "out vec4 FragColor;"
        "in vec2 TexCoord;"
        "uniform sampler2D texture1;"
        "void main() {"
        "   FragColor = texture(texture1, TexCoord);"
        "}";

    const char* vertex_shader =
        "#version 400\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 2) in vec2 aTexCoord;"
        "out vec2 TexCoord;"
        "void main() {"
        "   gl_Position = vec4(aPos, 1.0);"
        "   TexCoord = aTexCoord;"
        "}";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    glUniform1i(glGetUniformLocation(shader_programme, "texture1"), 0);

    return shader_programme;  
}


unsigned int Display::setupTexture() {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return texture;
}

unsigned int Display::setupBuffers() {
    float vertices[] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}