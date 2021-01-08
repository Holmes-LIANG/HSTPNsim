// DlgModelParmList.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgModelParmList.h"
#include "afxdialogex.h"


// CDlgModelParmList 对话框

IMPLEMENT_DYNAMIC(CDlgModelParmList, CDialogEx)

/*BEGIN_MESSAGE_MAP(CDlgModelParmList, CDialogEx)
	// 标准打印命令
	ON_MESSAGE(WM_MESSAGE_UPDATETREEOFMODEL,OnUpdateTreeofModel)

END_MESSAGE_MAP()*/

CDlgModelParmList::CDlgModelParmList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgModelParmList::IDD, pParent)
	, m_strvaluename1(_T(""))
//	, m_dblValueInt(0)
//, m_rdioQuoteGlobalL(0)
{
	b_isInitTree      = FALSE;
	m_rowG             = -1;
    m_columnG          = -1;
	m_rowforDG         = -1;
    m_columnforDG      = -1;
	p_Model            = NULL;

	m_rowforDL         = -1;
    m_columnforDL      = -1;
	m_rowL             = -1;
    m_columnL          = -1;
	p_Doc =            NULL;
}

CDlgModelParmList::~CDlgModelParmList()
{
}

void CDlgModelParmList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE2, m_treeListofModel);
	DDX_Control(pDX, IDC_LIST3, m_DataListAll);
	DDX_Control(pDX, IDC_EDTVALUENAEM1, m_edtvaluename1);
	DDX_Text(pDX, IDC_EDTVALUENAEM1, m_strvaluename1);
	DDX_Control(pDX, IDC_EDTVALUEINT1, m_edtValueInt1);
	DDX_Text(pDX, IDC_EDTVALUEINT1, m_dblValueInt);
	DDX_Control(pDX, IDC_LISTLOCAL, m_ListLocal);
	DDX_Control(pDX, IDC_STCLISTNAME, m_staticG);
	DDX_Control(pDX, IDC_STCLISTNAMEL, m_staticL);
	DDX_Control(pDX, IDC_BTNADD, m_btnAddG);
	DDX_Control(pDX, IDC_BTNDLETE, m_btnDeleteG);
	DDX_Control(pDX, IDC_BTNADDL, m_btnAddL);
	DDX_Control(pDX, IDC_BTNDLETEL, m_btnDeleteL);
	DDX_Control(pDX, IDC_EDTVALUENAEM_L, m_edtValueNameL);
	DDX_Control(pDX, IDC_CMBSQUENSE_L, m_cmbSquenceL);
	DDX_Control(pDX, IDC_CMBIOTYPE_L, m_cmbIOtypeL);
	DDX_Control(pDX, IDC_CMBGLOBAL_L, m_cmbGlobalNameL);
	DDX_Control(pDX, IDC_CMBSOURCE_L, m_cmbDataSourceL);
	DDX_Control(pDX, IDC_CMBSOURCEVALUE_L, m_cmbSourceValueL);
	DDX_Control(pDX, IDC_EDTVALUEINT_L, m_edtValueIntL);
//	DDX_Control(pDX, IDCANCEL, m_rdioQuoteG_L);
//	DDX_Control(pDX, IDC_CHKQUOTEG_L, m_checkIsQuoteG_L);
	DDX_Control(pDX, IDC_CMBQUOTE_L, m_cmbIsQuoteG_L);
}


