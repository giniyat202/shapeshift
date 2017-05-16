#ifndef SCENE_H
#define SCENE_H

class Scene
{
protected:
    Scene();
    Scene(const Scene &other);
    Scene &operator =(const Scene &other);
public:
    virtual void render();
    virtual void reshape(int width, int height);
    virtual void keyboard(unsigned char key, int x, int y);
    virtual void keyboardUp(unsigned char key, int x, int y);
    virtual void special(int key, int x, int y);
    virtual void specialUp(int key, int x, int y);
    virtual void mouse(int button, int state, int x, int y);
    virtual void motion(int x, int y);
    virtual void passiveMotion(int x, int y);
};

#endif
