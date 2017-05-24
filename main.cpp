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

    shader_test_vert.create();
    shader_test_vert.compile();
    shader_test_frag.create();
    shader_test_frag.compile();
    prog.create();
    prog += shader_test_vert;
    prog += shader_test_frag;
    prog.link();
    shader_test_vert.destroy();
    shader_test_frag.destroy();
    c = new GameScene(prog);

    prog.use();
    vec3 light_position(0.0f, 0.0f, -1.0f);
    vec3 light_ambient(0.5f, 0.5f, 0.5f);
    vec3 light_diffuse(0.8f, 0.8f, 0.8f);
    vec3 light_specular(1.0f, 1.0f, 1.0f);
    unsigned int light_position_id = glGetUniformLocation(prog.id(), "light.position");
    unsigned int light_ambient_id = glGetUniformLocation(prog.id(), "light.ambient");
    unsigned int light_diffuse_id = glGetUniformLocation(prog.id(), "light.diffuse");
    unsigned int light_specular_id = glGetUniformLocation(prog.id(), "light.specular");
    glUniform3fv(light_position_id, 1, &light_position[0]);
    glUniform3fv(light_ambient_id, 1, &light_ambient[0]);
    glUniform3fv(light_diffuse_id, 1, &light_diffuse[0]);
    glUniform3fv(light_specular_id, 1, &light_specular[0]);
    prog.unuse();
}

void display()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    c->render();

    glFlush();
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutCreateWindow("asdasd");
    glewInit();
    if (checkall())
    {
        init();
        glutDisplayFunc(display);
        glutMainLoop();
        delete c;
    }
    return 0;
}
