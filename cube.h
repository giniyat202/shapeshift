#ifndef CUBE_H
#define CUBE_H

#include "cubelet.h"

class Cube
{
public:
    Cube();
    ~Cube();
    void resetRotation();
    void beginRotation(int whichFace, int whichAngle);
    void updateRotation();
    bool isRotating() const;
    void render() const;
private:
    void rotate(int whichFace, int whichAngle);
    Cubelet *m_face[FACELETID_MAX][9];
    Cubelet m_cubelet[3][3][3];
    int m_animStep;
    int m_animFinalFace;
    int m_animFinalAngle;
    double m_animAngle;
    double m_angleDiff;
    double m_rotAxis[3];
    int m_rotCondition[3];
    static int m_stepsPerTurn;
};
#endif
