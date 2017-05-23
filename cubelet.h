#ifndef CUBELET_H
#define CUBELET_H

#include "material.h"
#include "program.h"
#include <map>
#include <glm/mat4x4.hpp>

enum MATERIALID
{
    MATERIALID_FRONT = 0,
    MATERIALID_RIGHT,
    MATERIALID_BACK,
    MATERIALID_LEFT,
    MATERIALID_BOTTOM,
    MATERIALID_TOP,
    MATERIALID_BLANK,
    MATERIALID_MAX,
};
enum FACELETID
{
    FACELETID_FRONT = 0,
    FACELETID_RIGHT,
    FACELETID_BACK,
    FACELETID_LEFT,
    FACELETID_BOTTOM,
    FACELETID_TOP,
    FACELETID_MAX,
};
enum ANGLEID
{
    ANGLEID_90,
    ANGLEID_180,
    ANGLEID_270,
    ANGLEID_MAX,
};

class Cubelet
{
public:
    Cubelet(const Program &program);
    ~Cubelet();
    const Material &getMaterial(int whichMaterial) const;
    void setMaterial(int whichMaterial, const Material &newMaterial);

    bool getFaceletVisible(int whichFacelet) const;
    void setFaceletVisible(int whichFacelet, bool visible);

    int getFaceletMaterialID(int whichFacelet) const;
    void setFaceletMaterialID(int whichFacelet, int materialID);

    void resetRotation();
    void rotate(int whichFacelet, int whichAngle);

    void render(const glm::mat4 &modelview) const;
private:
    struct PROGBUFS
    {
        PROGBUFS() :
            refcount(0),
            BVAO(0), BVBO(0), BEBO(0),
            FVAO(0), FVBO(0), FEBO(0)
        {
        }
        int refcount;
        unsigned int BVAO, BVBO, BEBO;
        unsigned int FVAO, FVBO, FEBO;
    };
    static std::map<const Program *, PROGBUFS> m_progBufs;
    static void genModels(PROGBUFS &bufs, const Program &program);
    static void genBlank(PROGBUFS &bufs, const Program &program);
    static void genFacelet(PROGBUFS &bufs, const Program &program);
    static void applyMaterial(const Program &program, const Material &material);
    static void renderBlank(const PROGBUFS &bufs, const Program &program);
    static void renderFacelet(const PROGBUFS &bufs, const Program &program);

    Material m_mtls[MATERIALID_MAX];
    int m_faceletMtls[FACELETID_MAX];
    bool m_faceletVisible[FACELETID_MAX];
    const Program *m_Program;

    static const Material m_defMtls[MATERIALID_MAX];
    static const float m_faceletLength;
    static const float m_faceletThickness;
    static const float m_faceletRadius;
};

#endif
