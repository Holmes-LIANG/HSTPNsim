// DlgModelChoose1.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgModelChoose1.h"

#include "MainFrm.h"
// CDlgModelChoose1

IMPLEMENT_DYNCREATE(CDlgModelChoose1, CFormView)

CDlgModelChoose1::CDlgModelChoose1()
	: CFormView(CDlgModelChoose1::IDD)
{
//	m_whichModel = 0;
	b_isInitTree = FALSE;
	//m_pDoc = (CHSTPN_SYSDoc*)(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument());
}

CDlgModelChoose1::~CDlgModelChoose1()
{
}

void CDlgModelChoose1::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HSTPNSTRUCTREE, m_HSTPNStrucTree);
}

BEGIN_MESSAGE_MAP(CDlgModelChoose1, CFormView)

	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_MESSAGE(WM_MESSAGE_UPDATETREE2,OnUpdateHSTPNTree2)
END_MESSAGE_MAP()


// CDlgModelChoose1 诊断

#ifdef _DEBUG
void CDlgModelChoose1::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgModelChoose1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDlgModelChoose1 消息处理程序


BOOL CDlgModelChoose1::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CDlgModelChoose1::OnPaint()
{
	m_pDoc = (CHSTPN_SYSDoc*)(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument());
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFormView::OnPaint()
	//=============================================
	// 更改IDC_STC_PETRI1控件的颜色和字体
	//CFont font;
	//font.CreatePointFont(150,_T("宋体"),NULL);
	//GetDlgItem(IDC_STC_PETRI1)->SetFont(&font);
	//========================================================================
	//添加背景色+Picture Control加载图片
	//========================================================================
	//-------------------为对话框添加背景色-----------------------------------
	CRect   rect;        
    GetClientRect(rect); 
	//CPen pen(PS_SOLID,3,RGB(0,255,0));
	//dc.SelectObject(&pen);
	
  //  dc.FillSolidRect(rect,RGB(250,250,250));//设置背景颜色

	

}



int CDlgModelChoose1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	return 0;
}




void CDlgModelChoose1::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	if(!b_isInitTree)
	{
		HICON hIcon[7];                                                             // 图标句柄数组   
//		HTREEITEM hRoot;                                                             // 树的根节点的句柄   
//		HTREEITEM hChild;                                                            // 树的子节点的句柄
		   
		hIcon[0] = theApp.LoadIcon(IDI_ICON11);                                       // 加载图标，并将它们的句柄保存到数组  
		hIcon[1] = theApp.LoadIcon(IDI_ICON4);
		hIcon[2] = theApp.LoadIcon(IDI_ICON5);
		hIcon[3] = theApp.LoadIcon(IDI_ICON6);
		hIcon[4] = theApp.LoadIcon(IDI_ICON7);
		hIcon[5] = theApp.LoadIcon(IDI_ICON8);
		hIcon[6] = theApp.LoadIcon(IDI_ICON9);

		m_imageList.DeleteImageList();                                               // 情况链表
		  
		m_imageList.Create(32, 32, ILC_COLOR32, 7, 3);                              // 创建图像序列CImageList对象， 其中，10表示存储的个数为10，3是一个扩展大小，如果还需要存，则无需再扩展，但也只能再存三个    
		   
		for (int i=0; i<7; i++)                                                     // 将图标添加到图像序列
		{   
			m_imageList.Add(hIcon[i]);   
		}   
		// 加载图标，并将它们的句柄保存到数组 
		m_HSTPNStrucTree.SetImageList(&m_imageList, TVSIL_NORMAL);                      // 为树形控件设置图像序列
		//-----------------------------------------------------------------------------------
		hRoot = m_HSTPNStrucTree.InsertItem(_T("网络结构"), 0, 0);                 // 插入根节点
		m_HSTPNStrucTree.SetItemData(hRoot,1);                                          // 为节点关联数据，方便后续的消息响应函数中找到节点

/*		hChild = m_HSTPNStrucTree.InsertItem(_T("离散库所"), 1, 1, hRoot, TVI_LAST);
		HTREEITEM hChild1=m_HSTPNStrucTree.InsertItem(_T("p0"), 1, 1, hChild, TVI_LAST);
		hChild1=m_HSTPNStrucTree.InsertItem(_T("p1"), 1, 1, hChild, TVI_LAST);

		hChild = m_HSTPNStrucTree.InsertItem(_T("连续库所"), 2, 2, hRoot, TVI_LAST);
		hChild1=m_HSTPNStrucTree.InsertItem(_T("p2"), 2, 2, hChild, TVI_LAST);

		hChild = m_HSTPNStrucTree.InsertItem(_T("随机库所"), 3, 3, hRoot, TVI_LAST);
		hChild1=m_HSTPNStrucTree.InsertItem(_T("p3"), 2, 2, hChild, TVI_LAST);

		hChild = m_HSTPNStrucTree.InsertItem(_T("时延库所"), 4, 4, hRoot, TVI_LAST);
		hChild1=m_HSTPNStrucTree.InsertItem(_T("p4"), 2, 2, hChild, TVI_LAST);

		hChild = m_HSTPNStrucTree.InsertItem(_T("控制库所"), 5, 5, hRoot, TVI_LAST);
		hChild1=m_HSTPNStrucTree.InsertItem(_T("p5"), 2, 2, hChild, TVI_LAST);


		hChild = m_HSTPNStrucTree.InsertItem(_T("变    迁"), 6, 6, hRoot, TVI_LAST);
		hChild1=m_HSTPNStrucTree.InsertItem(_T("t0"), 6, 6, hChild, TVI_LAST);*/

		b_isInitTree = TRUE;
	}
}


