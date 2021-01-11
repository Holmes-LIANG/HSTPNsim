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

	
	CRect mainFramRect, initRect;
	//��ȡ�����ڵľ�������
	(AfxGetApp()->m_pMainWnd)->GetWindowRect(mainFramRect);
	//�����Ի���
	CInit* pDlg = new CInit();
	pSceneDlg->Create(IDD_DLG_INIT);
	//��ȡ����ѡ�񴰿ڵľ�������
	pSceneDlg->GetWindowRect(initRect);
	//���öԻ����λ��
	pSceneDlg->SetWindowPos(&CWnd::wndTop, mainFramRect.left + mainFramRect.Width() / 2 - initRect.Width() / 2, mainFramRect.top + mainFramRect.Height() / 2 - initRect.Height() / 2, 0, 0, SWP_NOSIZE);
	//��ʾ�Ի���
	pSceneDlg->ShowWindow(SW_SHOW);
	//���¶Ի���
	pSceneDlg->UpdateWindow();
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

//��������ѡ��Ի���
void CScene::CreatSceneDlg() {
	//// TODO: Add your control notification handler code here
	CRect mainFramRect,sceneRect;
	//�½��Ի���ʵ��
	if (pSceneDlg == nullptr) {
		pSceneDlg = new CScene();
	}
	//��ȡ�����ڵľ�������
	(AfxGetApp()->m_pMainWnd)->GetWindowRect(mainFramRect);

	//�����Ի���
	pSceneDlg->Create(IDD_DLG_SCENE);
	//��ȡ����ѡ�񴰿ڵľ�������
	pSceneDlg->GetWindowRect(sceneRect);
	//���öԻ����λ��
	pSceneDlg->SetWindowPos(&CWnd::wndTop, mainFramRect.left+ mainFramRect.Width()/2-sceneRect.Width()/2, mainFramRect.top + mainFramRect.Height()/2-sceneRect.Height()/2, 0, 0, SWP_NOSIZE);
	//��ʾ�Ի���
	pSceneDlg->ShowWindow(SW_SHOW);
	//���¶Ի���
	pSceneDlg->UpdateWindow();

}