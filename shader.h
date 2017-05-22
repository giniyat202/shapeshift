#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <set>

class Program;

class Shader
{
public:
    Shader(const std::string &source = "", bool createShader = false, unsigned int shaderType = 0);
    ~Shader();
    void create();
    void destroy();
    bool isCreated() const;
    unsigned int id() const;
    void setType(unsigned int shaderType);
    unsigned int type() const;
    void setSource(const std::string &source);
    const std::string &getSource() const;
    bool compile();
    bool isCompiled() const;
    const std::string &errorLog() const;
    void addToProgram(Program *program);
    void removeFromProgram(Program *program);
    const std::set<Program *> &getPrograms() const;
private:
    void setSourceActual();
    unsigned int m_id;
    unsigned int m_type;
    bool m_created;
    bool m_compiled;
    std::string m_source;
    std::string m_errorLog;
    std::set<Program*> m_programs;
};

#endif
