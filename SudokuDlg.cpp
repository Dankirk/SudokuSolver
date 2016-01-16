// SudokuDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "Sudoku.h"
#include "SudokuDlg.h"
#include "SaveLoad.h"
#include "Settings.h"
#include "math.h"
#include <fstream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int lastselection = 0;
int iSelection = 0;
bool bOnce = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSudokuDlg dialog

CSudokuDlg::CSudokuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSudokuDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSudokuDlg)
	bPlaced = FALSE;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
	
void CSudokuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSudokuDlg)
	DDX_Control(pDX, IDC_Square9, m_Square[9]);
	DDX_Control(pDX, IDC_Square81, m_Square[81]);
	DDX_Control(pDX, IDC_Square80, m_Square[80]);
	DDX_Control(pDX, IDC_Square8, m_Square[8]);
	DDX_Control(pDX, IDC_Square79, m_Square[79]);
	DDX_Control(pDX, IDC_Square78, m_Square[78]);
	DDX_Control(pDX, IDC_Square75, m_Square[75]);
	DDX_Control(pDX, IDC_Square76, m_Square[76]);
	DDX_Control(pDX, IDC_Square77, m_Square[77]);
	DDX_Control(pDX, IDC_Square74, m_Square[74]);
	DDX_Control(pDX, IDC_Square73, m_Square[73]);
	DDX_Control(pDX, IDC_Square72, m_Square[72]);
	DDX_Control(pDX, IDC_Square71, m_Square[71]);
	DDX_Control(pDX, IDC_Square70, m_Square[70]);
	DDX_Control(pDX, IDC_Square7, m_Square[7]);
	DDX_Control(pDX, IDC_Square69, m_Square[69]);
	DDX_Control(pDX, IDC_Square68, m_Square[68]);
	DDX_Control(pDX, IDC_Square65, m_Square[65]);
	DDX_Control(pDX, IDC_Square66, m_Square[66]);
	DDX_Control(pDX, IDC_Square67, m_Square[67]);
	DDX_Control(pDX, IDC_Square64, m_Square[64]);
	DDX_Control(pDX, IDC_Square63, m_Square[63]);
	DDX_Control(pDX, IDC_Square62, m_Square[62]);
	DDX_Control(pDX, IDC_Square60, m_Square[60]);
	DDX_Control(pDX, IDC_Square61, m_Square[61]);
	DDX_Control(pDX, IDC_Square59, m_Square[59]);
	DDX_Control(pDX, IDC_Square6, m_Square[6]);
	DDX_Control(pDX, IDC_Square58, m_Square[58]);
	DDX_Control(pDX, IDC_Square57, m_Square[57]);
	DDX_Control(pDX, IDC_Square56, m_Square[56]);
	DDX_Control(pDX, IDC_Square55, m_Square[55]);
	DDX_Control(pDX, IDC_Square54, m_Square[54]);
	DDX_Control(pDX, IDC_Square53, m_Square[53]);
	DDX_Control(pDX, IDC_Square52, m_Square[52]);
	DDX_Control(pDX, IDC_Square51, m_Square[51]);
	DDX_Control(pDX, IDC_Square50, m_Square[50]);
	DDX_Control(pDX, IDC_Square5, m_Square[5]);
	DDX_Control(pDX, IDC_Square49, m_Square[49]);
	DDX_Control(pDX, IDC_Square47, m_Square[47]);
	DDX_Control(pDX, IDC_Square48, m_Square[48]);
	DDX_Control(pDX, IDC_Square46, m_Square[46]);
	DDX_Control(pDX, IDC_Square45, m_Square[45]);
	DDX_Control(pDX, IDC_Square44, m_Square[44]);
	DDX_Control(pDX, IDC_Square43, m_Square[43]);
	DDX_Control(pDX, IDC_Square42, m_Square[42]);
	DDX_Control(pDX, IDC_Square41, m_Square[41]);
	DDX_Control(pDX, IDC_Square4, m_Square[4]);
	DDX_Control(pDX, IDC_Square40, m_Square[40]);
	DDX_Control(pDX, IDC_Square39, m_Square[39]);
	DDX_Control(pDX, IDC_Square38, m_Square[38]);
	DDX_Control(pDX, IDC_Square37, m_Square[37]);
	DDX_Control(pDX, IDC_Square36, m_Square[36]);
	DDX_Control(pDX, IDC_Square35, m_Square[35]);
	DDX_Control(pDX, IDC_Square34, m_Square[34]);
	DDX_Control(pDX, IDC_Square33, m_Square[33]);
	DDX_Control(pDX, IDC_Square31, m_Square[31]);
	DDX_Control(pDX, IDC_Square32, m_Square[32]);
	DDX_Control(pDX, IDC_Square30, m_Square[30]);
	DDX_Control(pDX, IDC_Square3, m_Square[3]);
	DDX_Control(pDX, IDC_Square29, m_Square[29]);
	DDX_Control(pDX, IDC_Square28, m_Square[28]);
	DDX_Control(pDX, IDC_Square27, m_Square[27]);
	DDX_Control(pDX, IDC_Square26, m_Square[26]);
	DDX_Control(pDX, IDC_Square25, m_Square[25]);
	DDX_Control(pDX, IDC_Square24, m_Square[24]);
	DDX_Control(pDX, IDC_Square23, m_Square[23]);
	DDX_Control(pDX, IDC_Square22, m_Square[22]);
	DDX_Control(pDX, IDC_Square21, m_Square[21]);
	DDX_Control(pDX, IDC_Square20, m_Square[20]);
	DDX_Control(pDX, IDC_Square2, m_Square[2]);
	DDX_Control(pDX, IDC_Square19, m_Square[19]);
	DDX_Control(pDX, IDC_Square18, m_Square[18]);
	DDX_Control(pDX, IDC_Square17, m_Square[17]);
	DDX_Control(pDX, IDC_Square16, m_Square[16]);
	DDX_Control(pDX, IDC_Square15, m_Square[15]);
	DDX_Control(pDX, IDC_Square14, m_Square[14]);
	DDX_Control(pDX, IDC_Square13, m_Square[13]);
	DDX_Control(pDX, IDC_Square12, m_Square[12]);
	DDX_Control(pDX, IDC_Square11, m_Square[11]);
	DDX_Control(pDX, IDC_Square10, m_Square[10]);
	DDX_Control(pDX, IDC_Square1, m_Square[1]);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSudokuDlg, CDialog)
	//{{AFX_MSG_MAP(CSudokuDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_FILE_HELP, OnFileHelp)
	ON_COMMAND(ID_FILE_CLEAR, OnFileClear)
	ON_COMMAND(ID_FILE_SAVELOAD, OnFileSaveload)
	ON_COMMAND(ID_FILE_EASYMODE, OnFileEasymode)
	ON_COMMAND(ID_FILE_SETTINGS, OnFileSettings)
	ON_BN_CLICKED(IDC_Check, OnCheck)
	ON_BN_CLICKED(IDC_LoadLast, OnLoadLast)
	ON_BN_CLICKED(IDC_Square1, OnSquare1)
	ON_BN_CLICKED(IDC_Square10, OnSquare10)
	ON_BN_CLICKED(IDC_Square11, OnSquare11)
	ON_BN_CLICKED(IDC_Square12, OnSquare12)
	ON_BN_CLICKED(IDC_Square13, OnSquare13)
	ON_BN_CLICKED(IDC_Square14, OnSquare14)
	ON_BN_CLICKED(IDC_Square15, OnSquare15)
	ON_BN_CLICKED(IDC_Square16, OnSquare16)
	ON_BN_CLICKED(IDC_Square17, OnSquare17)
	ON_BN_CLICKED(IDC_Square18, OnSquare18)
	ON_BN_CLICKED(IDC_Square19, OnSquare19)
	ON_BN_CLICKED(IDC_Square2, OnSquare2)
	ON_BN_CLICKED(IDC_Square20, OnSquare20)
	ON_BN_CLICKED(IDC_Square21, OnSquare21)
	ON_BN_CLICKED(IDC_Square22, OnSquare22)
	ON_BN_CLICKED(IDC_Square23, OnSquare23)
	ON_BN_CLICKED(IDC_Square24, OnSquare24)
	ON_BN_CLICKED(IDC_Square25, OnSquare25)
	ON_BN_CLICKED(IDC_Square27, OnSquare27)
	ON_BN_CLICKED(IDC_Square26, OnSquare26)
	ON_BN_CLICKED(IDC_Square28, OnSquare28)
	ON_BN_CLICKED(IDC_Square29, OnSquare29)
	ON_BN_CLICKED(IDC_Square3, OnSquare3)
	ON_BN_CLICKED(IDC_Square30, OnSquare30)
	ON_BN_CLICKED(IDC_Square31, OnSquare31)
	ON_BN_CLICKED(IDC_Square32, OnSquare32)
	ON_BN_CLICKED(IDC_Square33, OnSquare33)
	ON_BN_CLICKED(IDC_Square34, OnSquare34)
	ON_BN_CLICKED(IDC_Square35, OnSquare35)
	ON_BN_CLICKED(IDC_Square36, OnSquare36)
	ON_BN_CLICKED(IDC_Square37, OnSquare37)
	ON_BN_CLICKED(IDC_Square38, OnSquare38)
	ON_BN_CLICKED(IDC_Square39, OnSquare39)
	ON_BN_CLICKED(IDC_Square4, OnSquare4)
	ON_BN_CLICKED(IDC_Square40, OnSquare40)
	ON_BN_CLICKED(IDC_Square41, OnSquare41)
	ON_BN_CLICKED(IDC_Square42, OnSquare42)
	ON_BN_CLICKED(IDC_Square43, OnSquare43)
	ON_BN_CLICKED(IDC_Square44, OnSquare44)
	ON_BN_CLICKED(IDC_Square45, OnSquare45)
	ON_BN_CLICKED(IDC_Square46, OnSquare46)
	ON_BN_CLICKED(IDC_Square47, OnSquare47)
	ON_BN_CLICKED(IDC_Square48, OnSquare48)
	ON_BN_CLICKED(IDC_Square49, OnSquare49)
	ON_BN_CLICKED(IDC_Square5, OnSquare5)
	ON_BN_CLICKED(IDC_Square50, OnSquare50)
	ON_BN_CLICKED(IDC_Square51, OnSquare51)
	ON_BN_CLICKED(IDC_Square52, OnSquare52)
	ON_BN_CLICKED(IDC_Square53, OnSquare53)
	ON_BN_CLICKED(IDC_Square54, OnSquare54)
	ON_BN_CLICKED(IDC_Square55, OnSquare55)
	ON_BN_CLICKED(IDC_Square56, OnSquare56)
	ON_BN_CLICKED(IDC_Square57, OnSquare57)
	ON_BN_CLICKED(IDC_Square58, OnSquare58)
	ON_BN_CLICKED(IDC_Square59, OnSquare59)
	ON_BN_CLICKED(IDC_Square6, OnSquare6)
	ON_BN_CLICKED(IDC_Square60, OnSquare60)
	ON_BN_CLICKED(IDC_Square62, OnSquare62)
	ON_BN_CLICKED(IDC_Square61, OnSquare61)
	ON_BN_CLICKED(IDC_Square63, OnSquare63)
	ON_BN_CLICKED(IDC_Square64, OnSquare64)
	ON_BN_CLICKED(IDC_Square65, OnSquare65)
	ON_BN_CLICKED(IDC_Square66, OnSquare66)
	ON_BN_CLICKED(IDC_Square67, OnSquare67)
	ON_BN_CLICKED(IDC_Square68, OnSquare68)
	ON_BN_CLICKED(IDC_Square69, OnSquare69)
	ON_BN_CLICKED(IDC_Square7, OnSquare7)
	ON_BN_CLICKED(IDC_Square70, OnSquare70)
	ON_BN_CLICKED(IDC_Square71, OnSquare71)
	ON_BN_CLICKED(IDC_Square72, OnSquare72)
	ON_BN_CLICKED(IDC_Square73, OnSquare73)
	ON_BN_CLICKED(IDC_Square75, OnSquare75)
	ON_BN_CLICKED(IDC_Square74, OnSquare74)
	ON_BN_CLICKED(IDC_Square76, OnSquare76)
	ON_BN_CLICKED(IDC_Square77, OnSquare77)
	ON_BN_CLICKED(IDC_Square78, OnSquare78)
	ON_BN_CLICKED(IDC_Square79, OnSquare79)
	ON_BN_CLICKED(IDC_Square8, OnSquare8)
	ON_BN_CLICKED(IDC_Square80, OnSquare80)
	ON_BN_CLICKED(IDC_Square81, OnSquare81)
	ON_BN_CLICKED(IDC_Square9, OnSquare9)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	ON_BN_CLICKED(IDC_RADIO8, OnRadio8)
	ON_BN_CLICKED(IDC_RADIO9, OnRadio9)
	ON_BN_CLICKED(IDC_RADIO0, OnRadio0)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSudokuDlg message handlers

