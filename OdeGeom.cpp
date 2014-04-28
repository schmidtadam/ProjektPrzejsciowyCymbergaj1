#include "StdAfx.h"
#include "OdeGeom.h"

COdeGeom::COdeGeom(void)
{}

COdeGeom::~COdeGeom(void)
{
}

void COdeGeom::loadTextureFromFile(char *filename)
{
	RgbImage theTexMap( filename );
	// Pixel alignment: each row is word aligned (aligned to a 4 byte boundary)
	// Therefore, no need to call glPixelStore( GL_UNPACK_ALIGNMENT, ... );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,theTexMap.GetNumCols(), theTexMap.GetNumRows(),
	GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData() );
}

//rysuje szescian
void COdeGeom::DrawBox(dReal * sides, const float * pos, const float * R, float red, float green, float blue)
{
	double side[3];
	side[0] = sides[0] / 2*10;
	side[1] = sides[1] / 2*10;
	side[2] = sides[2] / 2*10;
	float pos1[3];
	for (int i=0;i<3;i++) 
	{
		pos1[i]=pos[i]*10;
	}
	float mat_ambient[] = { 0.0, 0.0, 1.0, 1.0 };
	float mat_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
	GeomMatrix.ODEtoOGL(pos1, R);
	glMultMatrixf(GeomMatrix.Element);
	//float colors_rgb[12][3] =
	//{
	//	{0.5f,0.1f,0.1f }, {1.0f,0.1f,0.1f }, // Red
	//	{0.5f,0.5f,0.1f }, {1.0f,1.0f,0.1f }, // Yellow
	//	{0.1f,0.5f,0.1f }, {0.1f,1.0f,0.1f }, // Green
	//	{0.1f,0.5f,0.5f }, {0.1f,1.0f,1.0f }, // Cyan
	//	{0.1f,0.1f,0.5f }, {0.1f,0.1f,1.0f }, // Blue
	//	{0.5f,0.1f,0.5f }, {1.0f,0.1f,1.0f } // Magenta
	//	};
	float colors_rgb[12][3];
	for(int i=0; i<12; i++)
	{
		colors_rgb[i][0]=red;
		colors_rgb[i][1]=green;
		colors_rgb[i][2]=blue;
	}
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0.0, 0.0, 1.0);
	glColor3fv( colors_rgb[1] );
	glVertex3f(-side[0], -side[1], side[2]);
	glColor3fv( colors_rgb[2] );
	glVertex3f(side[0], -side[1], side[2]);
	glColor3fv( colors_rgb[3] );
	glVertex3f(side[0], side[1], side[2]);
	glColor3fv( colors_rgb[4] );
	glVertex3f(-side[0], side[1], side[2]);
	// Back Face
	glNormal3f(0.0, 0.0, -1.0);
	glColor3fv( colors_rgb[1] );
	glVertex3f(side[0], -side[1], -side[2]);
	glColor3fv( colors_rgb[2] );
	glVertex3f(-side[0], -side[1], -side[2]);
	glColor3fv( colors_rgb[3] );
	glVertex3f(-side[0], side[1], -side[2]);
	glColor3fv( colors_rgb[4] );
	glVertex3f(side[0], side[1], -side[2]);
	// Top Face
	//filename = "stol2.bmp"; // nazwa pliku z tekstura
	//glGenTextures(1,&textures[0]); //nazywamy teksture
	//glBindTexture(GL_TEXTURE_2D, textures[0]);
	//loadTextureFromFile( filename ); // odczytujemy dane z pliku

	//glEnable(GL_TEXTURE_2D);

	glNormal3f(0.0, 1.0, 0.0);
	glColor3fv( colors_rgb[1] );
	//glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-side[0], side[1], side[2]);
	glColor3fv( colors_rgb[2] );
	//glTexCoord2f(1.0f, 0.0f);
	glVertex3f(side[0], side[1], side[2]);
	glColor3fv( colors_rgb[3] );
	//glTexCoord2f(1.0f, 1.0f);
	glVertex3f(side[0], side[1], -side[2]);
	glColor3fv( colors_rgb[4] );
	//glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-side[0], side[1], -side[2]);
	//glDisable(GL_TEXTURE_2D);

	glNormal3f(0.0, -1.0, 0.0);
	glColor3fv( colors_rgb[1] );
	glVertex3f(-side[0], -side[1], side[2]);
	glColor3fv( colors_rgb[2] );
	glVertex3f(-side[0], -side[1], -side[2]);
	glColor3fv( colors_rgb[3] );
	glVertex3f(side[0], -side[1], -side[2]);
	glColor3fv( colors_rgb[4] );
	glVertex3f(side[0], -side[1], side[2]);
	// Right face
	glNormal3f(1.0, 0.0, 0.0);
	glColor3fv( colors_rgb[1] );
	glVertex3f(side[0], -side[1], side[2]);
	glColor3fv( colors_rgb[2] );
	glVertex3f(side[0], -side[1], -side[2]);
	glColor3fv( colors_rgb[3] );
	glVertex3f(side[0], side[1], -side[2]);
	glColor3fv( colors_rgb[4] );
	glVertex3f(side[0], side[1], side[2]);

	glNormal3f(-1.0, 0.0, 0.0);
	glColor3fv( colors_rgb[1] );
	glVertex3f(-side[0], -side[1], -side[2]);
	glColor3fv( colors_rgb[2] );
	glVertex3f(-side[0], -side[1], side[2]);
	glColor3fv( colors_rgb[3] );
	glVertex3f(-side[0], side[1], side[2]);
	glColor3fv( colors_rgb[4] );
	glVertex3f(-side[0], side[1], -side[2]);
	glEnd();
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}
//rysuje sfere
void COdeGeom::DrawSphere(float radius, const float *pos, const float *R)
{
	float pos1[3];
	for (int i=0;i<3;i++) 
	{
		pos1[i]=pos[i]*10;
	}
	GLUquadricObj *sphere;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere,GLU_LINE);
	float mat_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
	float mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glPushMatrix();
	GeomMatrix.ODEtoOGL(pos1, R);
	glMultMatrixf(GeomMatrix.Element);
	gluSphere(sphere, radius*10, 32, 32);
	glPopMatrix();
}
//rysuje pigule
void COdeGeom::DrawCappedCylinder(const float * pos, const float *R, float radius, float length)
{
	GLUquadricObj *sphere;
	float pos1[3];
	for (int i=0;i<3;i++) 
	{
		pos1[i]=pos[i]*10;
	}
	sphere = gluNewQuadric();
	gluQuadricTexture(sphere, GL_TRUE);
	gluQuadricDrawStyle(sphere,GLU_LINE);
	float mat_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
	float mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
	GeomMatrix.ODEtoOGL(pos1, R);
	glMultMatrixf(GeomMatrix.Element);
	gluCylinder( sphere, radius*10, radius*10, length*10/2, 32, 32);
	glTranslatef(0,0,length*10/2);
	gluSphere(sphere, radius*10, 32, 32);
	glTranslatef(0,0,-length*10);
	gluCylinder( sphere, radius*10, radius*10, length*10/2, 32, 32);
	gluSphere(sphere, radius*10, 32, 32);
	glPopMatrix();
}

