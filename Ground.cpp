#include "StdAfx.h"
#include "Ground.h"
CGround::CGround(void)
{}
CGround::~CGround(void)
{}
// metoda rysuj¹ca podloze
void CGround::DrawGrid(void)
{
glPushMatrix();
float Line = -10;
int Grid;
glBegin(GL_LINES);
for(Grid = 0; Grid <= 20; Grid += 1)
{
glColor3f(0.0f,1.0f,0.0f);
glVertex3f(Line + Grid, 0, -10);
glVertex3f(Line + Grid, 0, 10);
glVertex3f(-10, 0, Line + Grid);
glVertex3f(10, 0, Line + Grid);
}
glEnd();
glPopMatrix();
}