BEGIN_MESSAGE_MAP(CDlgModelParmList, CDialogEx)
	ON_WM_SIZE()
	ON_MESSAGE(WM_MESSAGE_UPDATETREEOFMODEL,OnUpdateTreeofModel)
	ON_BN_CLICKED(IDC_BTNADD, &CDlgModelParmList::OnBtnaAddValueG)
	ON_BN_CLICKED(IDC_BTNDLETE, &CDlgModelParmList::OnBtnDltValueG)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CDlgModelParmList::OnClick_ListG)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST3, &CDlgModelParmList::OnDblclk_ListG)
	ON_EN_CHANGE(IDC_EDTVALUENAEM1, &CDlgModelParmList::OnGetValueNameG)
	ON_EN_CHANGE(IDC_EDTVALUEINT1, &CDlgModelParmList::OnGetValueIntG)
	ON_NOTIFY(NM_CLICK, IDC_TREE2, &CDlgModelParmList::OnNMClick_ChoseNode)
	ON_NOTIFY(NM_CLICK, IDC_LISTLOCAL, &CDlgModelParmList::OnNMClick_ListLocal)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTLOCAL, &CDlgModelParmList::OnNMDblclk_ListLocal)
	ON_BN_CLICKED(IDC_BTNADDL, &CDlgModelParmList::OnBtnaAddValueL)
	ON_BN_CLICKED(IDC_BTNDLETEL, &CDlgModelParmList::OnBtnDltValueL)
	ON_BN_CLICKED(IDOK, &CDlgModelParmList::OnBtnOK)
	ON_EN_CHANGE(IDC_EDTVALUENAEM_L, &CDlgModelParmList::OnGetVluName_L)
	ON_CBN_SELCHANGE(IDC_CMBSQUENSE_L, &CDlgModelParmList::OnGetSquence_L)
	ON_CBN_SELCHANGE(IDC_CMBIOTYPE_L, &CDlgModelParmList::OnGetIOtype_L)
	ON_CBN_SELCHANGE(IDC_CMBQUOTE_L, &CDlgModelParmList::OnGetIsQuote_L)
	ON_CBN_SELCHANGE(IDC_CMBGLOBAL_L, &CDlgModelParmList::OnGetGlobalVlu_L)
	ON_CBN_SELCHANGE(IDC_CMBSOURCE_L, &CDlgModelParmList::OnGetDataSource_L)
	ON_CBN_SELCHANGE(IDC_CMBSOURCEVALUE_L, &CDlgModelParmList::OnGetDataSourceValue_L)
	ON_EN_CHANGE(IDC_EDTVALUEINT_L, &CDlgModelParmList::OnGetValueInt_L)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgModelParmList 消息处理程
 // 树更新函数
 LRESULT CDlgModelParmList::OnUpdateTreeofModel(WPARAM wParam, LPARAM lParam)
 {
	vector<CPlace*>::iterator iterP = ((CHSTPN_SYSView*)(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1)))->GetDocument()->Vector_Place.begin();
	vector<CPlace*>::iterator iterPend = ((CHSTPN_SYSView*)(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1)))->GetDocument()->Vector_Place.end();
	vector<CString> arrycptnDiscPlace;
	vector<CString> arrycptnCtnsPlace;
	vector<CString> arrycptnRandPlace;
	vector<CString> arrycptnTimePlace;
	vector<CString> arrycptnCtrlPlace;
	vector<CString> arrycptnDisicPlace;
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
		if((*iterP)->PlaceStyle == STYLE_DISIC)
		{
			arrycptnDisicPlace.push_back((*iterP)->m_caption);
		}
	}
	
	
	//--更新树结构------------------------------------------------------------------
	// 清空树结构的数据
	m_treeListofModel.DeleteAllItems();

	HTREEITEM hChild;                                                            // 树的子节点的句柄--一级子节点
	HTREEITEM hChild1;                                                            // 树的子节点的句柄--二级子节点
	m_treeListofModel.SetImageList(&m_imageList, TVSIL_NORMAL);                      // 为树形控件设置图像序列
	
	hRoot = m_treeListofModel.InsertItem(_T("HSTPN模型"), 0, 0);                 // 插入根节点
	m_treeListofModel.SetItemData(hRoot,1);                                          // 为节点关联数据，方便后续的消息响应函数中找到节点
	// 显示所用库所节点
	if(arrycptnDiscPlace.size()!=0)
	{
		CString str_place1size;
		str_place1size.Format(L"%d",arrycptnDiscPlace.size());
		str_place1size = _T("(")+str_place1size + _T(")");
		hChild = m_treeListofModel.InsertItem(_T("离散库所")+str_place1size, 1, 1, hRoot, TVI_LAST);
		m_treeListofModel.SetItemData(hChild,2);
		for(vector<CString>::iterator iterp1=arrycptnDiscPlace.begin();iterp1!=arrycptnDiscPlace.end();iterp1++)
		{
			hChild1=m_treeListofModel.InsertItem((*iterp1), 1, 1, hChild, TVI_LAST);
		}
	}
	if(arrycptnCtnsPlace.size()!=0)
	{
		CString str_place2size;
		str_place2size.Format(L"%d",arrycptnCtnsPlace.size());
		str_place2size = _T("(")+str_place2size + _T(")");
		hChild = m_treeListofModel.InsertItem(_T("连续库所")+str_place2size, 2, 2, hRoot, TVI_LAST);
		m_treeListofModel.SetItemData(hChild,3);
		for(vector<CString>::iterator iterp2=arrycptnCtnsPlace.begin();iterp2!=arrycptnCtnsPlace.end();iterp2++)
		{
			hChild1=m_treeListofModel.InsertItem((*iterp2), 2, 2, hChild, TVI_LAST);
		}
	}
	if(arrycptnRandPlace.size()!=0)
	{
		CString str_place3size;
		str_place3size.Format(L"%d",arrycptnRandPlace.size());
		str_place3size = _T("(")+str_place3size + _T(")");
		hChild = m_treeListofModel.InsertItem(_T("随机库所")+str_place3size, 3, 3, hRoot, TVI_LAST);
		m_treeListofModel.SetItemData(hChild,4);
		for(vector<CString>::iterator iterp3=arrycptnRandPlace.begin();iterp3!=arrycptnRandPlace.end();iterp3++)
		{
			hChild1=m_treeListofModel.InsertItem((*iterp3), 3, 3, hChild, TVI_LAST);
		}
	}
	if(arrycptnTimePlace.size()!=0)
	{
		CString str_place4size;
		str_place4size.Format(L"%d",arrycptnTimePlace.size());
		str_place4size = _T("(")+str_place4size + _T(")");
		hChild = m_treeListofModel.InsertItem(_T("时延库所")+str_place4size, 4, 4, hRoot, TVI_LAST);
		m_treeListofModel.SetItemData(hChild,5);
		for(vector<CString>::iterator iterp4=arrycptnTimePlace.begin();iterp4!=arrycptnTimePlace.end();iterp4++)
		{
			hChild1=m_treeListofModel.InsertItem((*iterp4), 4, 4, hChild, TVI_LAST);
		}
	}
	if(arrycptnCtrlPlace.size()!=0)
	{
		CString str_place5size;
		str_place5size.Format(L"%d",arrycptnCtrlPlace.size());
		str_place5size = _T("(")+str_place5size + _T(")");
		hChild = m_treeListofModel.InsertItem(_T("控制库所")+str_place5size, 5, 5, hRoot, TVI_LAST);
		m_treeListofModel.SetItemData(hChild,6);
		for(vector<CString>::iterator iterp5=arrycptnCtrlPlace.begin();iterp5!=arrycptnCtrlPlace.end();iterp5++)
		{
			hChild1=m_treeListofModel.InsertItem((*iterp5), 5, 5, hChild, TVI_LAST);
		}
	}
	if(arrycptnDisicPlace.size()!=0)
	{
		CString str_place6size;
		str_place6size.Format(L"%d",arrycptnDisicPlace.size());
		str_place6size = _T("(")+str_place6size + _T(")");
		hChild = m_treeListofModel.InsertItem(_T("决策库所")+str_place6size, 7, 7, hRoot, TVI_LAST);
		m_treeListofModel.SetItemData(hChild,8);
		for(vector<CString>::iterator iterp6=arrycptnDisicPlace.begin();iterp6!=arrycptnDisicPlace.end();iterp6++)
		{
			hChild1=m_treeListofModel.InsertItem((*iterp6), 7, 7, hChild, TVI_LAST);
		}
	}
	 return 0;
 }

 BOOL CDlgModelParmList::OnInitDialog()
 {
	 CDialogEx::OnInitDialog();

	// 初始化模型树
	if(!b_isInitTree)
	{
		HICON hIcon[8];                                                             // 图标句柄数组   
		   
		hIcon[0] = theApp.LoadIcon(IDI_ICON11);                                       // 加载图标，并将它们的句柄保存到数组  
		hIcon[1] = theApp.LoadIcon(IDI_ICON4);
		hIcon[2] = theApp.LoadIcon(IDI_ICON5);
		hIcon[3] = theApp.LoadIcon(IDI_ICON6);
		hIcon[4] = theApp.LoadIcon(IDI_ICON7);
		hIcon[5] = theApp.LoadIcon(IDI_ICON8);
		hIcon[6] = theApp.LoadIcon(IDI_ICON9);
		hIcon[7] = theApp.LoadIcon(IDI_ICON12);

		m_imageList.DeleteImageList();                                               // 情况链表
		  
		m_imageList.Create(32, 32, ILC_COLOR32, 8, 2);                              // 创建图像序列CImageList对象， 其中，10表示存储的个数为10，3是一个扩展大小，如果还需要存，则无需再扩展，但也只能再存三个    
		   
		for (int i=0; i<8; i++)                                                     // 将图标添加到图像序列
		{   
			m_imageList.Add(hIcon[i]);   
		}   
		// 加载图标，并将它们的句柄保存到数组 
		m_treeListofModel.SetImageList(&m_imageList, TVSIL_NORMAL);                      // 为树形控件设置图像序列
		//-----------------------------------------------------------------------------------
		hRoot = m_treeListofModel.InsertItem(_T("HSTPN模型"), 0, 0);                 // 插入根节点
		m_treeListofModel.SetItemData(hRoot,1);                                          // 为节点关联数据，方便后续的消息响应函数中找到节点


		b_isInitTree = TRUE;
	}

	 // 初始化全局变量表-----------------------------------------------------
	 m_DataListAll.ModifyStyle(0,LVS_REPORT);               // 设置为报表模式
	 m_DataListAll.SetExtendedStyle(m_DataListAll.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_FULLROWSELECT);// 间隔线+行选中

	 m_DataListAll.InsertColumn(0,L"变量名");               // 插入表头
	 m_DataListAll.InsertColumn(1,L"I/O类型");
	 m_DataListAll.InsertColumn(2,L"初始值");

	 m_DataListAll.SetBkColor(RGB(255,255,255));

	 CRect rect;  
	 m_DataListAll.GetClientRect(rect);                     //获得当前客户区信息   
	 m_DataListAll.SetColumnWidth(0, rect.Width() / 3);     //设置列的宽度。   
	 m_DataListAll.SetColumnWidth(1, rect.Width() / 3); 
	 m_DataListAll.SetColumnWidth(2, rect.Width() / 3);

	 m_edtvaluename1.ShowWindow(SW_HIDE);
	 m_edtValueInt1.ShowWindow(SW_HIDE);              // 设置控件不可见

	 // 初始化局部变量表--------------------------------------------------------
	 m_ListLocal.ModifyStyle(0,LVS_REPORT);               // 设置为报表模式
	 m_ListLocal.SetExtendedStyle(m_ListLocal.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_FULLROWSELECT);// 间隔线+行选中

	 m_ListLocal.InsertColumn(0,L"变量名");               // 插入表头
	 m_ListLocal.InsertColumn(1,L"序号(Lua)");
	 m_ListLocal.InsertColumn(2,L"I/O类型");
	 m_ListLocal.InsertColumn(3,L"引用全局");
	 m_ListLocal.InsertColumn(4,L"全局变量");
	 m_ListLocal.InsertColumn(5,L"数据源");
	 m_ListLocal.InsertColumn(6,L"数据源变量");
	 m_ListLocal.InsertColumn(7,L"初始值");

	 m_ListLocal.SetBkColor(RGB(255,255,255));

	 CRect rectL;  
	 m_ListLocal.GetClientRect(rectL);                     //获得当前客户区信息   
	 m_ListLocal.SetColumnWidth(0, rectL.Width() / 8);     //设置列的宽度。   
	 m_ListLocal.SetColumnWidth(1, rectL.Width() / 8); 
	 m_ListLocal.SetColumnWidth(2, rectL.Width() / 8);
	 m_ListLocal.SetColumnWidth(3, rectL.Width() / 8); 
	 m_ListLocal.SetColumnWidth(4, rectL.Width() / 8);
	 m_ListLocal.SetColumnWidth(5, rectL.Width() / 8); 
	 m_ListLocal.SetColumnWidth(6, rectL.Width() / 8);
	 m_ListLocal.SetColumnWidth(7, rectL.Width() / 8);

	 m_edtValueNameL.ShowWindow(SW_HIDE);
	 m_cmbSquenceL.ShowWindow(SW_HIDE);
	 m_cmbIOtypeL.ShowWindow(SW_HIDE);
	 m_cmbIsQuoteG_L.ShowWindow(SW_HIDE);
	 m_cmbGlobalNameL.ShowWindow(SW_HIDE);
	 m_cmbDataSourceL.ShowWindow(SW_HIDE);
	 m_cmbSourceValueL.ShowWindow(SW_HIDE);
	 m_edtValueIntL.ShowWindow(SW_HIDE);

	 m_cmbSquenceL.InsertString(0,_T("0"));
	 m_cmbSquenceL.SetCurSel(0);
	 m_cmbIOtypeL.InsertString(0,_T("input"));
	 m_cmbIOtypeL.InsertString(1,_T("output"));
	 m_cmbIOtypeL.InsertString(2,_T("in/output"));
	 m_cmbIOtypeL.SetCurSel(0);
	 m_cmbIsQuoteG_L.InsertString(0,_T("true"));
	 m_cmbIsQuoteG_L.InsertString(1,_T("false"));
	 m_cmbIsQuoteG_L.SetCurSel(0);
	 
	 m_cmbGlobalNameL.InsertString(0,_T("无"));
	 m_cmbGlobalNameL.SetCurSel(0);

//	 m_cmbDataSourceL.InsertString(0,_T("使能标识"));
//	 m_cmbDataSourceL.SetCurSel(0);



	 m_DataListAll.ShowWindow(SW_SHOW);
	 m_staticG.ShowWindow(SW_SHOW);
	 m_btnAddG.ShowWindow(SW_SHOW);
	 m_btnDeleteG.ShowWindow(SW_SHOW);

	 m_ListLocal.ShowWindow(SW_HIDE);
	 m_staticL.ShowWindow(SW_HIDE);
	 m_btnAddL.ShowWindow(SW_HIDE);
	 m_btnDeleteL.ShowWindow(SW_HIDE); 

	 // 再次打开后，在全局变量表


	 return TRUE;  // return TRUE unless you set the focus to a control
	 // 异常: OCX 属性页应返回 FALSE
 }

 void CDlgModelParmList::OnSize(UINT nType, int cx, int cy)
 {
	 CDialogEx::OnSize(nType, cx, cy);

	 // TODO: 在此处添加消息处理程序代码
 }

 // 全局变量表，点击“添加”按钮
 void CDlgModelParmList::OnBtnaAddValueG()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 if(p_Doc==NULL)
	 {
		 p_Doc = (CHSTPN_SYSDoc*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	 }
	 int n = p_Doc->arryIOputDataG.size(); // 获取已有变量的个数
	 m_DataListAll.InsertItem(n,_T(""));
	 CString str1;
	 str1.Format(_T("%d"),n);
	 m_DataListAll.SetItemText(n,0,_T("name") + str1);
	 m_DataListAll.SetItemText(n,1,_T("output"));
	 m_DataListAll.SetItemText(n,2,_T("0.0"));
	 
	 IOValue *value = new IOValue();
	 value->Name = _T("name") + str1;
	 value->Sequence = 0;
	 value->IOType = _T("output");
	 value->isQuoteG = false;
	 value->GlobalVluName = _T("");
	 value->FromModel = _T("");
	 value->FromMDVluName = _T("");
	 value->Value = 0.0;
	 value->InitValue = 0.0;

	 p_Doc->arryIOputDataG.push_back(value);
 }

 // 全局变量表，点击“删除”按钮
 void CDlgModelParmList::OnBtnDltValueG()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 if(p_Doc==NULL)
	 {
		 p_Doc = (CHSTPN_SYSDoc*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	 }

	 if(m_rowforDG != -1)
	 {
		 vector<IOValue*>::iterator iter = p_Doc->arryIOputDataG.begin() + m_rowforDG;
		 delete (*iter);
		 p_Doc->arryIOputDataG.erase(iter);
		
		 m_DataListAll.DeleteItem(m_rowforDG);// 更新列表的显示

		 m_rowforDG = -1;
	 }
 }

 // 单击全局变量表
 void CDlgModelParmList::OnClick_ListG(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	 // TODO: 在此添加控件通知处理程序代码
	 NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	 // TODO: 在此添加控件通知处理程序代码
	 CRect rc;
     if(pNMListView->iItem==-1)
	 {
	 	 return;
	 }
     m_rowforDG    = pNMListView->iItem;                  // m_row为被选中行的行序号（int类型成员变量）
     m_columnforDG = pNMListView->iSubItem;               // m_column为被选中行的列序号（int类型成员变量）

	 //UpdateData();                                    // 完成控件与变量的数据交换，从而获取m_editvalue_IOname值。

	 m_edtvaluename1.ShowWindow(SW_HIDE);
	 m_edtValueInt1.ShowWindow(SW_HIDE);              // 设置控件不可见



	 *pResult = 0;
 }

 // 双击全局变量表
 void CDlgModelParmList::OnDblclk_ListG(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	 // TODO: 在此添加控件通知处理程序代码
	 m_edtvaluename1.SetParent(this);
	 m_edtValueInt1.SetParent(this);

	 NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	 CRect rc;
     if(pNMListView->iItem==-1)
	 {
		 return;
	 }

     m_rowG    = pNMListView->iItem;                                                       // m_row为被选中行的行序号（int类型成员变量）
     m_columnG = pNMListView->iSubItem;                                                    // m_column为被选中行的列序号（int类型成员变量）

//	 UpdateData(TRUE);
	 m_DataListAll.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem,LVIR_LABEL,rc); // 获取被点击子项的矩形位置和大小
	
	 InitCtrl_GetIO(m_rowG, m_columnG, rc);                                                  // 根据双击的行和列，显示相应控件到单元格位置,其他控件隐藏

	 *pResult = 0;
 }

  void CDlgModelParmList::InitCtrl_GetIO(int rowofchart, int columnofchart, CRect rc)
  {

	  if(columnofchart==0)
	  {
/*		   rc.left+=243;
		   rc.right+=248;
		   rc.top+=30;
		   rc.bottom+=32;*/
		   m_edtvaluename1.SetParent(&m_DataListAll);
		   m_edtvaluename1.SetWindowText(m_DataListAll.GetItemText(rowofchart,columnofchart));//将该子项中的值放在Edit控件中； 
		   m_edtvaluename1.ShowWindow(SW_SHOW);
		   m_edtvaluename1.MoveWindow(&rc);
		   m_edtvaluename1.SetFocus();
		   m_edtvaluename1.ShowCaret();//显示光标 
		   m_edtvaluename1.SetSel(-1);
		   m_edtValueInt1.ShowWindow(SW_HIDE);
	   }
	   if(columnofchart==1)
	   {
		   // do nothing
	   }
	   if(columnofchart==2)
	   {
/*		   rc.left+=248;
		   rc.right+=248;
		   rc.top+=30;
		   rc.bottom+=32;*/
		   m_edtValueInt1.SetParent(&m_DataListAll);
		   m_edtValueInt1.SetWindowText(m_DataListAll.GetItemText(rowofchart,columnofchart));//将该子项中的值放在Edit控件中；
		   m_edtValueInt1.ShowWindow(SW_SHOW);
		   m_edtValueInt1.MoveWindow(&rc);
		   m_edtValueInt1.SetFocus();
		   m_edtValueInt1.ShowCaret();
		   m_edtValueInt1.SetSel(-1);
		   m_edtvaluename1.ShowWindow(SW_HIDE);
	   }
  }

  // 获取全局变量Name
  void CDlgModelParmList::OnGetValueNameG()
  {
	  // TODO:  在此添加控件通知处理程序代码
	  m_edtvaluename1.GetWindowText(m_strvaluename1);

	  m_DataListAll.SetItemText(m_rowG,m_columnG,m_strvaluename1);
	  p_Doc->arryIOputDataG.at(m_rowG)->Name = m_strvaluename1;
  }

  // 获取全局变量IntValue
  void CDlgModelParmList::OnGetValueIntG()
  {
	  // TODO:  在此添加控件通知处理程序代码
	  CString strvalueint;
	  m_edtValueInt1.GetWindowText(strvalueint);
	  m_dblValueInt = _ttof(strvalueint);

	  m_DataListAll.SetItemText(m_rowG,m_columnG,strvalueint);
	  p_Doc->arryIOputDataG.at(m_rowG)->Value = m_dblValueInt;
	  p_Doc->arryIOputDataG.at(m_rowG)->InitValue = m_dblValueInt;
  }

  // 当点击模型树时的响应
  void CDlgModelParmList::OnNMClick_ChoseNode(NMHDR *pNMHDR, LRESULT *pResult)
  {
	  // TODO: 在此添加控件通知处理程序代码
	  p_Doc = (CHSTPN_SYSDoc*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();

	  *pResult = 0;

	  NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;     // NMTREEVIEW结构体中包含了树形控件通知消息的相关信息。树形控件的大多数通知消息都会带有指向该结构体的指针。
	
	  CPoint p;  
      GetCursorPos(&p);                                    // 获取鼠标点击的位置，该点为屏幕坐标点
	  m_treeListofModel.ScreenToClient(&p);

	  UINT nFlag = 0;
	  HTREEITEM h=m_treeListofModel.HitTest(p,&nFlag);     //Returns the current position of the cursor related to the CTreeCtrl object.

	   if((h !=  NULL)&&(TVHT_ONITEM & nFlag))             // 判断点击位置是否在节点上面
	   {
		   m_treeListofModel.SelectItem(h);                // 选取指定节点
		   int nDat = m_treeListofModel.GetItemData(h);    // 获取节点关联数据
		   CString str_nodeText = m_treeListofModel.GetItemText(h);
		   switch(nDat)
		   {
		   case 1:
		   case 2:
		   case 3:
		   case 4:
		   case 5:
		   case 6:
			   m_DataListAll.ShowWindow(SW_SHOW);
			   m_staticG.ShowWindow(SW_SHOW);
			   m_btnAddG.ShowWindow(SW_SHOW);
			   m_btnDeleteG.ShowWindow(SW_SHOW);

			   m_ListLocal.ShowWindow(SW_HIDE);
			   m_staticL.ShowWindow(SW_HIDE);
			   m_btnAddL.ShowWindow(SW_HIDE);
			   m_btnDeleteL.ShowWindow(SW_HIDE);

			   reShowListGlobal();

			   break;
		   default:
			   m_DataListAll.ShowWindow(SW_HIDE);
			   m_staticG.ShowWindow(SW_HIDE);
			   m_btnAddG.ShowWindow(SW_HIDE);
			   m_btnDeleteG.ShowWindow(SW_HIDE);

			   m_ListLocal.ShowWindow(SW_SHOW);
			   m_staticL.ShowWindow(SW_SHOW);
			   m_btnAddL.ShowWindow(SW_SHOW);
			   m_btnDeleteL.ShowWindow(SW_SHOW);

			   vector<CPlace*>::iterator iterP = ((CHSTPN_SYSView*)(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1)))->GetDocument()->Vector_Place.begin();
			   vector<CPlace*>::iterator iterPend = ((CHSTPN_SYSView*)(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1)))->GetDocument()->Vector_Place.end();
			   for(;iterP!=iterPend;iterP++)
			   {
				   if((*iterP)->m_caption == str_nodeText)
				   {
					   m_staticL.SetWindowText(_T("局部变量表--" + str_nodeText));
					   p_Model = (*iterP);
				   }
			   }
			   m_edtvaluename1.ShowWindow(SW_HIDE);
			   m_edtValueInt1.ShowWindow(SW_HIDE);

			   m_edtValueNameL.ShowWindow(SW_HIDE);
			   m_cmbSquenceL.ShowWindow(SW_HIDE);
			   m_cmbIOtypeL.ShowWindow(SW_HIDE);
			   m_cmbIsQuoteG_L.ShowWindow(SW_HIDE);
			   m_cmbGlobalNameL.ShowWindow(SW_HIDE);
			   m_cmbDataSourceL.ShowWindow(SW_HIDE);
			   m_cmbSourceValueL.ShowWindow(SW_HIDE);
			   m_edtValueIntL.ShowWindow(SW_HIDE);

			   // 每次点击“库所”节点后，局部变量表应该重新显示变量
			   reShowListLocal();

			   break;

		   }
	   }


  }

  // 重新显示局部变量表
  void CDlgModelParmList::reShowListLocal()
  {
	  m_ListLocal.DeleteAllItems();
	  if(p_Model->arryIOputData.size() != 0)
	  {
		  vector<IOValue*>::iterator iter = p_Model->arryIOputData.begin();
		  int countRow = 0;
		  for(;iter!=p_Model->arryIOputData.end();iter++)
		  {
			  m_ListLocal.InsertItem(countRow, _T(""));
			  m_ListLocal.SetItemText(countRow, 0, (*iter)->Name);
			  CString strsquence;
			  strsquence.Format(_T("%d"),(*iter)->Sequence);
			  m_ListLocal.SetItemText(countRow, 1, strsquence);
			  m_ListLocal.SetItemText(countRow, 2, (*iter)->IOType);
			  CString strisquote;
			  if((*iter)->isQuoteG) 
				  strisquote = _T("true");
			  else
				  strisquote = _T("false");
			  m_ListLocal.SetItemText(countRow, 3, strisquote);
			  m_ListLocal.SetItemText(countRow, 4, (*iter)->GlobalVluName);
			  m_ListLocal.SetItemText(countRow, 5, (*iter)->FromModel);
			  m_ListLocal.SetItemText(countRow, 6, (*iter)->FromMDVluName);
			  CString strint;
			  strint.Format(_T("%.4f"),(*iter)->InitValue);
			  m_ListLocal.SetItemText(countRow, 7, strint);
			  countRow++;
		  }
	  }
	  else
		  return;
  }

   // 重新显示全局变量表
  void CDlgModelParmList::reShowListGlobal()
  {
	  this->m_DataListAll.DeleteAllItems();
	  if(p_Doc->arryIOputDataG.size()!=0)
	  {
		  int rowcount = 0;
		  for(vector<IOValue*>::iterator iterg=p_Doc->arryIOputDataG.begin();iterg!=p_Doc->arryIOputDataG.end();iterg++)
		  {
			  this->m_DataListAll.InsertItem(rowcount,_T(""));
			  this->m_DataListAll.SetItemText(rowcount,0,(*iterg)->Name);
			  this->m_DataListAll.SetItemText(rowcount,1,_T("output"));
			  CString strint;
			  strint.Format(_T("%.4f"),(*iterg)->InitValue);
			  this->m_DataListAll.SetItemText(rowcount,2,strint);
			  rowcount++;
		  }
	  }
	  else
		  return;
  }
  // 单击局部变量表
  void CDlgModelParmList::OnNMClick_ListLocal(NMHDR *pNMHDR, LRESULT *pResult)
  {
	  LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	  // TODO: 在此添加控件通知处理程序代码
	  NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	  // TODO: 在此添加控件通知处理程序代码
	  CRect rc;
      if(pNMListView->iItem==-1)
	  {
	 	  return;
	  }
      m_rowforDL    = pNMListView->iItem;                  // m_row为被选中行的行序号（int类型成员变量）
      m_columnforDL = pNMListView->iSubItem;               // m_column为被选中行的列序号（int类型成员变量）

	  m_edtValueNameL.ShowWindow(SW_HIDE);
	  m_cmbSquenceL.ShowWindow(SW_HIDE);
	  m_cmbIOtypeL.ShowWindow(SW_HIDE);
	  m_cmbIsQuoteG_L.ShowWindow(SW_HIDE);
	  m_cmbGlobalNameL.ShowWindow(SW_HIDE);
	  m_cmbDataSourceL.ShowWindow(SW_HIDE);
	  m_cmbSourceValueL.ShowWindow(SW_HIDE);
	  m_edtValueIntL.ShowWindow(SW_HIDE);

	  *pResult = 0;
  }

  // 双击局部变量表
  void CDlgModelParmList::OnNMDblclk_ListLocal(NMHDR *pNMHDR, LRESULT *pResult)
  {
	  LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	  // TODO: 在此添加控件通知处理程序代码
	  m_edtValueNameL.SetParent(this);
	  m_cmbSquenceL.SetParent(this);
	  m_cmbIOtypeL.SetParent(this);
	  m_cmbIsQuoteG_L.SetParent(this);
	  m_cmbGlobalNameL.SetParent(this);
	  m_cmbDataSourceL.SetParent(this);
	  m_cmbSourceValueL.SetParent(this);
	  m_edtValueIntL.SetParent(this);

	  NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	  CRect rc;
      if(pNMListView->iItem==-1)
	  {
		  return;
	  }

      m_rowL    = pNMListView->iItem;                                                       // m_row为被选中行的行序号（int类型成员变量）
      m_columnL = pNMListView->iSubItem;                                                    // m_column为被选中行的列序号（int类型成员变量）

//	  UpdateData(TRUE);
	  m_ListLocal.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem,LVIR_LABEL,rc); // 获取被点击子项的矩形位置和大小
	
	  InitCtrl_GetIOL(m_rowL, m_columnL, rc);                                                  // 根据双击的行和列，显示相应控件到单元格位置,其他控件隐藏

	  *pResult = 0;
  }

  // 局部变量表下，点击添加按钮
  void CDlgModelParmList::OnBtnaAddValueL()
  {
	  // TODO: 在此添加控件通知处理程序代码
	  if(p_Doc==NULL)
	  {
		  p_Doc = (CHSTPN_SYSDoc*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	  }

	  int n = p_Model->arryIOputData.size();// 获取已有变量的个数
	  m_ListLocal.InsertItem(n,_T(""));
	  CString str1;
	  str1.Format(_T("%d"),n);
	  m_ListLocal.SetItemText(n,0,_T("name") + str1);
	  m_ListLocal.SetItemText(n,1,_T("0"));
	  m_ListLocal.SetItemText(n,2,_T("output"));
	  m_ListLocal.SetItemText(n,3,_T("false"));
	  m_ListLocal.SetItemText(n,4,_T("无"));
	  m_ListLocal.SetItemText(n,5,_T("使能标识"));
	  m_ListLocal.SetItemText(n,6,_T("无需设置"));
	  m_ListLocal.SetItemText(n,7,_T("0.0"));
	 
	  IOValue *value = new IOValue();
	  value->Name = _T("name") + str1;
	  value->Sequence = 0;
	  value->IOType = _T("output");
	  value->isQuoteG = false;
	  value->GlobalVluName = _T("无");
	  value->FromModel = _T("使能标识");
	  value->FromMDVluName = _T("无需设置");
	  value->Value = 0.0;
	  value->InitValue = 0.0;

	  p_Model->arryIOputData.push_back(value);
  }

  // 局部变量表下，点击删除按钮
  void CDlgModelParmList::OnBtnDltValueL()
  {
	  // TODO: 在此添加控件通知处理程序代码
	  if(p_Doc==NULL)
	  {
		  p_Doc = (CHSTPN_SYSDoc*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	  }

	  if(m_rowforDL != -1)
	  {
		  vector<IOValue*>::iterator iter = p_Model->arryIOputData.begin() + m_rowforDL;
		  delete (*iter);
		  p_Model->arryIOputData.erase(iter);
		
		  m_ListLocal.DeleteItem(m_rowforDL);// 更新列表的显示

		  m_rowforDL = -1;
	  }
  }

  void CDlgModelParmList::InitCtrl_GetIOL(int rowofchart, int columnofchart, CRect rc)
  {
	  if(columnofchart==0)
	  {
		  m_edtValueNameL.SetParent(&m_ListLocal);
		  m_edtValueNameL.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//将该子项中的值放在Edit控件中； 
		  m_edtValueNameL.ShowWindow(SW_SHOW);
		  m_edtValueNameL.MoveWindow(&rc);
		  m_edtValueNameL.SetFocus();
		  m_edtValueNameL.ShowCaret();//显示光标 
		  m_edtValueNameL.SetSel(-1);

		  m_cmbSquenceL.ShowWindow(SW_HIDE);
	      m_cmbIOtypeL.ShowWindow(SW_HIDE);
	      m_cmbIsQuoteG_L.ShowWindow(SW_HIDE);
	      m_cmbGlobalNameL.ShowWindow(SW_HIDE);
	      m_cmbDataSourceL.ShowWindow(SW_HIDE);
	      m_cmbSourceValueL.ShowWindow(SW_HIDE);
	      m_edtValueIntL.ShowWindow(SW_HIDE);
	  }
	  if(columnofchart==1)
	  {
		  m_cmbSquenceL.SetParent(&m_ListLocal);
//		  m_cmbSquenceL.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//将该子项中的值放在Edit控件中； 
		  m_cmbSquenceL.ShowWindow(SW_SHOW);
		  m_cmbSquenceL.MoveWindow(&rc);
		  m_cmbSquenceL.SetFocus();

		  m_edtValueNameL.ShowWindow(SW_HIDE);
		  //m_cmbSquenceL.ShowWindow(SW_HIDE);
	      m_cmbIOtypeL.ShowWindow(SW_HIDE);
	      m_cmbIsQuoteG_L.ShowWindow(SW_HIDE);
	      m_cmbGlobalNameL.ShowWindow(SW_HIDE);
	      m_cmbDataSourceL.ShowWindow(SW_HIDE);
	      m_cmbSourceValueL.ShowWindow(SW_HIDE);
	      m_edtValueIntL.ShowWindow(SW_HIDE);

		  m_cmbSquenceL.ResetContent();
		  CString str_count;
		  int count = p_Model->arryIOputData.size()+1;
		  switch(p_Model->PlaceStyle)
		  {
		  case STYLE_DISCRETE:
		  case STYLE_TIME:
		  case STYLE_TCTRL:
		  case STYLE_LCTRL:
			  m_cmbSquenceL.InsertString(0,_T("0"));
			  break;
		  case STYLE_CNTNUS:
		  case STYLE_RAND:
		  case STYLE_DISIC:
			  for(int i=0;i<count;i++)
			  {
				  str_count.Format(L"%d",i);
				  m_cmbSquenceL.InsertString(i,str_count);
			  }
			  break;
		  default:
			  break;
		  }

		  m_cmbSquenceL.SetCurSel(0);

	  }
	  if(columnofchart==2)
	  {
		  m_cmbIOtypeL.SetParent(&m_ListLocal);
//		  m_cmbIOtypeL.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//将该子项中的值放在Edit控件中； 
		  m_cmbIOtypeL.ShowWindow(SW_SHOW);
		  m_cmbIOtypeL.MoveWindow(&rc);
		  m_cmbIOtypeL.SetFocus();

		  m_edtValueNameL.ShowWindow(SW_HIDE);
		  m_cmbSquenceL.ShowWindow(SW_HIDE);
	      //m_cmbIOtypeL.ShowWindow(SW_HIDE);
	      m_cmbIsQuoteG_L.ShowWindow(SW_HIDE);
	      m_cmbGlobalNameL.ShowWindow(SW_HIDE);
	      m_cmbDataSourceL.ShowWindow(SW_HIDE);
	      m_cmbSourceValueL.ShowWindow(SW_HIDE);
	      m_edtValueIntL.ShowWindow(SW_HIDE);

		  m_cmbIOtypeL.ResetContent();
		  switch(p_Model->PlaceStyle)
		  {
		  case STYLE_DISCRETE:
		  case STYLE_TIME:
		  case STYLE_TCTRL:
		  case STYLE_LCTRL:
			  m_cmbIOtypeL.InsertString(0,_T("input"));
			  m_cmbIOtypeL.InsertString(1,_T("output"));
			  break;
		  case STYLE_CNTNUS:
		  case STYLE_RAND:
		  case STYLE_DISIC:
			  m_cmbIOtypeL.InsertString(0,_T("input"));
			  m_cmbIOtypeL.InsertString(1,_T("output"));
			  m_cmbIOtypeL.InsertString(2,_T("in/output"));
			  break;
		  default:
			  break;
		  }
		  m_cmbIOtypeL.SetCurSel(0);

	  }
	  if(columnofchart==3)
	  {
		  m_cmbIsQuoteG_L.SetParent(&m_ListLocal);
//		  m_cmbIsQuoteG_L.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//将该子项中的值放在Edit控件中； 
		  m_cmbIsQuoteG_L.ShowWindow(SW_SHOW);
		  m_cmbIsQuoteG_L.MoveWindow(&rc);
		  m_cmbIsQuoteG_L.SetFocus();

		  m_edtValueNameL.ShowWindow(SW_HIDE);
		  m_cmbSquenceL.ShowWindow(SW_HIDE);
	      m_cmbIOtypeL.ShowWindow(SW_HIDE);
	      //m_cmbIsQuoteG_L.ShowWindow(SW_HIDE);
	      m_cmbGlobalNameL.ShowWindow(SW_HIDE);
	      m_cmbDataSourceL.ShowWindow(SW_HIDE);
	      m_cmbSourceValueL.ShowWindow(SW_HIDE);
	      m_edtValueIntL.ShowWindow(SW_HIDE);
	  }
	  if(columnofchart==4)
	  {
		  m_cmbGlobalNameL.SetParent(&m_ListLocal);
//		  m_cmbGlobalNameL.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//将该子项中的值放在Edit控件中； 
		  m_cmbGlobalNameL.ShowWindow(SW_SHOW);
		  m_cmbGlobalNameL.MoveWindow(&rc);
		  m_cmbGlobalNameL.SetFocus();

		  m_edtValueNameL.ShowWindow(SW_HIDE);
		  m_cmbSquenceL.ShowWindow(SW_HIDE);
	      m_cmbIOtypeL.ShowWindow(SW_HIDE);
	      m_cmbIsQuoteG_L.ShowWindow(SW_HIDE);
	      //m_cmbGlobalNameL.ShowWindow(SW_HIDE);
	      m_cmbDataSourceL.ShowWindow(SW_HIDE);
	      m_cmbSourceValueL.ShowWindow(SW_HIDE);
	      m_edtValueIntL.ShowWindow(SW_HIDE);

		  // 读全局变量表，初始化全局变量一栏的下拉列表框
		  m_cmbGlobalNameL.ResetContent();
		  vector<IOValue*>::iterator iter = p_Doc->arryIOputDataG.begin();
		  int index = 0;
		  m_cmbGlobalNameL.InsertString(0,_T("无"));

		  if(p_Model->arryIOputData.at(rowofchart)->isQuoteG)
		  {
			  for(;iter!=p_Doc->arryIOputDataG.end();iter++)
			  {
				  index++;
				  m_cmbGlobalNameL.InsertString(index, (*iter)->Name);
			  }
		  }
		  m_cmbGlobalNameL.SetCurSel(0);

	  }
	  if(columnofchart==5)
	  {
		  m_cmbDataSourceL.SetParent(&m_ListLocal);
//		  m_cmbDataSourceL.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//将该子项中的值放在Edit控件中； 
		  m_cmbDataSourceL.ShowWindow(SW_SHOW);
		  m_cmbDataSourceL.MoveWindow(&rc);
		  m_cmbDataSourceL.SetFocus();

		  m_edtValueNameL.ShowWindow(SW_HIDE);
		  m_cmbSquenceL.ShowWindow(SW_HIDE);
	      m_cmbIOtypeL.ShowWindow(SW_HIDE);
	      m_cmbIsQuoteG_L.ShowWindow(SW_HIDE);
	      m_cmbGlobalNameL.ShowWindow(SW_HIDE);
	      //m_cmbDataSourceL.ShowWindow(SW_HIDE);
	      m_cmbSourceValueL.ShowWindow(SW_HIDE);
	      m_edtValueIntL.ShowWindow(SW_HIDE);

		  if(p_Model->arryIOputData.at(rowofchart)->IOType == "input")
		  {
			  int i = 0;
			  m_cmbDataSourceL.ResetContent();
			  for(vector<CPlace*>::iterator iter=p_Doc->Vector_Place.begin();iter!=p_Doc->Vector_Place.end();iter++)
			  {
				  if((*iter)->m_caption != p_Model->m_caption)
				  {
					  CString str = (**iter).m_caption;             //获取已创建的库所的标题
					  m_cmbDataSourceL.InsertString(i,str);
					  i++;
				  }
			  }
			  m_cmbDataSourceL.SetCurSel(0);
		  }

		  if(p_Model->arryIOputData.at(rowofchart)->IOType == "output")
		  {
			  m_cmbDataSourceL.ResetContent();//清空所有项
	/*		  CString strofvalue[56] = {L"布尔标识",L"仿真步长",L"参数1",L"参数2",L"参数3",L"参数4",L"参数5",L"参数6",L"参数7",L"参数8",L"参数9",L"参数10",
					                                                           L"参数11",L"参数12",L"参数13",L"参数14",L"参数15",L"参数16",L"参数17",L"参数18",L"参数19",L"参数20",
																			   L"参数21",L"参数22",L"参数23",L"参数24",L"参数25",L"参数26",L"参数27",L"参数28",L"参数29",L"参数30",
																			   L"参数31",L"参数32",L"参数33",L"参数34",L"参数35",L"参数36",L"参数37",L"参数38",L"参数39",L"参数40",
																			   L"参数41",L"参数42",L"参数43",L"参数44",L"参数45",L"参数46",L"参数47",L"参数48",L"参数49",L"参数50",
																			   L"激活标识",L"激活标识（取反）",L"使能标识",L"使能标识（取反）"};
			  CString strofvalueforRand[56] = {L"布尔标识",L"概率阈值",L"参数1",L"参数2",L"参数3",L"参数4",L"参数5",L"参数6",L"参数7",L"参数8",L"参数9",L"参数10",
					                                                           L"参数11",L"参数12",L"参数13",L"参数14",L"参数15",L"参数16",L"参数17",L"参数18",L"参数19",L"参数20",
																			   L"参数21",L"参数22",L"参数23",L"参数24",L"参数25",L"参数26",L"参数27",L"参数28",L"参数29",L"参数30",
																			   L"参数31",L"参数32",L"参数33",L"参数34",L"参数35",L"参数36",L"参数37",L"参数38",L"参数39",L"参数40",
																			   L"参数41",L"参数42",L"参数43",L"参数44",L"参数45",L"参数46",L"参数47",L"参数48",L"参数49",L"参数50",
																			   L"激活标识",L"激活标识（取反）",L"使能标识",L"使能标识（取反）"};
	*/
	/*		  int iofvalue = 0;*/
			  int iofvalueforRand = 0;
	
			  switch(p_Model->PlaceStyle)
			  {
			  case STYLE_DISCRETE:
				  m_cmbDataSourceL.InsertString(0,L"使能标识");
				  m_cmbDataSourceL.InsertString(1,L"使能标识（取反）");
				  break;
			  case STYLE_CNTNUS:
			/*	  for(vector<IOValue*>::iterator iter = p_Model->arryIOputData.begin();iter!=p_Model->arryIOputData.end();iter++)
				  {
					  if((**iter).IOType == "output" && ((**iter).FromModel=="布尔标识" || (**iter).FromModel=="仿真步长"  
						  || (**iter).FromModel=="参数1" || (**iter).FromModel=="参数2" || (**iter).FromModel=="参数3" || (**iter).FromModel=="参数4" 
						  || (**iter).FromModel=="参数5" || (**iter).FromModel=="参数6" || (**iter).FromModel=="参数7" || (**iter).FromModel=="参数8" 
						  || (**iter).FromModel=="参数9" || (**iter).FromModel=="参数10"|| (**iter).FromModel=="参数11" || (**iter).FromModel=="参数12" 
						  || (**iter).FromModel=="参数13" || (**iter).FromModel=="参数14" || (**iter).FromModel=="参数15" || (**iter).FromModel=="参数16" 
						  || (**iter).FromModel=="参数17" || (**iter).FromModel=="参数18"|| (**iter).FromModel=="参数19" || (**iter).FromModel=="参数20"
						  || (**iter).FromModel=="参数21" || (**iter).FromModel=="参数22"|| (**iter).FromModel=="参数23" || (**iter).FromModel=="参数24"
						  || (**iter).FromModel=="参数25" || (**iter).FromModel=="参数26"|| (**iter).FromModel=="参数27" || (**iter).FromModel=="参数28"
						  || (**iter).FromModel=="参数29" || (**iter).FromModel=="参数30"|| (**iter).FromModel=="参数31" || (**iter).FromModel=="参数32"
						  || (**iter).FromModel=="参数33" || (**iter).FromModel=="参数34"|| (**iter).FromModel=="参数35" || (**iter).FromModel=="参数36"
						  || (**iter).FromModel=="参数37" || (**iter).FromModel=="参数38"|| (**iter).FromModel=="参数39" || (**iter).FromModel=="参数40"
						  || (**iter).FromModel=="参数41" || (**iter).FromModel=="参数42"|| (**iter).FromModel=="参数43" || (**iter).FromModel=="参数44"
						  || (**iter).FromModel=="参数45" || (**iter).FromModel=="参数46"|| (**iter).FromModel=="参数47" || (**iter).FromModel=="参数48"
						  || (**iter).FromModel=="参数49" || (**iter).FromModel=="参数50"
						  ))
					  {
						  m_cmbDataSourceL.InsertString(iofvalue,strofvalue[iofvalue]);
						  iofvalue++;
					  }
					}
			*/
				  m_cmbDataSourceL.InsertString(0,_T("使能标识"));
				  m_cmbDataSourceL.InsertString(1,_T("使能标识（取反）"));
				  break;
			  case STYLE_DISIC:
				  m_cmbDataSourceL.InsertString(0,_T("使能标识"));
				  m_cmbDataSourceL.InsertString(1,_T("使能标识（取反）"));
				  break;
			  case STYLE_RAND:
			/*	  for(vector<IOValue*>::iterator iter = p_Model->arryIOputData.begin();iter!=p_Model->arryIOputData.end();iter++)
				  {
					  if((**iter).IOType == "output" && ((**iter).FromModel=="布尔标识" || (**iter).FromModel=="概率阈值"  
						  || (**iter).FromModel=="参数1" || (**iter).FromModel=="参数2" || (**iter).FromModel=="参数3" || (**iter).FromModel=="参数4" 
						  || (**iter).FromModel=="参数5" || (**iter).FromModel=="参数6" || (**iter).FromModel=="参数7" || (**iter).FromModel=="参数8" 
						  || (**iter).FromModel=="参数9" || (**iter).FromModel=="参数10"|| (**iter).FromModel=="参数11" || (**iter).FromModel=="参数12" 
						  || (**iter).FromModel=="参数13" || (**iter).FromModel=="参数14" || (**iter).FromModel=="参数15" || (**iter).FromModel=="参数16" 
						  || (**iter).FromModel=="参数17" || (**iter).FromModel=="参数18"|| (**iter).FromModel=="参数19" || (**iter).FromModel=="参数20"
						  || (**iter).FromModel=="参数21" || (**iter).FromModel=="参数22"|| (**iter).FromModel=="参数23" || (**iter).FromModel=="参数24"
						  || (**iter).FromModel=="参数25" || (**iter).FromModel=="参数26"|| (**iter).FromModel=="参数27" || (**iter).FromModel=="参数28"
						  || (**iter).FromModel=="参数29" || (**iter).FromModel=="参数30"|| (**iter).FromModel=="参数31" || (**iter).FromModel=="参数32"
						  || (**iter).FromModel=="参数33" || (**iter).FromModel=="参数34"|| (**iter).FromModel=="参数35" || (**iter).FromModel=="参数36"
						  || (**iter).FromModel=="参数37" || (**iter).FromModel=="参数38"|| (**iter).FromModel=="参数39" || (**iter).FromModel=="参数40"
						  || (**iter).FromModel=="参数41" || (**iter).FromModel=="参数42"|| (**iter).FromModel=="参数43" || (**iter).FromModel=="参数44"
						  || (**iter).FromModel=="参数45" || (**iter).FromModel=="参数46"|| (**iter).FromModel=="参数47" || (**iter).FromModel=="参数48"
						  || (**iter).FromModel=="参数49" || (**iter).FromModel=="参数50"
						  ))
					  {
						  m_cmbDataSourceL.InsertString(iofvalueforRand,strofvalueforRand[iofvalueforRand]);
						  iofvalueforRand++;
					  }
					}
				*/
				  m_cmbDataSourceL.InsertString(iofvalueforRand,L"使能标识");
				  m_cmbDataSourceL.InsertString(iofvalueforRand+1,L"使能标识（取反）");
				  m_cmbDataSourceL.InsertString(iofvalueforRand+2,L"随机结果");
				  m_cmbDataSourceL.InsertString(iofvalueforRand+3,L"随机结果（取反）");
				  m_cmbDataSourceL.InsertString(iofvalueforRand+4,L"随机概率值");
				  break;
			  case STYLE_TIME:
				  m_cmbDataSourceL.InsertString(0,L"时间阈值");
				  m_cmbDataSourceL.InsertString(1,L"使能标识");
				  m_cmbDataSourceL.InsertString(2,L"使能标识（取反）");
				  break;
			  case STYLE_TCTRL:
				  m_cmbDataSourceL.InsertString(0,L"使能标识");
				  m_cmbDataSourceL.InsertString(1,L"使能标识（取反）");
				  break;
			  case STYLE_LCTRL:
				  m_cmbDataSourceL.InsertString(0,L"使能标识");
				  m_cmbDataSourceL.InsertString(1,L"使能标识（取反）");
				  break;
			  default:
				  break;
			  }
			  m_cmbDataSourceL.SetCurSel(0);
		  }
		  if(p_Model->arryIOputData.at(rowofchart)->IOType == "in/output")
		  {
			  m_cmbDataSourceL.ResetContent();//清空所有项
			  m_cmbDataSourceL.InsertString(0,p_Model->m_caption);
			  m_cmbDataSourceL.SetCurSel(0);
		  }

	  }
	  if(columnofchart==6)
	  {
		  m_cmbSourceValueL.SetParent(&m_ListLocal);
//		  m_cmbSourceValueL.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//将该子项中的值放在Edit控件中； 
		  m_cmbSourceValueL.ShowWindow(SW_SHOW);
		  m_cmbSourceValueL.MoveWindow(&rc);
		  m_cmbSourceValueL.SetFocus();

		  m_edtValueNameL.ShowWindow(SW_HIDE);
		  m_cmbSquenceL.ShowWindow(SW_HIDE);
	      m_cmbIOtypeL.ShowWindow(SW_HIDE);
	      m_cmbIsQuoteG_L.ShowWindow(SW_HIDE);
	      m_cmbGlobalNameL.ShowWindow(SW_HIDE);
	      m_cmbDataSourceL.ShowWindow(SW_HIDE);
	      //m_cmbSourceValueL.ShowWindow(SW_HIDE);
	      m_edtValueIntL.ShowWindow(SW_HIDE);

		  m_cmbSourceValueL.ResetContent();
		  if(p_Model->arryIOputData.at(rowofchart)->IOType == "input") // 如果数据源是其自身
		  {
			  if(p_Model->arryIOputData.at(rowofchart)->FromModel == p_Model->m_caption)
			  {
				  int n_1 = 0;
				  for(vector<IOValue*>::iterator iter=p_Model->arryIOputData.begin();iter!=p_Model->arryIOputData.end();iter++)
				  {
					  if((*iter)->IOType == "output")
					  {
						  m_cmbSourceValueL.InsertString(n_1,(*iter)->Name);
					  }
				  }
			  }
			  else // 如果数据源不是其自身
			  {
				  for(vector<CPlace*>::iterator iterp=p_Doc->Vector_Place.begin();iterp!=p_Doc->Vector_Place.end();iterp++)
				  {
					  if((*iterp)->m_caption == p_Model->arryIOputData.at(rowofchart)->FromModel)
					  {
						  int n_2 = 0;
						  for(vector<IOValue*>::iterator iter=(*iterp)->arryIOputData.begin();iter!=(*iterp)->arryIOputData.end();iter++)
						  {
							  if(((*iter)->IOType == "output")||((*iter)->IOType == "in/output"))
							  {
								  m_cmbSourceValueL.InsertString(n_2,(*iter)->Name);
								  n_2 ++;
							  }
						  }
					  }
				  }
			  }
		  }
		  
		  if(p_Model->arryIOputData.at(rowofchart)->IOType == "output")
		  {
			  m_cmbSourceValueL.ShowWindow(SW_HIDE);
			  m_ListLocal.SetItemText(rowofchart,columnofchart,_T("无需设置"));
		  }
		  if(p_Model->arryIOputData.at(rowofchart)->IOType == "in/output")
		  {
			  int n_6 = 0;
			  for(vector<IOValue*>::iterator iter=p_Model->arryIOputData.begin();iter!=p_Model->arryIOputData.end();iter++)
			  {
				  if((*iter)->IOType == "in/output")
				  {
					  m_cmbSourceValueL.InsertString(n_6,(*iter)->Name);
					  n_6 ++;
				  }
			  }
		  }
		  m_cmbSourceValueL.SetCurSel(0);


	  }
	  if(columnofchart==7)
	  {
		  m_edtValueIntL.SetParent(&m_ListLocal);
		  m_edtValueIntL.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//将该子项中的值放在Edit控件中； 
		  m_edtValueIntL.ShowWindow(SW_SHOW);
		  m_edtValueIntL.MoveWindow(&rc);
		  m_edtValueIntL.SetFocus();
		  m_edtValueIntL.ShowCaret();//显示光标 
		  m_edtValueIntL.SetSel(-1);

		  m_edtValueNameL.ShowWindow(SW_HIDE);
		  m_cmbSquenceL.ShowWindow(SW_HIDE);
	      m_cmbIOtypeL.ShowWindow(SW_HIDE);
	      m_cmbIsQuoteG_L.ShowWindow(SW_HIDE);
	      m_cmbGlobalNameL.ShowWindow(SW_HIDE);
	      m_cmbDataSourceL.ShowWindow(SW_HIDE);
	      m_cmbSourceValueL.ShowWindow(SW_HIDE);
	      //m_edtValueIntL.ShowWindow(SW_HIDE);
	  }
  }

  // 点击“确定”按钮
  void CDlgModelParmList::OnBtnOK()
  {
	  m_edtvaluename1.SetParent(this);
	  m_edtValueInt1.SetParent(this);

	  m_edtValueNameL.SetParent(this);
	  m_cmbSquenceL.SetParent(this);
	  m_cmbIOtypeL.SetParent(this);
	  m_cmbIsQuoteG_L.SetParent(this);
	  m_cmbGlobalNameL.SetParent(this);
	  m_cmbDataSourceL.SetParent(this);
	  m_cmbSourceValueL.SetParent(this);
	  m_edtValueIntL.SetParent(this);


	  vector<IOValue*> InputofLuaTS;
	  vector<IOValue*> OutputofLuaTS;
	  bool isAllRight = true;
	  for(vector<CPlace*>::iterator iterp=p_Doc->Vector_Place.begin();iterp!=p_Doc->Vector_Place.end();iterp++)
	  {
		  switch((*iterp)->PlaceStyle)
		  {
		  case STYLE_DISCRETE:
		  case STYLE_TIME:
		  case STYLE_TCTRL:
			  (*iterp)->arryInputofLua.clear();
			  (*iterp)->arryOutputofLua.clear();
			  break;
		  case STYLE_CNTNUS:
		  case STYLE_RAND:
		  case STYLE_DISIC:
			  (*iterp)->arryInputofLua.clear();
			  (*iterp)->arryOutputofLua.clear();
			  InputofLuaTS.clear();
			  OutputofLuaTS.clear();

			  for(vector<IOValue*>::iterator iter=(*iterp)->arryIOputData.begin();iter!=(*iterp)->arryIOputData.end();iter++)
			  {
				  if((*iter)->IOType == "input")
				  {
					  if((*iter)->Sequence != 0)
					  {
						  InputofLuaTS.push_back(*iter);
						  (*iterp)->arryInputofLua.push_back(*iter);
					  }
				  }
				  if((*iter)->IOType == "output")
				  {
					  if((*iter)->Sequence != 0)
					  {
						  OutputofLuaTS.push_back(*iter);
						  (*iterp)->arryOutputofLua.push_back(*iter);
					  }
				  }
				  if((*iter)->IOType == "in/output")
				  {
					  if((*iter)->Sequence != 0)
					  {
						  InputofLuaTS.push_back(*iter);
						  OutputofLuaTS.push_back(*iter);
						  (*iterp)->arryInputofLua.push_back(*iter);
						  (*iterp)->arryOutputofLua.push_back(*iter);
					  }
				  }
			  }

			  // 重新排序
			  for(vector<IOValue*>::iterator iter_i=InputofLuaTS.begin();iter_i!=InputofLuaTS.end();iter_i++)
			  {
				//  if((*iter_i)->Sequence != 0)
				  if((*iterp)->arryInputofLua.at((*iter_i)->Sequence-1) != NULL)
					  (*iterp)->arryInputofLua.at((*iter_i)->Sequence-1) = *iter_i;
				  else
					  MessageBox(_T("输入/输出参数设置错误"));
			  }
			  for(vector<IOValue*>::iterator iter_o=OutputofLuaTS.begin();iter_o!=OutputofLuaTS.end();iter_o++)
			  {
				  if((*iterp)->arryOutputofLua.at((*iter_o)->Sequence-1)!=NULL)
					  (*iterp)->arryOutputofLua.at((*iter_o)->Sequence-1) = (*iter_o);
				  else
					  MessageBox(_T("输入/输出参数设置错误"));
			  }
			  break;
		  default:
			  break;
		  }

		  // 输入/输出列表错误检测
		  
		  for(vector<IOValue*>::iterator iter=(*iterp)->arryIOputData.begin();iter!=(*iterp)->arryIOputData.end();iter++)
		  {
			  if((*iter)->isQuoteG && (*iter)->GlobalVluName=="无")
			  {
				  MessageBox((*iterp)->m_caption+_T("_")+(*iter)->Name+_T("_引用全局变量错误！ 请选择要引用的全局变量！"));
				  isAllRight = false;
			  }
		  }
	  }
	  if(isAllRight)
		CDialogEx::OnOK();
  }

  // 局部变量表下，获取变量名
  void CDlgModelParmList::OnGetVluName_L()
  {
	  CString str;
	  m_edtValueNameL.GetWindowText(str);

	  m_ListLocal.SetItemText(m_rowL,m_columnL,str);
	  p_Model->arryIOputData.at(m_rowL)->Name = str;
  }

  // 局部变量表下，获取序号
  void CDlgModelParmList::OnGetSquence_L()
  {
	  int nSel = m_cmbSquenceL.GetCurSel();
	  m_cmbSquenceL.SetCurSel(nSel);
	  CString str;
	  m_cmbSquenceL.GetWindowText(str);
	  m_cmbSquenceL.ShowWindow(SW_HIDE);

	  m_ListLocal.SetItemText(m_rowL,m_columnL,str);
	  if(p_Model->arryIOputData.at(m_rowL) != NULL)
	  {
		  p_Model->arryIOputData.at(m_rowL)->Sequence = _ttoi(str);
	  }
	  else
		  return;
	  
  }

  // 局部变量表下，获取IOType	  //m_cmbIOtypeL
  void CDlgModelParmList::OnGetIOtype_L()
  {
	  int nSel = m_cmbIOtypeL.GetCurSel();
	  m_cmbIOtypeL.SetCurSel(nSel);
	  CString str;
	  switch(nSel)
	  {
	  case 0:
		  str = _T("input");
		  break;
	  case 1:
		  str = _T("output");
		  break;
	  case 2:
		  str = _T("in/output");
		  break;
	  default:
		  break;
	  }
	  m_cmbIOtypeL.ShowWindow(SW_HIDE);
	  m_ListLocal.SetItemText(m_rowL,m_columnL,str);
	  if(p_Model->arryIOputData.at(m_rowL) != NULL)
	  {
		  p_Model->arryIOputData.at(m_rowL)->IOType = str;
	  }
	  else
		  return;
  }

  // 局部变量表下，获取是否引用全局变量//m_cmbIsQuoteG_L
  void CDlgModelParmList::OnGetIsQuote_L()
  {
	  int nSel = m_cmbIsQuoteG_L.GetCurSel();
	  m_cmbIsQuoteG_L.SetCurSel(nSel);
	  CString str;
	  switch(nSel)
	  {
	  case 0:
		  str = _T("true");
		  if(p_Model->arryIOputData.at(m_rowL) != NULL)
		  {
			  p_Model->arryIOputData.at(m_rowL)->isQuoteG = true;
		  }
		  else
			  return;
		  break;
	  case 1:
		  str = _T("false");
		  if(p_Model->arryIOputData.at(m_rowL) != NULL)
		  {
			  p_Model->arryIOputData.at(m_rowL)->isQuoteG = false;
		  }
		  else
			  return;
		  break;
	  default:
		  break;
	  }
	  m_cmbIsQuoteG_L.ShowWindow(SW_HIDE);
	  m_ListLocal.SetItemText(m_rowL,m_columnL,str);

  }

  // 局部变量表下，获取所引用的全局变量//m_cmbGlobalNameL
  void CDlgModelParmList::OnGetGlobalVlu_L()
  {
	  int nSel = m_cmbGlobalNameL.GetCurSel();
	  m_cmbGlobalNameL.SetCurSel(nSel);
	  CString str;
	  m_cmbGlobalNameL.GetWindowText(str);
	  m_cmbGlobalNameL.ShowWindow(SW_HIDE);
	  if(p_Model->arryIOputData.at(m_rowL) != NULL)
	  {
		  p_Model->arryIOputData.at(m_rowL)->GlobalVluName = str;
	  }
	  else
		  return;
	  m_ListLocal.SetItemText(m_rowL,m_columnL,str);
	
  }

  // 局部变量表下，获取数据源//m_cmbDataSourceL
  void CDlgModelParmList::OnGetDataSource_L()
  {
	  int nSel = m_cmbDataSourceL.GetCurSel();
	  m_cmbDataSourceL.SetCurSel(nSel);
	  CString str;
	  m_cmbDataSourceL.GetWindowText(str);
	  m_cmbDataSourceL.ShowWindow(SW_HIDE);
	  if(p_Model->arryIOputData.at(m_rowL) != NULL)
	  {
		  p_Model->arryIOputData.at(m_rowL)->FromModel = str;
	  }
	  else
		  return;
	  m_ListLocal.SetItemText(m_rowL,m_columnL,str);
	
  }

  // 局部变量表下，获取数据源变量//m_cmbSourceValueL
  void CDlgModelParmList::OnGetDataSourceValue_L()
  {
	  int nSel = m_cmbSourceValueL.GetCurSel();
	  m_cmbSourceValueL.SetCurSel(nSel);
	  CString str;
	  m_cmbSourceValueL.GetWindowText(str);
	  m_cmbSourceValueL.ShowWindow(SW_HIDE);
	  if(p_Model->arryIOputData.at(m_rowL) != NULL)
	  {
		  p_Model->arryIOputData.at(m_rowL)->FromMDVluName = str;
	  }
	  else
		  return;
	  m_ListLocal.SetItemText(m_rowL,m_columnL,str);
  }

  // 局部变量表下，获取数据的初始值
  void CDlgModelParmList::OnGetValueInt_L()
  {
	  CString strvalueint;
	  m_edtValueIntL.GetWindowText(strvalueint);
	  double intvalue = _ttof(strvalueint);

	  m_ListLocal.SetItemText(m_rowL,m_columnL,strvalueint);
	  p_Model->arryIOputData.at(m_rowL)->Value = intvalue;
	  p_Model->arryIOputData.at(m_rowL)->InitValue = intvalue;
  }


  void CDlgModelParmList::OnPaint()
  {
	  CPaintDC dc(this); // device context for painting
	  // TODO: 在此处添加消息处理程序代码
	  // 不为绘图消息调用 CDialogEx::OnPaint()
	 if(p_Doc==NULL)
	 {
		 p_Doc = (CHSTPN_SYSDoc*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	 }
  }
