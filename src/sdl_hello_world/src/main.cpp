#include "Display.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <iostream>


int main(int argc, char* argv[])
{
    Display display(800, 600, "My Display");

    Vertex vertices[] = {
        Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f)), // 左下
        Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f)),  // 右下
        Vertex(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec2(0.5f, 1.0f))    // 顶部居中
    };

    Mesh mesh(vertices, 3);

    Shader shader(R"(.\assert\shader\basicShader)");

    Texture texture(R"(.\assert\textures\container.jpg)");

    while (!display.isClosed())
    {
        display.clear(0.f, 0.15f, 0.3f, 1.f);

        shader.bind();
        texture.bind();
        mesh.draw();

        display.update();
    }

    getchar();
    return 0;
}
