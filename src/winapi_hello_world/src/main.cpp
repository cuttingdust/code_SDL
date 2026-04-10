#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>

#include <Windows.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

/// 常量定义
constexpr GLsizei gWidth  = 500;
constexpr GLsizei gHeight = 500;
#define GLUT_KEY_ESCAPE 27

/// 全局变量
HWND  hWnd      = NULL;
HDC   hDC       = NULL;
HGLRC hRC       = NULL;
bool  isRunning = true;

GLfloat cubeRotateX = 45.f;
GLfloat cubeRotateY = 45.f;
bool    keys[255];

GLvoid estableishProjectionMatrix(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);

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
    static long    lastTime = ::GetTickCount();
    static long    loops    = 0;
    static GLfloat fps      = 0.0f;

    int newTime = ::GetTickCount();

    if (newTime - lastTime > 100)
    {
        float newFPS = (float)loops / (float)(newTime - lastTime) * 1000.f;

        fps = (fps + newFPS) / 2.0f;

        char title[80];
        sprintf(title, "OpenGL Demo - %.2f", fps);

        ::SetWindowTextA(hWnd, title);

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

    SwapBuffers(hDC);

    displayFPS();
}

GLboolean checkKeys(GLvoid)
{
    const GLfloat speed = 5.f;

    if (keys[VK_ESCAPE]) /// Escape key
    {
        isRunning = false;
    }

    if (keys[VK_UP]) /// Up arrow key
    {
        cubeRotateX += speed;
    }

    if (keys[VK_DOWN]) /// Down arrow key
    {
        cubeRotateX -= speed;
    }

    if (keys[VK_LEFT]) /// Left arrow key
    {
        cubeRotateY += speed;
    }

    if (keys[VK_RIGHT]) /// Right arrow key
    {
        cubeRotateY -= speed;
    }

    return false;
}


/// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
            isRunning = false;
            ::PostQuitMessage(0);
            return 0;

        case WM_SIZE:
            {
                int width  = LOWORD(lParam);
                int height = HIWORD(lParam);
                if (width > 0 && height > 0)
                {
                    estableishProjectionMatrix(width, height);
                }
            }
            return 0;

        case WM_KEYDOWN:
            keys[wParam] = true;
            return 0;

        case WM_KEYUP:
            keys[wParam] = false;
            return 0;

        case WM_CLOSE:
            isRunning = false;
            ::DestroyWindow(hWnd);
            return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

int APIENTRY wWinMain(_In_ HINSTANCE     hInstance,     /// 当前实例句柄
                      _In_opt_ HINSTANCE hPrevInstance, /// 前一个实例 总是 NULL（Win16兼容性遗留）
                      _In_ LPWSTR        lpCmdLine,     /// 命令行参数
                      _In_ int           nCmdShow       /// 窗口显示模式
)
{
    /// 忽略未使用的参数
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    ::timeBeginPeriod(1);

    /// 1. 注册窗口类
    WNDCLASS wc      = { 0 };
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"OpenGLWindow";

    if (!::RegisterClass(&wc))
    {
        ::MessageBox(NULL, L"窗口类注册失败!", L"错误", MB_OK);
        return 1;
    }

    /// 2. 创建窗口
    hWnd = ::CreateWindowEx(0, L"OpenGLWindow", L"Hello, OpenGL!", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                            gWidth, gHeight, NULL, NULL, hInstance, NULL);
    if (!hWnd)
    {
        ::MessageBox(NULL, L"窗口创建失败!", L"错误", MB_OK);
        return 1;
    }

    /// 3. 获取设备上下文
    hDC = ::GetDC(hWnd);
    if (!hDC)
    {
        MessageBox(NULL, L"获取设备上下文失败!", L"错误", MB_OK);
        return 1;
    }

    /// 4. 设置像素格式
    PIXELFORMATDESCRIPTOR pfd         = { sizeof(PIXELFORMATDESCRIPTOR),
                                          1,
                                          PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
                                          PFD_TYPE_RGBA,
                                          32, /// 颜色深度
                                          0,
                                          0,
                                          0,
                                          0,
                                          0,
                                          0, /// 颜色位忽略
                                          0,
                                          0,
                                          0,
                                          0,
                                          0,
                                          0,
                                          0,
                                          0,  /// 其他位忽略
                                          16, /// 深度缓冲
                                          0,  /// 模板缓冲
                                          0,  /// 辅助缓冲
                                          PFD_MAIN_PLANE,
                                          0,
                                          0,
                                          0 };
    int                   pixelFormat = ::ChoosePixelFormat(hDC, &pfd);
    if (!pixelFormat)
    {
        MessageBox(NULL, L"选择像素格式失败!", L"错误", MB_OK);
        return 1;
    }

    if (!::SetPixelFormat(hDC, pixelFormat, &pfd))
    {
        MessageBox(NULL, L"设置像素格式失败!", L"错误", MB_OK);
        return 1;
    }

    /// 5. 创建 OpenGL 上下文
    hRC = ::wglCreateContext(hDC);
    if (!hRC)
    {
        MessageBox(NULL, L"创建 OpenGL 上下文失败!", L"错误", MB_OK);
        return 1;
    }

    /// 6. 激活 OpenGL 上下文
    if (!::wglMakeCurrent(hDC, hRC))
    {
        MessageBox(NULL, L"激活 OpenGL 上下文失败!", L"错误", MB_OK);
        return 1;
    }

    /// 初始化 glad（必须在创建 OpenGL 上下文之后）
    if (!gladLoadGL())
    {
        MessageBox(NULL, L"GLAD 初始化失败!", L"错误", MB_OK);
        return 1;
    }

    /// 8. 初始化 OpenGL
    initGL(gWidth, gHeight);

    /// 9. 显示窗口
    ::ShowWindow(hWnd, nCmdShow);
    ::UpdateWindow(hWnd);

    /// 10. 主消息循环
    MSG msg;

    while (isRunning)
    {
        static DWORD lastFrame = GetTickCount();

        /// 处理 Windows 消息
        while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                isRunning = false;
                break;
            }
        }

        /// 处理键盘输入
        checkKeys();

        // 绘制场景
        drawScene();

        // 控制帧率（约 60 FPS）
        // Sleep(16); ///sleep 精度不够导致 帧率不对
        /// 精确延迟
        DWORD now     = ::GetTickCount();
        DWORD elapsed = now - lastFrame;
        if (elapsed < 16)
        {
            Sleep(16 - elapsed);
        }
        lastFrame = now;
    }

    ::timeEndPeriod(1);
    return 0;
}
