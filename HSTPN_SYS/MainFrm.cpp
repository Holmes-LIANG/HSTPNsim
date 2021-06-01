
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "HSTPN_SYS.h"

#include "MainFrm.h"
#include "MySQL.h"
#include "Scene.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	//------------------------------------------------
	ON_MESSAGE(WM_MESSAGE_CREATEMODEL1,OnCreateModel1)
	ON_MESSAGE(WM_MESSAGE_MOVEMODEL1,OnMoveModel1)
	ON_MESSAGE(WM_MESSAGE_DRAWLINEEND1,OnDrawLineend1)

//	ON_COMMAND(IDR_EDITTIMESTEP, &CMainFrame::OnClickEditTimeStep)// ������Ӹ���Ϣ��Ӧ���ؼ�Ϊ������״̬
	ON_EN_CHANGE(IDR_EDITTIMESTEP, &CMainFrame::OnGetTimeStep1)
//  ON_COMMAND(IDR_EDITSIMTIME, &CMainFrame::OnClickEditSimTime)
	ON_EN_CHANGE(IDR_EDITSIMTIME, &CMainFrame::OnGetSimTime1)
//	ON_COMMAND(IDR_EDITINTERVALTIME, &CMainFrame::OnClickEditIntervalTime)
	ON_EN_CHANGE(IDR_EDITINTERVALTIME, &CMainFrame::OnGetIntervalTime1)

	ON_MESSAGE(WM_MESSAGE_MDDELETE1,OnMDdelete1)
	ON_MESSAGE(WM_MESSAGE_UPDATETREE1,OnUpdateHSTPNTree1)

	ON_MESSAGE(WM_MESSAGE_CHANGEPROGRESS,OnUpdateProgess)
//	ON_WM_PAINT()
	ON_COMMAND(ID_SHOWPRAMLIST, &CMainFrame::OnShowpramlist)
	ON_COMMAND(ID_SIM_MOVE, &CMainFrame::OnSimMove)
	ON_COMMAND(ID_COMBAT, &CMainFrame::OnCombat)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	IDS_PROGRESS,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
	
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_WIN_2000);//ID_VIEW_APPLOOK_WIN_XP);
	m_pParmListDlg = NULL;
	m_pParmListDlg = new CDlgModelParmList();
	m_pParmListDlg->Create(IDD_DLG_DATALIST,NULL);

}

