// OGL2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "OGL2.h"
#include "OGL2Dlg.h"
#include "openGLview.h"
#include "afxdialogex.h"
#include "math.h"
#include <gl\glut.h> // glut.h includes gl.h and glu.h
#include "zpr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


	// CAboutDlg dialog used for App About
	openGLview gl;

	//okreslenie swiatel
	static GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	static GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	//okreslenie powierzchni
	static GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
	static GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	static GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	static GLfloat high_shininess[] = { 100.0 };

void display(void)
{
	gl.display();
}
void resize(int w, int h)
{
	gl.resize(w, h);
}
void animacja(int value)
{
	gl.animacja();
	glutTimerFunc(30,animacja,0); //ustawiamy funkcje timera – po 30ms zostanie wywołana funkcja
}

void keyboard(unsigned char key, int x, int y)
{
	gl.keyboard(key, x, y);
}

void pick(GLint name) // wybrany zostal, ktorys z elementow na scenie
{
	gl.selected_id=name;//przepisujemy do klasy opisujacej scene numer wybranego obiektu
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COGL2Dlg dialog




COGL2Dlg::COGL2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COGL2Dlg::IDD, pParent)
	, m_czolg1_kat(_T("0"))
	, m_czolg2_v0(_T("0"))
	, czolg1(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COGL2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_czolg1_kat);
	DDX_Text(pDX, IDC_EDIT2, m_czolg2_v0);
	DDX_Text(pDX, IDC_EDIT3, czolg1);
}

BEGIN_MESSAGE_MAP(COGL2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &COGL2Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &COGL2Dlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &COGL2Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON2, &COGL2Dlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT2, &COGL2Dlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// COGL2Dlg message handlers

BOOL COGL2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.
	OnBnClickedOk();
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COGL2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COGL2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COGL2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COGL2Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here


	glutInitWindowSize (800, 600);// ustawiamy początkową wielkosc okna
	glutInitWindowPosition (200, 150);// poczatkowa pozycja okna (lewy górny róg)
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);//ustawienie kolorów, precyzji i glebi
	glutCreateWindow("openGL test");//tworzymy nowe okno
	// ustawienie odpowiednich funkcji, które bade wywolywane przy odpowiednich zdarzeniach
	glutDisplayFunc(display);//ustawiamy funkcje display
	glutReshapeFunc(resize);//ustawiamy funkcje resize
	glutTimerFunc(30,animacja,0); //ustawiamy funkcje timera
	glutKeyboardFunc(keyboard);
	//this->ShowWindow(SW_HIDE);// ukrywamy g³ówne okno
	gracz=1;
/*	GLfloat density = 0.8; //gestosc mgly
	GLfloat fogColor[4]={0.5,0.5,0.5,1.0}; //color mgly
     glEnable(GL_FOG); // uruchamienie mgly
	glFogi(GL_FOG_MODE,GL_EXP); //tryb generowania mgly
	glFogfv(GL_FOG_COLOR,fogColor); // ustawienie koloru
	glFogf(GL_FOG_DENSITY,density); // ustawienie gestosci
	glHint(GL_FOG_HINT,GL_NICEST); */
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	zprInit(); // inicjalizacja obslugi myszki
	zprSelectionFunc(display); // wskazujemy funkcje rysujaca obiekty na scenie
	zprPickFunc(pick); // wskazujemy funkcje obslugujaca zdarzenie wybrania obiektu myszka

	char* filename = "stol2.bmp"; // nazwa pliku z tekstura
	glGenTextures(1,&gl.textures[0]); //nazywamy teksture
	glBindTexture(GL_TEXTURE_2D, gl.textures[0]);// aktywujemy teksture 1
	loadTextureFromFile( filename ); // odczytujemy dane z pliku

	glutMainLoop(); //uruchamiamy petle opengl
}

void COGL2Dlg::loadTextureFromFile(char *filename)
{    
	RgbImage theTexMap( filename );
// Pixel alignment: each row is word aligned (aligned to a 4 byte boundary)
//    Therefore, no need to call glPixelStore( GL_UNPACK_ALIGNMENT, ... );
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
gluBuild2DMipmaps(GL_TEXTURE_2D, 3,theTexMap.GetNumCols(), theTexMap.GetNumRows(),
 GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData() );
}
void COGL2Dlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if(gracz==1)
	{
		UpdateData(true);//konieczne przed odczytywaniem z editow
	
	}

}

void COGL2Dlg::OnBnClickedButton2()
{
	if(gracz==2)
	{
		gracz=1;
	}
}

void COGL2Dlg::OnEnChangeEdit1()
{

	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
//	wcstod(m_czolg1_kat, 
}

void COGL2Dlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}