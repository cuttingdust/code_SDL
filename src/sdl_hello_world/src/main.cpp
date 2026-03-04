#include "Camera.h"
#include "Display.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

#include <iostream>
#include <vector>

#define WIDTH  800
#define HEIGHT 600

int main(int argc, char* argv[])
{
    Display display(800, 600, "My Display");

    // ========== 创建第一个三角形（红色纹理） ==========
    Vertex vertices1[] = {
        Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f)), // 左下
        Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f)),  // 右下
        Vertex(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec2(0.5f, 1.0f))    // 顶部居中
    };
    Mesh mesh1(vertices1, 3);

    // ========== 创建第二个三角形（放在右边，稍微靠后） ==========
    Vertex vertices2[] = {
        Vertex(glm::vec3(-0.3f, -0.3f, 0.5f), glm::vec2(0.0f, 0.0f)), // 左下
        Vertex(glm::vec3(0.3f, -0.3f, 0.5f), glm::vec2(1.0f, 0.0f)),  // 右下
        Vertex(glm::vec3(0.0f, 0.3f, 0.5f), glm::vec2(0.5f, 1.0f))    // 顶部居中
    };
    Mesh mesh2(vertices2, 3);

    // ========== 创建第三个三角形（放在左边，更靠前） ==========
    Vertex vertices3[] = {
        Vertex(glm::vec3(-0.4f, -0.4f, -0.5f), glm::vec2(0.0f, 0.0f)), // 左下
        Vertex(glm::vec3(0.4f, -0.4f, -0.5f), glm::vec2(1.0f, 0.0f)),  // 右下
        Vertex(glm::vec3(0.0f, 0.4f, -0.5f), glm::vec2(0.5f, 1.0f))    // 顶部居中
    };
    Mesh mesh3(vertices3, 3);

    Shader  shader(R"(.\assert\shader\basicShader)");
    Texture texture(R"(.\assert\textures\container.jpg)");

    // 初始化摄像机
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f),  /// 位置
                  glm::vec3(0.0f, 0.0f, -1.0f), /// 前方方向
                  glm::vec3(0.0f, 1.0f, 0.0f),  /// 上方方向
                  (float)WIDTH / (float)HEIGHT, /// 宽高比
                  60.0f,                        /// FOV
                  0.1f,                         /// 近平面
                  1000.0f                       /// 远平面
    );

    // 为每个三角形创建独立的变换对象
    Transform transform1; // 中心三角形
    Transform transform2; // 右边三角形
    Transform transform3; // 左边三角形

    // 设置第二个三角形的位置（右边）
    transform2.setPos(glm::vec3(1.2f, 0.0f, 0.0f));

    // 设置第三个三角形的位置（左边）
    transform3.setPos(glm::vec3(-1.2f, 0.0f, 0.0f));

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    float counter = 0.f;
    std::cout << "开始渲染循环... 按窗口关闭按钮退出" << std::endl;
    std::cout << "三个三角形应该位于：中心、右边、左边" << std::endl;

    while (!display.isClosed())
    {
        display.clear(0.f, 0.15f, 0.3f, 1.f);

        // 中心三角形：绕Z轴旋转
        float angle = counter * 2.f;
        transform1.setRot(glm::vec3(0.f, 0.f, angle));

        // 右边三角形：绕Y轴旋转（产生不同的视觉效果）
        transform2.setRot(glm::vec3(0.f, angle * 0.5f, 0.f));

        // 左边三角形：上下浮动 + 旋转
        float yOffset = std::sin(counter) * 0.5f;
        transform3.setPos(glm::vec3(-1.2f, yOffset, 0.0f));
        transform3.setRot(glm::vec3(angle, angle * 0.3f, 0.f));

        shader.bind();
        texture.bind();

        // 绘制三个三角形
        shader.update(transform1, camera);
        mesh1.draw();

        shader.update(transform2, camera);
        mesh2.draw();

        shader.update(transform3, camera);
        mesh3.draw();

        display.update();

        counter += 0.01f;
    }

    std::cout << "\n渲染循环结束" << std::endl;
    getchar();
    return 0;
}