void CDlgModelChoose1::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);
	if(m_HSTPNStrucTree.GetSafeHwnd()!=NULL)
	{
		m_HSTPNStrucTree.SetWindowPos(NULL,rect.left,rect.top+30,rect.Width(),rect.Height()-30,SWP_NOACTIVATE | SWP_NOZORDER);
	}
}
// 接收来自Mainframe（始于HSTPN_SYSView）的消息，更新树结构
LRESULT CDlgModelChoose1::OnUpdateHSTPNTree2(WPARAM wParam, LPARAM lParam)
{
	//--获取库所、变迁信息-----------------------------------------------------
	vector<CPlace*>::iterator iterP = m_pDoc->Vector_Place.begin();
	vector<CPlace*>::iterator iterPend = m_pDoc->Vector_Place.end();
	vector<CString> arrycptnDiscPlace;
	vector<CString> arrycptnCtnsPlace;
	vector<CString> arrycptnRandPlace;
	vector<CString> arrycptnTimePlace;
	vector<CString> arrycptnCtrlPlace;
	for(;iterP != iterPend;iterP++)
	{
		if((*iterP)->PlaceStyle == STYLE_DISCRETE)
		{
			arrycptnDiscPlace.push_back((*iterP)->m_caption);
		}
		if((*iterP)->PlaceStyle == STYLE_CNTNUS)
		{
			arrycptnCtnsPlace.push_back((*iterP)->m_caption);
		}
		if((*iterP)->PlaceStyle == STYLE_RAND)
		{
			arrycptnRandPlace.push_back((*iterP)->m_caption);
		}
		if((*iterP)->PlaceStyle == STYLE_TIME)
		{
			arrycptnTimePlace.push_back((*iterP)->m_caption);
		}
		if((*iterP)->PlaceStyle == STYLE_TCTRL || (*iterP)->PlaceStyle == STYLE_LCTRL)
		{
			arrycptnCtrlPlace.push_back((*iterP)->m_caption);
		}
	}
	// 获取所用的变迁信息
	vector<CTransition*>::iterator iterT = m_pDoc->Vector_Trans.begin();
	vector<CTransition*>::iterator iterTend = m_pDoc->Vector_Trans.end();
	vector<CString> arrycptnTrans;
	for(;iterT != iterTend;iterT++)
	{
		arrycptnTrans.push_back((*iterT)->m_caption);
	}
	
	//--更新树结构------------------------------------------------------------------
	// 清空树结构的数据
	m_HSTPNStrucTree.DeleteAllItems();

	HTREEITEM hChild;                                                            // 树的子节点的句柄--一级子节点
	HTREEITEM hChild1;                                                            // 树的子节点的句柄--二级子节点
	m_HSTPNStrucTree.SetImageList(&m_imageList, TVSIL_NORMAL);                      // 为树形控件设置图像序列
	
	hRoot = m_HSTPNStrucTree.InsertItem(_T("网络结构"), 0, 0);                 // 插入根节点
	m_HSTPNStrucTree.SetItemData(hRoot,1);                                          // 为节点关联数据，方便后续的消息响应函数中找到节点
	// 显示所用库所节点
	if(arrycptnDiscPlace.size()!=0)
	{
		CString str_place1size;
		str_place1size.Format(L"%d",arrycptnDiscPlace.size());
		str_place1size = _T("(")+str_place1size + _T(")");
		hChild = m_HSTPNStrucTree.InsertItem(_T("离散库所")+str_place1size, 1, 1, hRoot, TVI_LAST);
		for(vector<CString>::iterator iterp1=arrycptnDiscPlace.begin();iterp1!=arrycptnDiscPlace.end();iterp1++)
		{
			hChild1=m_HSTPNStrucTree.InsertItem((*iterp1), 1, 1, hChild, TVI_LAST);
		}
	}
	if(arrycptnCtnsPlace.size()!=0)
	{
		CString str_place2size;
		str_place2size.Format(L"%d",arrycptnCtnsPlace.size());
		str_place2size = _T("(")+str_place2size + _T(")");
		hChild = m_HSTPNStrucTree.InsertItem(_T("连续库所")+str_place2size, 2, 2, hRoot, TVI_LAST);
		for(vector<CString>::iterator iterp2=arrycptnCtnsPlace.begin();iterp2!=arrycptnCtnsPlace.end();iterp2++)
		{
			hChild1=m_HSTPNStrucTree.InsertItem((*iterp2), 2, 2, hChild, TVI_LAST);
		}
	}
	if(arrycptnRandPlace.size()!=0)
	{
		CString str_place3size;
		str_place3size.Format(L"%d",arrycptnRandPlace.size());
		str_place3size = _T("(")+str_place3size + _T(")");
		hChild = m_HSTPNStrucTree.InsertItem(_T("随机库所")+str_place3size, 3, 3, hRoot, TVI_LAST);
		for(vector<CString>::iterator iterp3=arrycptnRandPlace.begin();iterp3!=arrycptnRandPlace.end();iterp3++)
		{
			hChild1=m_HSTPNStrucTree.InsertItem((*iterp3), 3, 3, hChild, TVI_LAST);
		}
	}
	if(arrycptnTimePlace.size()!=0)
	{
		CString str_place4size;
		str_place4size.Format(L"%d",arrycptnTimePlace.size());
		str_place4size = _T("(")+str_place4size + _T(")");
		hChild = m_HSTPNStrucTree.InsertItem(_T("时延库所")+str_place4size, 4, 4, hRoot, TVI_LAST);
		for(vector<CString>::iterator iterp4=arrycptnTimePlace.begin();iterp4!=arrycptnTimePlace.end();iterp4++)
		{
			hChild1=m_HSTPNStrucTree.InsertItem((*iterp4), 4, 4, hChild, TVI_LAST);
		}
	}
	if(arrycptnCtrlPlace.size()!=0)
	{
		CString str_place5size;
		str_place5size.Format(L"%d",arrycptnCtrlPlace.size());
		str_place5size = _T("(")+str_place5size + _T(")");
		hChild = m_HSTPNStrucTree.InsertItem(_T("控制库所")+str_place5size, 5, 5, hRoot, TVI_LAST);
		for(vector<CString>::iterator iterp5=arrycptnCtrlPlace.begin();iterp5!=arrycptnCtrlPlace.end();iterp5++)
		{
			hChild1=m_HSTPNStrucTree.InsertItem((*iterp5), 5, 5, hChild, TVI_LAST);
		}
	}
	// 显示所有变迁节点
	if(arrycptnTrans.size()!=0)
	{
		CString str_transsize;
		str_transsize.Format(L"%d",arrycptnTrans.size());
		str_transsize = _T("(")+str_transsize + _T(")");
		hChild = m_HSTPNStrucTree.InsertItem(_T("变    迁")+str_transsize, 6, 6, hRoot, TVI_LAST);
		for(vector<CString>::iterator itert=arrycptnTrans.begin();itert!=arrycptnTrans.end();itert++)
		{
			hChild1=m_HSTPNStrucTree.InsertItem((*itert), 6, 6, hChild, TVI_LAST);
		}
	}
	
	return 0;
}