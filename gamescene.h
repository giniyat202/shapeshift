#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "scene.h"
#include "cube.h"
#include <set>

class GameScene : public Scene
{
public:
    GameScene();
    ~GameScene();
    virtual void render();
    virtual void keyboard(unsigned char key, int x, int y);
    virtual void keyboardUp(unsigned char key, int x, int y);
    virtual void special(int key, int x, int y);
    virtual void specialUp(int key, int x, int y);
    virtual void reshape(int width, int height);
    virtual void mouse(int button, int state, int x, int y);
    virtual void motion(int x, int y);
private:
    static void s_timer(int x);
    void timer();
    static std::set<GameScene*> s_scenes;
    Cube m_cube;
    bool m_keyDown[256];
    bool m_specialDown[265];
    double m_mouseX, m_mouseY;
    bool m_mouseRotation;

    int m_viewport[4];
    double m_projection[16];
};

#endif
