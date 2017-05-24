#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "scene.h"
#include "cube.h"
#include <set>

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class GameScene : public Scene
{
public:
    GameScene(const Program &program);
    ~GameScene();
    virtual void render();
    virtual void keyboard(unsigned char key, int x, int y);
    virtual void keyboardUp(unsigned char key, int x, int y);
    virtual void special(int key, int x, int y);
    virtual void specialUp(int key, int x, int y);
    virtual void reshape(int width, int height);
    virtual void mouse(int button, int state, int x, int y);
    virtual void motion(int x, int y);
    void someshit();
private:
    static void s_timer(int x);
    void timer();
    static std::set<GameScene*> s_scenes;
    Cube *m_cube;
    const Program *m_program;
    bool m_keyDown[256];
    bool m_specialDown[265];
    float m_mouseX, m_mouseY;
    bool m_mouseRotation;

    glm::vec4 m_viewport;
    glm::mat4 m_projection;
    glm::mat4 m_pickMat;

    bool m_selMode;
    unsigned int m_selBuf[1024];
};

#endif