BOOL CSudokuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	INITCOMMONCONTROLSEX InitCtrlEx;
	InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCtrlEx.dwICC  = ICC_STANDARD_CLASSES|ICC_BAR_CLASSES;
	InitCommonControlsEx(&InitCtrlEx);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	easyModeActive = false;
	dlg = this;
	
	solver.SetCallbacks(&DrawNumCallback, &BlockCallback);
	/*
	Solver::drawNumCallback = &DrawNumCallback;
	Square::onBlockCallback = &BlockCallback;
	*/
	UpdateData();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSudokuDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSudokuDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

HCURSOR CSudokuDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//ohjaa napin painallukset OnSquare() funktioon
//ja syötä kyseisen napin numero funktioon
void CSudokuDlg::OnSquare1() { OnSquare(1); }
void CSudokuDlg::OnSquare2() { OnSquare(2); }
void CSudokuDlg::OnSquare3() { OnSquare(3); }
void CSudokuDlg::OnSquare4() { OnSquare(4); }
void CSudokuDlg::OnSquare5() { OnSquare(5); }
void CSudokuDlg::OnSquare6() { OnSquare(6); }
void CSudokuDlg::OnSquare7() { OnSquare(7); }
void CSudokuDlg::OnSquare8() { OnSquare(8); }
void CSudokuDlg::OnSquare9() { OnSquare(9); }
void CSudokuDlg::OnSquare10() { OnSquare(10); }
void CSudokuDlg::OnSquare11() { OnSquare(11); }
void CSudokuDlg::OnSquare12() { OnSquare(12); }
void CSudokuDlg::OnSquare13() { OnSquare(13); }
void CSudokuDlg::OnSquare14() { OnSquare(14); }
void CSudokuDlg::OnSquare15() { OnSquare(15); }
void CSudokuDlg::OnSquare16() { OnSquare(16); }
void CSudokuDlg::OnSquare17() { OnSquare(17); }
void CSudokuDlg::OnSquare18() { OnSquare(18); }
void CSudokuDlg::OnSquare19() { OnSquare(19); }
void CSudokuDlg::OnSquare20() { OnSquare(20); }
void CSudokuDlg::OnSquare21() { OnSquare(21); }
void CSudokuDlg::OnSquare22() { OnSquare(22); }
void CSudokuDlg::OnSquare23() { OnSquare(23); }
void CSudokuDlg::OnSquare24() { OnSquare(24); }
void CSudokuDlg::OnSquare25() { OnSquare(25); }
void CSudokuDlg::OnSquare26() { OnSquare(26); }
void CSudokuDlg::OnSquare27() { OnSquare(27); }
void CSudokuDlg::OnSquare28() { OnSquare(28); }
void CSudokuDlg::OnSquare29() { OnSquare(29); }
void CSudokuDlg::OnSquare30() { OnSquare(30); }
void CSudokuDlg::OnSquare31() { OnSquare(31); }
void CSudokuDlg::OnSquare32() { OnSquare(32); }
void CSudokuDlg::OnSquare33() { OnSquare(33); }
void CSudokuDlg::OnSquare34() { OnSquare(34); }
void CSudokuDlg::OnSquare35() { OnSquare(35); }
void CSudokuDlg::OnSquare36() { OnSquare(36); }
void CSudokuDlg::OnSquare37() { OnSquare(37); }
void CSudokuDlg::OnSquare38() { OnSquare(38); }
void CSudokuDlg::OnSquare39() { OnSquare(39); }
void CSudokuDlg::OnSquare40() { OnSquare(40); }
void CSudokuDlg::OnSquare41() { OnSquare(41); }
void CSudokuDlg::OnSquare42() { OnSquare(42); }
void CSudokuDlg::OnSquare43() { OnSquare(43); }
void CSudokuDlg::OnSquare44() { OnSquare(44); }
void CSudokuDlg::OnSquare45() { OnSquare(45); }
void CSudokuDlg::OnSquare46() { OnSquare(46); }
void CSudokuDlg::OnSquare47() { OnSquare(47); }
void CSudokuDlg::OnSquare48() { OnSquare(48); }
void CSudokuDlg::OnSquare49() { OnSquare(49); }
void CSudokuDlg::OnSquare50() { OnSquare(50); }
void CSudokuDlg::OnSquare51() { OnSquare(51); }
void CSudokuDlg::OnSquare52() { OnSquare(52); }
void CSudokuDlg::OnSquare53() { OnSquare(53); }
void CSudokuDlg::OnSquare54() { OnSquare(54); }
void CSudokuDlg::OnSquare55() { OnSquare(55); }
void CSudokuDlg::OnSquare56() { OnSquare(56); }
void CSudokuDlg::OnSquare57() { OnSquare(57); }
void CSudokuDlg::OnSquare58() { OnSquare(58); }
void CSudokuDlg::OnSquare59() { OnSquare(59); }
void CSudokuDlg::OnSquare60() { OnSquare(60); }
void CSudokuDlg::OnSquare61() { OnSquare(61); }
void CSudokuDlg::OnSquare62() { OnSquare(62); }
void CSudokuDlg::OnSquare63() { OnSquare(63); }
void CSudokuDlg::OnSquare64() { OnSquare(64); }
void CSudokuDlg::OnSquare65() { OnSquare(65); }
void CSudokuDlg::OnSquare66() { OnSquare(66); }
void CSudokuDlg::OnSquare67() { OnSquare(67); }
void CSudokuDlg::OnSquare68() { OnSquare(68); }
void CSudokuDlg::OnSquare69() { OnSquare(69); }
void CSudokuDlg::OnSquare70() { OnSquare(70); }
void CSudokuDlg::OnSquare71() { OnSquare(71); }
void CSudokuDlg::OnSquare72() { OnSquare(72); }
void CSudokuDlg::OnSquare73() { OnSquare(73); }
void CSudokuDlg::OnSquare74() { OnSquare(74); }
void CSudokuDlg::OnSquare75() { OnSquare(75); }
void CSudokuDlg::OnSquare76() { OnSquare(76); }
void CSudokuDlg::OnSquare77() { OnSquare(77); }
void CSudokuDlg::OnSquare78() { OnSquare(78); }
void CSudokuDlg::OnSquare79() { OnSquare(79); }
void CSudokuDlg::OnSquare80() { OnSquare(80); }
void CSudokuDlg::OnSquare81() { OnSquare(81); }


