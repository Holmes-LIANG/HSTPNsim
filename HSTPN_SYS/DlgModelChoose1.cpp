// DlgModelChoose1.cpp : ʵ���ļ�
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


// CDlgModelChoose1 ���

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


// CDlgModelChoose1 ��Ϣ�������


BOOL CDlgModelChoose1::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CDlgModelChoose1::OnPaint()
{
	m_pDoc = (CHSTPN_SYSDoc*)(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveDocument());
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CFormView::OnPaint()
	//=============================================
	// ����IDC_STC_PETRI1�ؼ�����ɫ������
	//CFont font;
	//font.CreatePointFont(150,_T("����"),NULL);
	//GetDlgItem(IDC_STC_PETRI1)->SetFont(&font);
	//========================================================================
	//��ӱ���ɫ+Picture Control����ͼƬ
	//========================================================================
	//-------------------Ϊ�Ի�����ӱ���ɫ-----------------------------------
	CRect   rect;        
    GetClientRect(rect); 
	//CPen pen(PS_SOLID,3,RGB(0,255,0));
	//dc.SelectObject(&pen);
	
  //  dc.FillSolidRect(rect,RGB(250,250,250));//���ñ�����ɫ

	

}



int CDlgModelChoose1::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	return 0;
}




void CDlgModelChoose1::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	if(!b_isInitTree)
	{
		HICON hIcon[7];                                                             // ͼ��������   
//		HTREEITEM hRoot;                                                             // ���ĸ��ڵ�ľ��   
//		HTREEITEM hChild;                                                            // �����ӽڵ�ľ��
		   
		hIcon[0] = theApp.LoadIcon(IDI_ICON11);                                       // ����ͼ�꣬�������ǵľ�����浽����  
		hIcon[1] = theApp.LoadIcon(IDI_ICON4);
		hIcon[2] = theApp.LoadIcon(IDI_ICON5);
		hIcon[3] = theApp.LoadIcon(IDI_ICON6);
		hIcon[4] = theApp.LoadIcon(IDI_ICON7);
		hIcon[5] = theApp.LoadIcon(IDI_ICON8);
		hIcon[6] = theApp.LoadIcon(IDI_ICON9);

		m_imageList.DeleteImageList();                                               // �������
		  
		m_imageList.Create(32, 32, ILC_COLOR32, 7, 3);                              // ����ͼ������CImageList���� ���У�10��ʾ�洢�ĸ���Ϊ10��3��һ����չ��С���������Ҫ�棬����������չ����Ҳֻ���ٴ�����    
		   
		for (int i=0; i<7; i++)                                                     // ��ͼ����ӵ�ͼ������
		{   
			m_imageList.Add(hIcon[i]);   
		}   
		// ����ͼ�꣬�������ǵľ�����浽���� 
		m_HSTPNStrucTree.SetImageList(&m_imageList, TVSIL_NORMAL);                      // Ϊ���οؼ�����ͼ������
		//-----------------------------------------------------------------------------------
		hRoot = m_HSTPNStrucTree.InsertItem(_T("����ṹ"), 0, 0);                 // ������ڵ�
		m_HSTPNStrucTree.SetItemData(hRoot,1);                                          // Ϊ�ڵ�������ݣ������������Ϣ��Ӧ�������ҵ��ڵ�

/*		hChild = m_HSTPNStrucTree.InsertItem(_T("��ɢ����"), 1, 1, hRoot, TVI_LAST);
		HTREEITEM hChild1=m_HSTPNStrucTree.InsertItem(_T("p0"), 1, 1, hChild, TVI_LAST);
		hChild1=m_HSTPNStrucTree.InsertItem(_T("p1"), 1, 1, hChild, TVI_LAST);

		hChild = m_HSTPNStrucTree.InsertItem(_T("��������"), 2, 2, hRoot, TVI_LAST);
		hChild1=m_HSTPNStrucTree.InsertItem(_T("p2"), 2, 2, hChild, TVI_LAST);

		hChild = m_HSTPNStrucTree.InsertItem(_T("�������"), 3, 3, hRoot, TVI_LAST);
		hChild1=m_HSTPNStrucTree.InsertItem(_T("p3"), 2, 2, hChild, TVI_LAST);

		hChild = m_HSTPNStrucTree.InsertItem(_T("ʱ�ӿ���"), 4, 4, hRoot, TVI_LAST);
		hChild1=m_HSTPNStrucTree.InsertItem(_T("p4"), 2, 2, hChild, TVI_LAST);

		hChild = m_HSTPNStrucTree.InsertItem(_T("���ƿ���"), 5, 5, hRoot, TVI_LAST);
		hChild1=m_HSTPNStrucTree.InsertItem(_T("p5"), 2, 2, hChild, TVI_LAST);


		hChild = m_HSTPNStrucTree.InsertItem(_T("��    Ǩ"), 6, 6, hRoot, TVI_LAST);
		hChild1=m_HSTPNStrucTree.InsertItem(_T("t0"), 6, 6, hChild, TVI_LAST);*/

		b_isInitTree = TRUE;
	}
}


