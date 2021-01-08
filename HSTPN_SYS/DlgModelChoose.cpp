// DlgModelChoose.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgModelChoose.h"


// CDlgModelChoose

IMPLEMENT_DYNCREATE(CDlgModelChoose, CFormView)

CDlgModelChoose::CDlgModelChoose()
	: CFormView(CDlgModelChoose::IDD)
{
	b_isInitTree = FALSE;
}

CDlgModelChoose::~CDlgModelChoose()
{
}

void CDlgModelChoose::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MDCHOSETREE, m_mdchoseTree);
}

BEGIN_MESSAGE_MAP(CDlgModelChoose, CFormView)
	ON_NOTIFY(NM_CLICK, IDC_MDCHOSETREE, &CDlgModelChoose::OnNMClickMdchosetree)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgModelChoose 诊断

#ifdef _DEBUG
void CDlgModelChoose::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDlgModelChoose::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDlgModelChoose 消息处理程序


BOOL CDlgModelChoose::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CDlgModelChoose::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	if(!b_isInitTree)
	{
		HICON hIcon[14];                                                             // 图标句柄数组   
		HTREEITEM hRoot;                                                             // 树的根节点的句柄   
		HTREEITEM hChild;                                                            // 树的子节点的句柄
		   
		hIcon[0] = theApp.LoadIcon(IDI_ICON1);                                       // 加载图标，并将它们的句柄保存到数组  
		hIcon[1] = theApp.LoadIcon(IDI_ICON2);   
		hIcon[2] = theApp.LoadIcon(IDI_ICON3); 
		hIcon[3] = theApp.LoadIcon(IDI_ICON4);   
		hIcon[4] = theApp.LoadIcon(IDI_ICON5);   
		hIcon[5] = theApp.LoadIcon(IDI_ICON6);
		hIcon[6] = theApp.LoadIcon(IDI_ICON7);   
		hIcon[7] = theApp.LoadIcon(IDI_ICON8);   
		hIcon[8] = theApp.LoadIcon(IDI_ICON9);
		hIcon[9] = theApp.LoadIcon(IDI_ICON10);
		hIcon[10] = theApp.LoadIcon(IDI_ICONSHOWMATRIX);   
		hIcon[11] = theApp.LoadIcon(IDI_ICONTOFILE);
		hIcon[12] = theApp.LoadIcon(IDI_ICONACCUMULATOR);
		hIcon[13] = theApp.LoadIcon(IDI_ICON12);                                     // 加载决策库所图标

		m_imageList.DeleteImageList();                                               // 情况链表
		  
		m_imageList.Create(32, 32, ILC_COLOR32, 14, 3);                              // 创建图像序列CImageList对象， 其中，10表示存储的个数为10，3是一个扩展大小，如果还需要存，则无需再扩展，但也只能再存三个    
		   
		for (int i=0; i<14; i++)                                                     // 将图标添加到图像序列
		{   
			m_imageList.Add(hIcon[i]);   
		}   
		// 加载图标，并将它们的句柄保存到数组 
		m_mdchoseTree.SetImageList(&m_imageList, TVSIL_NORMAL);                      // 为树形控件设置图像序列
		//-----------------------------------------------------------------------------------
		hRoot = m_mdchoseTree.InsertItem(_T("HSTPN基本元素"), 0, 0);                 // 插入根节点
		m_mdchoseTree.SetItemData(hRoot,1);                                          // 为节点关联数据，方便后续的消息响应函数中找到节点

		hChild = m_mdchoseTree.InsertItem(_T("离散库所"), 3, 3, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,4);
		hChild = m_mdchoseTree.InsertItem(_T("连续库所"), 4, 4, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,5);
		hChild = m_mdchoseTree.InsertItem(_T("随机库所"), 5, 5, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,6);
		hChild = m_mdchoseTree.InsertItem(_T("时延库所"), 6, 6, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,7);
		hChild = m_mdchoseTree.InsertItem(_T("控制库所"), 7, 7, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,8);
		hChild = m_mdchoseTree.InsertItem(_T("决策库所"), 13, 13, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,14);                                        //插入决策库所
		hChild = m_mdchoseTree.InsertItem(_T("变    迁"), 8, 8, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,9);
		hChild = m_mdchoseTree.InsertItem(_T("有 向 弧"), 9, 9, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,10);
		//-----------------------------------------------------------------------------------
		hRoot = m_mdchoseTree.InsertItem(_T("输出模块"), 1, 1); 
		m_mdchoseTree.SetItemData(hRoot,2);

		hChild = m_mdchoseTree.InsertItem(_T("输出到矩阵"), 10, 10, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,11);
		hChild = m_mdchoseTree.InsertItem(_T("输出至文件"), 11, 11, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,12);
	    //hChild = m_mdchoseTree.InsertItem(_T("封装模块"), 2, 2, hRoot, TVI_LAST);
		//m_mdchoseTree.SetItemData(hChild,15);
		//-----------------------------------------------------------------------------------
		//hRoot = m_mdchoseTree.InsertItem(_T("封装模块"), 2, 2);
		//m_mdchoseTree.SetItemData(hRoot,3);

		b_isInitTree = TRUE;
	}
}

