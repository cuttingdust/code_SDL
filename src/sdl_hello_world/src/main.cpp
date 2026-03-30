#include "Camera.h"
#include "Display.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

#include <algorithm>
#include <iostream>

#define SDL_MAIN_HANDLED /// 告诉 SDL 我们自己处理 main 函数
#include <SDL2/SDL.h>

#define WIDTH  800
#define HEIGHT 600

// 创建立方体的顶点数据
std::vector<Vertex> createCubeVertices()
{
    std::vector<Vertex> vertices;

    // 前面
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f)));

    // 后面
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)));

    // 左面
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)));

    // 右面
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)));

    // 上面
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 0.0f)));

    // 下面
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 1.0f)));
    vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f)));

    return vertices;
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "zh_CN.UTF-8");

    Display display(800, 600, "Camera Comparison");

    /// 创建立方体
    auto cubeVertices = createCubeVertices();
    Mesh cubeMesh(cubeVertices.data(), cubeVertices.size());

    /// 创建着色器和纹理
    Shader  shader(R"(.\assert\shader\basicShader)");
    Texture texture(R"(.\assert\textures\container.jpg)");

    /// 创建透视摄像机
    PerspectiveCamera perspCam(glm::vec3(0.0f, 0.0f, 3.0f),  // 位置
                               glm::vec3(0.0f, 0.0f, -1.0f), // 前方
                               glm::vec3(0.0f, 1.0f, 0.0f),  // 上方
                               (float)WIDTH / (float)HEIGHT, // 宽高比
                               60.0f                         // FOV
    );

    /// 创建正交摄像机
    OrthographicCamera orthoCam = OrthographicCamera::createWithSize(2.0f, // size = 2，视野高度为4个单位
                                                                     (float)WIDTH / (float)HEIGHT, // 宽高比
                                                                     0.1f,                         // 近平面
                                                                     1000.0f                       // 远平面
    );
    orthoCam.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));

    /// 创建三个物体来演示深度效果
    Transform cube1; /// 中心
    Transform cube2; /// 右边远处
    Transform cube3; /// 左边近处

    cube2.setPos(glm::vec3(1.5f, 0.0f, 1.0f));   // 右边，Z正方向（更远）
    cube3.setPos(glm::vec3(-1.5f, 0.0f, -1.0f)); // 左边，Z负方向（更近）

    cube2.setScale(glm::vec3(0.7f)); // 稍微小一点
    cube3.setScale(glm::vec3(0.7f));

    std::cout << "===== 摄像机对比演示 =====" << std::endl;
    std::cout << "按 1 键: 透视摄像机 (近大远小)" << std::endl;
    std::cout << "按 2 键: 正交摄像机 (大小不变)" << std::endl;
    std::cout << "按 + / - 键: 调整正交摄像机视野大小" << std::endl;
    std::cout << "按 W/A/S/D: 移动摄像机位置" << std::endl;
    std::cout << "========================" << std::endl;

    bool  usePerspective = true;
    float orthoSize      = 2.0f;
    float counter        = 0.0f;

    while (!display.isClosed())
    {
        display.clear(0.0f, 0.15f, 0.3f, 1.0f);

        // 处理键盘输入
        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        // 切换摄像机类型
        if (keystate[SDL_SCANCODE_1])
            usePerspective = true;
        if (keystate[SDL_SCANCODE_2])
            usePerspective = false;

        // 调整正交摄像机大小
        if (keystate[SDL_SCANCODE_EQUALS] || keystate[SDL_SCANCODE_KP_PLUS])
        {
            orthoSize -= 0.02f;
            orthoSize = std::max(orthoSize, 0.5f);
            orthoCam.setSize(orthoSize, (float)WIDTH / (float)HEIGHT);
        }
        if (keystate[SDL_SCANCODE_MINUS] || keystate[SDL_SCANCODE_KP_MINUS])
        {
            orthoSize += 0.02f;
            orthoSize = std::min(orthoSize, 5.0f);
            orthoCam.setSize(orthoSize, (float)WIDTH / (float)HEIGHT);
        }

        /// 移动摄像机
        float     moveSpeed = 0.05f;
        glm::vec3 camPos    = usePerspective ? perspCam.getPosition() : orthoCam.getPosition();

        if (keystate[SDL_SCANCODE_W])
            camPos.z -= moveSpeed;
        if (keystate[SDL_SCANCODE_S])
            camPos.z += moveSpeed;
        if (keystate[SDL_SCANCODE_A])
            camPos.x -= moveSpeed;
        if (keystate[SDL_SCANCODE_D])
            camPos.x += moveSpeed;

        if (usePerspective)
        {
            perspCam.setPosition(camPos);
            perspCam.setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        }
        else
        {
            orthoCam.setPosition(camPos);
        }

        /// 让物体旋转
        cube1.setRot(glm::vec3(0.0f, counter * 0.5f, 0.0f));
        cube2.setRot(glm::vec3(0.0f, counter * 0.8f, 0.0f));
        cube3.setRot(glm::vec3(0.0f, counter * 1.2f, 0.0f));

        shader.bind();
        texture.bind();

        /// 根据摄像机类型渲染
        if (usePerspective)
        {
            shader.update(cube1, perspCam);
            cubeMesh.draw();
            shader.update(cube2, perspCam);
            cubeMesh.draw();
            shader.update(cube3, perspCam);
            cubeMesh.draw();

            static int frame = 0;
            if (++frame % 60 == 0)
            {
                glm::vec3 pos = perspCam.getPosition();
                std::cout << "\r使用: 透视摄像机 | 位置: (" << pos.x << ", " << pos.y << ", " << pos.z << ") | FOV: 60°"
                          << std::flush;
            }
        }
        else
        {
            shader.update(cube1, orthoCam);
            cubeMesh.draw();
            shader.update(cube2, orthoCam);
            cubeMesh.draw();
            shader.update(cube3, orthoCam);
            cubeMesh.draw();

            static int frame = 0;
            if (++frame % 60 == 0)
            {
                glm::vec3 pos = orthoCam.getPosition();
                std::cout << "\r使用: 正交摄像机 | 位置: (" << pos.x << ", " << pos.y << ", " << pos.z
                          << ") | 大小: " << orthoSize << std::flush;
            }
        }

        display.update();
        counter += 0.01f;
    }

    std::cout << "\n程序退出" << std::endl;
    getchar();
    return 0;
}
