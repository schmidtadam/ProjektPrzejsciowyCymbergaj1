#include "StdAfx.h"
#include "Recorder.h"


CRecorder::CRecorder(void)
{
}


CRecorder::~CRecorder(void)
{
}

void CRecorder:: save(float x, float y, float z)
{
	SPos temp;
	temp.x=x;
	temp.y=y;
	temp.z=z;
	first.push_back(temp);
}

void CRecorder:: plot ()
{
	glBegin(GL_LINE_STRIP);
	for (list<SPos>::iterator it = first.begin(); it != first.end(); it++)
		glVertex3f(it->x,it->y,it->z);
	glEnd();
}
// glBegin(GL_LINE_STRIP)    glVertex(x,y,z)    glEnd()
