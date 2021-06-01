// Init.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "Init.h"
#include "afxdialogex.h"
#include "DlgMove.h"

// CInit 对话框

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
	p_Doc = NULL;
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


// CInit 消息处理程序
void CInit::OnBnClickedMoveButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel(); 

	//CRect mainFramRect;
	////获取主窗口的矩形区域
	//(AfxGetApp()->m_pMainWnd)->GetWindowRect(mainFramRect);
	////创建对话框
	////CDlgMove* pDlg = &(CDlgMove::getInstance());
	//CDlgMove* pDlg = new CDlgMove;
	//pDlg->Create(IDD_DLG_MOVE);
	////设置对话框的位置
	//pDlg->SetWindowPos(&CWnd::wndTop, mainFramRect.left + mainFramRect.Width() / 2 - 1024 / 2, mainFramRect.top + mainFramRect.Height() / 2 - 768 / 2, 1024, 768, NULL);
	////显示对话框
	//pDlg->ShowWindow(SW_SHOW);
	////更新对话框
	//pDlg->UpdateWindow();

	//获取输入的初始值
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
	//雷达生命值
	mapMoveDataTmp["health_radar_bjx"] = m_editBJXLD;
	mapMoveDataTmp["health_radar_dlbl"] = m_editDLBLLD;
	mapMoveDataTmp["health_radar_yc"] = m_editYCLD;
	//战机数量
	mapMoveDataTmp["plane_hzj"] = m_editHZJ;
	mapMoveDataTmp["plane_gjj"] = m_editGJJ;
	mapMoveDataTmp["plane_dzj"] = m_editDZZ;
	//导弹数量
	mapMoveDataTmp["missile_bjx"] = m_editBJXDD;
	mapMoveDataTmp["missile_dlbl"] = m_editDLBLDD;
	mapMoveDataTmp["missile_yc"] = m_editYCDD;
	//基地生命值
	mapMoveDataTmp["health_bjx"] = m_editBJXJD;
	mapMoveDataTmp["health_dlbl"] = m_editDLBLJD;
	CHSTPN_SYSSim::mapMoveData.push_back(mapMoveDataTmp);
	InitDataToPDoc();

	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_COMMAND, ID_DEBUG_START, NULL);

	//m_moveThread = AfxBeginThread(pDlg->CreatMove, (LPVOID)pDlg);// 开辟新线程，进行程序仿真。参数1 线程的入口函数,声明一定要如下: UINT MyThreadFunction( LPVOID pParam ); 参数2 传递入线程的参数, 注意它的类型为:LPVOID, 所以我们可以传递一个结构体入线程。

}

//使用对话框中的值对变量进行赋值
void CInit::InitDataToPDoc() {
	if (p_Doc == NULL)
	{
		p_Doc = (CHSTPN_SYSDoc*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	}
	if (p_Doc->arryIOputDataG.size() != 0)
	{
		for (vector<IOValue*>::iterator iterg = p_Doc->arryIOputDataG.begin(); iterg != p_Doc->arryIOputDataG.end(); iterg++)
		{

			if ((*iterg)->Name == "plane_dzj") {
				(*iterg)->Value = m_editDZZ;
			}
			else if ((*iterg)->Name == "plane_gjj") {
				(*iterg)->Value = m_editGJJ;
			}
			else if ((*iterg)->Name == "plane_hzj") {
				(*iterg)->Value = m_editHZJ;
			}
			else if ((*iterg)->Name == "missile_bjx") {
				(*iterg)->Value = m_editBJXDD;
			}
			else if ((*iterg)->Name == "missile_dlbl") {
				(*iterg)->Value = m_editDLBLDD;
			}
			else if ((*iterg)->Name == "missile_yc") {
				(*iterg)->Value = m_editYCDD;
			}
			else if ((*iterg)->Name == "health_bjx") {
				(*iterg)->Value = m_editBJXJD;
			}
			else if ((*iterg)->Name == "health_dlbl") {
				(*iterg)->Value = m_editDLBLJD;
			}
			else if ((*iterg)->Name == "health_radar_bjx") {
				(*iterg)->Value = m_editBJXLD;
			}
			else if ((*iterg)->Name == "health_radar_dlbl") {
				(*iterg)->Value = m_editDLBLLD;
			}
			else if ((*iterg)->Name == "health_radar_yc") {
				(*iterg)->Value = m_editYCLD;
			}
			else if ((*iterg)->Name == "launcher_bjx") {
				(*iterg)->Value = 10;
			}
			else if ((*iterg)->Name == "launcher_dlbl") {
				(*iterg)->Value = 10;
			}
			else if ((*iterg)->Name == "launcher_yc") {
				(*iterg)->Value = 10;
			}
		}
	}

}




