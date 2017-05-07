#include "cubelet.h"
#include <string.h>
#include <assert.h>
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>

const float Cubelet::m_defCols[COLORID_MAX][3] = {
    { 1.0f, 0.0f, 0.0f, }, // red
    { 0.0f, 1.0f, 0.0f, }, // green
    { 1.0f, 0.5f, 0.0f, }, // orange
    { 0.0f, 0.0f, 1.0f, }, // blue
    { 1.0f, 1.0f, 1.0f, }, // white
    { 1.0f, 1.0f, 0.0f, }, // yellow
    { 0.1f, 0.1f, 0.1f, }, // black
};
const float Cubelet::m_faceletShine = 70.0f;
const float Cubelet::m_blankShine = 1.0f;
const double Cubelet::m_faceletLength = 0.45;
const double Cubelet::m_faceletThickness = 0.001;
const double Cubelet::m_faceletRadius = 0.10;
const float Cubelet::m_faceletSpecular[3] = { 0.6f, 0.6f, 0.6f, };
const float Cubelet::m_blankSpecular[3] = { 0.1f, 0.1f, 0.1f, };

const int VERTS_PER_CIRCLE = 80;
const double VERTS_DIFF = 2.0 * M_PI / VERTS_PER_CIRCLE;

Cubelet::Cubelet()
{
    memcpy(m_cols, m_defCols, sizeof(m_cols));
    memset(m_faceletVisible, 0, sizeof(m_faceletVisible));
    resetRotation();
}

Cubelet::~Cubelet()
{

}

const float *Cubelet::getColor(int whichColor) const
{
    assert(whichColor >= 0 && whichColor < COLORID_MAX);
    return m_cols[whichColor];
}

void Cubelet::setColor(int whichColor, const float *newColor)
{
    assert(whichColor >= 0 && whichColor < COLORID_MAX);
    memcpy(m_cols[whichColor], newColor, sizeof(m_cols[whichColor]));
}

void Cubelet::setColor(int whichColor, float r, float g, float b)
{
    float newColor[] = { r, g, b, };
    setColor(whichColor, newColor);
}

bool Cubelet::getFaceletVisible(int whichFacelet) const
{
    assert(whichFacelet >= 0 && whichFacelet < FACELETID_MAX);
    return m_faceletVisible[whichFacelet];
}

void Cubelet::setFaceletVisible(int whichFacelet, bool visible)
{
    assert(whichFacelet >= 0 && whichFacelet < FACELETID_MAX);
    m_faceletVisible[whichFacelet] = visible;
}

int Cubelet::getFaceletColorID(int whichFacelet) const
{
    assert(whichFacelet >= 0 && whichFacelet < FACELETID_MAX);
    return m_faceletCols[whichFacelet];
}

void Cubelet::setFaceletColorID(int whichFacelet, int colorID)
{
    assert(whichFacelet >= 0 && whichFacelet < FACELETID_MAX);
    assert(colorID >= 0 && colorID < COLORID_MAX);
    m_faceletCols[whichFacelet] = colorID;
}

void Cubelet::resetRotation()
{
    m_faceletCols[FACELETID_FRONT] = COLORID_FRONT;
    m_faceletCols[FACELETID_RIGHT] = COLORID_RIGHT;
    m_faceletCols[FACELETID_BACK] = COLORID_BACK;
    m_faceletCols[FACELETID_LEFT] = COLORID_LEFT;
    m_faceletCols[FACELETID_BOTTOM] = COLORID_BOTTOM;
    m_faceletCols[FACELETID_TOP] = COLORID_TOP;
}

