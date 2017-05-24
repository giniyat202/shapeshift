#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include "test_frag.h"
#include "test_vert.h"
#include "shader.h"
#include "program.h"
#include "gamescene.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;
#define checksingle(a) do {\
    if (!GLEW_##a) { \
        fprintf(stderr, "GL_"#a" is not supported\n"); \
        return false; \
    } else { \
        fprintf(stderr, "GL_"#a" is supported\n"); \
    } \
} while(false)
bool checkall()
{
    checksingle(ARB_vertex_buffer_object);
    checksingle(ARB_vertex_array_object);
    checksingle(ARB_shader_objects);
    checksingle(ARB_vertex_shader);
    checksingle(ARB_fragment_shader);
    checksingle(ARB_multisample);
    fflush(stderr);
    return true;
}

Program prog;
GameScene *c;
void init()
{
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);


    c = new GameScene;


}

void display()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    c->render();

    glFlush();
    glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y)
{
    c->keyboard(key, x, y);
}
void keyboardUp(unsigned char key, int x, int y)
{
    c->keyboardUp(key, x, y);
}
void special(int key, int x, int y)
{
    c->special(key, x, y);
}
void specialUp(int key, int x, int y)
{
    c->specialUp(key, x, y);
}
void reshape(int width, int height)
{
    c->reshape(width, height);
}
void mouse(int button, int state, int x, int y)
{
    c->mouse(button, state, x, y);
}
void motion(int x, int y)
{
    c->motion(x, y);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutCreateWindow("memes");
    glewInit();
    if (checkall())
    {
        init();
        glutDisplayFunc(display);
        glutKeyboardFunc(keyboard);
        glutKeyboardUpFunc(keyboardUp);
        glutSpecialFunc(special);
        glutSpecialUpFunc(specialUp);
        glutReshapeFunc(reshape);
        glutMouseFunc(mouse);
        glutMotionFunc(motion);
        glutMainLoop();
        delete c;
    }
    return 0;
}