void CSudokuDlg::OnOK() 
{
	solver.UpdateBoard(&GetBoardState());

	if (solver.BoardHasDuplicates()) {
		MessageBox("Board has atleast 1 duplicate value on same row, column or region","Sudoku",MB_OK);
		return;
	}

	// Tallenna taulukko SudokuLast.sav tiedostoon uudelleen tekemistä varten
	if (bOnce == FALSE) {
		ofstream WriteLast("SudokuLast.sav",ios::out|ios::binary);
		if (WriteLast.is_open() == TRUE) {

			bOnce = TRUE;
			CString value;

			for (int iSquare = 1; iSquare <= MAX*MAX; iSquare++) {
				
				m_Square[iSquare].GetWindowText(value);
				WriteLast<<atoi(value)<<endl;
			}
			WriteLast.close();
		}
		else MessageBox("Could not save current board","Sudoku",MB_OK|MB_ICONWARNING);
	}

	solver.Solve(easyModeActive);

}

vector<int> CSudokuDlg::GetBoardState() {

	UpdateData(TRUE);

	vector<int> board;
	CString squareValue;

	for (int i = 0; i < MAX*MAX; i++) {

		m_Square[i+1].GetWindowText(squareValue);
		board.push_back(atoi(squareValue));
	}

	return board;
}

