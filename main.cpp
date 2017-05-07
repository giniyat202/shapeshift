#include <GL/glut.h>
#include "cubelet.h"
#include <math.h>
Cubelet c;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    c.render();
    glFlush();
    glutSwapBuffers();
}

void init3d()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(70.0, 640.0 / 480.0, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glTranslated(0.0, 0.0, -5.0);
    glRotated(45.0, 0.0, 1.0, 0.0);
    glRotated(30.0, cos(3.1415 / 4.0), 0.0, sin(3.1415 / 4.0));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    for (int i = 0; i < FACELETID_MAX; i++)
        c.setFaceletVisible(i, true);
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
    glutMainLoop();
    return 0;
}
