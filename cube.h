#ifndef CUBE_H
#define CUBE_H

#include "cubelet.h"

class Cube
{
public:
    Cube();
    ~Cube();
    void resetRotation();
    void rotate(int whichFace, int whichAngle);
    void render() const;
private:
    Cubelet *m_face[FACELETID_MAX][9];
    Cubelet m_cubelet[3][3][3];
};
#endif
