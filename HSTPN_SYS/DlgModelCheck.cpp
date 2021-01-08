// DlgModelCheck.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgModelCheck.h"
//#include "afxdialogex.h"


// CDlgModelCheck �Ի���

IMPLEMENT_DYNAMIC(CDlgModelCheck, CDialogEx)

CDlgModelCheck::CDlgModelCheck(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgModelCheck::IDD, pParent)
	, m_btnIsLtdNode(FALSE)
	, m_btnConflict(FALSE)
	, m_btnContact(FALSE)
	, m_btnDeadlock(FALSE)
	, m_btnTrap(FALSE)
{
	m_simThread = NULL;
	m_hstpnSim = NULL;
	m_hstpnData = NULL;
}

CDlgModelCheck::~CDlgModelCheck()
{
}

void CDlgModelCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_ISLTDNODE, m_btnIsLtdNode);
	DDX_Check(pDX, IDC_CONFLICT, m_btnConflict);
	DDX_Check(pDX, IDC_CONTACT, m_btnContact);
	DDX_Check(pDX, IDC_DEADLOCK, m_btnDeadlock);
	DDX_Check(pDX, IDC_TRAP, m_btnTrap);
}


BEGIN_MESSAGE_MAP(CDlgModelCheck, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgModelCheck::OnStartModelCheck)
	ON_BN_CLICKED(IDC_ISLTDNODE, &CDlgModelCheck::OnBnClickedIsltdnode)
	ON_BN_CLICKED(IDC_CONFLICT, &CDlgModelCheck::OnBnClickedConflict)
	ON_BN_CLICKED(IDC_CONTACT, &CDlgModelCheck::OnBnClickedContact)
	ON_BN_CLICKED(IDC_DEADLOCK, &CDlgModelCheck::OnBnClickedDeadlock)
	ON_BN_CLICKED(IDC_TRAP, &CDlgModelCheck::OnBnClickedTrap)
END_MESSAGE_MAP()


// CDlgModelCheck ��Ϣ�������


void CDlgModelCheck::OnBnClickedIsltdnode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������IDC_ISLTDNODE, m_btnIsLtdNode
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_ISLTDNODE))->GetCheck())
	{
		m_btnIsLtdNode = true;
	}
	else
	{
		m_btnIsLtdNode = false;
	}
}


void CDlgModelCheck::OnBnClickedConflict()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������IDC_CONFLICT, m_btnConflict
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CONFLICT))->GetCheck())
	{
		m_btnConflict = true;
	}
	else
	{
		m_btnConflict = false;
	}
}


void CDlgModelCheck::OnBnClickedContact()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������IDC_CONTACT, m_btnContact
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_CONTACT))->GetCheck())
	{
		m_btnContact = true;
	}
	else
	{
		m_btnContact = false;
	}
}


void CDlgModelCheck::OnBnClickedDeadlock()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������IDC_DEADLOCK, m_btnDeadlock
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_DEADLOCK))->GetCheck())
	{
		m_btnDeadlock = true;
	}
	else
	{
		m_btnDeadlock = false;
	}
}


void CDlgModelCheck::OnBnClickedTrap()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������IDC_TRAP, m_btnTrap
	if(BST_CHECKED==((CButton*)GetDlgItem(IDC_TRAP))->GetCheck())
	{
		m_btnTrap = true;
	}
	else
	{
		m_btnTrap = false;
	}
}


// ��ʼģ�ͼ��
void CDlgModelCheck::OnStartModelCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
	if(m_simThread==NULL)
	{
		m_modelcheckData.m_hstpnData = this->m_hstpnData;
		m_modelcheckData.m_btnIsLtdNode = this->m_btnIsLtdNode;
		m_modelcheckData.m_btnConflict = this->m_btnConflict;
		m_modelcheckData.m_btnContact = this->m_btnContact;
		m_modelcheckData.m_btnDeadlock = this->m_btnDeadlock;
		m_modelcheckData.m_btnTrap = this->m_btnTrap;
		m_simThread = AfxBeginThread(m_hstpnSim->ModelCheck,(LPVOID)&m_modelcheckData);
	}
}



BOOL CDlgModelCheck::Create(LPCTSTR lpszTemplateName,CHSTPN_SYSSim * m_hstpnSim1, LPVOID m_hstpnData1, CWnd* pParentWnd)
{
	// TODO: �ڴ����ר�ô����/����û���
	this->m_hstpnSim = m_hstpnSim1;
	this->m_hstpnData = m_hstpnData1;
	return CDialogEx::Create(lpszTemplateName, pParentWnd);
}