CMainFrame::~CMainFrame()
{
	if(m_Edit_TimeStep!=NULL)
		delete m_Edit_TimeStep;
	if(m_Text_TimeStep!=NULL)
		delete m_Text_TimeStep;

	if(m_Edit_SimTime!=NULL)
		delete m_Edit_SimTime;
	if(m_Text_SimTime!=NULL)
		delete m_Text_SimTime;
	if(m_Text_IntervalTime!=NULL)
		delete m_Text_IntervalTime;
	if(m_Edit_IntervalTime!=NULL)
		delete m_Edit_IntervalTime;

	if(m_pParmListDlg != NULL)
		delete m_pParmListDlg;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("δ�ܴ����˵���\n");
		return -1;      // δ�ܴ���
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// ��ֹ�˵����ڼ���ʱ��ý���
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}
//====================����SimToolBarͣ��������===========Begin1=====
	if (!m_simToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | 
        CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_SIMTOOLBAR) ||
        !m_simToolBar.LoadToolBar(IDR_SIMTOOLBAR))
    {
        TRACE0("δ�ܴ���������\n");
        return -1;      // δ�ܴ���
    }

	m_simToolBar.InsertSeparator();//����ӷָ���
	
	m_simToolBar.InsertButton(CMFCToolBarEditBoxButton(IDR_TEXTTIMESTEP,0,128UL,35),7);
	m_Text_TimeStep = new CMFCToolBarEditBoxButton();
	m_Text_TimeStep->CanBeStretched();
	m_Text_TimeStep->HaveHotBorder();
	m_Text_TimeStep->SetContents(_T(""));
	m_Text_TimeStep->SetContentsAll(IDR_TEXTTIMESTEP,L"����");
	m_Text_TimeStep->SetFlatMode(true);
	m_Text_TimeStep->SetStyle(TBBS_PRESSED);

	//---------------------
	m_simToolBar.InsertButton(CMFCToolBarEditBoxButton(IDR_EDITTIMESTEP,0,128UL,80),8);
	m_Edit_TimeStep = new CMFCToolBarEditBoxButton();
	m_Edit_TimeStep->CanBeStretched();
	m_Edit_TimeStep->HaveHotBorder();
	m_Edit_TimeStep->SetContents(_T("edit box button"));
	m_Edit_TimeStep->SetContentsAll(IDR_EDITTIMESTEP,L"0.1");
	m_Edit_TimeStep->SetFlatMode(true);
	m_Edit_TimeStep->SetStyle(TBBS_PRESSED);
	
	//---------------------
	m_simToolBar.InsertButton(CMFCToolBarEditBoxButton(IDR_TEXTSIMTIME,0,128UL,80),9);
	m_Text_SimTime = new CMFCToolBarEditBoxButton();
	m_Text_SimTime->CanBeStretched();
	m_Text_SimTime->HaveHotBorder();
	m_Text_SimTime->SetContents(_T(""));
	m_Text_SimTime->SetContentsAll(IDR_TEXTSIMTIME,L"����ʱ��");
	m_Text_SimTime->SetFlatMode(true);
	m_Text_SimTime->SetStyle(TBBS_PRESSED);

	//---------------------
	m_simToolBar.InsertButton(CMFCToolBarEditBoxButton(IDR_EDITSIMTIME,0,128UL,80),10);
	m_Edit_SimTime = new CMFCToolBarEditBoxButton();
	m_Edit_SimTime->CanBeStretched();
	m_Edit_SimTime->HaveHotBorder();
	m_Edit_SimTime->SetContents(_T("edit box button"));
	m_Edit_SimTime->SetContentsAll(IDR_EDITSIMTIME,L"INF");
	m_Edit_SimTime->SetFlatMode(true);
	m_Edit_SimTime->SetStyle(TBBS_PRESSED);

	//---------------------
	m_simToolBar.InsertButton(CMFCToolBarEditBoxButton(IDR_TEXTINTERVALTIME,0,128UL,80),11);
	m_Text_IntervalTime = new CMFCToolBarEditBoxButton();
	m_Text_IntervalTime->CanBeStretched();
	m_Text_IntervalTime->HaveHotBorder();
	m_Text_IntervalTime->SetContents(_T(""));
	m_Text_IntervalTime->SetContentsAll(IDR_TEXTINTERVALTIME,L"���ʱ��");
	m_Text_IntervalTime->SetFlatMode(true);
	m_Text_IntervalTime->SetStyle(TBBS_PRESSED);

	//---------------------
	m_simToolBar.InsertButton(CMFCToolBarEditBoxButton(IDR_EDITINTERVALTIME,0,128UL,80),12);
	m_Edit_IntervalTime = new CMFCToolBarEditBoxButton();
	m_Edit_IntervalTime->CanBeStretched();
	m_Edit_IntervalTime->HaveHotBorder();
	m_Edit_IntervalTime->SetContents(_T("edit box button"));
	m_Edit_IntervalTime->SetContentsAll(IDR_EDITINTERVALTIME,L"200");
	m_Edit_IntervalTime->SetFlatMode(true);
	m_Edit_IntervalTime->SetStyle(TBBS_PRESSED);

//====================����SimToolBarͣ��������===========End1======
	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// �����û�����Ĺ���������:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	m_wndStatusBar.SetPaneInfo(1,IDS_PROGRESS,SBPS_NORMAL,180);
	m_wndStatusBar.SetTipText(1,_T("�������"));
	//m_wndStatusBar.EnablePaneProgressBar(1,180,true);
	//m_wndStatusBar.SetPaneProgress(1,90,true);
	// TODO: �������ϣ���������Ͳ˵�����ͣ������ɾ��������
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
//	DockPane(&m_wndToolBar);
//====================����SimToolBarͣ��������===========Begin2=====
	m_simToolBar.SetWindowText(_T("���湤����"));
	m_simToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_simToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	DockPane(&m_simToolBar);
	DockPaneLeftOf(&m_wndToolBar,&m_simToolBar);
