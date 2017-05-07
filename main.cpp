#include <GL/glut.h>
#include "cube.h"
#include <math.h>
Cube cu;
int angle;
double rot;
bool autorot;

void timer(int)
{
    if (cu.isRotating())
    {
        cu.updateRotation();
        glutTimerFunc(10, timer, 0);
    }
}

void t2(int)
{
    if (autorot)
    {
        rot += 2.0;
        glutPostRedisplay();
    }
    glutTimerFunc(50, t2, 0);
}

void keyboard(unsigned char c, int, int)
{
    bool refresh = !cu.isRotating();

    switch (c)
    {
    case 'f':
        cu.beginRotation(FACELETID_FRONT, angle);
        break;
    case 'b':
        cu.beginRotation(FACELETID_BACK, angle);
        break;
    case 'l':
        cu.beginRotation(FACELETID_LEFT, angle);
        break;
    case 'r':
        cu.beginRotation(FACELETID_RIGHT, angle);
        break;
    case 't':
        cu.beginRotation(FACELETID_TOP, angle);
        break;
    case 'm':
        cu.beginRotation(FACELETID_BOTTOM, angle);
        break;
    case 'z':
        refresh = false;
        angle = (angle + 1) % ANGLEID_MAX;
        break;
    case 'q':
        refresh = false;
        autorot = !autorot;
        break;
    default:
        refresh = false;
    }
    if (refresh)
    {
        timer(0);
        glutPostRedisplay();
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotated(rot, 0.0, 1.0, 0.0);
    cu.render();
    glPopMatrix();
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
    t2(0);
    glutMainLoop();
    return 0;
}
