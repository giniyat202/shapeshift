#include "gamescene.h"
#include <GL/glew.h>
#include <gl/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include "test_frag.h"
#include "test_vert.h"
using namespace std;

std::set<GameScene*> GameScene::s_scenes;

GameScene::GameScene() :
    m_mouseRotation(false),
    m_selMode(false),
    m_firstSelResFound(false)
{
    initshader();
    reshape(800, 600);
    srand(time(0));
    m_cube = new Cube(*m_program);
    m_cube->rotateCube(ROTAXIS_Y, glm::radians(45.0f), m_projection);
    m_cube->rotateCube(ROTAXIS_X, glm::radians(30.0f), m_projection);
    m_cube->setStepsPerTurn(4);
    for (int i = 0; i < 100; i++)
    {
        m_cube->beginRotation(rand() % FACELETID_MAX, rand() % ANGLEID_MAX);
        m_cube->updateRotation();
        m_cube->updateRotation();
    }
    m_cube->setStepsPerTurn(200);
    memset(m_keyDown, 0, sizeof(m_keyDown));
    memset(m_specialDown, 0, sizeof(m_specialDown));
    s_scenes.insert(this);
    s_timer(reinterpret_cast<int>(this));
}

GameScene::~GameScene()
{
    delete m_cube;
    s_scenes.erase(this);
    delete m_program;
}

void GameScene::render()
{
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport((int)m_viewport[0], (int)m_viewport[1], (int)m_viewport[2], (int)m_viewport[3]);

    glm::mat4 mult = m_selMode ? m_pickMat : glm::mat4(1.0f);
    glm::mat4 proj = mult * m_projection;
    glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
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
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        m_firstSelRes = find_sel((float)x, (float)y);
        for (int i = 0; i < m_firstSelRes.count; ++i)
            cout << m_firstSelRes.matches[i]<< endl;
        if (m_firstSelRes.count > 0)
            m_firstSelResFound = true;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        m_firstSelResFound = false;
    }
}

void GameScene::motion(int x, int y)
{
    if (m_mouseRotation)
    {
        float diffx = x - m_mouseX;
        float diffy = y - m_mouseY;
        m_cube->rotateCube(ROTAXIS_Y, glm::radians(0.5f * diffx), m_projection);
        m_cube->rotateCube(ROTAXIS_X, glm::radians(0.5f * diffy), m_projection);
        m_mouseX = (float)x; m_mouseY = (float)y;
        glutPostRedisplay();
    }
    else if (m_firstSelResFound)
    {
        SELRES second = find_sel((float)x, (float)y);
        vector<int> v(m_firstSelRes.matches, m_firstSelRes.matches + m_firstSelRes.count);
        vector<int>oko(second.matches, second.matches + second.count);
        v.insert(v.end(), oko.begin(), oko.end());
        sort(v.begin(), v.end());
        vector<int>::iterator it = unique(v.begin(), v.end());
        if (v.end() - it == 1)
        {
            int fta7 = *it;
            cout <<"aaaa"<< fta7 << endl;
            m_firstSelResFound = false;
        }
    }
}

void GameScene::initshader()
{
    shader_test_vert.create();
    shader_test_vert.compile();
    shader_test_frag.create();
    shader_test_frag.compile();
    m_program = new Program();
    Program &prog = *m_program;
    prog.create();
    prog += shader_test_vert;
    prog += shader_test_frag;
    prog.link();
    shader_test_vert.destroy();
    shader_test_frag.destroy();

    prog.use();
    glm::vec3 light_position(0.0f, 0.0f, -1.0f);
    glm::vec3 light_ambient(0.2f, 0.2f, 0.2f);
    glm::vec3 light_diffuse(0.7f, 0.7f, 0.7f);
    glm::vec3 light_specular(1.0f, 1.0f, 1.0f);
    unsigned int light_position_id = glGetUniformLocation(prog.id(), "light.position");
    unsigned int light_ambient_id = glGetUniformLocation(prog.id(), "light.ambient");
    unsigned int light_diffuse_id = glGetUniformLocation(prog.id(), "light.diffuse");
    unsigned int light_specular_id = glGetUniformLocation(prog.id(), "light.specular");
    glUniform3fv(light_position_id, 1, &light_position[0]);
    glUniform3fv(light_ambient_id, 1, &light_ambient[0]);
    glUniform3fv(light_diffuse_id, 1, &light_diffuse[0]);
    glUniform3fv(light_specular_id, 1, &light_specular[0]);
    Program::unuse();
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
        m_cube->rotateCube(ROTAXIS_Y, glm::radians(-1.0f), glm::mat4(m_projection));
        glutPostRedisplay();
    }
    else if (m_specialDown[GLUT_KEY_RIGHT])
    {
        m_cube->rotateCube(ROTAXIS_Y, glm::radians(1.0f), glm::mat4(m_projection));
        glutPostRedisplay();
    }
    if (m_specialDown[GLUT_KEY_UP])
    {
        m_cube->rotateCube(ROTAXIS_X, glm::radians(-1.0f), glm::mat4(m_projection));
        glutPostRedisplay();
    }
    else if (m_specialDown[GLUT_KEY_DOWN])
    {
        m_cube->rotateCube(ROTAXIS_X, glm::radians(1.0f), glm::mat4(m_projection));
        glutPostRedisplay();
    }
    if (m_keyDown['/'])
    {
        m_cube->rotateCube(ROTAXIS_Z, glm::radians(-1.0f), glm::mat4(m_projection));
        glutPostRedisplay();
    }
    else if (m_keyDown['.'])
    {
        m_cube->rotateCube(ROTAXIS_Z, glm::radians(1.0f), glm::mat4(m_projection));
        glutPostRedisplay();
    }
    m_cube->updateRotation();
    glutTimerFunc(10, s_timer, reinterpret_cast<int>(this));
}

GameScene::SELRES GameScene::find_sel(float x, float y)
{
    SELRES result;
    struct NAMES
    {
        int count;
        unsigned int depmin, depmax;
        int i, j, k;
        int face;
    };
    glSelectBuffer(sizeof(m_selBuf), m_selBuf);
    m_pickMat = glm::pickMatrix(glm::vec2(x, m_viewport[3] - y),
            glm::vec2(5.0f, 5.0f),
            m_viewport);
    glRenderMode(GL_SELECT);
    glInitNames();
    m_selMode = true;
    render();
    m_selMode = false;
    int count = glRenderMode(GL_RENDER);
    NAMES *ptr = (NAMES*)m_selBuf;
    NAMES *pmin = ptr;
    if (count > 0)
    {
        while (count--)
        {
            assert(ptr->count == 4);
            if (pmin->depmin > ptr->depmin)
                pmin = ptr;
            ptr++;
        }
        if (pmin->i == -1 && pmin->face != FACELETID_LEFT)
            result.matches[result.count++] = FACELETID_LEFT;
        else if (pmin->i == 1 && pmin->face != FACELETID_RIGHT)
            result.matches[result.count++] = FACELETID_RIGHT;
        if (pmin->j == -1 && pmin->face != FACELETID_BOTTOM)
            result.matches[result.count++] = FACELETID_BOTTOM;
        else if (pmin->j == 1 && pmin->face != FACELETID_TOP)
            result.matches[result.count++] = FACELETID_TOP;
        if (pmin->k == -1 && pmin->face != FACELETID_BACK)
            result.matches[result.count++] = FACELETID_BACK;
        else if (pmin->k == 1 && pmin->face != FACELETID_FRONT)
            result.matches[result.count++] = FACELETID_FRONT;
    }
    return result;
}
