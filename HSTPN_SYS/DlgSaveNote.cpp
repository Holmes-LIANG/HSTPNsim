// DlgSaveNote.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgSaveNote.h"
//#include "afxdialogex.h"


// CDlgSaveNote �Ի���

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


// CDlgSaveNote ��Ϣ�������


void CDlgSaveNote::OnBnCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//this->DestroyWindow();
	//CDialogEx::OnCancel();
	this->EndDialog(3);
}