void COdeGeom::DrawCylinder(const float * pos, const float *R, float radius, float length, float red, float green, float blue)
{
	GLUquadricObj *walec;
	radius=radius*10;
	length=length*10;
	float pos1[3];
	for (int i=0;i<3;i++)
	{
		pos1[i]=pos[i]*10;
	}
	float colors[3]={red,green,blue};
	// ---- walec -----
	walec = gluNewQuadric();
	gluQuadricTexture(walec, GL_FALSE); // GL_FALSE - bez tekstury
	gluQuadricDrawStyle(walec, GLU_FILL); // GLU_FILL - ciagla powloka walca, bylo GLU_LINE
	float mat_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
	float mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);


	glPushMatrix();
	GeomMatrix.ODEtoOGL(pos1, R);
	glMultMatrixf(GeomMatrix.Element);
	glTranslatef(0,0,-length*0.5); // dodeane, poniważ inaczej grafika nie pokrywa się z obiektem fizycznym
	glColor3fv(colors);
	gluCylinder(walec, radius, radius, length, 32, 32);
	
	//------ pierwsza podstawa ------
	glColor3fv(colors);
	double angle, vectorX, vectorY, vectorX1, vectorY1;
	double promien;
	promien=radius;
	glBegin(GL_TRIANGLES);
		vectorX1=0;
		vectorY1=0;
		for(int i=0;i<=360;i++)//rysujemy kolo skladajace sie z 360 trojkatow
		{
			angle=i/57.29577957795135;
			vectorX=0+(promien*sin(angle));
			vectorY=0+(promien*cos(angle));
			glVertex3d(0,0,0);
			glVertex3d(vectorX1,vectorY1,0);
			glVertex3d(vectorX,vectorY,0);
			vectorY1=vectorY;
			vectorX1=vectorX;
		}
		glEnd();

		//---- druga podstawa ----
		glTranslatef(0,0,length);
		glColor3fv(colors);
		glBegin(GL_TRIANGLES);
			vectorX1=0;
			vectorY1=0;
			for(int i=0;i<=360;i++)//rysujemy ko³o skladajace sie z 360 trojkatow
			{
				angle=i/57.29577957795135;
				vectorX=0+(promien*sin(angle));
				vectorY=0+(promien*cos(angle));
				glVertex3d(0,0,0);
				glVertex3d(vectorX1,vectorY1,0);
				glVertex3d(vectorX,vectorY,0);
				vectorY1=vectorY;
				vectorX1=vectorX;
			}
		glEnd();
	glPopMatrix();
}
