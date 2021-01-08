// WndAnalyLog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "WndAnalyLog.h"
//#include "afxdialogex.h"


// CWndAnalyLog �Ի���

IMPLEMENT_DYNAMIC(CWndAnalyLog, CDialogEx)

CWndAnalyLog::CWndAnalyLog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWndAnalyLog::IDD, pParent)
{
	
}

CWndAnalyLog::CWndAnalyLog(vector<CString> IsltdnodeSet, UINT AnalysismodelID,CWnd* pParent/* = NULL*/)   // �Զ��幹�캯��
	: CDialogEx(CWndAnalyLog::IDD, pParent)
{
	IsLtdNodeSet = IsltdnodeSet;
	AnalysisModelID = AnalysismodelID;
}

CWndAnalyLog::CWndAnalyLog(vector<CString> Incdncemat,int nSizep, int nSizet,UINT analysismodelID, CWnd* pParent/* = NULL*/)
	: CDialogEx(CWndAnalyLog::IDD, pParent)
{
	IncdnceMatStr = Incdncemat;
	nSizeP = nSizep;
	nSizeT = nSizet;
	AnalysisModelID = analysismodelID;
}

CWndAnalyLog::~CWndAnalyLog()
{
}

void CWndAnalyLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOWANALLYLOG, m_editShowLog);
}


BEGIN_MESSAGE_MAP(CWndAnalyLog, CDialogEx)

	ON_EN_CHANGE(IDC_SHOWANALLYLOG, &CWndAnalyLog::OnEnChangeShowanallylog)
	ON_WM_SIZE()
END_MESSAGE_MAP()





void CWndAnalyLog::OnEnChangeShowanallylog()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CWndAnalyLog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetClientRect(&rect);
	if(m_editShowLog.GetSafeHwnd()!=NULL)
	{
		m_editShowLog.SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	}
}


BOOL CWndAnalyLog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
/*	m_chartOfIncdnce.ModifyStyle(0,LVS_REPORT);               // ����Ϊ����ģʽ
	m_chartOfIncdnce.SetExtendedStyle(m_chartOfIncdnce.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_FULLROWSELECT);// �����+��ѡ��

	m_chartOfIncdnce.InsertColumn(0,L"������");               // �����ͷ
	m_chartOfIncdnce.InsertColumn(1,L"����"); 
	m_chartOfIncdnce.InsertColumn(2,L"I/O");
	//m_List_IOput.InsertColumn(3,L"�˿ں�");
	m_chartOfIncdnce.InsertColumn(3,L"����Դ");
	m_chartOfIncdnce.InsertColumn(4,L"����Դ�ı���");
	m_chartOfIncdnce.SetBkColor(RGB(255,255,255));

	for(int n=0;n<20;n++)
	{
		m_chartOfIncdnce.InsertItem(n,L"");
	}
	
	CRect rect;  
	m_chartOfIncdnce.GetClientRect(rect);                     //��õ�ǰ�ͻ�����Ϣ   
	m_chartOfIncdnce.SetColumnWidth(0, rect.Width() / 5);     //�����еĿ�ȡ�   
	m_chartOfIncdnce.SetColumnWidth(1, rect.Width() / 5); 
	m_chartOfIncdnce.SetColumnWidth(2, rect.Width() / 5);
	m_chartOfIncdnce.SetColumnWidth(3, rect.Width() / 5);
	m_chartOfIncdnce.SetColumnWidth(4, rect.Width() / 5);*/


	CString analylog("-------------HSTPNsim 1.0 ������־-------------\r\n \r\n");
	if(AnalysisModelID==1)
	{
		if(IsLtdNodeSet.size()==0)
		{
			analylog = analylog + _T("���ۣ������ڹ�����㣡");
		}
		else
		{
			analylog = analylog + _T("��⵽�Ĺ�������У�\r\n");
			for(vector<CString>::iterator iter1=IsLtdNodeSet.begin();iter1!=IsLtdNodeSet.end();iter1++)
			{
				analylog = analylog + (*iter1) + _T("\r\n");
			}
			analylog = analylog + _T("\r\n���ۣ����ڹ�����㣬��ɾ������������������޸ģ�");
		}
	}
	if(AnalysisModelID==2)
	{
		if(IncdnceMatStr.size()==0)
		{
			if(nSizeP==0 && nSizeT==0)
			{
				analylog = analylog + _T("û���ҵ�������ģ�ͣ�");
			}
			else
			{
				analylog = analylog + _T("��������������");
			}
		}
		else
		{
			analylog = analylog + _T("ģ�͵Ĺ����������£�\r\n \r\n      ");
			for(int i=0;i<nSizeP;i++)
			{
				CString stri;
				stri.Format(L"%d",i);
				analylog = analylog +  _T("p")+ stri + _T("  ");
			}
			analylog = analylog +  _T("\r\n \r\n");
			int n = 1;// �м�����
			int m = 1;// �м�����
			int j = 1;// ���Ʊ�Ǩ��������ļ�����
			for(vector<CString>::iterator iter2=IncdnceMatStr.begin();iter2!=IncdnceMatStr.end();iter2++)
			{
				//-----------
				if(j==1)
				{
					CString strm;
					strm.Format(L"%d",m-1);
					analylog = analylog +  _T("t")+ strm + _T("    ");
				}
				j++;
				if(j==nSizeP+1)
				{
					j=1;
				}
				//------------
				if(n<nSizeP)
				{
					analylog = analylog + (*iter2) + _T("    ");
					n++;
				}
				else
				{

					analylog = analylog + (*iter2) + _T("\r\n");
					n =1 ;
					m++;
				}
				//------------
			}
		}
	}
	m_editShowLog.SetWindowTextW(analylog);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
