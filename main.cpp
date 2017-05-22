#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include "test_frag.h"
#include "test_vert.h"
#include "shader.h"
#include "program.h"

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
    return true;
}

GLuint VBO, VAO, EBO;
GLuint shaderProgram;
Program prog;

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

    struct customv
    {
        float pos[3];
        float col[3];
    };

    float angx[] = {0.0f, 0.0f, 0.0f, 0.0f, 90.0f, 270.0f};
    float angy[] = {0.0f, 90.0f, 180.0f, 270.0f, 0.0f, 0.0f};
    float cols[][3] = {
        { 1.0f, 0.0f, 0.0f, },
        { 0.0f, 1.0f, 0.0f, },
        { 0.0f, 0.0f, 1.0f, },
        { 1.0f, 1.0f, 0.0f, },
        { 1.0f, 0.0f, 1.0f, },
        { 0.0f, 1.0f, 1.0f, },
    };

    customv vertex[24];
    unsigned int index[36];
    vec4 face[] = {
        vec4(0.5f, 0.5f, 0.5f, 1.0f),
        vec4(-0.5f, 0.5f, 0.5f, 1.0f),
        vec4(-0.5f, -0.5f, 0.5f, 1.0f),
        vec4(0.5f, -0.5f, 0.5f, 1.0f),
    };

    for (int i = 0; i < 6; i++)
    {

        mat4 rotx = rotate(mat4(1.0f), radians(angx[i]), vec3(1.0f, 0.0f, 0.0f));
        mat4 roty = rotate(mat4(1.0f), radians(angy[i]), vec3(0.0f, 1.0f, 0.0f));
        for (int j = 0; j < 4; j++)
        {
            vec4 tr = roty * rotx * face[j];
            for (int k = 0; k < 3; k++)
            {
                vertex[i * 4 + j].pos[k] = tr[k];
            }
            memcpy(vertex[i * 4 + j].col, cols[i], sizeof(cols[i]));
        }
        unsigned int ind[] = { 0, 1, 2, 2, 3, 0 };
        for (int j = 0; j < 6; j++)
        {
            index[i * 6 + j] = i * 4 + ind[j];
        }
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
    unsigned int position = glGetAttribLocation(prog.id(), "position");
    unsigned int color = glGetAttribLocation(prog.id(), "color");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(customv), 0);
    glEnableVertexAttribArray(color);
    glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, sizeof(customv), (void*)12);
    glBindVertexArray(0);
}

float angle;
void timer(int)
{
    angle += 1.0f;
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}

void display()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
           glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

           // Draw our first triangle
           prog.use();
           GLuint id = glGetUniformLocation(prog.id(), "mvp");
           //Matrix projection = Matrix::perspective(70.0f, 4.0f / 3.0f, 1.0f, 100.0f);
           //Matrix projection = Matrix::ortho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0f);
           //Matrix vp = projection * Matrix::translate(0.0f, 0.0f, -7.0f);
           //Matrix model = Matrix::rotate(angle, 0.0f, 1.0f, 0.0f) ;//*
                   //Matrix::scale(2.0f, 2.0f, 2.0f);
           //Matrix mvp = vp * model;
           glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 1.0f, 100.0f);
           glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -7.0f));
           glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.0f));
           glm::mat4 mvp = projection * view * model;

           glUniformMatrix4fv(id, 1, GL_FALSE, &mvp[0][0]);
           glBindVertexArray(VAO);
           //glDrawArrays(GL_TRIANGLES, 0, 6);
           glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * 4));
           Program::unuse();
           glBindVertexArray(0);
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
        timer(0);
        glutDisplayFunc(display);
        glutMainLoop();
    }
    return 0;
}
