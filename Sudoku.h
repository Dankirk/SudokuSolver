// Sudoku.h : main header file for the SUDOKU application
//

#if !defined(AFX_SUDOKU_H__6BE35623_F73B_4E53_9591_8053668A4DF7__INCLUDED_)
#define AFX_SUDOKU_H__6BE35623_F73B_4E53_9591_8053668A4DF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSudokuApp:
// See Sudoku.cpp for the implementation of this class
//

class CSudokuApp : public CWinApp
{
public:
	CSudokuApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSudokuApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSudokuApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUDOKU_H__6BE35623_F73B_4E53_9591_8053668A4DF7__INCLUDED_)
