#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <set>
#include "shader.h"
class Program
{
public:
    Program(bool createProgram = false);
    ~Program();
    void create();
    void destroy();
    bool isCreated() const;
    unsigned int id() const;
    void addShader(Shader *shader);
    void removeShader(Shader *shader);
    Program &operator +=(Shader &shader);
    Program &operator -=(Shader &shader);
    const std::set<Shader *> &getShaders() const;
    bool link();
    bool isLinked() const;
    void use() const;
    static void unuse();
    const std::string &errorLog() const;
private:
    unsigned int m_id;
    bool m_created;
    bool m_linked;
    std::string m_errorLog;
    std::set<Shader*> m_shaders;
};

#endif
