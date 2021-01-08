// Scene.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "Scene.h"
#include "afxdialogex.h"
#include "DlgMove.h"
#include "HSTPN_SYSDoc.h"
#include "Init.h"
// CScene �Ի���

IMPLEMENT_DYNAMIC(CScene, CDialogEx)

CScene::CScene(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SCENE, pParent)
{

}

CScene::~CScene()
{
}

void CScene::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScene, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ONE, &CScene::OnBnClickedButtonOne)
	ON_BN_CLICKED(IDC_BUTTON_TWO, &CScene::OnBnClickedButtonTwo)
	ON_BN_CLICKED(IDC_BUTTON_THREE, &CScene::OnBnClickedButtonThree)
	ON_BN_CLICKED(IDC_BUTTON_FOUR, &CScene::OnBnClickedButtonFour)

END_MESSAGE_MAP()


// CScene ��Ϣ�������


//�ؿչ����Կ���ս
void CScene::OnBnClickedButtonOne()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_COMMAND, WM_MESSAGE_SCENE, NULL);//��HST�ļ�
	CDialogEx::OnOK();
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ
	CInit DlgInit;           // ģ̬����Ի�����CTipDlg��ʵ��   
	nRes = DlgInit.DoModal();
	if (IDCANCEL == nRes)     // �ж϶Ի����˳��󷵻�ֵ�Ƿ�ΪIDCANCEL���������return�������������ִ��   
		return;

	CInit* pDlg = new CInit();
	pDlg->Create(IDD_DLG_INIT);

	pDlg->MoveWindow(CRect(100, 10, 1124, 778), TRUE);//���öԻ����С1024*768
	HWND hw = pDlg->GetSafeHwnd();
	pDlg->ShowWindow(SW_SHOWNORMAL);
	
}


void CScene::OnBnClickedButtonTwo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBoxW(L"���˻���ս������");
	
}


void CScene::OnBnClickedButtonThree()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBoxW(L"���������Կ���ս������");

	
}


void CScene::OnBnClickedButtonFour()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBoxW(L"½��������ս������");
}

