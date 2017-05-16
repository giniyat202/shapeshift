#ifndef CUBE_H
#define CUBE_H

#include "cubelet.h"

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
    Cube();
    ~Cube();
    void resetRotation();
    void beginRotation(int whichFace, int whichAngle);
    void updateRotation();
    bool isRotating() const;
    void rotateCube(int whichAxis, double angle);
    void render() const;
private:
    void rotate(int whichFace, int whichAngle);
    static void getAxes(double axes[][3]);
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
