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

	
	CRect mainFramRect, initRect;
	//获取主窗口的矩形区域
	(AfxGetApp()->m_pMainWnd)->GetWindowRect(mainFramRect);
	//创建对话框
	CInit* pDlg = new CInit();
	pSceneDlg->Create(IDD_DLG_INIT);
	//获取场景选择窗口的矩形区域
	pSceneDlg->GetWindowRect(initRect);
	//设置对话框的位置
	pSceneDlg->SetWindowPos(&CWnd::wndTop, mainFramRect.left + mainFramRect.Width() / 2 - initRect.Width() / 2, mainFramRect.top + mainFramRect.Height() / 2 - initRect.Height() / 2, 0, 0, SWP_NOSIZE);
	//显示对话框
	pSceneDlg->ShowWindow(SW_SHOW);
	//更新对话框
	pSceneDlg->UpdateWindow();
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

//创建场景选择对话框
void CScene::CreatSceneDlg() {
	//// TODO: Add your control notification handler code here
	CRect mainFramRect,sceneRect;
	//新建对话框实例
	if (pSceneDlg == nullptr) {
		pSceneDlg = new CScene();
	}
	//获取主窗口的矩形区域
	(AfxGetApp()->m_pMainWnd)->GetWindowRect(mainFramRect);

	//创建对话框
	pSceneDlg->Create(IDD_DLG_SCENE);
	//获取场景选择窗口的矩形区域
	pSceneDlg->GetWindowRect(sceneRect);
	//设置对话框的位置
	pSceneDlg->SetWindowPos(&CWnd::wndTop, mainFramRect.left+ mainFramRect.Width()/2-sceneRect.Width()/2, mainFramRect.top + mainFramRect.Height()/2-sceneRect.Height()/2, 0, 0, SWP_NOSIZE);
	//显示对话框
	pSceneDlg->ShowWindow(SW_SHOW);
	//更新对话框
	pSceneDlg->UpdateWindow();

}