#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>

#include <wx/wx.h>
#include <wx/glcanvas.h>

/// 常量定义
constexpr GLsizei gWidth  = 500;
constexpr GLsizei gHeight = 500;

GLfloat cubeRotateX = 45.f;
GLfloat cubeRotateY = 45.f;
bool    keys[255];


class MainFrame : public wxFrame
{
public:
    MainFrame() :
        wxFrame(nullptr, wxID_ANY, "OpenGL Demo - wxWidgets", wxDefaultPosition, wxSize(gWidth, gHeight)),
        m_cubeRotateX(cubeRotateX), m_cubeRotateY(cubeRotateY), m_timer(this, wxID_ANY)
    {
        /// 初始化键盘数组
        memset(m_keys, 0, sizeof(m_keys));

        /// 创建 OpenGL 画布
        int attribList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, WX_GL_STENCIL_SIZE, 8, 0 };

        m_glCanvas  = new wxGLCanvas(this, wxID_ANY, attribList, wxDefaultPosition, GetClientSize());
        m_glContext = new wxGLContext(m_glCanvas);

        m_glCanvas->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyDown, this);
        m_glCanvas->Bind(wxEVT_KEY_UP, &MainFrame::OnKeyUp, this);

        /// 绑定 OpenGL 上下文
        m_glCanvas->SetCurrent(*m_glContext);

        if (!gladLoadGL())
        {
            wxLogError("Failed to initialize GLAD");
            return;
        }


        wxString glVersion(glGetString(GL_VERSION), wxConvUTF8);
        wxString glRenderer(glGetString(GL_RENDERER), wxConvUTF8);

        /// 可选：打印 OpenGL 版本验证
        // wxLogMessage("OpenGL Version: %s", glVersion);
        // wxLogMessage("Renderer: %s", glRenderer);

        /// 初始化 OpenGL
        int width, height;
        GetClientSize(&width, &height);
        InitGL();
        EstablishProjectionMatrix(width, height);

        /// 启动定时器（16ms ≈ 60 FPS）
        // m_timer.Start(16); 精度较低，可能导致帧率不稳定 太低了 还是 交给系统得垂直同步吧
        m_timer.Start(1);

        /// 设置焦点，接收键盘事件
        m_glCanvas->SetFocus();
        m_glCanvas->SetFocusFromKbd();

        this->SetFocus();
    }

    ~MainFrame() override
    {
        if (m_timer.IsRunning())
            m_timer.Stop();

        /// 等待最后一次定时器事件处理完
        wxYield();

        delete m_glContext;
    }

private:
    wxGLCanvas*  m_glCanvas  = NULL;
    wxGLContext* m_glContext = NULL;

    /// OpenGL 相关
    float m_cubeRotateX = cubeRotateX;
    float m_cubeRotateY = cubeRotateY;
    bool  m_keys[256];

    /// 定时器
    wxTimer m_timer;

public:
    void OnTimer(wxTimerEvent& event)
    {
        /// 绘制场景
        m_glCanvas->SetCurrent(*m_glContext);
        DrawScene();
        DisplayFPS();
    }

    void OnKeyDown(wxKeyEvent& event)
    {
        int         key  = event.GetKeyCode();
        const float step = 5.f;

        switch (key)
        {
            case WXK_ESCAPE:
                Close();
                return;
            case WXK_UP:
                m_cubeRotateX += step;
                break;
            case WXK_DOWN:
                m_cubeRotateX -= step;
                break;
            case WXK_LEFT:
                m_cubeRotateY += step;
                break;
            case WXK_RIGHT:
                m_cubeRotateY -= step;
                break;
            default:
                /// 其他按键记录到数组
                if (key >= 0 && key < 256)
                    m_keys[key] = true;
                break;
        }
    }

    void OnKeyUp(wxKeyEvent& event)
    {
        int key = event.GetKeyCode();
        if (key >= 0 && key < 256)
        {
            m_keys[key] = false;
        }


        /// 特殊键处理
        switch (key)
        {
            case WXK_ESCAPE:
                m_keys[WXK_ESCAPE] = false;
                break;
            case WXK_UP:
                m_keys[WXK_UP] = false;
                break;
            case WXK_DOWN:
                m_keys[WXK_DOWN] = false;
                break;
            case WXK_LEFT:
                m_keys[WXK_LEFT] = false;
                break;
            case WXK_RIGHT:
                m_keys[WXK_RIGHT] = false;
                break;
        }
    }

    void OnClose(wxCloseEvent& event)
    {
        m_timer.Stop();
        event.Skip();
    }

    void InitGL()
    {
        glShadeModel(GL_SMOOTH);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glDisable(GL_CULL_FACE);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    }

    void EstablishProjectionMatrix(int width, int height)
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

    void DrawScene()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(0.0f, 0.0f, -5.0f);
        glRotatef(m_cubeRotateX, 1.0f, 0.0f, 0.0f);
        glRotatef(m_cubeRotateY, 0.0f, 1.0f, 0.0f);

        glBegin(GL_QUADS);

        /// 前面 - 红色
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);

        /// 后面 - 黄色
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);

        /// 左面 - 蓝色
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);

        /// 右面 - 紫色
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);

        /// 上面 - 橙色
        glColor3f(1.0f, 0.5f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);

        /// 下面 - 绿色
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);

        glEnd();

        glFlush();
        m_glCanvas->SwapBuffers();
    }

    void DisplayFPS()
    {
        static long  lastTime = wxGetLocalTimeMillis().GetValue();
        static long  loops    = 0;
        static float fps      = 0.0f;

        long newTime = wxGetLocalTimeMillis().GetValue();

        if (newTime - lastTime > 100)
        {
            float newFPS = (float)loops / (float)(newTime - lastTime) * 1000.0f;
            fps          = (fps + newFPS) / 2.0f;

            wxString title;
            title.Printf("OpenGL Demo - %.2f fps", fps);
            SetTitle(title);

            lastTime = newTime;
            loops    = 0;
        }

        loops++;
    }

    wxDECLARE_EVENT_TABLE();
};

/// 事件表
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)         //
        EVT_TIMER(wxID_ANY, MainFrame::OnTimer) //
        EVT_KEY_DOWN(MainFrame::OnKeyDown)      //
        EVT_KEY_UP(MainFrame::OnKeyUp)          //
        EVT_CLOSE(MainFrame::OnClose)           //
        wxEND_EVENT_TABLE();                    //

//////////////////////////////////////////////////////////////////


class MyApp : public wxApp
{
public:
    bool OnInit() override
    {
        timeBeginPeriod(1);

        MainFrame* frame = new MainFrame();
        frame->Show(true);
        return true;
    }

    int OnExit() override
    {
        timeEndPeriod(1);
        return wxApp::OnExit();
    }
};

wxDECLARE_APP(MyApp);

wxIMPLEMENT_APP(MyApp);
