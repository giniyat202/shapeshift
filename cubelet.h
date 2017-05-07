#ifndef CUBELET_H
#define CUBELET_H

enum COLORID
{
    COLORID_FRONT = 0,
    COLORID_RIGHT,
    COLORID_BACK,
    COLORID_LEFT,
    COLORID_BOTTOM,
    COLORID_TOP,
    COLORID_BLANK,
    COLORID_MAX,
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
    Cubelet();
    ~Cubelet();
    const float *getColor(int whichColor) const;
    void setColor(int whichColor, const float *newColor);
    void setColor(int whichColor, float r, float g, float b);

    bool getFaceletVisible(int whichFacelet) const;
    void setFaceletVisible(int whichFacelet, bool visible);

    int getFaceletColorID(int whichFacelet) const;
    void setFaceletColorID(int whichFacelet, int colorID);

    void resetRotation();
    void rotate(int whichFacelet, int whichAngle);

    void render() const;
private:
    static void renderFacelet();
    float m_cols[COLORID_MAX][3];
    int m_faceletCols[FACELETID_MAX];
    bool m_faceletVisible[FACELETID_MAX];

    static const float m_defCols[COLORID_MAX][3];
    static const float m_faceletShine;
    static const float m_blankShine;
    static const double m_faceletLength;
    static const double m_faceletThickness;
    static const double m_faceletRadius;
    static const float m_faceletSpecular[3];
    static const float m_blankSpecular[3];
};

#endif
