// DlgModelChoose.cpp : ʵ���ļ�
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


// CDlgModelChoose ���

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


// CDlgModelChoose ��Ϣ�������


BOOL CDlgModelChoose::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CDlgModelChoose::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	if(!b_isInitTree)
	{
		HICON hIcon[14];                                                             // ͼ��������   
		HTREEITEM hRoot;                                                             // ���ĸ��ڵ�ľ��   
		HTREEITEM hChild;                                                            // �����ӽڵ�ľ��
		   
		hIcon[0] = theApp.LoadIcon(IDI_ICON1);                                       // ����ͼ�꣬�������ǵľ�����浽����  
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
		hIcon[13] = theApp.LoadIcon(IDI_ICON12);                                     // ���ؾ��߿���ͼ��

		m_imageList.DeleteImageList();                                               // �������
		  
		m_imageList.Create(32, 32, ILC_COLOR32, 14, 3);                              // ����ͼ������CImageList���� ���У�10��ʾ�洢�ĸ���Ϊ10��3��һ����չ��С���������Ҫ�棬����������չ����Ҳֻ���ٴ�����    
		   
		for (int i=0; i<14; i++)                                                     // ��ͼ����ӵ�ͼ������
		{   
			m_imageList.Add(hIcon[i]);   
		}   
		// ����ͼ�꣬�������ǵľ�����浽���� 
		m_mdchoseTree.SetImageList(&m_imageList, TVSIL_NORMAL);                      // Ϊ���οؼ�����ͼ������
		//-----------------------------------------------------------------------------------
		hRoot = m_mdchoseTree.InsertItem(_T("HSTPN����Ԫ��"), 0, 0);                 // ������ڵ�
		m_mdchoseTree.SetItemData(hRoot,1);                                          // Ϊ�ڵ�������ݣ������������Ϣ��Ӧ�������ҵ��ڵ�

		hChild = m_mdchoseTree.InsertItem(_T("��ɢ����"), 3, 3, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,4);
		hChild = m_mdchoseTree.InsertItem(_T("��������"), 4, 4, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,5);
		hChild = m_mdchoseTree.InsertItem(_T("�������"), 5, 5, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,6);
		hChild = m_mdchoseTree.InsertItem(_T("ʱ�ӿ���"), 6, 6, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,7);
		hChild = m_mdchoseTree.InsertItem(_T("���ƿ���"), 7, 7, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,8);
		hChild = m_mdchoseTree.InsertItem(_T("���߿���"), 13, 13, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,14);                                        //������߿���
		hChild = m_mdchoseTree.InsertItem(_T("��    Ǩ"), 8, 8, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,9);
		hChild = m_mdchoseTree.InsertItem(_T("�� �� ��"), 9, 9, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,10);
		//-----------------------------------------------------------------------------------
		hRoot = m_mdchoseTree.InsertItem(_T("���ģ��"), 1, 1); 
		m_mdchoseTree.SetItemData(hRoot,2);

		hChild = m_mdchoseTree.InsertItem(_T("���������"), 10, 10, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,11);
		hChild = m_mdchoseTree.InsertItem(_T("������ļ�"), 11, 11, hRoot, TVI_LAST);
		m_mdchoseTree.SetItemData(hChild,12);
	    //hChild = m_mdchoseTree.InsertItem(_T("��װģ��"), 2, 2, hRoot, TVI_LAST);
		//m_mdchoseTree.SetItemData(hChild,15);
		//-----------------------------------------------------------------------------------
		//hRoot = m_mdchoseTree.InsertItem(_T("��װģ��"), 2, 2);
		//m_mdchoseTree.SetItemData(hRoot,3);

		b_isInitTree = TRUE;
	}
}

// ��Ӧtree control�ĵ��������Ŀ����ȷ���ĸ��ڵ㱻�����������ɵ���ڵ����Ϣ��Ӧ
void CDlgModelChoose::OnNMClickMdchosetree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;     // NMTREEVIEW�ṹ���а��������οؼ�֪ͨ��Ϣ�������Ϣ�����οؼ��Ĵ����֪ͨ��Ϣ�������ָ��ýṹ���ָ�롣
	
	CPoint p;  
    GetCursorPos(&p);                                    // ��ȡ�������λ�ã��õ�Ϊ��Ļ�����
	m_mdchoseTree.ScreenToClient(&p);

	UINT nFlag = 0;
	HTREEITEM h=m_mdchoseTree.HitTest(p,&nFlag);         //Returns the current position of the cursor related to the CTreeCtrl object.

	 if((h !=  NULL)&&(TVHT_ONITEM & nFlag))             // �жϵ��λ���Ƿ��ڽڵ�����
	 {
		 m_mdchoseTree.SelectItem(h);                    // ѡȡָ���ڵ�
		 int nDat = m_mdchoseTree.GetItemData(h);        // ��ȡ�ڵ��������
		 int u_whichModel = 0;
		 switch(nDat)
		 {
		 case 1:
			 // չ���ӽڵ�
			 u_whichModel = 0;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 2:
			 // չ���ӽڵ�
			 u_whichModel = 0;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 3:
			 // չ���ӽڵ�
			 u_whichModel = 0;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 4:                     // ������ɢ����
			 u_whichModel = 1;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 5:                     // ������������
			 u_whichModel = 2;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 6:                     // ����ʱ�ӿ���
			 u_whichModel = 3;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 7:                     // �����������
			 u_whichModel = 4;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 8:                     // �������ƿ���
			 u_whichModel = 5;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 9:                     // ������Ǩ
			 u_whichModel = 7;
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 10:                    // ��������
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
			 u_whichModel = 11;     //�������߿���
			 (AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_CREATEMODEL1,u_whichModel,0);
			 break;
		 case 15:
			 u_whichModel = 12;     //������װģ��
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

	// TODO: �ڴ˴������Ϣ����������
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CFormView::OnPaint()
}


BOOL CDlgModelChoose::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//return TRUE;
	return CFormView::OnEraseBkgnd(pDC);
}