void CSudokuDlg::Solve(bool easyMode) {

	solver.UpdateBoard(&GetBoardState());
	solver.Solve(easyMode);
}

void CSudokuDlg::EasyMode(bool forcereset) {

	// If easy mode is enabled disable some squares you can't place currently selected number
	if (!easyModeActive)
		return;

	if (iSelection != lastselection || forcereset) {
		lastselection = iSelection;
		for (int i = 1; i <= MAX*MAX; i++)
			m_Square[i].EnableWindow(1);
	}

	if (iSelection != 0) {
		solver.UpdateBoard(&GetBoardState());
		solver.Solve(true);
	}
}

void CSudokuDlg::OnSquare(int iSquare) {
	UpdateData(TRUE);

	//alustuksia
	CString cLine;
	cLine.Format("%d",iSelection);
	bPlaced = TRUE;

	if (iSelection == 0) m_Square[iSquare].SetWindowText(" ");	// Tyhjennä ruutu, jos valittuna numerona on 0
	else m_Square[iSquare].SetWindowText(cLine);				// Aseta valittu numero ruutuun

	UpdateData(FALSE);

	EasyMode();
}

// Lataa Sudoku taulukko, joka oli ennen "Solve":n painamista
void CSudokuDlg::OnLoadLast() 
{ 
	char LastLine[100] = "";

	// Varmista käyttäjältä
	if (bPlaced == TRUE) {
		if (MessageBox("You have modified the Sudoku table. Your modifications will be lost if you load last board.\n"
					  "Are you sure you want to load last board?","Sudoku",MB_YESNO|MB_ICONQUESTION) == IDNO) return;
	}

	// Varmista, että tallennus löytyy
	ifstream ifLoadLast("SudokuLast.sav",ios::in|ios::binary);
	if (ifLoadLast.is_open() == FALSE) {
		MessageBox("No last board found","Sudoku",MB_OK);
		return;
	}
	bPlaced = FALSE;

	// Lataa taulukko tiedostosta
	for (int iSquare = 1; iSquare <= MAX*MAX; iSquare++) { ;
		ifLoadLast.getline(LastLine,sizeof(LastLine));
		if (atoi(LastLine) >= 1 && atoi(LastLine) <= MAX) m_Square[iSquare].SetWindowText(LastLine);
		else m_Square[iSquare].SetWindowText(" ");
			
	}

	// Sulje tiedosto
	ifLoadLast.close();
	
	UpdateData(FALSE);

	EasyMode(true);
}

