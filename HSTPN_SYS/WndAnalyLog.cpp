// WndAnalyLog.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "WndAnalyLog.h"
//#include "afxdialogex.h"


// CWndAnalyLog 对话框

IMPLEMENT_DYNAMIC(CWndAnalyLog, CDialogEx)

CWndAnalyLog::CWndAnalyLog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWndAnalyLog::IDD, pParent)
{
	
}

CWndAnalyLog::CWndAnalyLog(vector<CString> IsltdnodeSet, UINT AnalysismodelID,CWnd* pParent/* = NULL*/)   // 自定义构造函数
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
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CWndAnalyLog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
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

	// TODO:  在此添加额外的初始化
/*	m_chartOfIncdnce.ModifyStyle(0,LVS_REPORT);               // 设置为报表模式
	m_chartOfIncdnce.SetExtendedStyle(m_chartOfIncdnce.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_FULLROWSELECT);// 间隔线+行选中

	m_chartOfIncdnce.InsertColumn(0,L"变量名");               // 插入表头
	m_chartOfIncdnce.InsertColumn(1,L"类型"); 
	m_chartOfIncdnce.InsertColumn(2,L"I/O");
	//m_List_IOput.InsertColumn(3,L"端口号");
	m_chartOfIncdnce.InsertColumn(3,L"数据源");
	m_chartOfIncdnce.InsertColumn(4,L"数据源的变量");
	m_chartOfIncdnce.SetBkColor(RGB(255,255,255));

	for(int n=0;n<20;n++)
	{
		m_chartOfIncdnce.InsertItem(n,L"");
	}
	
	CRect rect;  
	m_chartOfIncdnce.GetClientRect(rect);                     //获得当前客户区信息   
	m_chartOfIncdnce.SetColumnWidth(0, rect.Width() / 5);     //设置列的宽度。   
	m_chartOfIncdnce.SetColumnWidth(1, rect.Width() / 5); 
	m_chartOfIncdnce.SetColumnWidth(2, rect.Width() / 5);
	m_chartOfIncdnce.SetColumnWidth(3, rect.Width() / 5);
	m_chartOfIncdnce.SetColumnWidth(4, rect.Width() / 5);*/


	CString analylog("-------------HSTPNsim 1.0 分析日志-------------\r\n \r\n");
	if(AnalysisModelID==1)
	{
		if(IsLtdNodeSet.size()==0)
		{
			analylog = analylog + _T("结论：不存在孤立结点！");
		}
		else
		{
			analylog = analylog + _T("检测到的孤立结点有：\r\n");
			for(vector<CString>::iterator iter1=IsLtdNodeSet.begin();iter1!=IsLtdNodeSet.end();iter1++)
			{
				analylog = analylog + (*iter1) + _T("\r\n");
			}
			analylog = analylog + _T("\r\n结论：存在孤立结点，请删除孤立结点或进行其他修改！");
		}
	}
	if(AnalysisModelID==2)
	{
		if(IncdnceMatStr.size()==0)
		{
			if(nSizeP==0 && nSizeT==0)
			{
				analylog = analylog + _T("没有找到待分析模型！");
			}
			else
			{
				analylog = analylog + _T("不能求解关联矩阵！");
			}
		}
		else
		{
			analylog = analylog + _T("模型的关联矩阵如下：\r\n \r\n      ");
			for(int i=0;i<nSizeP;i++)
			{
				CString stri;
				stri.Format(L"%d",i);
				analylog = analylog +  _T("p")+ stri + _T("  ");
			}
			analylog = analylog +  _T("\r\n \r\n");
			int n = 1;// 列计数器
			int m = 1;// 行计数器
			int j = 1;// 控制变迁名字输出的计数器
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
	// 异常: OCX 属性页应返回 FALSE
}
