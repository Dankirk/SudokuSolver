// SaveLoad.cpp : implementation file
//

#include "stdafx.h"
#include "Sudoku.h"
#include "SaveLoad.h"
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveAndLoad dialog


CSaveAndLoad::CSaveAndLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveAndLoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveAndLoad)
	m_Value = _T("");
	//}}AFX_DATA_INIT
}


void CSaveAndLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveAndLoad)
	DDX_Control(pDX, IDC_COMBO, m_Combo);
	DDX_CBString(pDX, IDC_COMBO, m_Value);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveAndLoad, CDialog)
	//{{AFX_MSG_MAP(CSaveAndLoad)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveAndLoad message handlers

void CSaveAndLoad::OnLoad() 
{

	UpdateData(TRUE);
	if(m_Value == "") return;

	char sPath[512];
	strcpy_s(sPath,512,m_Value);
	int iLen = strlen(sPath);
	if(	sPath[iLen-4] != '.' ||
		sPath[iLen-3] != 's' ||
		sPath[iLen-2] != 'a' ||
		sPath[iLen-1] != 'v')
	m_Value += ".sav";
	UpdateData(FALSE);

	ifstream iRead(m_Value,ios::in |ios::binary);
	if(iRead.is_open() == FALSE) { 
		MessageBox("File couldn't be opened","Sudoku",MB_OK|MB_ICONWARNING);
		return;
	}

	char LastLine[512];

	for(int iLines = 1;iLines <= MAX*MAX;iLines++) { ;
		iRead.getline(LastLine,sizeof(LastLine));
		if(atoi(LastLine) >= 1 && atoi(LastLine) <= 9) iSquares[iLines] = atoi(LastLine);
		else iSquares[iLines] = 0;
			
	}

	iRead.close();
	bLoad = TRUE;
	CDialog::OnOK();
	
}

void CSaveAndLoad::OnSave() 
{

	UpdateData(TRUE);

	if(m_Value == "") return;

	char sPath[512];
	strcpy_s(sPath,512,m_Value);
	int iLen = strlen(sPath);
	if(	sPath[iLen-4] != '.' ||
		sPath[iLen-3] != 's' ||
		sPath[iLen-2] != 'a' ||
		sPath[iLen-1] != 'v')
	m_Value += ".sav";
	UpdateData(FALSE);

	ifstream iRead(m_Value,ios::in|ios::binary);
	if(iRead.is_open() == TRUE) {
		if(MessageBox("File already exists. Do you want to overwrite it?","Sudoku",MB_YESNO|MB_ICONQUESTION) == IDNO) return;
	}
	iRead.close();

	LPTSTR cValue = "";

	ofstream oWrite(m_Value,ios::binary);
	if(oWrite.is_open() == TRUE) {
		for(int iTemp = 1;iTemp <= MAX*MAX;iTemp++) {
			if(iSquares[iTemp] < 0 || iSquares[iTemp]> MAX) iSquares[iTemp] = 0;
			oWrite<<iSquares[iTemp]<<endl;
		}
		oWrite.close();
		bLoad = FALSE;
		CDialog::OnOK();
	}
	else MessageBox("Could not save current board","Sudoku",MB_OK|MB_ICONWARNING);
	
}

BOOL CSaveAndLoad::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Combo.ResetContent( );
	char sPath[512] = "";
	GetCurrentDirectory(512,sPath);
	strcat_s(sPath,512,"\\*.sav");

	m_Combo.Dir(DDL_READWRITE, sPath);

	UpdateData(FALSE);
	if(m_Combo.GetCount() > 0) m_Combo.SetCurSel(0);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
