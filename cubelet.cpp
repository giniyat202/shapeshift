#include "cubelet.h"
#include <gl/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

const Material Cubelet::m_defMtls[MATERIALID_MAX] = {
    // red
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.9f, 0.0f, 0.0f),
        glm::vec3(0.1f, 0.2f, 0.2f),
        32.0f
    },
    // green
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.1f, 0.55f, 0.1f),
        glm::vec3(0.2f, 0.1f, 0.2f),
        32.0f
    },
    // orange
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.9f, 0.4f, 0.08f),
        glm::vec3(0.1f, 0.15f, 0.2f),
        32.0f
    },
    // blue
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.5f),
        glm::vec3(0.2f, 0.2f, 0.1f),
        32.0f
    },
    // white
    {
        glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.77f, 0.77f, 0.77f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        32.0f
    },
    //  yellow
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.0f),
        glm::vec3(0.1f, 0.1f, 0.2f),
        32.0f
    },
    // black
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.01f, 0.01f, 0.01f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        32.0f
    },
};
const float Cubelet::m_faceletLength = 0.45f;
const float Cubelet::m_faceletThickness = 0.003f;
const float Cubelet::m_faceletRadius = 0.10f;
std::map<const Program *, Cubelet::PROGBUFS> Cubelet::m_progBufs;

Cubelet::Cubelet(const Program &program)
{
    m_Program = &program;
    PROGBUFS &bufs = m_progBufs[m_Program];
    if (bufs.refcount == 0)
        genModels(bufs, program);
    bufs.refcount++;
    for (int i = 0; i < MATERIALID_MAX; i++)
        m_mtls[i] = m_defMtls[i];
    for (int i = 0; i < FACELETID_MAX; i++)
        m_faceletVisible[i] = false;
    resetRotation();
}

Cubelet::~Cubelet()
{
    PROGBUFS &bufs = m_progBufs[m_Program];
    --bufs.refcount;
    if (bufs.refcount <= 0)
    {
        glDeleteBuffers(1, &bufs.BVBO);
        glDeleteBuffers(1, &bufs.BEBO);
        glDeleteBuffers(1, &bufs.FVBO);
        glDeleteBuffers(1, &bufs.FEBO);
        glDeleteVertexArrays(1, &bufs.BVAO);
        glDeleteVertexArrays(1, &bufs.FVAO);
        m_progBufs.erase(m_Program);
    }
}

const Material &Cubelet::getMaterial(int whichMaterial) const
{
    assert(whichMaterial >= 0 && whichMaterial < MATERIALID_MAX);
    return m_mtls[whichMaterial];
}

