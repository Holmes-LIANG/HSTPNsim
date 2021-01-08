// DlgResetState.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgResetState.h"
#include "afxdialogex.h"


// CDlgResetState 对话框

IMPLEMENT_DYNAMIC(CDlgResetState, CDialogEx)

CDlgResetState::CDlgResetState(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgResetState::IDD, pParent)
{

}

CDlgResetState::~CDlgResetState()
{
}

void CDlgResetState::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgResetState, CDialogEx)
END_MESSAGE_MAP()


// CDlgResetState 消息处理程序