void CSudokuDlg::OnCheck() {

	solver.UpdateBoard(&GetBoardState());

	if (solver.BoardHasDuplicates())
		MessageBox("Board has atleast 1 duplicate value on same row, column or region","Sudoku",MB_OK);
	else
		MessageBox("Board OK","Sudoku",MB_OK);

}


void CSudokuDlg::OnRadio0() { iSelection = 0; EasyMode(); }
void CSudokuDlg::OnRadio1() { iSelection = 1; EasyMode(); }
void CSudokuDlg::OnRadio2() { iSelection = 2; EasyMode(); }
void CSudokuDlg::OnRadio3() { iSelection = 3; EasyMode(); }
void CSudokuDlg::OnRadio4() { iSelection = 4; EasyMode(); }
void CSudokuDlg::OnRadio5() { iSelection = 5; EasyMode(); }
void CSudokuDlg::OnRadio6() { iSelection = 6; EasyMode(); }
void CSudokuDlg::OnRadio7() { iSelection = 7; EasyMode(); }
void CSudokuDlg::OnRadio8() { iSelection = 8; EasyMode(); }
void CSudokuDlg::OnRadio9() { iSelection = 9; EasyMode(); }

// Menun Exit nappi
void CSudokuDlg::OnFileExit() 
{
	CDialog::OnOK();
	
}

