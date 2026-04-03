#include <cstdio>

#include <gl/freeglut.h>

#define GLUT_KEY_ESCAPE 27

constexpr GLsizei gWidth  = 500;
constexpr GLsizei gHeight = 500;

GLfloat cubeRotateX = 45.f;
GLfloat cubeRotateY = 45.f;
bool    keys[255];

GLvoid estableishProjectionMatrix(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluPerspective(45, (GLfloat)width / (GLfloat)height, 0.1f, 200.0f);
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
    static long    lastTime = glutGet(GLUT_ELAPSED_TIME);
    static long    loops    = 0;
    static GLfloat fps      = 0.0f;

    int newTime = glutGet(GLUT_ELAPSED_TIME);

    if (newTime - lastTime > 100)
    {
        float newFPS = (float)loops / (float)(newTime - lastTime) * 1000.f;

        fps = (fps + newFPS) / 2.0f;

        char title[80];
        sprintf(title, "OpenGL Demo - %.2f", fps);

        glutSetWindowTitle(title);

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

    glutSwapBuffers();

    displayFPS();
}

GLboolean checkKeys(GLvoid)
{
    const GLfloat speed = 5.f;

    if (keys[GLUT_KEY_ESCAPE]) /// Escape key
    {
        return true;
    }

    if (keys[GLUT_KEY_UP]) /// Up arrow key
    {
        cubeRotateX += speed;
    }

    if (keys[GLUT_KEY_DOWN]) /// Down arrow key
    {
        cubeRotateX -= speed;
    }

    if (keys[GLUT_KEY_LEFT]) /// Left arrow key
    {
        cubeRotateY += speed;
    }

    if (keys[GLUT_KEY_RIGHT]) /// Right arrow key
    {
        cubeRotateY -= speed;
    }

    return false;
}

GLvoid timerLoop(int value)
{
    if (checkKeys())
        exit(0);

    glutPostRedisplay();
    glutTimerFunc(16, timerLoop, 0); /// 改成 16ms (约60fps)
}


GLvoid keybordCB(unsigned char key, int x, int y)
{
    keys[key] = true;
}

GLvoid keybordUpCB(unsigned char key, int x, int y)
{
    keys[key] = false;
}

GLvoid keybordSpecialCB(int key, int x, int y)
{
    keys[key] = true;
}

GLvoid keybordSpecialUpCB(int key, int x, int y)
{
    keys[key] = false;
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    // glutInitDisplayMode(GL_DOUBLE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);


    // glutInitWindowSize(gWidth, gHeight);
    int winId = glutCreateWindow("Hello, OpenGL!");
    glutReshapeWindow(gWidth, gHeight);


    initGL(gWidth, gHeight);
    glutDisplayFunc(drawScene);

    glutKeyboardFunc(keybordCB);
    glutKeyboardUpFunc(keybordUpCB);
    glutSpecialFunc(keybordSpecialCB);
    glutSpecialUpFunc(keybordSpecialUpCB);

    glutTimerFunc(16, timerLoop, 0); /// 约60 FPS，响应更流畅

    glutMainLoop();

    return 0;
}
