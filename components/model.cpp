#include "model.h"

Model::Model()
{
    this->verticies = std::vector<std::array<float, 3>>();
}

void cube(float size, Transform pos)
{
    glBegin(GL_QUADS);
        glColor3f(0.0f,1.0f,0.0f);    // Color Blue
        glVertex3f( 0.25f, 0.25f,-0.25f);    // Top Right Of The Quad (Top)
        glVertex3f(-0.25f, 0.25f,-0.25f);    // Top Left Of The Quad (Top)
        glVertex3f(-0.25f, 0.25f, 0.25f);    // Bottom Left Of The Quad (Top)
        glVertex3f( 0.25f, 0.25f, 0.25f);    // Bottom Right Of The Quad (Top)
        glColor3f(1.0f,0.5f,0.0f);    // Color Orange
        glVertex3f( 0.25f,-0.25f, 0.25f);    // Top Right Of The Quad (Bottom)
        glVertex3f(-0.25f,-0.25f, 0.25f);    // Top Left Of The Quad (Bottom)
        glVertex3f(-0.25f,-0.25f,-0.25f);    // Bottom Left Of The Quad (Bottom)
        glVertex3f( 0.25f,-0.25f,-0.25f);    // Bottom Right Of The Quad (Bottom)
        glColor3f(1.0f,0.0f,0.0f);    // Color Red
        glVertex3f( 0.25f, 0.25f, 0.25f);    // Top Right Of The Quad (Front)
        glVertex3f(-0.25f, 0.25f, 0.25f);    // Top Left Of The Quad (Front)
        glVertex3f(-0.25f,-0.25f, 0.25f);    // Bottom Left Of The Quad (Front)
        glVertex3f( 0.25f,-0.25f, 0.25f);    // Bottom Right Of The Quad (Front)
        glColor3f(1.0f,1.0f,0.0f);    // Color Yellow
        glVertex3f( 0.25f,-0.25f,-0.25f);    // Top Right Of The Quad (Back)
        glVertex3f(-0.25f,-0.25f,-0.25f);    // Top Left Of The Quad (Back)
        glVertex3f(-0.25f, 0.25f,-0.25f);    // Bottom Left Of The Quad (Back)
        glVertex3f( 0.25f, 0.25f,-0.25f);    // Bottom Right Of The Quad (Back)
        glColor3f(0.0f,0.0f,1.0f);    // Color Blue
        glVertex3f(-0.25f, 0.25f, 0.25f);    // Top Right Of The Quad (Left)
        glVertex3f(-0.25f, 0.25f,-0.25f);    // Top Left Of The Quad (Left)
        glVertex3f(-0.25f,-0.25f,-0.25f);    // Bottom Left Of The Quad (Left)
        glVertex3f(-0.25f,-0.25f, 0.25f);    // Bottom Right Of The Quad (Left)
        glColor3f(1.0f,0.0f,1.0f);    // Color Violet
        glVertex3f( 0.25f, 0.25f,-0.25f);    // Top Right Of The Quad (Right)
        glVertex3f( 0.25f, 0.25f, 0.25f);    // Top Left Of The Quad (Right)
        glVertex3f( 0.25f,-0.25f, 0.25f);    // Bottom Left Of The Quad (Right)
        glVertex3f( 0.25f,-0.25f,-0.25f);    // Bottom Right Of The Quad (Right)
    glEnd();
}