void CDlgModelChoose1::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetClientRect(&rect);
	if(m_HSTPNStrucTree.GetSafeHwnd()!=NULL)
	{
		m_HSTPNStrucTree.SetWindowPos(NULL,rect.left,rect.top+30,rect.Width(),rect.Height()-30,SWP_NOACTIVATE | SWP_NOZORDER);
	}
}
// ��������Mainframe��ʼ��HSTPN_SYSView������Ϣ���������ṹ
LRESULT CDlgModelChoose1::OnUpdateHSTPNTree2(WPARAM wParam, LPARAM lParam)
{
	//--��ȡ��������Ǩ��Ϣ-----------------------------------------------------
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
	// ��ȡ���õı�Ǩ��Ϣ
	vector<CTransition*>::iterator iterT = m_pDoc->Vector_Trans.begin();
	vector<CTransition*>::iterator iterTend = m_pDoc->Vector_Trans.end();
	vector<CString> arrycptnTrans;
	for(;iterT != iterTend;iterT++)
	{
		arrycptnTrans.push_back((*iterT)->m_caption);
	}
	
	//--�������ṹ------------------------------------------------------------------
	// ������ṹ������
	m_HSTPNStrucTree.DeleteAllItems();

	HTREEITEM hChild;                                                            // �����ӽڵ�ľ��--һ���ӽڵ�
	HTREEITEM hChild1;                                                            // �����ӽڵ�ľ��--�����ӽڵ�
	m_HSTPNStrucTree.SetImageList(&m_imageList, TVSIL_NORMAL);                      // Ϊ���οؼ�����ͼ������
	
	hRoot = m_HSTPNStrucTree.InsertItem(_T("����ṹ"), 0, 0);                 // ������ڵ�
	m_HSTPNStrucTree.SetItemData(hRoot,1);                                          // Ϊ�ڵ�������ݣ������������Ϣ��Ӧ�������ҵ��ڵ�
	// ��ʾ���ÿ����ڵ�
	if(arrycptnDiscPlace.size()!=0)
	{
		CString str_place1size;
		str_place1size.Format(L"%d",arrycptnDiscPlace.size());
		str_place1size = _T("(")+str_place1size + _T(")");
		hChild = m_HSTPNStrucTree.InsertItem(_T("��ɢ����")+str_place1size, 1, 1, hRoot, TVI_LAST);
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
		hChild = m_HSTPNStrucTree.InsertItem(_T("��������")+str_place2size, 2, 2, hRoot, TVI_LAST);
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
		hChild = m_HSTPNStrucTree.InsertItem(_T("�������")+str_place3size, 3, 3, hRoot, TVI_LAST);
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
		hChild = m_HSTPNStrucTree.InsertItem(_T("ʱ�ӿ���")+str_place4size, 4, 4, hRoot, TVI_LAST);
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
		hChild = m_HSTPNStrucTree.InsertItem(_T("���ƿ���")+str_place5size, 5, 5, hRoot, TVI_LAST);
		for(vector<CString>::iterator iterp5=arrycptnCtrlPlace.begin();iterp5!=arrycptnCtrlPlace.end();iterp5++)
		{
			hChild1=m_HSTPNStrucTree.InsertItem((*iterp5), 5, 5, hChild, TVI_LAST);
		}
	}
	// ��ʾ���б�Ǩ�ڵ�
	if(arrycptnTrans.size()!=0)
	{
		CString str_transsize;
		str_transsize.Format(L"%d",arrycptnTrans.size());
		str_transsize = _T("(")+str_transsize + _T(")");
		hChild = m_HSTPNStrucTree.InsertItem(_T("��    Ǩ")+str_transsize, 6, 6, hRoot, TVI_LAST);
		for(vector<CString>::iterator itert=arrycptnTrans.begin();itert!=arrycptnTrans.end();itert++)
		{
			hChild1=m_HSTPNStrucTree.InsertItem((*itert), 6, 6, hChild, TVI_LAST);
		}
	}
	
	return 0;
}