#include <gl/glut.h>

constexpr GLsizei gWidth  = 500;
constexpr GLsizei gHeight = 500;

GLvoid estableishProjectionMatrix(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluPerspective(80.f, (GLfloat)width / (GLfloat)height, 0.1f, 200.0f);
}

GLvoid initGL(GLsizei width, GLsizei height)
{
    estableishProjectionMatrix(width, height);

    glShadeModel(GL_SMOOTH);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_PERSPECTIVE_CORRECTION_HINT);
}

GLvoid drawScene(GLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.f, 0.f, -5.f);
    glRotatef(45, 1, 0, 0);
    glRotatef(45, 0, 1, 0);
    /// Draw a cube
    glBegin(GL_QUADS);
    /// Top Face
    glColor3f(1.f, 0.5f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    /// Bottom Face
    glColor3f(0.f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    /// Front Face
    glColor3f(1.f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    /// Back Face
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    /// Left Face
    glColor3f(0.f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    /// Right Face
    glColor3f(1.f, 0.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glEnd();


    glFlush();

    glutSwapBuffers();
}

GLvoid timerLoop(int value)
{
    glutPostRedisplay();
    glutTimerFunc(1000, timerLoop, 0);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GL_DOUBLE);

    int winId = glutCreateWindow("Hello, OpenGL!");
    glutReshapeWindow(gWidth, gHeight);


    initGL(gWidth, gHeight);
    glutDisplayFunc(drawScene);
    glutTimerFunc(1000, timerLoop, 0);

    glutMainLoop();

    return 0;
}