void Cubelet::rotate(int whichFacelet, int whichAngle)
{
    assert(whichFacelet >= 0 && whichFacelet < FACELETID_MAX);
    assert(whichAngle >= 0 && whichAngle < ANGLEID_MAX);
    static const int rotTable[][4] = {
        { FACELETID_TOP, FACELETID_RIGHT, FACELETID_BOTTOM, FACELETID_LEFT, },
        { FACELETID_TOP, FACELETID_BACK, FACELETID_BOTTOM, FACELETID_FRONT, },
        { FACELETID_TOP, FACELETID_LEFT, FACELETID_BOTTOM, FACELETID_RIGHT, },
        { FACELETID_TOP, FACELETID_FRONT, FACELETID_BOTTOM, FACELETID_BACK, },
        { FACELETID_FRONT, FACELETID_RIGHT, FACELETID_BACK, FACELETID_LEFT, },
        { FACELETID_BACK, FACELETID_RIGHT, FACELETID_FRONT, FACELETID_LEFT, },
    };
    static const int angleTable[] = { 1, 2, 3 };
    int temp = m_faceletCols[rotTable[whichFacelet][0]];
    for (int i = 0; i < 3; i++)
    {
        int replace = (i + angleTable[whichAngle]) % 4;
        m_faceletCols[rotTable[whichFacelet][i]] = m_faceletCols[rotTable[whichFacelet][replace]];
    }
    m_faceletCols[rotTable[whichFacelet][3]] = temp;
}

void Cubelet::render() const
{
    static const double angley[] = { 0.0, 90.0, 180.0, 270.0, 0.0, 0.0, };
    static const double anglex[] = { 0.0, 0.0, 0.0, 0.0, 90.0, 270.0, };
    for (int i = 0; i < FACELETID_MAX; i++)
    {
        glPushMatrix();
        glRotated(angley[i], 0.0, 1.0, 0.0);
        glRotated(anglex[i], 1.0, 0.0, 0.0);
        glTranslated(0.0, 0.0, 0.5);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, m_cols[COLORID_BLANK]);
        glMaterialfv(GL_FRONT, GL_SPECULAR, m_blankSpecular);
        glMaterialf(GL_FRONT, GL_SHININESS, m_blankShine);
        glBegin(GL_QUADS);
        glVertex3d(0.5, 0.5, 0.0);
        glVertex3d(-0.5, 0.5, 0.0);
        glVertex3d(-0.5, -0.5, 0.0);
        glVertex3d(0.5, -0.5, 0.0);
        glEnd();
        if (m_faceletVisible[i])
        {
            glMaterialfv(GL_FRONT, GL_DIFFUSE, m_cols[m_faceletCols[i]]);
            glMaterialfv(GL_FRONT, GL_SPECULAR, m_faceletSpecular);
            glMaterialf(GL_FRONT, GL_SHININESS, m_faceletShine);
            renderFacelet();
        }
        glPopMatrix();
    }
}

void Cubelet::renderFacelet()
{
    static const double shifted = m_faceletLength - m_faceletRadius;
    static const double guidex[] = { shifted, -shifted, -shifted, shifted, };
    static const double guidey[] = { shifted, shifted, -shifted, -shifted, };
    static const int VERTS_PER_QUARTER = VERTS_PER_CIRCLE / 4;
    static double vertx[VERTS_PER_CIRCLE], verty[VERTS_PER_CIRCLE];
    static bool calculated = false;
    if (!calculated)
    {
        double angle = 0.0;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < VERTS_PER_QUARTER; j++)
            {
                vertx[i * VERTS_PER_QUARTER + j] = guidex[i] + cos(angle) * m_faceletRadius;
                verty[i * VERTS_PER_QUARTER + j] = guidey[i] + sin(angle) * m_faceletRadius;
                angle += VERTS_DIFF;
            }
        }
        calculated = true;
    }

    glBegin(GL_POLYGON);
    for (int i = 0; i < VERTS_PER_CIRCLE; i++)
        glVertex3d(vertx[i], verty[i], m_faceletThickness);
    glEnd();
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < VERTS_PER_CIRCLE; i++)
    {
        glVertex3d(vertx[i], verty[i], m_faceletThickness);
        glVertex3d(vertx[i], verty[i], 0.0);
    }
    glEnd();
}
