// DlgSaveCurrentState.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgSaveCurrentState.h"
#include "afxdialogex.h"


// CDlgSaveCurrentState 对话框

IMPLEMENT_DYNAMIC(CDlgSaveCurrentState, CDialogEx)

CDlgSaveCurrentState::CDlgSaveCurrentState(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSaveCurrentState::IDD, pParent)
{

}

CDlgSaveCurrentState::~CDlgSaveCurrentState()
{
}

void CDlgSaveCurrentState::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSaveCurrentState, CDialogEx)
END_MESSAGE_MAP()


// CDlgSaveCurrentState 消息处理程序