// Avaa Sudoku sivu
void CSudokuDlg::OnFileHelp() 
{
	ShellExecute(NULL,"open","iexplore","http://www.sudoku.org.uk",NULL,SW_SHOW);
	
}

// Menun Save / Load nappi
void CSudokuDlg::OnFileSaveload() 
{
	UpdateData(TRUE);

	CString cValue = _T("");

	CSaveAndLoad SaL;
	for(int iTemp = 1; iTemp <= MAX*MAX; iTemp++) { 
		m_Square[iTemp].GetWindowText(cValue); 
		SaL.iSquares[iTemp] = atoi(cValue); 
	}
	SaL.DoModal();

	if(SaL.bLoad == TRUE) {
		bOnce = TRUE;
		for(int iSquare = 1; iSquare <= MAX*MAX; iSquare++) { 
			if (SaL.iSquares[iSquare] == 0) m_Square[iSquare].SetWindowText(" ");
			else { 
				cValue.Format("%d",SaL.iSquares[iSquare]);
				m_Square[iSquare].SetWindowText(cValue);
			}
		}

		UpdateData(FALSE);

		EasyMode(true);
	}

}

void CSudokuDlg::OnFileSettings() 
{
	CSettings SettingsDlg;
	SettingsDlg.DoModal();

	UpdateData(FALSE);
}

