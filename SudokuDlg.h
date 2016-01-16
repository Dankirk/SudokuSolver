// SudokuDlg.h : header file
//

#if !defined(AFX_SUDOKUDLG_H__EB033AF8_89A0_461A_9B26_951BAC63F640__INCLUDED_)
#define AFX_SUDOKUDLG_H__EB033AF8_89A0_461A_9B26_951BAC63F640__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEFAULT_SIZE 8

class Square;
class Set;

/////////////////////////////////////////////////////////////////////////////
// CSudokuDlg dialog

class CSudokuDlg : public CDialog
{
// Construction
public:
	CSudokuDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSudokuDlg)
	enum { IDD = IDD_SUDOKU_DIALOG };
	CButton m_Square[82];
	bool bPlaced;
	//}}AFX_DATA

	bool easyModeActive;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSudokuDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	Solver solver;

	bool CheckBoard();
	void EasyMode(bool forcereset = false);
	void Solve(bool easyMode = false);
	vector<int> GetBoardState();

	// Generated message map functions
	//{{AFX_MSG(CSudokuDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFileExit();
	afx_msg void OnFileHelp();
	afx_msg void OnFileClear();
	afx_msg void OnFileSaveload();
	afx_msg void OnFileSettings();
	virtual void OnOK();
	afx_msg void OnCheck();
	afx_msg void OnLoadLast();
	afx_msg void OnSquare1();
	afx_msg void OnSquare10();
	afx_msg void OnSquare11();
	afx_msg void OnSquare12();
	afx_msg void OnSquare13();
	afx_msg void OnSquare14();
	afx_msg void OnSquare15();
	afx_msg void OnSquare16();
	afx_msg void OnSquare17();
	afx_msg void OnSquare18();
	afx_msg void OnSquare19();
	afx_msg void OnSquare2();
	afx_msg void OnSquare20();
	afx_msg void OnSquare21();
	afx_msg void OnSquare22();
	afx_msg void OnSquare23();
	afx_msg void OnSquare24();
	afx_msg void OnSquare25();
	afx_msg void OnSquare27();
	afx_msg void OnSquare26();
	afx_msg void OnSquare28();
	afx_msg void OnSquare29();
	afx_msg void OnSquare3();
	afx_msg void OnSquare30();
	afx_msg void OnSquare31();
	afx_msg void OnSquare32();
	afx_msg void OnSquare33();
	afx_msg void OnSquare34();
	afx_msg void OnSquare35();
	afx_msg void OnSquare36();
	afx_msg void OnSquare37();
	afx_msg void OnSquare38();
	afx_msg void OnSquare39();
	afx_msg void OnSquare4();
	afx_msg void OnSquare40();
	afx_msg void OnSquare41();
	afx_msg void OnSquare42();
	afx_msg void OnSquare43();
	afx_msg void OnSquare44();
	afx_msg void OnSquare45();
	afx_msg void OnSquare46();
	afx_msg void OnSquare47();
	afx_msg void OnSquare48();
	afx_msg void OnSquare49();
	afx_msg void OnSquare5();
	afx_msg void OnSquare50();
	afx_msg void OnSquare51();
	afx_msg void OnSquare52();
	afx_msg void OnSquare53();
	afx_msg void OnSquare54();
	afx_msg void OnSquare55();
	afx_msg void OnSquare56();
	afx_msg void OnSquare57();
	afx_msg void OnSquare58();
	afx_msg void OnSquare59();
	afx_msg void OnSquare6();
	afx_msg void OnSquare60();
	afx_msg void OnSquare62();
	afx_msg void OnSquare61();
	afx_msg void OnSquare63();
	afx_msg void OnSquare64();
	afx_msg void OnSquare65();
	afx_msg void OnSquare66();
	afx_msg void OnSquare67();
	afx_msg void OnSquare68();
	afx_msg void OnSquare69();
	afx_msg void OnSquare7();
	afx_msg void OnSquare70();
	afx_msg void OnSquare71();
	afx_msg void OnSquare72();
	afx_msg void OnSquare73();
	afx_msg void OnSquare75();
	afx_msg void OnSquare74();
	afx_msg void OnSquare76();
	afx_msg void OnSquare77();
	afx_msg void OnSquare78();
	afx_msg void OnSquare79();
	afx_msg void OnSquare8();
	afx_msg void OnSquare80();
	afx_msg void OnSquare81();
	afx_msg void OnSquare9();
	afx_msg void OnSquare(int iSquare);
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	afx_msg void OnRadio7();
	afx_msg void OnRadio8();
	afx_msg void OnRadio9();
	afx_msg void OnRadio0();
	afx_msg void OnFileEasymode();
	afx_msg void OnUndo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

static CSudokuDlg* dlg = NULL;
void BlockCallback(int id, int number);
void DrawNumCallback(int id, int number);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUDOKUDLG_H__EB033AF8_89A0_461A_9B26_951BAC63F640__INCLUDED_)
