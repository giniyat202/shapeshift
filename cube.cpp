#include "cube.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>

const int Cube::m_stepsPerTurn = 800;

Cube::Cube(const Program &program) :
    m_animStep(0),
    m_animFinalFace(0),
    m_animFinalAngle(0),
    m_animAngle(0.0f),
    m_angleDiff(0.0f),
    m_rotMat(1.0f),
    m_rotAxis(0.0f)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                Cubelet *cubelet = new Cubelet(program);
                m_cubelet[i][j][k] = cubelet;
                cubelet->setFaceletVisible(FACELETID_FRONT, k == 2);
                cubelet->setFaceletVisible(FACELETID_RIGHT, i == 2);
                cubelet->setFaceletVisible(FACELETID_BACK, k == 0);
                cubelet->setFaceletVisible(FACELETID_LEFT, i == 0);
                cubelet->setFaceletVisible(FACELETID_BOTTOM, j == 0);
                cubelet->setFaceletVisible(FACELETID_TOP, j == 2);
            }
        }
    }
    Cubelet *seed[FACELETID_MAX][9] = {
        {
            m_cubelet[0][2][2], m_cubelet[1][2][2], m_cubelet[2][2][2],
            m_cubelet[0][1][2], m_cubelet[1][1][2], m_cubelet[2][1][2],
            m_cubelet[0][0][2], m_cubelet[1][0][2], m_cubelet[2][0][2],
        }, {
            m_cubelet[2][2][2], m_cubelet[2][2][1], m_cubelet[2][2][0],
            m_cubelet[2][1][2], m_cubelet[2][1][1], m_cubelet[2][1][0],
            m_cubelet[2][0][2], m_cubelet[2][0][1], m_cubelet[2][0][0],
        }, {
            m_cubelet[2][2][0], m_cubelet[1][2][0], m_cubelet[0][2][0],
            m_cubelet[2][1][0], m_cubelet[1][1][0], m_cubelet[0][1][0],
            m_cubelet[2][0][0], m_cubelet[1][0][0], m_cubelet[0][0][0],
        }, {
            m_cubelet[0][2][0], m_cubelet[0][2][1], m_cubelet[0][2][2],
            m_cubelet[0][1][0], m_cubelet[0][1][1], m_cubelet[0][1][2],
            m_cubelet[0][0][0], m_cubelet[0][0][1], m_cubelet[0][0][2],
        }, {
            m_cubelet[0][0][2], m_cubelet[1][0][2], m_cubelet[2][0][2],
            m_cubelet[0][0][1], m_cubelet[1][0][1], m_cubelet[2][0][1],
            m_cubelet[0][0][0], m_cubelet[1][0][0], m_cubelet[2][0][0],
        }, {
            m_cubelet[0][2][0], m_cubelet[1][2][0], m_cubelet[2][2][0],
            m_cubelet[0][2][1], m_cubelet[1][2][1], m_cubelet[2][2][1],
            m_cubelet[0][2][2], m_cubelet[1][2][2], m_cubelet[2][2][2],
        },
    };
    memcpy(m_face, seed, sizeof(m_face));
    memset(m_rotCondition, 0, sizeof(m_rotCondition));
}

Cube::~Cube()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                delete m_cubelet[i][j][k];
}

void Cube::resetRotation()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                m_cubelet[i][j][k]->resetRotation();
}

void Cube::beginRotation(int whichFace, int whichAngle)
{
    assert(whichFace >= 0 && whichFace < FACELETID_MAX);
    assert(whichAngle >= 0 && whichAngle < ANGLEID_MAX);
    static const glm::vec3 axismap[] = {
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
    };
    static const int condmap[][3] = {
        { 0, 0, 1, },
        { 1, 0, 0, },
        { 0, 0, -1, },
        { -1, 0, 0, },
        { 0, -1, 0, },
        { 0, 1, 0, },
    };
    static const float diffmap[] = {
        360.0f / m_stepsPerTurn,
        360.0f / m_stepsPerTurn,
        -360.0f / m_stepsPerTurn,
    };
    static const int stepmap[] = {
        m_stepsPerTurn / 4,
        m_stepsPerTurn / 2,
        m_stepsPerTurn / 4,
    };

    if (m_animStep != 0) return;
    m_animFinalFace = whichFace;
    m_animFinalAngle = whichAngle;
    m_animStep = stepmap[whichAngle];
    m_animAngle = 0.0f;
    m_angleDiff = diffmap[whichAngle];
    m_rotAxis = axismap[whichFace];
    memcpy(m_rotCondition, condmap[whichFace], sizeof(m_rotCondition));
}

