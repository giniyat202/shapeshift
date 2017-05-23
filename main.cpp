#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include "test_frag.h"
#include "test_vert.h"
#include "shader.h"
#include "program.h"
#include "cubelet.h"

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

GLuint VBO, VAO, EBO;
GLuint shaderProgram;
Program prog;
Cubelet *c;

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
    c = new Cubelet(prog);
    struct customv
    {
        float pos[3];
        float norm[3];
    };

    static const float angx[] = {0.0f, 0.0f, 0.0f, 0.0f, 90.0f, 270.0f};
    static const float angy[] = {0.0f, 90.0f, 180.0f, 270.0f, 0.0f, 0.0f};
    static const vec3 norms[] = {
        vec3(0.0f, 0.0f, 1.0f),
        vec3(1.0f, 0.0f, 0.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(-1.0f, 0.0f, 0.0f),
        vec3(0.0f, -1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
    };

    customv vertex[] = {
        {  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f },
        { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f },
        { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f },
        {  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f },
        {  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
        { -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
        { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
        {  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
    };
    unsigned int index[] = {
        0, 1, 2,
        2, 3, 0,

        4, 0, 3,
        3, 7, 4,

        5, 4, 7,
        7, 6, 5,

        1, 5, 6,
        6, 2, 1,

        3, 2, 6,
        6, 7, 3,

        4, 5, 1,
        1, 0, 4,
    };

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            for (int k = 0; k < 3; k++)
                vertex[index[i * 6 + j]].norm[k] += norms[i][k];
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
    unsigned int normal = glGetAttribLocation(prog.id(), "normal");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(customv), 0);
    glEnableVertexAttribArray(normal);
    glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(customv), (void*)12);
    glBindVertexArray(0);

    prog.use();
    vec3 light_position(0.0f, 0.0f, 0.0f);
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
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 1.0f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -7.0f));
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
    glm::mat4 modelview = view * model;
    mat3 timodel = mat3(transpose(inverse(modelview)));
    vec3 material_ambient(0.0f, 0.0f, 0.0f);
    vec3 material_diffuse(0.01f, 0.01f, 0.01f);
    vec3 material_specular(0.5f, 0.5f, 0.5f);
    float material_shininess = 0.25f * 128.0f;

    unsigned int modelview_id = glGetUniformLocation(prog.id(), "modelview");
    unsigned int projection_id = glGetUniformLocation(prog.id(), "projection");
    unsigned int timodel_id = glGetUniformLocation(prog.id(), "timodel");
    unsigned int material_ambient_id = glGetUniformLocation(prog.id(), "material.ambient");
    unsigned int material_diffuse_id = glGetUniformLocation(prog.id(), "material.diffuse");
    unsigned int material_specular_id = glGetUniformLocation(prog.id(), "material.specular");
    unsigned int material_shininess_id = glGetUniformLocation(prog.id(), "material.shininess");
    glUniformMatrix4fv(modelview_id, 1, GL_FALSE, &modelview[0][0]);
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix3fv(timodel_id, 1, GL_FALSE, &timodel[0][0]);
    glUniform3fv(material_ambient_id, 1, &material_ambient[0]);
    glUniform3fv(material_diffuse_id, 1, &material_diffuse[0]);
    glUniform3fv(material_specular_id, 1, &material_specular[0]);
    glUniform1fv(material_shininess_id, 1, &material_shininess);

    c->render(modelview);

//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0));
//    Program::unuse();
//    glBindVertexArray(0);
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
        delete c;
    }
    return 0;
}