//====================����SimToolBarͣ��������===========End2======
	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���ù�������ͣ�����ڲ˵��滻
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ���ÿ���(��ס Alt �϶�)�������Զ���
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// �����û�����Ĺ�����ͼ��
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// ���ò˵����Ի�(���ʹ�õ�����)
	// TODO: �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);


	//EnableToolTips(TRUE);    
	//m_ToolTipCtrl.Create(this, TTS_ALWAYSTIP);     
	//m_ToolTipCtrl.Activate(TRUE); 
	//m_ToolTipCtrl.AddTool((CWnd*)m_Edit_TimeStep, _T("����"));


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.lpszName = _T("New file-HSTPNSim");
//	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
//		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX &~FWS_ADDTOTITLE;
	cs.style &= ~FWS_ADDTOTITLE;

	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// ���ཫִ�������Ĺ���

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// Ϊ�����û������������Զ��尴ť
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
	//����һ����̬�������ڣ���Ϊһ�ж��� 
    m_splitterWnd1.CreateStatic(this,1,2);
   
	//��CCSplitterWndView���ӵ�0��0�д�����
    m_splitterWnd1.CreateView(0,0,RUNTIME_CLASS(CViewModelChoose),CSize(180,200), pContext);

	//��FormView1�����ӵ��ڶ������������0��0��
    m_splitterWnd1.CreateView(0,1,RUNTIME_CLASS(CHSTPN_SYSView),CSize(200,300),pContext);

//	return CFrameWndEx::OnCreateClient(lpcs, pContext);
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// DlgModelChoose��ѡ��Ҫ������PetriԪ�غ���Ӧ����Ӧ��������һ��WM_MESSAGE_CREATEMODEL1
// ���ˣ�MainFrame����Ȼ�󣬷���WM_MESSAGE_CREATEMODEL2��Ϣ��CHSTPN_SYSView
// Ŀ�ģ�����һ��m_whichModel����
///////////////////////////////////////////////////////////////////////////
LRESULT CMainFrame::OnCreateModel1(WPARAM wParam, LPARAM lParam)
{
	(CHSTPN_SYSView*)(m_splitterWnd1.GetPane(0,1))->SendMessage(WM_MESSAGE_CREATEMODEL2,wParam,0);
	return 0;
}
///////////////////////////////////////////////////////////////////////////
// ��View���У����ĳ��ģ��ʱ��CPlace���л���ӦLButtonDown��Ϣ��������һ��WM_MESSAGE_MOVEMODEL1��Ϣ
// ���ˣ�MainFrame����Ȼ�󣬷���WM_MESSAGE_MOVEMODEL2��Ϣ��CHSTPN_SYSView
// Ŀ��:����һ��u_SubscriptofModel����
LRESULT CMainFrame::OnMoveModel1(WPARAM wParam, LPARAM lParam)
{
	(CHSTPN_SYSView*)(m_splitterWnd1.GetPane(0,1))->SendMessage(WM_MESSAGE_MOVEMODEL2,wParam,lParam);
	(CArrow*)(m_splitterWnd1.GetPane(0,1))->SendMessage(WM_MESSAGE_MOVEMODEL2,wParam,lParam);
	return 0;
}
//========================================================================
// ��View���У���������ĳ��ģ����̧��ʱ��CPlace���л���ӦLButtonUp��Ϣ��������һ��WM_MESSAGE_DRAWLINEEND1��Ϣ
// ���ˣ�MainFrame����Ȼ�󣬷���WM_MESSAGE_DRAWLINEEND2��Ϣ��CHSTPN_SYSView
// Ŀ�ģ������ʱ��ͼ���ڴ��ڻ���״̬�������̧��λ�þ����߶ν���λ��
LRESULT CMainFrame::OnDrawLineend1(WPARAM wParam, LPARAM lParam)
{
	(CHSTPN_SYSView*)(m_splitterWnd1.GetPane(0,1))->SendMessage(WM_MESSAGE_DRAWLINEEND2,wParam,lParam);
	return 0;
}


// ������Ϣ��CView�࣬ʹCView���ȡʱ�䲽�� 
void CMainFrame::OnGetTimeStep1()
{
	(CHSTPN_SYSView*)(m_splitterWnd1.GetPane(0,1))->SendMessage(WM_MESSAGE_GETTIMESTEP,0,0);
}

// ������Ϣ��CView�࣬ʹCView���ȡ����ʱ��
void CMainFrame::OnGetSimTime1()
{
	(CHSTPN_SYSView*)(m_splitterWnd1.GetPane(0,1))->SendMessage(WM_MESSAGE_GETTIMESTEP,0,0);
}
// ������Ϣ��CView�࣬ʹCView���ȡtoken�����ļ��ʱ��
void CMainFrame::OnGetIntervalTime1()
{
	(CHSTPN_SYSView*)(m_splitterWnd1.GetPane(0,1))->SendMessage(WM_MESSAGE_GETTIMESTEP,0,0);
}
// ɾ������
LRESULT CMainFrame::OnMDdelete1(WPARAM wParam, LPARAM lParam)
{
	((CHSTPN_SYSView*)m_splitterWnd1.GetPane(0,1))->SendMessage(WM_MESSAGE_MDDELETE2,wParam,lParam);
	return 0;
}
// HSTPN�ṹ���ĸ���
LRESULT CMainFrame::OnUpdateHSTPNTree1(WPARAM wParam, LPARAM lParam)
{
	((CViewModelChoose*)m_splitterWnd1.GetPane(0,0))->m_dlgmodelchoose1->SendMessage(WM_MESSAGE_UPDATETREE2,0,0);
	return 0;
}