void CSudokuDlg::OnFileClear() 
{
	if(MessageBox("Do you wish to clear the table?","Sudoku",MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	for(int iSquare = 1; iSquare <= MAX*MAX; iSquare++) {
		m_Square[iSquare].SetWindowText(" ");
	}

	bPlaced = FALSE;
	bOnce = FALSE;

	UpdateData(FALSE);

	EasyMode(true);

}

void CSudokuDlg::OnFileEasymode() 
{
	UINT newmode;
	UINT lastmode = GetMenu()->GetMenuState(ID_FILE_EASYMODE,MF_BYCOMMAND) == MF_CHECKED;
	if (lastmode) {
		newmode = MF_UNCHECKED;
		for (int i = 1; i <= MAX*MAX; i++)
			m_Square[i].EnableWindow(1);
	}
	else
		newmode = MF_CHECKED;

	GetMenu()->CheckMenuItem(ID_FILE_EASYMODE, newmode );
	easyModeActive = !lastmode;

	if (easyModeActive)
		EasyMode();
	
}

void BlockCallback(int id, int number) {
	if (dlg->easyModeActive && iSelection == number) {
		dlg->m_Square[id+1].EnableWindow(0);
	}
}

void DrawNumCallback(int id, int number) {

	CString squareValue;

	squareValue.Format("%d",number);
	dlg->m_Square[ id + 1 ].SetWindowText(squareValue);	
	dlg->UpdateData(FALSE);
}