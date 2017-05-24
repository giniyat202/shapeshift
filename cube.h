#ifndef CUBE_H
#define CUBE_H

#include "cubelet.h"
#include "program.h"
#include <glm/mat4x4.hpp>

enum ROTAXIS
{
    ROTAXIS_X = 0,
    ROTAXIS_Y,
    ROTAXIS_Z,
    ROTAXIS_MAX,
};

class Cube
{
public:
    Cube(const Program &program);
    ~Cube();
    void resetRotation();
    void beginRotation(int whichFace, int whichAngle);
    void updateRotation();
    bool isRotating() const;
    void rotateCube(int whichAxis, float angle, const glm::mat4 &modelview, const glm::mat4 &projection);
    void render(const glm::mat4 &modelview) const;
private:
    void rotate(int whichFace, int whichAngle);
    static void getAxes(const glm::mat4 &modelview, const glm::mat4 &projection, glm::vec3 *axes);
    Cubelet *m_face[FACELETID_MAX][9];
    Cubelet *m_cubelet[3][3][3];
    int m_animStep;
    int m_animFinalFace;
    int m_animFinalAngle;
    float m_animAngle;
    float m_angleDiff;
    glm::vec3 m_rotAxis;
    int m_rotCondition[3];
    glm::mat4 m_rotMat;
    static const int m_stepsPerTurn;
};
#endif

