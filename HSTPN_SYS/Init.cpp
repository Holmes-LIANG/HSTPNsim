// Init.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "Init.h"
#include "afxdialogex.h"
#include "DlgMove.h"

// CInit �Ի���

IMPLEMENT_DYNAMIC(CInit, CDialogEx)

CInit::CInit(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_INIT, pParent)
	, m_editDZZ(0)
	, m_editGJJ(0)
	, m_editHZJ(0)
	, m_editBJXJD(0)
	, m_editDLBLJD(0)
	, m_editBJXDD(0)
	, m_editDLBLDD(0)
	, m_editYCDD(0)
	, m_editBJXLD(0)
	, m_editDLBLLD(0)
	, m_editYCLD(0)
{

}

CInit::~CInit()
{
}

void CInit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DZZ, m_editDZZ);
	DDX_Text(pDX, IDC_EDIT_GJJ, m_editGJJ);
	DDX_Text(pDX, IDC_EDIT_HZJ, m_editHZJ);
	DDX_Text(pDX, IDC_EDIT_BJXJD, m_editBJXJD);
	DDX_Text(pDX, IDC_EDIT_DLBLJD, m_editDLBLJD);
	DDX_Text(pDX, IDC_EDIT_BJXDD, m_editBJXDD);
	DDX_Text(pDX, IDC_EDIT_DLBLDD, m_editDLBLDD);
	DDX_Text(pDX, IDC_EDIT_YCDD, m_editYCDD);
	DDX_Text(pDX, IDC_EDIT_BJXLD, m_editBJXLD);
	DDX_Text(pDX, IDC_EDIT_DLBLLD, m_editDLBLLD);
	DDX_Text(pDX, IDC_EDIT1_YCLD, m_editYCLD);
}


BEGIN_MESSAGE_MAP(CInit, CDialogEx)
	ON_BN_CLICKED(IDC_MOVE_START, &CInit::OnBnClickedMoveButton)
END_MESSAGE_MAP()


// CInit ��Ϣ�������





void CInit::OnBnClickedMoveButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel(); 

	//CDlgMove* pDlg = &(CDlgMove::getInstance());
	//pDlg->Create(IDD_DLG_MOVE);

	//pDlg->MoveWindow(CRect(100, 10, 1124, 778), TRUE);//���öԻ����С1024*768
	//HWND hw = pDlg->GetSafeHwnd();
	//pDlg->ShowWindow(SW_SHOWNORMAL);

	CRect mainFramRect;
	//��ȡ�����ڵľ�������
	(AfxGetApp()->m_pMainWnd)->GetWindowRect(mainFramRect);
	//�����Ի���
	CDlgMove* pDlg = &(CDlgMove::getInstance());
	pDlg->Create(IDD_DLG_MOVE);
	//���öԻ����λ��
	pDlg->SetWindowPos(&CWnd::wndTop, mainFramRect.left + mainFramRect.Width() / 2 - 1024 / 2, mainFramRect.top + mainFramRect.Height() / 2 - 768 / 2, 1024, 768, NULL);
	//��ʾ�Ի���
	pDlg->ShowWindow(SW_SHOW);
	//���¶Ի���
	pDlg->UpdateWindow();

	//��ȡ����ĳ�ʼֵ
	UpdateData(TRUE);
	unordered_map<string, int> mapMoveDataTmp;
	IOValue tmp;
	mapMoveDataTmp["plane_hzj_bjx"] = 12;
	mapMoveDataTmp["plane_hzj_dlbl"] = 12;
	mapMoveDataTmp["plane_hzj_yc"] = 0;
	mapMoveDataTmp["plane_dzj_bjx"] = 6;
	mapMoveDataTmp["plane_dzj_dlbl"] = 6;
	mapMoveDataTmp["plane_dzj_yc"] = 6;
	mapMoveDataTmp["plane_gjj_bjx"] = 8;
	mapMoveDataTmp["plane_gjj_dlbl"] = 8;
	mapMoveDataTmp["plane_gjj_yc"] = 0;
	//�״�����ֵ
	mapMoveDataTmp["health_radar_bjx"] = 100;
	mapMoveDataTmp["health_radar_dlbl"] = 100;
	mapMoveDataTmp["health_radar_yc"] = 100;
	//ս������
	mapMoveDataTmp["plane_hzj"] = m_editDZZ;
	mapMoveDataTmp["plane_gjj"] = m_editGJJ;
	mapMoveDataTmp["plane_dzj"] = m_editHZJ;
	//��������
	mapMoveDataTmp["missile_bjx"] = 100;
	mapMoveDataTmp["missile_dlbl"] = 100;
	mapMoveDataTmp["missile_yc"] = 100;
	//��������ֵ
	mapMoveDataTmp["health_bjx"] = 100;
	mapMoveDataTmp["health_dlbl"] = 100;
	CHSTPN_SYSSim::mapMoveData.push_back(mapMoveDataTmp);


	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_COMMAND, ID_DEBUG_START, NULL);

	m_moveThread = AfxBeginThread(pDlg->CreatMove, (LPVOID)pDlg);// �������̣߳����г�����档����1 �̵߳���ں���,����һ��Ҫ����: UINT MyThreadFunction( LPVOID pParam ); ����2 �������̵߳Ĳ���, ע����������Ϊ:LPVOID, �������ǿ��Դ���һ���ṹ�����̡߳�

}






