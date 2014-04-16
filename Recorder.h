#include <list>
#include <gl\glut.h>
#pragma once
using namespace std;
struct SPos
{
	float x;
	float y;
	float z;
};

class CRecorder
{
public:
	CRecorder(void);
	~CRecorder(void);
	
	void save(float x, float y, float z);
	list<SPos> first;
	void plot();
	
};