void Cube::updateRotation()
{
    if (m_animStep > 0)
    {
        m_animStep--;
        m_animAngle += m_angleDiff;
        if (m_animStep == 0)
            rotate(m_animFinalFace, m_animFinalAngle);
        glutPostRedisplay();
    }
}

bool Cube::isRotating() const
{
    return m_animStep != 0;
}

void Cube::rotateCube(int whichAxis, float angle, const glm::mat4 &modelview, const glm::mat4 &projection)
{
    assert(whichAxis >= 0 && whichAxis < ROTAXIS_MAX);
    glm::vec3 axes[ROTAXIS_MAX];
    getAxes(modelview, projection, axes);
    m_rotMat = glm::rotate(m_rotMat, angle, axes[whichAxis]);
}

void Cube::render(const glm::mat4 &modelview) const
{
    glm::mat4 rotated = modelview * m_rotMat;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            for (int k = -1; k <= 1; k++)
            {
                glm::mat4 rotated2 = rotated;
                if (m_animStep != 0 &&
                        (m_rotCondition[0] == 0 || i == m_rotCondition[0]) &&
                        (m_rotCondition[1] == 0 || j == m_rotCondition[1]) &&
                        (m_rotCondition[2] == 0 || k == m_rotCondition[2]))
                {
                    rotated2 = glm::rotate(rotated2, glm::radians(m_animAngle), m_rotAxis);
                }
                glm::mat4 translated = glm::translate(rotated2, glm::vec3((float)i, (float)j, (float)k));
                m_cubelet[i + 1][j + 1][k + 1]->render(translated);
            }
        }
    }
}

void Cube::rotate(int whichFace, int whichAngle)
{
    assert(whichFace >= 0 && whichFace < FACELETID_MAX);
    assert(whichAngle >= 0 && whichAngle < ANGLEID_MAX);
    static const int mapped[9] = {
        2, 5, 8,
        1, 4, 7,
        0, 3, 6,
    };
    int temp[9][FACELETID_MAX];
    for (int k = 0; k <= whichAngle; k++)
    {
        for (int i = 0; i < 9; i++)
            m_face[whichFace][i]->rotate(whichFace, ANGLEID_90);
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < FACELETID_MAX; j++)
                temp[i][j] = m_face[whichFace][i]->getFaceletMaterialID(j);
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < FACELETID_MAX; j++)
            {
                int replace = mapped[i];
                m_face[whichFace][i]->setFaceletMaterialID(j, temp[replace][j]);
            }
        }
    }
}

void Cube::getAxes(const glm::mat4 &modelview, const glm::mat4 &projection, glm::vec3 *axes)
{
    glm::vec4 viewport;
    int viewporti[4];
    glGetIntegerv(GL_VIEWPORT, viewporti);
    for (int i = 0; i < 4; i++)
        viewport[i] = (float)viewporti[i];
    glm::vec3 win[] = {
        glm::vec3(viewport[0], viewport[1], 0.0f),
        glm::vec3(viewport[2] - viewport[0], viewport[1], 0.0f),
        glm::vec3(viewport[0], viewport[3] - viewport[1], 0.0f),
    };
    glm::vec3 p[4];
    for (int i = 0; i < 3; i++)
        p[i] = glm::unProject(win[i], modelview, projection, viewport);
    axes[ROTAXIS_X] = glm::normalize(p[1] - p[0]);
    axes[ROTAXIS_Y] = glm::normalize(p[2] - p[0]);
    axes[ROTAXIS_Z] = glm::cross(axes[ROTAXIS_X], axes[ROTAXIS_Y]);
}
