#include <GL/glut.h>
#include "cube.h"
#include <math.h>
Cube cu;

void keyboard(unsigned char c, int, int)
{
    bool refresh = true;

    switch (c)
    {
    case 'f':
        cu.rotate(FACELETID_FRONT, ANGLEID_90);
        break;
    case 'b':
        cu.rotate(FACELETID_BACK, ANGLEID_90);
        break;
    case 'l':
        cu.rotate(FACELETID_LEFT, ANGLEID_90);
        break;
    case 'r':
        cu.rotate(FACELETID_RIGHT, ANGLEID_90);
        break;
    case 't':
        cu.rotate(FACELETID_TOP, ANGLEID_90);
        break;
    case 'm':
        cu.rotate(FACELETID_BOTTOM, ANGLEID_90);
        break;
    default:
        refresh = false;
    }
    if (refresh) glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cu.render();
    glFlush();
    glutSwapBuffers();
}

void init3d()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(70.0, 640.0 / 480.0, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glTranslated(0.0, 0.0, -7.0);
    glRotated(45.0, 0.0, 1.0, 0.0);
    glRotated(30.0, cos(3.1415 / 4.0), 0.0, sin(3.1415 / 4.0));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    glutCreateWindow("memes");
    init3d();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
