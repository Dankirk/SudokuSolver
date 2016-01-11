#if !defined(AFX_SAVELOAD_H__7B3086AA_44FB_479A_97D1_70D05FFFFBC7__INCLUDED_)
#define AFX_SAVELOAD_H__7B3086AA_44FB_479A_97D1_70D05FFFFBC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveLoad.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSaveAndLoad dialog

class CSaveAndLoad : public CDialog
{
// Construction
public:
	CSaveAndLoad(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveAndLoad)
	enum { IDD = IDD_SAVEANDLOAD };
	CComboBox	m_Combo;
	CString	m_Value;
	int iSquares[82];
	bool bLoad;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveAndLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveAndLoad)
	afx_msg void OnLoad();
	afx_msg void OnSave();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVELOAD_H__7B3086AA_44FB_479A_97D1_70D05FFFFBC7__INCLUDED_)
