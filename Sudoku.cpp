// Sudoku.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Sudoku.h"
#include "SudokuDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSudokuApp

BEGIN_MESSAGE_MAP(CSudokuApp, CWinApp)
	//{{AFX_MSG_MAP(CSudokuApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSudokuApp construction

CSudokuApp::CSudokuApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSudokuApp object

CSudokuApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSudokuApp initialization

BOOL CSudokuApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
//#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSudokuDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();


	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
