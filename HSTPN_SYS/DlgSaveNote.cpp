// DlgSaveNote.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgSaveNote.h"
//#include "afxdialogex.h"


// CDlgSaveNote 对话框

IMPLEMENT_DYNAMIC(CDlgSaveNote, CDialogEx)

CDlgSaveNote::CDlgSaveNote(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSaveNote::IDD, pParent)
{

}

CDlgSaveNote::~CDlgSaveNote()
{
}

void CDlgSaveNote::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSaveNote, CDialogEx)
	ON_BN_CLICKED(IDCANCEL_2, &CDlgSaveNote::OnBnCancel)
END_MESSAGE_MAP()


// CDlgSaveNote 消息处理程序


void CDlgSaveNote::OnBnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//this->DestroyWindow();
	//CDialogEx::OnCancel();
	this->EndDialog(3);
}
