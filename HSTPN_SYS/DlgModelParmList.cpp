// DlgModelParmList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgModelParmList.h"
#include "afxdialogex.h"


// CDlgModelParmList �Ի���

IMPLEMENT_DYNAMIC(CDlgModelParmList, CDialogEx)

/*BEGIN_MESSAGE_MAP(CDlgModelParmList, CDialogEx)
	// ��׼��ӡ����
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


// CDlgModelParmList ��Ϣ�����
 // �����º���
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
	
	
	//--�������ṹ------------------------------------------------------------------
	// ������ṹ������
	m_treeListofModel.DeleteAllItems();

	HTREEITEM hChild;                                                            // �����ӽڵ�ľ��--һ���ӽڵ�
	HTREEITEM hChild1;                                                            // �����ӽڵ�ľ��--�����ӽڵ�
	m_treeListofModel.SetImageList(&m_imageList, TVSIL_NORMAL);                      // Ϊ���οؼ�����ͼ������
	
	hRoot = m_treeListofModel.InsertItem(_T("HSTPNģ��"), 0, 0);                 // ������ڵ�
	m_treeListofModel.SetItemData(hRoot,1);                                          // Ϊ�ڵ�������ݣ������������Ϣ��Ӧ�������ҵ��ڵ�
	// ��ʾ���ÿ����ڵ�
	if(arrycptnDiscPlace.size()!=0)
	{
		CString str_place1size;
		str_place1size.Format(L"%d",arrycptnDiscPlace.size());
		str_place1size = _T("(")+str_place1size + _T(")");
		hChild = m_treeListofModel.InsertItem(_T("��ɢ����")+str_place1size, 1, 1, hRoot, TVI_LAST);
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
		hChild = m_treeListofModel.InsertItem(_T("��������")+str_place2size, 2, 2, hRoot, TVI_LAST);
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
		hChild = m_treeListofModel.InsertItem(_T("�������")+str_place3size, 3, 3, hRoot, TVI_LAST);
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
		hChild = m_treeListofModel.InsertItem(_T("ʱ�ӿ���")+str_place4size, 4, 4, hRoot, TVI_LAST);
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
		hChild = m_treeListofModel.InsertItem(_T("���ƿ���")+str_place5size, 5, 5, hRoot, TVI_LAST);
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
		hChild = m_treeListofModel.InsertItem(_T("���߿���")+str_place6size, 7, 7, hRoot, TVI_LAST);
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

	// ��ʼ��ģ����
	if(!b_isInitTree)
	{
		HICON hIcon[8];                                                             // ͼ��������   
		   
		hIcon[0] = theApp.LoadIcon(IDI_ICON11);                                       // ����ͼ�꣬�������ǵľ�����浽����  
		hIcon[1] = theApp.LoadIcon(IDI_ICON4);
		hIcon[2] = theApp.LoadIcon(IDI_ICON5);
		hIcon[3] = theApp.LoadIcon(IDI_ICON6);
		hIcon[4] = theApp.LoadIcon(IDI_ICON7);
		hIcon[5] = theApp.LoadIcon(IDI_ICON8);
		hIcon[6] = theApp.LoadIcon(IDI_ICON9);
		hIcon[7] = theApp.LoadIcon(IDI_ICON12);

		m_imageList.DeleteImageList();                                               // �������
		  
		m_imageList.Create(32, 32, ILC_COLOR32, 8, 2);                              // ����ͼ������CImageList���� ���У�10��ʾ�洢�ĸ���Ϊ10��3��һ����չ��С���������Ҫ�棬����������չ����Ҳֻ���ٴ�����    
		   
		for (int i=0; i<8; i++)                                                     // ��ͼ����ӵ�ͼ������
		{   
			m_imageList.Add(hIcon[i]);   
		}   
		// ����ͼ�꣬�������ǵľ�����浽���� 
		m_treeListofModel.SetImageList(&m_imageList, TVSIL_NORMAL);                      // Ϊ���οؼ�����ͼ������
		//-----------------------------------------------------------------------------------
		hRoot = m_treeListofModel.InsertItem(_T("HSTPNģ��"), 0, 0);                 // ������ڵ�
		m_treeListofModel.SetItemData(hRoot,1);                                          // Ϊ�ڵ�������ݣ������������Ϣ��Ӧ�������ҵ��ڵ�


		b_isInitTree = TRUE;
	}

	 // ��ʼ��ȫ�ֱ�����-----------------------------------------------------
	 m_DataListAll.ModifyStyle(0,LVS_REPORT);               // ����Ϊ����ģʽ
	 m_DataListAll.SetExtendedStyle(m_DataListAll.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_FULLROWSELECT);// �����+��ѡ��

	 m_DataListAll.InsertColumn(0,L"������");               // �����ͷ
	 m_DataListAll.InsertColumn(1,L"I/O����");
	 m_DataListAll.InsertColumn(2,L"��ʼֵ");

	 m_DataListAll.SetBkColor(RGB(255,255,255));

	 CRect rect;  
	 m_DataListAll.GetClientRect(rect);                     //��õ�ǰ�ͻ�����Ϣ   
	 m_DataListAll.SetColumnWidth(0, rect.Width() / 3);     //�����еĿ�ȡ�   
	 m_DataListAll.SetColumnWidth(1, rect.Width() / 3); 
	 m_DataListAll.SetColumnWidth(2, rect.Width() / 3);

	 m_edtvaluename1.ShowWindow(SW_HIDE);
	 m_edtValueInt1.ShowWindow(SW_HIDE);              // ���ÿؼ����ɼ�

	 // ��ʼ���ֲ�������--------------------------------------------------------
	 m_ListLocal.ModifyStyle(0,LVS_REPORT);               // ����Ϊ����ģʽ
	 m_ListLocal.SetExtendedStyle(m_ListLocal.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_FULLROWSELECT);// �����+��ѡ��

	 m_ListLocal.InsertColumn(0,L"������");               // �����ͷ
	 m_ListLocal.InsertColumn(1,L"���(Lua)");
	 m_ListLocal.InsertColumn(2,L"I/O����");
	 m_ListLocal.InsertColumn(3,L"����ȫ��");
	 m_ListLocal.InsertColumn(4,L"ȫ�ֱ���");
	 m_ListLocal.InsertColumn(5,L"����Դ");
	 m_ListLocal.InsertColumn(6,L"����Դ����");
	 m_ListLocal.InsertColumn(7,L"��ʼֵ");

	 m_ListLocal.SetBkColor(RGB(255,255,255));

	 CRect rectL;  
	 m_ListLocal.GetClientRect(rectL);                     //��õ�ǰ�ͻ�����Ϣ   
	 m_ListLocal.SetColumnWidth(0, rectL.Width() / 8);     //�����еĿ�ȡ�   
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
	 
	 m_cmbGlobalNameL.InsertString(0,_T("��"));
	 m_cmbGlobalNameL.SetCurSel(0);

//	 m_cmbDataSourceL.InsertString(0,_T("ʹ�ܱ�ʶ"));
//	 m_cmbDataSourceL.SetCurSel(0);



	 m_DataListAll.ShowWindow(SW_SHOW);
	 m_staticG.ShowWindow(SW_SHOW);
	 m_btnAddG.ShowWindow(SW_SHOW);
	 m_btnDeleteG.ShowWindow(SW_SHOW);

	 m_ListLocal.ShowWindow(SW_HIDE);
	 m_staticL.ShowWindow(SW_HIDE);
	 m_btnAddL.ShowWindow(SW_HIDE);
	 m_btnDeleteL.ShowWindow(SW_HIDE); 

	 // �ٴδ򿪺���ȫ�ֱ�����


	 return TRUE;  // return TRUE unless you set the focus to a control
	 // �쳣: OCX ����ҳӦ���� FALSE
 }

 void CDlgModelParmList::OnSize(UINT nType, int cx, int cy)
 {
	 CDialogEx::OnSize(nType, cx, cy);

	 // TODO: �ڴ˴������Ϣ����������
 }

 // ȫ�ֱ������������ӡ���ť
 void CDlgModelParmList::OnBtnaAddValueG()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 if(p_Doc==NULL)
	 {
		 p_Doc = (CHSTPN_SYSDoc*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	 }
	 int n = p_Doc->arryIOputDataG.size(); // ��ȡ���б����ĸ���
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

 // ȫ�ֱ����������ɾ������ť
 void CDlgModelParmList::OnBtnDltValueG()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 if(p_Doc==NULL)
	 {
		 p_Doc = (CHSTPN_SYSDoc*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	 }

	 if(m_rowforDG != -1)
	 {
		 vector<IOValue*>::iterator iter = p_Doc->arryIOputDataG.begin() + m_rowforDG;
		 delete (*iter);
		 p_Doc->arryIOputDataG.erase(iter);
		
		 m_DataListAll.DeleteItem(m_rowforDG);// �����б����ʾ

		 m_rowforDG = -1;
	 }
 }

 // ����ȫ�ֱ�����
 void CDlgModelParmList::OnClick_ListG(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 CRect rc;
     if(pNMListView->iItem==-1)
	 {
	 	 return;
	 }
     m_rowforDG    = pNMListView->iItem;                  // m_rowΪ��ѡ���е�����ţ�int���ͳ�Ա������
     m_columnforDG = pNMListView->iSubItem;               // m_columnΪ��ѡ���е�����ţ�int���ͳ�Ա������

	 //UpdateData();                                    // ��ɿؼ�����������ݽ������Ӷ���ȡm_editvalue_IOnameֵ��

	 m_edtvaluename1.ShowWindow(SW_HIDE);
	 m_edtValueInt1.ShowWindow(SW_HIDE);              // ���ÿؼ����ɼ�



	 *pResult = 0;
 }

 // ˫��ȫ�ֱ�����
 void CDlgModelParmList::OnDblclk_ListG(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 m_edtvaluename1.SetParent(this);
	 m_edtValueInt1.SetParent(this);

	 NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	 CRect rc;
     if(pNMListView->iItem==-1)
	 {
		 return;
	 }

     m_rowG    = pNMListView->iItem;                                                       // m_rowΪ��ѡ���е�����ţ�int���ͳ�Ա������
     m_columnG = pNMListView->iSubItem;                                                    // m_columnΪ��ѡ���е�����ţ�int���ͳ�Ա������

//	 UpdateData(TRUE);
	 m_DataListAll.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem,LVIR_LABEL,rc); // ��ȡ���������ľ���λ�úʹ�С
	
	 InitCtrl_GetIO(m_rowG, m_columnG, rc);                                                  // ����˫�����к��У���ʾ��Ӧ�ؼ�����Ԫ��λ��,�����ؼ�����

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
		   m_edtvaluename1.SetWindowText(m_DataListAll.GetItemText(rowofchart,columnofchart));//���������е�ֵ����Edit�ؼ��У� 
		   m_edtvaluename1.ShowWindow(SW_SHOW);
		   m_edtvaluename1.MoveWindow(&rc);
		   m_edtvaluename1.SetFocus();
		   m_edtvaluename1.ShowCaret();//��ʾ��� 
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
		   m_edtValueInt1.SetWindowText(m_DataListAll.GetItemText(rowofchart,columnofchart));//���������е�ֵ����Edit�ؼ��У�
		   m_edtValueInt1.ShowWindow(SW_SHOW);
		   m_edtValueInt1.MoveWindow(&rc);
		   m_edtValueInt1.SetFocus();
		   m_edtValueInt1.ShowCaret();
		   m_edtValueInt1.SetSel(-1);
		   m_edtvaluename1.ShowWindow(SW_HIDE);
	   }
  }

  // ��ȡȫ�ֱ���Name
  void CDlgModelParmList::OnGetValueNameG()
  {
	  // TODO:  �ڴ���ӿؼ�֪ͨ����������
	  m_edtvaluename1.GetWindowText(m_strvaluename1);

	  m_DataListAll.SetItemText(m_rowG,m_columnG,m_strvaluename1);
	  p_Doc->arryIOputDataG.at(m_rowG)->Name = m_strvaluename1;
  }

  // ��ȡȫ�ֱ���IntValue
  void CDlgModelParmList::OnGetValueIntG()
  {
	  // TODO:  �ڴ���ӿؼ�֪ͨ����������
	  CString strvalueint;
	  m_edtValueInt1.GetWindowText(strvalueint);
	  m_dblValueInt = _ttof(strvalueint);

	  m_DataListAll.SetItemText(m_rowG,m_columnG,strvalueint);
	  p_Doc->arryIOputDataG.at(m_rowG)->Value = m_dblValueInt;
	  p_Doc->arryIOputDataG.at(m_rowG)->InitValue = m_dblValueInt;
  }

  // �����ģ����ʱ����Ӧ
  void CDlgModelParmList::OnNMClick_ChoseNode(NMHDR *pNMHDR, LRESULT *pResult)
  {
	  // TODO: �ڴ���ӿؼ�֪ͨ����������
	  p_Doc = (CHSTPN_SYSDoc*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();

	  *pResult = 0;

	  NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;     // NMTREEVIEW�ṹ���а��������οؼ�֪ͨ��Ϣ�������Ϣ�����οؼ��Ĵ����֪ͨ��Ϣ�������ָ��ýṹ���ָ�롣
	
	  CPoint p;  
      GetCursorPos(&p);                                    // ��ȡ�������λ�ã��õ�Ϊ��Ļ�����
	  m_treeListofModel.ScreenToClient(&p);

	  UINT nFlag = 0;
	  HTREEITEM h=m_treeListofModel.HitTest(p,&nFlag);     //Returns the current position of the cursor related to the CTreeCtrl object.

	   if((h !=  NULL)&&(TVHT_ONITEM & nFlag))             // �жϵ��λ���Ƿ��ڽڵ�����
	   {
		   m_treeListofModel.SelectItem(h);                // ѡȡָ���ڵ�
		   int nDat = m_treeListofModel.GetItemData(h);    // ��ȡ�ڵ��������
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
					   m_staticL.SetWindowText(_T("�ֲ�������--" + str_nodeText));
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

			   // ÿ�ε�����������ڵ�󣬾ֲ�������Ӧ��������ʾ����
			   reShowListLocal();

			   break;

		   }
	   }


  }

  // ������ʾ�ֲ�������
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

   // ������ʾȫ�ֱ�����
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
  // �����ֲ�������
  void CDlgModelParmList::OnNMClick_ListLocal(NMHDR *pNMHDR, LRESULT *pResult)
  {
	  LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	  // TODO: �ڴ���ӿؼ�֪ͨ����������
	  NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	  // TODO: �ڴ���ӿؼ�֪ͨ����������
	  CRect rc;
      if(pNMListView->iItem==-1)
	  {
	 	  return;
	  }
      m_rowforDL    = pNMListView->iItem;                  // m_rowΪ��ѡ���е�����ţ�int���ͳ�Ա������
      m_columnforDL = pNMListView->iSubItem;               // m_columnΪ��ѡ���е�����ţ�int���ͳ�Ա������

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

  // ˫���ֲ�������
  void CDlgModelParmList::OnNMDblclk_ListLocal(NMHDR *pNMHDR, LRESULT *pResult)
  {
	  LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	  // TODO: �ڴ���ӿؼ�֪ͨ����������
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

      m_rowL    = pNMListView->iItem;                                                       // m_rowΪ��ѡ���е�����ţ�int���ͳ�Ա������
      m_columnL = pNMListView->iSubItem;                                                    // m_columnΪ��ѡ���е�����ţ�int���ͳ�Ա������

//	  UpdateData(TRUE);
	  m_ListLocal.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem,LVIR_LABEL,rc); // ��ȡ���������ľ���λ�úʹ�С
	
	  InitCtrl_GetIOL(m_rowL, m_columnL, rc);                                                  // ����˫�����к��У���ʾ��Ӧ�ؼ�����Ԫ��λ��,�����ؼ�����

	  *pResult = 0;
  }

  // �ֲ��������£������Ӱ�ť
  void CDlgModelParmList::OnBtnaAddValueL()
  {
	  // TODO: �ڴ���ӿؼ�֪ͨ����������
	  if(p_Doc==NULL)
	  {
		  p_Doc = (CHSTPN_SYSDoc*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	  }

	  int n = p_Model->arryIOputData.size();// ��ȡ���б����ĸ���
	  m_ListLocal.InsertItem(n,_T(""));
	  CString str1;
	  str1.Format(_T("%d"),n);
	  m_ListLocal.SetItemText(n,0,_T("name") + str1);
	  m_ListLocal.SetItemText(n,1,_T("0"));
	  m_ListLocal.SetItemText(n,2,_T("output"));
	  m_ListLocal.SetItemText(n,3,_T("false"));
	  m_ListLocal.SetItemText(n,4,_T("��"));
	  m_ListLocal.SetItemText(n,5,_T("ʹ�ܱ�ʶ"));
	  m_ListLocal.SetItemText(n,6,_T("��������"));
	  m_ListLocal.SetItemText(n,7,_T("0.0"));
	 
	  IOValue *value = new IOValue();
	  value->Name = _T("name") + str1;
	  value->Sequence = 0;
	  value->IOType = _T("output");
	  value->isQuoteG = false;
	  value->GlobalVluName = _T("��");
	  value->FromModel = _T("ʹ�ܱ�ʶ");
	  value->FromMDVluName = _T("��������");
	  value->Value = 0.0;
	  value->InitValue = 0.0;

	  p_Model->arryIOputData.push_back(value);
  }

  // �ֲ��������£����ɾ����ť
  void CDlgModelParmList::OnBtnDltValueL()
  {
	  // TODO: �ڴ���ӿؼ�֪ͨ����������
	  if(p_Doc==NULL)
	  {
		  p_Doc = (CHSTPN_SYSDoc*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	  }

	  if(m_rowforDL != -1)
	  {
		  vector<IOValue*>::iterator iter = p_Model->arryIOputData.begin() + m_rowforDL;
		  delete (*iter);
		  p_Model->arryIOputData.erase(iter);
		
		  m_ListLocal.DeleteItem(m_rowforDL);// �����б����ʾ

		  m_rowforDL = -1;
	  }
  }

  void CDlgModelParmList::InitCtrl_GetIOL(int rowofchart, int columnofchart, CRect rc)
  {
	  if(columnofchart==0)
	  {
		  m_edtValueNameL.SetParent(&m_ListLocal);
		  m_edtValueNameL.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//���������е�ֵ����Edit�ؼ��У� 
		  m_edtValueNameL.ShowWindow(SW_SHOW);
		  m_edtValueNameL.MoveWindow(&rc);
		  m_edtValueNameL.SetFocus();
		  m_edtValueNameL.ShowCaret();//��ʾ��� 
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
//		  m_cmbSquenceL.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//���������е�ֵ����Edit�ؼ��У� 
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
//		  m_cmbIOtypeL.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//���������е�ֵ����Edit�ؼ��У� 
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
//		  m_cmbIsQuoteG_L.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//���������е�ֵ����Edit�ؼ��У� 
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
//		  m_cmbGlobalNameL.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//���������е�ֵ����Edit�ؼ��У� 
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

		  // ��ȫ�ֱ�������ʼ��ȫ�ֱ���һ���������б��
		  m_cmbGlobalNameL.ResetContent();
		  vector<IOValue*>::iterator iter = p_Doc->arryIOputDataG.begin();
		  int index = 0;
		  m_cmbGlobalNameL.InsertString(0,_T("��"));

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
//		  m_cmbDataSourceL.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//���������е�ֵ����Edit�ؼ��У� 
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
					  CString str = (**iter).m_caption;             //��ȡ�Ѵ����Ŀ����ı���
					  m_cmbDataSourceL.InsertString(i,str);
					  i++;
				  }
			  }
			  m_cmbDataSourceL.SetCurSel(0);
		  }

		  if(p_Model->arryIOputData.at(rowofchart)->IOType == "output")
		  {
			  m_cmbDataSourceL.ResetContent();//���������
	/*		  CString strofvalue[56] = {L"������ʶ",L"���沽��",L"����1",L"����2",L"����3",L"����4",L"����5",L"����6",L"����7",L"����8",L"����9",L"����10",
					                                                           L"����11",L"����12",L"����13",L"����14",L"����15",L"����16",L"����17",L"����18",L"����19",L"����20",
																			   L"����21",L"����22",L"����23",L"����24",L"����25",L"����26",L"����27",L"����28",L"����29",L"����30",
																			   L"����31",L"����32",L"����33",L"����34",L"����35",L"����36",L"����37",L"����38",L"����39",L"����40",
																			   L"����41",L"����42",L"����43",L"����44",L"����45",L"����46",L"����47",L"����48",L"����49",L"����50",
																			   L"�����ʶ",L"�����ʶ��ȡ����",L"ʹ�ܱ�ʶ",L"ʹ�ܱ�ʶ��ȡ����"};
			  CString strofvalueforRand[56] = {L"������ʶ",L"������ֵ",L"����1",L"����2",L"����3",L"����4",L"����5",L"����6",L"����7",L"����8",L"����9",L"����10",
					                                                           L"����11",L"����12",L"����13",L"����14",L"����15",L"����16",L"����17",L"����18",L"����19",L"����20",
																			   L"����21",L"����22",L"����23",L"����24",L"����25",L"����26",L"����27",L"����28",L"����29",L"����30",
																			   L"����31",L"����32",L"����33",L"����34",L"����35",L"����36",L"����37",L"����38",L"����39",L"����40",
																			   L"����41",L"����42",L"����43",L"����44",L"����45",L"����46",L"����47",L"����48",L"����49",L"����50",
																			   L"�����ʶ",L"�����ʶ��ȡ����",L"ʹ�ܱ�ʶ",L"ʹ�ܱ�ʶ��ȡ����"};
	*/
	/*		  int iofvalue = 0;*/
			  int iofvalueforRand = 0;
	
			  switch(p_Model->PlaceStyle)
			  {
			  case STYLE_DISCRETE:
				  m_cmbDataSourceL.InsertString(0,L"ʹ�ܱ�ʶ");
				  m_cmbDataSourceL.InsertString(1,L"ʹ�ܱ�ʶ��ȡ����");
				  break;
			  case STYLE_CNTNUS:
			/*	  for(vector<IOValue*>::iterator iter = p_Model->arryIOputData.begin();iter!=p_Model->arryIOputData.end();iter++)
				  {
					  if((**iter).IOType == "output" && ((**iter).FromModel=="������ʶ" || (**iter).FromModel=="���沽��"  
						  || (**iter).FromModel=="����1" || (**iter).FromModel=="����2" || (**iter).FromModel=="����3" || (**iter).FromModel=="����4" 
						  || (**iter).FromModel=="����5" || (**iter).FromModel=="����6" || (**iter).FromModel=="����7" || (**iter).FromModel=="����8" 
						  || (**iter).FromModel=="����9" || (**iter).FromModel=="����10"|| (**iter).FromModel=="����11" || (**iter).FromModel=="����12" 
						  || (**iter).FromModel=="����13" || (**iter).FromModel=="����14" || (**iter).FromModel=="����15" || (**iter).FromModel=="����16" 
						  || (**iter).FromModel=="����17" || (**iter).FromModel=="����18"|| (**iter).FromModel=="����19" || (**iter).FromModel=="����20"
						  || (**iter).FromModel=="����21" || (**iter).FromModel=="����22"|| (**iter).FromModel=="����23" || (**iter).FromModel=="����24"
						  || (**iter).FromModel=="����25" || (**iter).FromModel=="����26"|| (**iter).FromModel=="����27" || (**iter).FromModel=="����28"
						  || (**iter).FromModel=="����29" || (**iter).FromModel=="����30"|| (**iter).FromModel=="����31" || (**iter).FromModel=="����32"
						  || (**iter).FromModel=="����33" || (**iter).FromModel=="����34"|| (**iter).FromModel=="����35" || (**iter).FromModel=="����36"
						  || (**iter).FromModel=="����37" || (**iter).FromModel=="����38"|| (**iter).FromModel=="����39" || (**iter).FromModel=="����40"
						  || (**iter).FromModel=="����41" || (**iter).FromModel=="����42"|| (**iter).FromModel=="����43" || (**iter).FromModel=="����44"
						  || (**iter).FromModel=="����45" || (**iter).FromModel=="����46"|| (**iter).FromModel=="����47" || (**iter).FromModel=="����48"
						  || (**iter).FromModel=="����49" || (**iter).FromModel=="����50"
						  ))
					  {
						  m_cmbDataSourceL.InsertString(iofvalue,strofvalue[iofvalue]);
						  iofvalue++;
					  }
					}
			*/
				  m_cmbDataSourceL.InsertString(0,_T("ʹ�ܱ�ʶ"));
				  m_cmbDataSourceL.InsertString(1,_T("ʹ�ܱ�ʶ��ȡ����"));
				  break;
			  case STYLE_DISIC:
				  m_cmbDataSourceL.InsertString(0,_T("ʹ�ܱ�ʶ"));
				  m_cmbDataSourceL.InsertString(1,_T("ʹ�ܱ�ʶ��ȡ����"));
				  break;
			  case STYLE_RAND:
			/*	  for(vector<IOValue*>::iterator iter = p_Model->arryIOputData.begin();iter!=p_Model->arryIOputData.end();iter++)
				  {
					  if((**iter).IOType == "output" && ((**iter).FromModel=="������ʶ" || (**iter).FromModel=="������ֵ"  
						  || (**iter).FromModel=="����1" || (**iter).FromModel=="����2" || (**iter).FromModel=="����3" || (**iter).FromModel=="����4" 
						  || (**iter).FromModel=="����5" || (**iter).FromModel=="����6" || (**iter).FromModel=="����7" || (**iter).FromModel=="����8" 
						  || (**iter).FromModel=="����9" || (**iter).FromModel=="����10"|| (**iter).FromModel=="����11" || (**iter).FromModel=="����12" 
						  || (**iter).FromModel=="����13" || (**iter).FromModel=="����14" || (**iter).FromModel=="����15" || (**iter).FromModel=="����16" 
						  || (**iter).FromModel=="����17" || (**iter).FromModel=="����18"|| (**iter).FromModel=="����19" || (**iter).FromModel=="����20"
						  || (**iter).FromModel=="����21" || (**iter).FromModel=="����22"|| (**iter).FromModel=="����23" || (**iter).FromModel=="����24"
						  || (**iter).FromModel=="����25" || (**iter).FromModel=="����26"|| (**iter).FromModel=="����27" || (**iter).FromModel=="����28"
						  || (**iter).FromModel=="����29" || (**iter).FromModel=="����30"|| (**iter).FromModel=="����31" || (**iter).FromModel=="����32"
						  || (**iter).FromModel=="����33" || (**iter).FromModel=="����34"|| (**iter).FromModel=="����35" || (**iter).FromModel=="����36"
						  || (**iter).FromModel=="����37" || (**iter).FromModel=="����38"|| (**iter).FromModel=="����39" || (**iter).FromModel=="����40"
						  || (**iter).FromModel=="����41" || (**iter).FromModel=="����42"|| (**iter).FromModel=="����43" || (**iter).FromModel=="����44"
						  || (**iter).FromModel=="����45" || (**iter).FromModel=="����46"|| (**iter).FromModel=="����47" || (**iter).FromModel=="����48"
						  || (**iter).FromModel=="����49" || (**iter).FromModel=="����50"
						  ))
					  {
						  m_cmbDataSourceL.InsertString(iofvalueforRand,strofvalueforRand[iofvalueforRand]);
						  iofvalueforRand++;
					  }
					}
				*/
				  m_cmbDataSourceL.InsertString(iofvalueforRand,L"ʹ�ܱ�ʶ");
				  m_cmbDataSourceL.InsertString(iofvalueforRand+1,L"ʹ�ܱ�ʶ��ȡ����");
				  m_cmbDataSourceL.InsertString(iofvalueforRand+2,L"������");
				  m_cmbDataSourceL.InsertString(iofvalueforRand+3,L"��������ȡ����");
				  m_cmbDataSourceL.InsertString(iofvalueforRand+4,L"�������ֵ");
				  break;
			  case STYLE_TIME:
				  m_cmbDataSourceL.InsertString(0,L"ʱ����ֵ");
				  m_cmbDataSourceL.InsertString(1,L"ʹ�ܱ�ʶ");
				  m_cmbDataSourceL.InsertString(2,L"ʹ�ܱ�ʶ��ȡ����");
				  break;
			  case STYLE_TCTRL:
				  m_cmbDataSourceL.InsertString(0,L"ʹ�ܱ�ʶ");
				  m_cmbDataSourceL.InsertString(1,L"ʹ�ܱ�ʶ��ȡ����");
				  break;
			  case STYLE_LCTRL:
				  m_cmbDataSourceL.InsertString(0,L"ʹ�ܱ�ʶ");
				  m_cmbDataSourceL.InsertString(1,L"ʹ�ܱ�ʶ��ȡ����");
				  break;
			  default:
				  break;
			  }
			  m_cmbDataSourceL.SetCurSel(0);
		  }
		  if(p_Model->arryIOputData.at(rowofchart)->IOType == "in/output")
		  {
			  m_cmbDataSourceL.ResetContent();//���������
			  m_cmbDataSourceL.InsertString(0,p_Model->m_caption);
			  m_cmbDataSourceL.SetCurSel(0);
		  }

	  }
	  if(columnofchart==6)
	  {
		  m_cmbSourceValueL.SetParent(&m_ListLocal);
//		  m_cmbSourceValueL.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//���������е�ֵ����Edit�ؼ��У� 
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
		  if(p_Model->arryIOputData.at(rowofchart)->IOType == "input") // �������Դ��������
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
			  else // �������Դ����������
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
			  m_ListLocal.SetItemText(rowofchart,columnofchart,_T("��������"));
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
		  m_edtValueIntL.SetWindowText(m_ListLocal.GetItemText(rowofchart,columnofchart));//���������е�ֵ����Edit�ؼ��У� 
		  m_edtValueIntL.ShowWindow(SW_SHOW);
		  m_edtValueIntL.MoveWindow(&rc);
		  m_edtValueIntL.SetFocus();
		  m_edtValueIntL.ShowCaret();//��ʾ��� 
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

  // �����ȷ������ť
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

			  // ��������
			  for(vector<IOValue*>::iterator iter_i=InputofLuaTS.begin();iter_i!=InputofLuaTS.end();iter_i++)
			  {
				//  if((*iter_i)->Sequence != 0)
				  if((*iterp)->arryInputofLua.at((*iter_i)->Sequence-1) != NULL)
					  (*iterp)->arryInputofLua.at((*iter_i)->Sequence-1) = *iter_i;
				  else
					  MessageBox(_T("����/����������ô���"));
			  }
			  for(vector<IOValue*>::iterator iter_o=OutputofLuaTS.begin();iter_o!=OutputofLuaTS.end();iter_o++)
			  {
				  if((*iterp)->arryOutputofLua.at((*iter_o)->Sequence-1)!=NULL)
					  (*iterp)->arryOutputofLua.at((*iter_o)->Sequence-1) = (*iter_o);
				  else
					  MessageBox(_T("����/����������ô���"));
			  }
			  break;
		  default:
			  break;
		  }

		  // ����/����б������
		  
		  for(vector<IOValue*>::iterator iter=(*iterp)->arryIOputData.begin();iter!=(*iterp)->arryIOputData.end();iter++)
		  {
			  if((*iter)->isQuoteG && (*iter)->GlobalVluName=="��")
			  {
				  MessageBox((*iterp)->m_caption+_T("_")+(*iter)->Name+_T("_����ȫ�ֱ������� ��ѡ��Ҫ���õ�ȫ�ֱ�����"));
				  isAllRight = false;
			  }
		  }
	  }
	  if(isAllRight)
		CDialogEx::OnOK();
  }

  // �ֲ��������£���ȡ������
  void CDlgModelParmList::OnGetVluName_L()
  {
	  CString str;
	  m_edtValueNameL.GetWindowText(str);

	  m_ListLocal.SetItemText(m_rowL,m_columnL,str);
	  p_Model->arryIOputData.at(m_rowL)->Name = str;
  }

  // �ֲ��������£���ȡ���
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

  // �ֲ��������£���ȡIOType	  //m_cmbIOtypeL
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

  // �ֲ��������£���ȡ�Ƿ�����ȫ�ֱ���//m_cmbIsQuoteG_L
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

  // �ֲ��������£���ȡ�����õ�ȫ�ֱ���//m_cmbGlobalNameL
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

  // �ֲ��������£���ȡ����Դ//m_cmbDataSourceL
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

  // �ֲ��������£���ȡ����Դ����//m_cmbSourceValueL
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

  // �ֲ��������£���ȡ���ݵĳ�ʼֵ
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
	  // TODO: �ڴ˴������Ϣ����������
	  // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	 if(p_Doc==NULL)
	 {
		 p_Doc = (CHSTPN_SYSDoc*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument();
	 }
  }
