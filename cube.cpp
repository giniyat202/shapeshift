#include "cube.h"
#include <assert.h>
#include <GL/glut.h>
#include <stdio.h>

Cube::Cube()
{
    Cubelet *seed[FACELETID_MAX][9] = {
        {
            &m_cubelet[0][2][2], &m_cubelet[1][2][2], &m_cubelet[2][2][2],
            &m_cubelet[0][1][2], &m_cubelet[1][1][2], &m_cubelet[2][1][2],
            &m_cubelet[0][0][2], &m_cubelet[1][0][2], &m_cubelet[2][0][2],
        }, {
            &m_cubelet[2][2][2], &m_cubelet[2][2][1], &m_cubelet[2][2][0],
            &m_cubelet[2][1][2], &m_cubelet[2][1][1], &m_cubelet[2][1][0],
            &m_cubelet[2][0][2], &m_cubelet[2][0][1], &m_cubelet[2][0][0],
        }, {
            &m_cubelet[2][2][0], &m_cubelet[1][2][0], &m_cubelet[0][2][0],
            &m_cubelet[2][1][0], &m_cubelet[1][1][0], &m_cubelet[0][1][0],
            &m_cubelet[2][0][0], &m_cubelet[1][0][0], &m_cubelet[0][0][0],
        }, {
            &m_cubelet[0][2][0], &m_cubelet[0][2][1], &m_cubelet[0][2][2],
            &m_cubelet[0][1][0], &m_cubelet[0][1][1], &m_cubelet[0][1][2],
            &m_cubelet[0][0][0], &m_cubelet[0][0][1], &m_cubelet[0][0][2],
        }, {
            &m_cubelet[0][0][2], &m_cubelet[1][0][2], &m_cubelet[2][0][2],
            &m_cubelet[0][0][1], &m_cubelet[1][0][1], &m_cubelet[2][0][1],
            &m_cubelet[0][0][0], &m_cubelet[1][0][0], &m_cubelet[2][0][0],
        }, {
            &m_cubelet[0][2][0], &m_cubelet[1][2][0], &m_cubelet[2][2][0],
            &m_cubelet[0][2][1], &m_cubelet[1][2][1], &m_cubelet[2][2][1],
            &m_cubelet[0][2][2], &m_cubelet[1][2][2], &m_cubelet[2][2][2],
        },
    };
    memcpy(m_face, seed, sizeof(m_face));
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                Cubelet *cubelet = &m_cubelet[i][j][k];
                cubelet->setFaceletVisible(FACELETID_FRONT, true);
                cubelet->setFaceletVisible(FACELETID_RIGHT, true);
                cubelet->setFaceletVisible(FACELETID_BACK, true);
                cubelet->setFaceletVisible(FACELETID_LEFT, true);
                cubelet->setFaceletVisible(FACELETID_BOTTOM, true);
                cubelet->setFaceletVisible(FACELETID_TOP, true);
            }
        }
    }
}

Cube::~Cube()
{

}

void Cube::resetRotation()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++)
                m_cubelet[i][j][k].resetRotation();
}

void Cube::rotate(int whichFace, int whichAngle)
{
    assert(whichFace >= 0 && whichFace < FACELETID_MAX);
    assert(whichAngle >= 0 && whichAngle < ANGLEID_MAX);
    static const int mapped[][9] = {
        {
            2, 5, 8,
            1, 4, 7,
            0, 3, 6,
        }, {
            8, 7, 6,
            5, 4, 3,
            2, 1, 0,
        }, {
            6, 3, 0,
            7, 4, 1,
            8, 5, 2,
        },
    };
    int temp[9][FACELETID_MAX];
    for (int i = 0; i < 9; i++)
        m_face[whichFace][i]->rotate(whichFace, whichAngle);
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < FACELETID_MAX; j++)
            temp[i][j] = m_face[whichFace][i]->getFaceletColorID(j);
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < FACELETID_MAX; j++)
        {
            int replace = mapped[whichAngle][i];
            m_face[whichFace][i]->setFaceletColorID(j, temp[replace][j]);
        }
}

void Cube::render() const
{
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            for (int k = -1; k <= 1; k++)
            {
                glPushMatrix();
                glTranslated((double)i, (double)j, (double)k);
                m_cubelet[i + 1][j + 1][k + 1].render();
                const Cubelet *cubelet = &m_cubelet[i+1][j+1][k+1];
                glPopMatrix();
            }
        }
    }
}
