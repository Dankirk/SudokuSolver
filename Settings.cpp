// Settings.cpp : implementation file
//

#include "stdafx.h"
#include "Sudoku.h"
#include "Settings.h"


// CSettings dialog

IMPLEMENT_DYNAMIC(CSettings, CDialog)

CSettings::CSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CSettings::IDD, pParent)
{

}

CSettings::~CSettings()
{
}

void CSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettings, CDialog)
END_MESSAGE_MAP()


// CSettings message handlers
