#include "program.h"
#include <GL/glew.h>
#include <assert.h>

Program::Program(bool createProgram) :
    m_id(0),
    m_created(false),
    m_linked(false)
{
    if (createProgram)
        create();
}

Program::~Program()
{
    destroy();
}

void Program::create()
{
    assert(!m_created);
    m_id = glCreateProgram();
    m_created = true;
    m_linked = false;
}

void Program::destroy()
{
    if (m_created)
    {
        while (!m_shaders.empty())
            removeShader(*m_shaders.begin());
        glDeleteProgram(m_id);
        m_created = false;
        m_linked = false;
    }
}

bool Program::isCreated() const
{
    return m_created;
}

unsigned int Program::id() const
{
    return m_id;
}

void Program::addShader(Shader *shader)
{
    assert(m_created);
    assert(shader != NULL);
    assert(shader->isCreated());
    assert(shader->isCompiled());

    if (m_shaders.find(shader) == m_shaders.end())
    {
        m_shaders.insert(shader);
        shader->addToProgram(this);
        glAttachShader(m_id, shader->id());
    }
}

void Program::removeShader(Shader *shader)
{
    assert(m_created);
    assert(shader != NULL);
    std::set<Shader*>::iterator it = m_shaders.find(shader);
    if (it != m_shaders.end())
    {
        m_shaders.erase(it);
        shader->removeFromProgram(this);
        glDetachShader(m_id, shader->id());
    }
}

Program &Program::operator +=(Shader &shader)
{
    addShader(&shader);
    return *this;
}

Program &Program::operator -=(Shader &shader)
{
    removeShader(&shader);
    return *this;
}

const std::set<Shader *> &Program::getShaders() const
{
    return m_shaders;
}

bool Program::link()
{
    assert(m_created);
    int success, loglen;
    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    m_linked = success == GL_TRUE;
    if (!m_linked)
    {
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &loglen);
        m_errorLog.resize(loglen, 0);
        glGetProgramInfoLog(m_id, loglen, NULL, &m_errorLog[0]);
    }
    return m_linked;
}

bool Program::isLinked() const
{
    return m_linked;
}

void Program::use() const
{
    assert(m_created);
    assert(m_linked);
    glUseProgram(m_id);
}

void Program::unuse()
{
    glUseProgram(0);
}

const std::string &Program::errorLog() const
{
    return m_errorLog;
}
