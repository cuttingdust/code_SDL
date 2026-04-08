#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>

// #define GLUT_KEY_ESCAPE 27

constexpr GLsizei gWidth  = 500;
constexpr GLsizei gHeight = 500;

GLfloat cubeRotateX = 45.f;
GLfloat cubeRotateY = 45.f;
bool    keys[255];

/// 窗口和 OpenGL 上下文
SDL_Window   *gWindow   = nullptr;
SDL_GLContext gContext  = nullptr;
bool          isRunning = true;

GLvoid estableishProjectionMatrix(GLsizei width, GLsizei height)
{
    /// 1. 设置视口 (这部分保持不变)
    glViewport(0, 0, width, height);

    /// 2. 切换到投影矩阵模式 (这部分也保持不变)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /// 3. 核心改动：用 GLM 计算透视投影矩阵
    float aspect = (float)width / (float)height;
    /// glm::perspective 的参数：视角(FOV, 弧度), 宽高比, 近平面, 远平面
    /// 注意：glm::radians(45.0f) 将45度角转换为弧度
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 200.0f);

    /// 4. 将计算好的矩阵加载到 OpenGL 的投影矩阵栈中
    glLoadMatrixf(glm::value_ptr(projectionMatrix));

    /// 5. 最后，切换回模型视图矩阵模式 (这部分也保持不变)
    glMatrixMode(GL_MODELVIEW);
}

GLvoid initGL(GLsizei width, GLsizei height)
{
    estableishProjectionMatrix(width, height);

    glShadeModel(GL_SMOOTH);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glDisable(GL_CULL_FACE);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_PERSPECTIVE_CORRECTION_HINT);
}

GLvoid displayFPS(GLvoid)
{
    static Uint32 lastTime = SDL_GetTicks();
    static int    loops    = 0;
    static float  fps      = 0.0f;

    int newTime = SDL_GetTicks();

    if (newTime - lastTime > 100)
    {
        float newFPS = (float)loops / (float)(newTime - lastTime) * 1000.f;

        fps = (fps + newFPS) / 2.0f;

        char title[80];
        sprintf(title, "OpenGL Demo - %.2f", fps);

        SDL_SetWindowTitle(gWindow, title);

        lastTime = newTime;

        loops = 0;
    }


    loops++;
}

GLvoid drawScene(GLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.f, 0.f, -5.f);
    glRotatef(cubeRotateX, 1, 0, 0);
    glRotatef(cubeRotateY, 0, 1, 0);

    glBegin(GL_QUADS);

    /// 前面 (Front Face) - 红色
    glColor3f(1.f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f); /// 左下
    glVertex3f(1.0f, -1.0f, 1.0f);  /// 右下
    glVertex3f(1.0f, 1.0f, 1.0f);   /// 右上
    glVertex3f(-1.0f, 1.0f, 1.0f);  /// 左上

    /// 后面 (Back Face) - 黄色
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);  /// 右下
    glVertex3f(-1.0f, -1.0f, -1.0f); /// 左下
    glVertex3f(-1.0f, 1.0f, -1.0f);  /// 左上
    glVertex3f(1.0f, 1.0f, -1.0f);   /// 右上

    /// 左面 (Left Face) - 蓝色
    glColor3f(0.f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f); /// 左下
    glVertex3f(-1.0f, -1.0f, 1.0f);  /// 右下
    glVertex3f(-1.0f, 1.0f, 1.0f);   /// 右上
    glVertex3f(-1.0f, 1.0f, -1.0f);  /// 左上

    /// 右面 (Right Face) - 紫色
    glColor3f(1.f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);  /// 左下
    glVertex3f(1.0f, -1.0f, -1.0f); /// 右下
    glVertex3f(1.0f, 1.0f, -1.0f);  /// 右上
    glVertex3f(1.0f, 1.0f, 1.0f);   /// 左上

    /// 上面 (Top Face) - 橙色
    glColor3f(1.f, 0.5f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);  /// 左下
    glVertex3f(1.0f, 1.0f, 1.0f);   /// 右下
    glVertex3f(1.0f, 1.0f, -1.0f);  /// 右上
    glVertex3f(-1.0f, 1.0f, -1.0f); /// 左上

    /// 下面 (Bottom Face) - 绿色
    glColor3f(0.f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f); /// 左下
    glVertex3f(1.0f, -1.0f, -1.0f);  /// 右下
    glVertex3f(1.0f, -1.0f, 1.0f);   /// 右上
    glVertex3f(-1.0f, -1.0f, 1.0f);  /// 左上

    glEnd();

    glFlush();

    SDL_GL_SwapWindow(gWindow);

    displayFPS();
}

void checkKeys(GLvoid)
{
    const float speed = 5.0f;

    if (keys[SDL_SCANCODE_ESCAPE])
    {
        isRunning = false;
    }

    if (keys[SDL_SCANCODE_UP])
    {
        cubeRotateX += speed;
    }

    if (keys[SDL_SCANCODE_DOWN])
    {
        cubeRotateX -= speed;
    }

    if (keys[SDL_SCANCODE_LEFT])
    {
        cubeRotateY += speed;
    }

    if (keys[SDL_SCANCODE_RIGHT])
    {
        cubeRotateY -= speed;
    }
}

// GLvoid timerLoop(int value)
// {
//     if (checkKeys())
//         exit(0);
//
//     glutPostRedisplay();
//     glutTimerFunc(16, timerLoop, 0); /// 改成 16ms (约60fps)
// }


// GLvoid keybordCB(unsigned char key, int x, int y)
// {
//     keys[key] = true;
// }
//
// GLvoid keybordUpCB(unsigned char key, int x, int y)
// {
//     keys[key] = false;
// }
//
// GLvoid keybordSpecialCB(int key, int x, int y)
// {
//     keys[key] = true;
// }
//
// GLvoid keybordSpecialUpCB(int key, int x, int y)
// {
//     keys[key] = false;
// }

int main(int argc, char *argv[])
{
    /// 1. 初始化 SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    /// 2. 设置 OpenGL 属性
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


    /// 3. 创建窗口
    gWindow = SDL_CreateWindow("Hello, OpenGL!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gWidth, gHeight,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!gWindow)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    /// 4. 创建 OpenGL 上下文
    gContext = SDL_GL_CreateContext(gWindow);
    if (!gContext)
    {
        printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(gWindow);
        SDL_Quit();
        return 1;
    }

    /// 5. 初始化 GLAD（如果使用 GLAD）
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return 1;
    }

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    /// 6. 初始化 OpenGL 状态
    initGL(gWidth, gHeight);

    /// 7. 主循环
    SDL_Event event;
    while (isRunning)
    {
        /// 处理事件
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    isRunning = false;
                    break;

                case SDL_KEYDOWN:
                    keys[event.key.keysym.scancode] = true;
                    break;

                case SDL_KEYUP:
                    keys[event.key.keysym.scancode] = false;
                    break;


                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        int newWidth  = event.window.data1;
                        int newHeight = event.window.data2;
                        estableishProjectionMatrix(newWidth, newHeight);
                    }
                    break;
            }
        }

        /// 检查键盘输入
        checkKeys();

        /// 绘制场景
        drawScene();

        /// 控制帧率（约 60 FPS）
        SDL_Delay(16);
    }

    /// 8. 清理资源
    SDL_GL_DeleteContext(gContext);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    return 0;
}