// 响应tree control的点击操作，目的是确定哪个节点被点击，进而完成点击节点的消息响应
void CDlgModelChoose::OnNMClickMdchosetree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;     // NMTREEVIEW结构体中包含了树形控件通知消息的相关信息。树形控件的大多数通知消息都会带有指向该结构体的指针。
	
	CPoint p;  
    GetCursorPos(&p);                                    // 获取鼠标点击的位置，该点为屏幕坐标点
	m_mdchoseTree.ScreenToClient(&p);

	UINT nFlag = 0;
	HTREEITEM h=m_mdchoseTree.HitTest(p,&nFlag);         //Returns the current position of the cursor related to the CTreeCtrl object.

	 if((h !=  NULL)&&(TVHT_ONITEM & nFlag))             // 判断点击位置是否在节点上面
	 {
		 m_mdchoseTree.SelectItem(h);                    // 选取指定节点
		 int nDat = m_mdchoseTree.GetItemData(h);        // 获取节点关联数据
		 int u_whichModel = 0;
		 switch(nDat)
		 {
		 case 1:
			 // 展开子节点
			 u_whichModel = 0;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 2:
			 // 展开子节点
			 u_whichModel = 0;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 3:
			 // 展开子节点
			 u_whichModel = 0;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 4:                     // 创建离散库所
			 u_whichModel = 1;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 5:                     // 创建连续库所
			 u_whichModel = 2;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 6:                     // 创建时延库所
			 u_whichModel = 3;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 7:                     // 创建随机库所
			 u_whichModel = 4;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 8:                     // 创建控制库所
			 u_whichModel = 5;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 9:                     // 创建变迁
			 u_whichModel = 7;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 10:                    // 创建有向弧
			 u_whichModel = 8;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 11:
			 u_whichModel = 9;      // Create OUT_SHOWMATRIX OutputModel
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 12:
			 u_whichModel = 10;     // Create OUT_TOFILE OutputModel
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 13:

			 break;
		 case 14:
			 u_whichModel = 11;     //创建决策库所
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 15:
			 u_whichModel = 12;     //创建封装模块
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1, u_whichModel, 0);
			 break;
		 default:
			 break;
		 }
		 u_whichModel = 0;
	 }

}


void CDlgModelChoose::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);
	if(m_mdchoseTree.GetSafeHwnd()!=NULL)
	{
		m_mdchoseTree.SetWindowPos(NULL,rect.left,rect.top+25,rect.Width(),rect.Height()-25,SWP_NOACTIVATE | SWP_NOZORDER);
	}
}


void CDlgModelChoose::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFormView::OnPaint()
}


BOOL CDlgModelChoose::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return TRUE;
	return CFormView::OnEraseBkgnd(pDC);
}