LRESULT CMainFrame::OnUpdateProgess(WPARAM wParam, LPARAM lParam)
{
	m_wndStatusBar.SetPaneProgress(1,wParam/10000,true);
	return 0;
}

// ��ʾ��������б�ģ�����Ա༭��
void CMainFrame::OnShowpramlist()
{
	// TODO: �ڴ���������������
	if(m_pParmListDlg==NULL)
	{
		m_pParmListDlg = new CDlgModelParmList();
		m_pParmListDlg->Create(IDD_DLG_DATALIST,NULL);
	}
	m_pParmListDlg->ShowWindow(SW_SHOW);

}


void CMainFrame::OnSimMove()
{
	// TODO: �ڴ���������������


 //   CScene  *pDlg = &(CScene::getInstance());
 //   pDlg->Create(IDD_DLG_SCENE);
	//pDlg->MoveWindow(CRect(500, 110, 1124, 478), TRUE);//���öԻ����С1024*768
	//HWND hw = pDlg->GetSafeHwnd();
	//pDlg->ShowWindow(SW_SHOWNORMAL);
	CScene  *pDlg=new CScene();
	pDlg->CreatSceneDlg();
	
	//if (CHSTPN_SYSSim::getMoveData().size() > 0) {
	//	CDlgMove  *pDlg = new CDlgMove;
	//	pDlg->Create(IDD_DLG_MOVE);
	//	
	//	pDlg->MoveWindow(CRect(100, 10, 1124, 778), TRUE);//���öԻ����С1024*768
	//	HWND hw = pDlg->GetSafeHwnd();
	//	pDlg->ShowWindow(SW_SHOWNORMAL);

	//	m_moveThread = AfxBeginThread(pDlg->CreatMove, (LPVOID)pDlg);// �������̣߳����г�����档����1 �̵߳���ں���,����һ��Ҫ����: UINT MyThreadFunction( LPVOID pParam ); ����2 �������̵߳Ĳ���, ע����������Ϊ:LPVOID, �������ǿ��Դ���һ���ṹ�����̡߳�
	//																 //m_moveThread = AfxBeginThread(OnTest, (LPVOID)this);
	//}
	//else
	//	MessageBoxW(L"ģ��δ���л�ģ���в�����ȫ�ֱ����������ģ�ͼ��");

	//���ݿ���Ժ���
	//mySQL.ConnectMySQL();
	//mySQL.QueryDatabase1();
	//mySQL.InsertData();
	//mySQL.DeleteData();
	//mySQL.ModifyData();


}


void CMainFrame::OnCombat()
{
	// TODO: �ڴ���������������
	CRect mainFramRect;
	//��ȡ�����ڵľ�������
	(AfxGetApp()->m_pMainWnd)->GetWindowRect(mainFramRect);
	//�����Ի���
	CDlgMove* pDlg = new CDlgMove;
	pDlg->Create(IDD_DLG_MOVE);
	//���öԻ����λ��
	pDlg->SetWindowPos(&CWnd::wndTop, mainFramRect.left + mainFramRect.Width() / 2 - 1024 / 2, mainFramRect.top + mainFramRect.Height() / 2 - 768 / 2, 1024, 768, NULL);
	//��ʾ�Ի���
	pDlg->ShowWindow(SW_SHOW);
	//���¶Ի���
	pDlg->UpdateWindow();

	//��ȡ����ĳ�ʼֵ
	UpdateData(TRUE);
	
	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_COMMAND, ID_DEBUG_START, NULL);

	m_moveThread = AfxBeginThread(pDlg->CreatMove, (LPVOID)pDlg);// �������̣߳����г�����档����1 �̵߳���ں���,����һ��Ҫ����: UINT MyThreadFunction( LPVOID pParam ); ����2 �������̵߳Ĳ���, ע����������Ϊ:LPVOID, �������ǿ��Դ���һ���ṹ�����̡߳�

}
