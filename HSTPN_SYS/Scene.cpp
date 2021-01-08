// Scene.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "Scene.h"
#include "afxdialogex.h"
#include "DlgMove.h"
#include "HSTPN_SYSDoc.h"
#include "Init.h"
// CScene 对话框

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


// CScene 消息处理程序


//地空攻防对抗作战
void CScene::OnBnClickedButtonOne()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_COMMAND, WM_MESSAGE_SCENE, NULL);//打开HST文件
	CDialogEx::OnOK();
	INT_PTR nRes;             // 用于保存DoModal函数的返回值
	CInit DlgInit;           // 模态构造对话框类CTipDlg的实例   
	nRes = DlgInit.DoModal();
	if (IDCANCEL == nRes)     // 判断对话框退出后返回值是否为IDCANCEL，如果是则return，否则继续向下执行   
		return;

	CInit* pDlg = new CInit();
	pDlg->Create(IDD_DLG_INIT);

	pDlg->MoveWindow(CRect(100, 10, 1124, 778), TRUE);//设置对话框大小1024*768
	HWND hw = pDlg->GetSafeHwnd();
	pDlg->ShowWindow(SW_SHOWNORMAL);
	
}


void CScene::OnBnClickedButtonTwo()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBoxW(L"无人化作战！！！");
	
}


void CScene::OnBnClickedButtonThree()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBoxW(L"导弹攻防对抗作战！！！");

	
}


void CScene::OnBnClickedButtonFour()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBoxW(L"陆军机动作战！！！");
}

