#include "shader.h"
#include <gl/glew.h>
#include <assert.h>
#include "program.h"

Shader::Shader(const std::string &source, bool createShader, unsigned int shaderType) :
    m_id(0),
    m_created(false),
    m_compiled(false),
    m_type(shaderType)
{
    if (createShader)
        create();
    setSource(source);
}

Shader::~Shader()
{
    destroy();
}

void Shader::create()
{
    assert(!m_created);
    m_id = glCreateShader(m_type);
    m_created = true;
    setSourceActual();
}

void Shader::destroy()
{
    if (m_created)
    {
        while (!m_programs.empty())
            removeFromProgram(*m_programs.begin());
        glDeleteShader(m_id);
        m_created = false;
        m_compiled = false;
    }
}

bool Shader::isCreated() const
{
    return m_created;
}

unsigned int Shader::id() const
{
    return m_id;
}

void Shader::setType(unsigned int shaderType)
{
    m_type = shaderType;
}

unsigned int Shader::type() const
{
    return m_type;
}

void Shader::setSource(const std::string &source)
{
    m_source = source;
    if (m_created)
        setSourceActual();
}

const std::string &Shader::getSource() const
{
    return m_source;
}

bool Shader::compile()
{
    assert(m_created);
    int success, loglen;
    glCompileShader(m_id);
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    m_compiled = success == GL_TRUE;
    if (!m_compiled)
    {
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &loglen);
        m_errorLog.resize(loglen, 0);
        glGetShaderInfoLog(m_id, loglen, NULL, &m_errorLog[0]);
    }
    return m_compiled;
}

bool Shader::isCompiled() const
{
    return m_compiled;
}

const std::string &Shader::errorLog() const
{
    return m_errorLog;
}

void Shader::addToProgram(Program *program)
{
    assert(m_created);
    assert(program != NULL);
    assert(program->isCreated());
    if (m_programs.find(program) == m_programs.end())
    {
        m_programs.insert(program);
        program->addShader(this);
    }
}

void Shader::removeFromProgram(Program *program)
{
    assert(m_created);
    std::set<Program*>::iterator it = m_programs.find(program);
    if (it != m_programs.end())
    {
        m_programs.erase(it);
        program->removeShader(this);
    }
}

const std::set<Program *> &Shader::getPrograms() const
{
    return m_programs;
}

void Shader::setSourceActual()
{
    assert(m_created);
    if (m_source.size() > 0)
    {
        const char *source = &m_source[0];
        glShaderSource(m_id, 1, &source, NULL);
    }
    m_compiled = false;
}
