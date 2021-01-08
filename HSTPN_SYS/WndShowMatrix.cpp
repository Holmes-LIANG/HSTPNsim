// WndShowMatrix.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "WndShowMatrix.h"
//#include "afxdialogex.h"


// CWndShowMatrix 对话框

IMPLEMENT_DYNAMIC(CWndShowMatrix, CDialogEx)

CWndShowMatrix::CWndShowMatrix(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWndShowMatrix::IDD, pParent)
{

}

CWndShowMatrix::CWndShowMatrix(vector<CString> Incdncemat,int nSizep, int nSizet,UINT analysismodelID, CWnd* pParent/* = NULL*/)
	: CDialogEx(CWndShowMatrix::IDD, pParent)
{
	IncdnceMatStr = Incdncemat;
	nSizeP = nSizep;
	nSizeT = nSizet;
	AnalysisModelID = analysismodelID;
}

CWndShowMatrix::~CWndShowMatrix()
{
}

void CWndShowMatrix::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Chart_Matrix);
}


BEGIN_MESSAGE_MAP(CWndShowMatrix, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWndShowMatrix 消息处理程序


BOOL CWndShowMatrix::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_Chart_Matrix.ModifyStyle(0,LVS_REPORT);               // 设置为报表模式
	m_Chart_Matrix.SetExtendedStyle(m_Chart_Matrix.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_FULLROWSELECT);// 间隔线+行选中
	
	m_Chart_Matrix.InsertColumn(0,L"");               // 插入表头
	m_Chart_Matrix.SetColumnWidth(0, 40);
	CString str_i;
	for(int i = 0;i<nSizeP;i++)
	{
		str_i.Format(L"%d",i);
		str_i = L"p" + str_i;
		m_Chart_Matrix.InsertColumn(i+1,str_i);
		m_Chart_Matrix.SetColumnWidth(i+1, 40);
	}

	for(int j=0;j<nSizeT;j++)
	{
		m_Chart_Matrix.InsertItem(j,L"");
		
	}

	CString str_m;
	for(int m=0;m<nSizeT;m++)
	{
		str_m.Format(L"%d",m);
		str_m = L"t" + str_m;
		m_Chart_Matrix.SetItemText(m,0,str_m);
	}

	int row = 0;  // 定义关联矩阵的行
	int column = 1;// 定义关联矩阵的列
	for(vector<CString>::iterator iter=IncdnceMatStr.begin();iter!=IncdnceMatStr.end();iter++)
	{
		if(column<=nSizeP)
		{
			m_Chart_Matrix.SetItemText(row,column,(*iter));
			column++;
		}
		if(column == nSizeP+1)
		{
			column = 1;
			row++;
		}

	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CWndShowMatrix::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect);
	if(m_Chart_Matrix.GetSafeHwnd()!=NULL)
	{
		m_Chart_Matrix.SetWindowPos(NULL,rect.left+10,rect.top+10,rect.Width()-20,rect.Height()-20,SWP_NOACTIVATE | SWP_NOZORDER);
	}
}


