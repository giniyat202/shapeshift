#include <GL/glut.h>
#include "gamescene.h"
GameScene *zz;

void keyboard(unsigned char key, int x, int y)
{
    zz->keyboard(key, x, y);
}
void keyboardUp(unsigned char key, int x, int y)
{
    zz->keyboardUp(key, x, y);
}
void special(int key, int x, int y)
{
    zz->special(key, x, y);
}
void specialUp(int key, int x, int y)
{
    zz->specialUp(key, x, y);
}
void reshape(int width, int height)
{
    zz->reshape(width, height);
}
void mouse(int button, int state, int x, int y)
{
    zz->mouse(button, state, x, y);
}
void motion(int x, int y)
{
    zz->motion(x, y);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    zz->render();
    glFlush();
    glutSwapBuffers();
}

void init3d()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_CULL_FACE);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    glutCreateWindow("memes");
    init3d();
    zz = new GameScene;
    zz->reshape(640, 480);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}