void Cubelet::setMaterial(int whichMaterial, const Material &newMaterial)
{
    assert(whichMaterial >= 0 && whichMaterial < MATERIALID_MAX);
    m_mtls[whichMaterial] = newMaterial;
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

int Cubelet::getFaceletMaterialID(int whichFacelet) const
{
    assert(whichFacelet >= 0 && whichFacelet < FACELETID_MAX);
    return m_faceletMtls[whichFacelet];
}

void Cubelet::setFaceletMaterialID(int whichFacelet, int materialID)
{
    assert(whichFacelet >= 0 && whichFacelet < FACELETID_MAX);
    assert(materialID >= 0 && materialID < MATERIALID_MAX);
    m_faceletMtls[whichFacelet] = materialID;
}

void Cubelet::resetRotation()
{
    m_faceletMtls[FACELETID_FRONT] = MATERIALID_FRONT;
    m_faceletMtls[FACELETID_RIGHT] = MATERIALID_RIGHT;
    m_faceletMtls[FACELETID_BACK] = MATERIALID_BACK;
    m_faceletMtls[FACELETID_LEFT] = MATERIALID_LEFT;
    m_faceletMtls[FACELETID_BOTTOM] = MATERIALID_BOTTOM;
    m_faceletMtls[FACELETID_TOP] = MATERIALID_TOP;
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
    int temp = m_faceletMtls[rotTable[whichFacelet][0]];
    for (int i = 0; i < 3; i++)
    {
        int replace = (i + angleTable[whichAngle]) % 4;
        m_faceletMtls[rotTable[whichFacelet][i]] = m_faceletMtls[rotTable[whichFacelet][replace]];
    }
    m_faceletMtls[rotTable[whichFacelet][3]] = temp;
}

void Cubelet::render(const glm::mat4 &modelview) const
{
    const Program &program = *m_Program;
    const PROGBUFS &bufs = m_progBufs[m_Program];
    static const float anglex[] = { 0.0f, 0.0f, 0.0f, 0.0f, 90.0f, 270.0f };
    static const float angley[] = { 0.0f, 90.0f, 180.0f, 270.0f, 0.0f, 0.0f };
    const unsigned int modelview_id = glGetUniformLocation(m_Program->id(), "modelview");
    const unsigned int timodel_id = glGetUniformLocation(m_Program->id(), "timodel");
    glm::mat3 ti = glm::mat3(glm::transpose(glm::inverse(modelview)));
    program.use();
    glUniformMatrix4fv(modelview_id, 1, GL_FALSE, &modelview[0][0]);
    glUniformMatrix3fv(timodel_id, 1, GL_FALSE, &ti[0][0]);
    Program::unuse();
    applyMaterial(program, m_mtls[MATERIALID_BLANK]);
    glPushName(FACELETID_MAX);
    renderBlank(bufs, program);
    glPopName();
    for (int i = 0; i < FACELETID_MAX; i++)
    {
        if (!m_faceletVisible[i]) continue;
        glm::mat4 rotx = glm::rotate(glm::mat4(1.0f), glm::radians(anglex[i]), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 roty = glm::rotate(glm::mat4(1.0f), glm::radians(angley[i]), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 tr = modelview * rotx * roty;
        ti = glm::mat3(glm::transpose(glm::inverse(tr)));
        program.use();
        glUniformMatrix4fv(modelview_id, 1, GL_FALSE, &tr[0][0]);
        glUniformMatrix3fv(timodel_id, 1, GL_FALSE, &ti[0][0]);
        Program::unuse();
        applyMaterial(program, m_mtls[m_faceletMtls[i]]);
        glPushName(i);
        renderFacelet(bufs, program);
        glPopName();
    }
}

void Cubelet::genModels(PROGBUFS &bufs, const Program &program)
{
    genBlank(bufs, program);
    genFacelet(bufs, program);
}

void Cubelet::genBlank(PROGBUFS &bufs, const Program &program)
{
    struct customv
    {
        float pos[3];
        float norm[3];
    };

    static const glm::vec3 norms[] = {
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
    };

    customv vertex[] = {
        {  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f },
        { -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f },
        { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f },
        {  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f },
        {  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
        { -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
        { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
        {  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
    };
    static const unsigned int index[] = {
        0, 1, 2,
        2, 3, 0,

        4, 0, 3,
        3, 7, 4,

        5, 4, 7,
        7, 6, 5,

        1, 5, 6,
        6, 2, 1,

        3, 2, 6,
        6, 7, 3,

        4, 5, 1,
        1, 0, 4,
    };

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            for (int k = 0; k < 3; k++)
                vertex[index[i * 6 + j]].norm[k] += norms[i][k];
        }
    }
    glGenVertexArrays(1, &bufs.BVAO);
    glGenBuffers(1, &bufs.BVBO);
    glGenBuffers(1, &bufs.BEBO);
    glBindVertexArray(bufs.BVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bufs.BVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufs.BEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    program.use();
    unsigned int position_id = glGetAttribLocation(program.id(), "position");
    unsigned int normal_id = glGetAttribLocation(program.id(), "normal");
    glEnableVertexAttribArray(position_id);
    glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, sizeof(customv), 0);
    glEnableVertexAttribArray(normal_id);
    glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, sizeof(customv), (void*)12);
    glBindVertexArray(0);
    Program::unuse();
}

void Cubelet::genFacelet(Cubelet::PROGBUFS &bufs, const Program &program)
{
    struct customv
    {
        float pos[3];
        float norm[3];
    };

    static const glm::vec3 norms[] = {
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
    };

    customv vertex[] = {
        {  m_faceletLength,  m_faceletLength, 0.5f + m_faceletThickness, 0.0f, 0.0f, 0.0f },
        { -m_faceletLength,  m_faceletLength, 0.5f + m_faceletThickness, 0.0f, 0.0f, 0.0f },
        { -m_faceletLength, -m_faceletLength, 0.5f + m_faceletThickness, 0.0f, 0.0f, 0.0f },
        {  m_faceletLength, -m_faceletLength, 0.5f + m_faceletThickness, 0.0f, 0.0f, 0.0f },
        {  m_faceletLength,  m_faceletLength, 0.5f, 0.0f, 0.0f, 0.0f },
        { -m_faceletLength,  m_faceletLength, 0.5f, 0.0f, 0.0f, 0.0f },
        { -m_faceletLength, -m_faceletLength, 0.5f, 0.0f, 0.0f, 0.0f },
        {  m_faceletLength, -m_faceletLength, 0.5f, 0.0f, 0.0f, 0.0f },
    };
    static const unsigned int index[] = {
        0, 1, 2,
        2, 3, 0,

        4, 0, 3,
        3, 7, 4,

        5, 4, 7,
        7, 6, 5,

        1, 5, 6,
        6, 2, 1,

        3, 2, 6,
        6, 7, 3,

        4, 5, 1,
        1, 0, 4,
    };

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            for (int k = 0; k < 3; k++)
                vertex[index[i * 6 + j]].norm[k] += norms[i][k];
        }
    }
    glGenVertexArrays(1, &bufs.FVAO);
    glGenBuffers(1, &bufs.FVBO);
    glGenBuffers(1, &bufs.FEBO);
    glBindVertexArray(bufs.FVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bufs.FVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufs.FEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    unsigned int position_id = glGetAttribLocation(program.id(), "position");
    unsigned int normal_id = glGetAttribLocation(program.id(), "normal");
    glEnableVertexAttribArray(position_id);
    glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, sizeof(customv), 0);
    glEnableVertexAttribArray(normal_id);
    glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, sizeof(customv), (void*)12);
    glBindVertexArray(0);
}

void Cubelet::applyMaterial(const Program &program, const Material &material)
{
    program.use();
    unsigned int material_ambient_id = glGetUniformLocation(program.id(), "material.ambient");
    unsigned int material_diffuse_id = glGetUniformLocation(program.id(), "material.diffuse");
    unsigned int material_specular_id = glGetUniformLocation(program.id(), "material.specular");
    unsigned int material_shininess_id = glGetUniformLocation(program.id(), "material.shininess");
    glUniform3fv(material_ambient_id, 1, &material.ambient[0]);
    glUniform3fv(material_diffuse_id, 1, &material.diffuse[0]);
    glUniform3fv(material_specular_id, 1, &material.specular[0]);
    glUniform1fv(material_shininess_id, 1, &material.shininess);
    Program::unuse();
}

void Cubelet::renderBlank(const Cubelet::PROGBUFS &bufs, const Program &program)
{
    program.use();
    glBindVertexArray(bufs.BVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    Program::unuse();
}

void Cubelet::renderFacelet(const Cubelet::PROGBUFS &bufs, const Program &program)
{
    program.use();
    glBindVertexArray(bufs.FVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    Program::unuse();
}
