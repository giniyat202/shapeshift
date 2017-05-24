#include "gamescene.h"
#include <GL/glew.h>
#include <gl/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace std;

std::set<GameScene*> GameScene::s_scenes;

GameScene::GameScene(const Program &program) :
    m_mouseRotation(false),
    m_selMode(false)
{
    m_program = &program;
    reshape(800, 600);
    m_cube = new Cube(program);
    m_cube->rotateCube(ROTAXIS_Y, 45.0, glm::mat4(1.0f), m_projection);
    m_cube->rotateCube(ROTAXIS_X, 30.0, glm::mat4(1.0f), m_projection);
    memset(m_keyDown, 0, sizeof(m_keyDown));
    memset(m_specialDown, 0, sizeof(m_specialDown));
    s_scenes.insert(this);
    s_timer(reinterpret_cast<int>(this));
}

GameScene::~GameScene()
{
    delete m_cube;
    s_scenes.erase(this);
}

void GameScene::render()
{
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport((int)m_viewport[0], (int)m_viewport[1], (int)m_viewport[2], (int)m_viewport[3]);

    glm::mat4 mult = m_selMode ? m_pickMat : glm::mat4(1.0f);
    glm::mat4 proj = mult * m_projection;
    glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -7.0f));
    m_program->use();
    unsigned int projection_id = glGetUniformLocation(m_program->id(), "projection");
    glUniformMatrix4fv(projection_id, 1, GL_FALSE, &proj[0][0]);
    Program::unuse();

    m_cube->render(modelview);

    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void GameScene::keyboard(unsigned char key, int x, int y)
{
    m_keyDown[key] = true;
    if (key == 'r')
        m_cube->beginRotation(FACELETID_FRONT, ANGLEID_270);
    else if (key == 'g')
        m_cube->beginRotation(FACELETID_RIGHT, ANGLEID_270);
    else if (key == 'o')
        m_cube->beginRotation(FACELETID_BACK, ANGLEID_270);
    else if (key == 'b')
        m_cube->beginRotation(FACELETID_LEFT, ANGLEID_270);
    else if (key == 'w')
        m_cube->beginRotation(FACELETID_BOTTOM, ANGLEID_270);
    else if (key == 'y')
        m_cube->beginRotation(FACELETID_TOP, ANGLEID_270);
    else if (key == 'p')
        someshit();
}

void GameScene::keyboardUp(unsigned char key, int x, int y)
{
    m_keyDown[key] = false;
}

void GameScene::special(int key, int x, int y)
{
    m_specialDown[key] = true;
}

void GameScene::specialUp(int key, int x, int y)
{
    m_specialDown[key] = false;
}

void GameScene::reshape(int width, int height)
{
    m_viewport = glm::vec4(0.0f, 0.0f, (float)width, (float)height);
    m_projection = glm::perspective(90.0f, m_viewport[2] / m_viewport[3], 1.0f, 100.0f);
}

void GameScene::mouse(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        m_mouseRotation = true;
        m_mouseX = (float)x;
        m_mouseY = (float)y;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        m_mouseRotation = false;
    }
}

void GameScene::motion(int x, int y)
{
    if (!m_mouseRotation) return;
    float diffx = x - m_mouseX;
    float diffy = y - m_mouseY;
    m_cube->rotateCube(ROTAXIS_Y, 0.5f * diffx, glm::mat4(1.0f), m_projection);
    m_cube->rotateCube(ROTAXIS_X, 0.5f * diffy, glm::mat4(1.0f), m_projection);
    m_mouseX = (float)x; m_mouseY = (float)y;
    glutPostRedisplay();
}

void GameScene::someshit()
{
    struct zb
    {
        int count;
        unsigned int depmin, depmax;
        int x, y, z;
        int col;
    };
    glSelectBuffer(sizeof(m_selBuf), m_selBuf);
    m_pickMat = glm::pickMatrix(glm::vec2(m_mouseX, m_viewport[3] - m_mouseY),
            glm::vec2(5.0f, 5.0f),
            m_viewport);
    glRenderMode(GL_SELECT);
    glInitNames();
    m_selMode = true;
    render();
    m_selMode = false;
    int count = glRenderMode(GL_RENDER);
    zb *kos = (zb*)m_selBuf;
    zb *ww = kos;
    while (count--)
    {
        if (ww->depmin > kos->depmin)
            ww = kos;
        cout << "hit: " << kos->depmin << " " << kos->depmax << " " << kos->x << " " << kos->y << " " << kos->z << " " << kos->col << endl;
        kos++;
        cout << "min: " << ww->depmin << " " << ww->depmax << " " << ww->x << " " << ww->y << " " << ww->z << " " << ww->col << endl;
    }

}

void GameScene::s_timer(int x)
{
    GameScene *that = reinterpret_cast<GameScene*>(x);
    if (s_scenes.find(that) != s_scenes.end())
        that->timer();
}

void GameScene::timer()
{
    if (m_specialDown[GLUT_KEY_LEFT])
    {
        m_cube->rotateCube(ROTAXIS_Y, -1.0, glm::mat4(1.0), glm::mat4(m_projection));
        glutPostRedisplay();
    }
    else if (m_specialDown[GLUT_KEY_RIGHT])
    {
        m_cube->rotateCube(ROTAXIS_Y, 1.0, glm::mat4(1.0), glm::mat4(m_projection));
        glutPostRedisplay();
    }
    if (m_specialDown[GLUT_KEY_UP])
    {
        m_cube->rotateCube(ROTAXIS_X, -1.0, glm::mat4(1.0), glm::mat4(m_projection));
        glutPostRedisplay();
    }
    else if (m_specialDown[GLUT_KEY_DOWN])
    {
        m_cube->rotateCube(ROTAXIS_X, 1.0, glm::mat4(1.0), glm::mat4(m_projection));
        glutPostRedisplay();
    }
    if (m_keyDown['/'])
    {
        m_cube->rotateCube(ROTAXIS_Z, -1.0, glm::mat4(1.0), glm::mat4(m_projection));
        glutPostRedisplay();
    }
    else if (m_keyDown['.'])
    {
        m_cube->rotateCube(ROTAXIS_Z, 1.0, glm::mat4(1.0), glm::mat4(m_projection));
        glutPostRedisplay();
    }
    m_cube->updateRotation();
    glutTimerFunc(10, s_timer, reinterpret_cast<int>(this));
}
