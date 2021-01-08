// DlgChooseOValue.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgChooseOValue.h"
//#include "afxdialogex.h"


// CDlgChooseOValue 对话框

IMPLEMENT_DYNAMIC(CDlgChooseOValue, CDialogEx)

CDlgChooseOValue::CDlgChooseOValue(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgChooseOValue::IDD, pParent)
{
	m_pDoc = NULL;
	OutPName = L"";
	i_PposnOValue = -1;
}

CDlgChooseOValue::CDlgChooseOValue(CHSTPN_SYSDoc* pdoc, CWnd* pParent/* = NULL*/)
	: CDialogEx(CDlgChooseOValue::IDD, pParent)
{
	m_pDoc = pdoc;
	OutPName = L"";
	i_PposnOValue = -1;
}

CDlgChooseOValue::~CDlgChooseOValue()
{
}

void CDlgChooseOValue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_CHOOSEPLACE, m_cmbPlace);
	DDX_Control(pDX, IDC_TREE_CHOOSEVALUE, m_treePOutValue);
}


BEGIN_MESSAGE_MAP(CDlgChooseOValue, CDialogEx)
	ON_CBN_SELCHANGE(IDC_CMB_CHOOSEPLACE, &CDlgChooseOValue::OnCmbSelchange_Chooseplace)
	ON_NOTIFY(NM_CLICK, IDC_TREE_CHOOSEVALUE, &CDlgChooseOValue::OnChoosevalue_TreeClick)
	ON_BN_CLICKED(IDOK, &CDlgChooseOValue::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgChooseOValue 消息处理程序


BOOL CDlgChooseOValue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//-----------初始化DropDownList控件------------
	int i_place = 1;
	m_cmbPlace.ResetContent();
	m_cmbPlace.InsertString(0,_T("全局变量"));
	for(vector<CPlace*>::iterator iterp = m_pDoc->Vector_Place.begin();iterp!=m_pDoc->Vector_Place.end();iterp++)
	{
		m_cmbPlace.InsertString(i_place,(*iterp)->m_caption);
		i_place++;
	}

	if(OutPName == "")
	{
		m_cmbPlace.SetCurSel(0);
	}
	else
	{
		//m_cmbPlace.SetDlgItemText(IDC_CMB_CHOOSEPLACE,OutPName);
		int nSel = m_cmbPlace.FindStringExact(0,OutPName);
		m_cmbPlace.SetCurSel(nSel);

	}

	m_treePOutValue.DeleteAllItems();
	HTREEITEM hChild;
	hRoot = m_treePOutValue.InsertItem(_T("输出量"),TVI_ROOT);
	if(OutPName == _T("全局变量")) // 如果库所项选择的“下拉项”为“全局变量”
	{
		bool flag = false;
		int posinList = 0;
		for(vector<IOValue*>::iterator itero=m_pDoc->arryIOputDataG.begin();itero!=m_pDoc->arryIOputDataG.end();itero++)
		{
			if(!flag)
			{
				hChild = m_treePOutValue.InsertItem((*itero)->Name,hRoot,hRoot);
				flag   = true;
				vec_subMap.push_back(posinList);
			}
			else
			{
				hChild = m_treePOutValue.InsertItem((*itero)->Name,hRoot,hChild);
				vec_subMap.push_back(posinList);
			}
		}
	}
	else                           // 如果库所项选择的“下拉项”为具体的库所名
	{
		for(vector<CPlace*>::iterator iterp = m_pDoc->Vector_Place.begin();iterp!=m_pDoc->Vector_Place.end();iterp++)
		{
			if((*iterp)->m_caption == OutPName)
			{
				bool flag = false;
				int posninList = 0;  // 每个参数在arryIOputData中的位置，存储于vec_subMap中
				for(vector<IOValue*>::iterator iter=(*iterp)->arryIOputData.begin();iter!=(*iterp)->arryIOputData.end();iter++)
				{
					if((*iter)->IOType == "output" | (*iter)->IOType == "in/output")
					{
						if(!flag)
						{
							hChild = m_treePOutValue.InsertItem((*iter)->Name,hRoot,hRoot);
							flag = true;
							vec_subMap.push_back(posninList);
						}
						else
						{
							hChild = m_treePOutValue.InsertItem((*iter)->Name,hRoot,hChild);
							vec_subMap.push_back(posninList);
						}
					}
					posninList++;
				}
			}
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// do when change the text of comobox
void CDlgChooseOValue::OnCmbSelchange_Chooseplace()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString str_placename;
	int nSel = m_cmbPlace.GetCurSel();  
	m_cmbPlace.SetCurSel(nSel);                     // 先设置选中项为当前项
	m_cmbPlace.GetWindowText(OutPName);        // 然后获取当前项的内容
	//----------获取OutPName表示的库所在Vector_Place 中的位置
	int posnofP = 0;
	if(OutPName == _T("全局变量"))
	{
		this->i_PposnOValue = posnofP;
	}
	else
	{
		for(vector<CPlace*>::iterator iterp=m_pDoc->Vector_Place.begin();iterp!=m_pDoc->Vector_Place.end();iterp++)
		{
			if((*iterp)->m_caption == OutPName)
			{
				this->i_PposnOValue = posnofP;
			}
			posnofP++;
		}
	}
	//----------初始化TreeControl-------------------
	m_treePOutValue.DeleteAllItems();
	HTREEITEM hChild;
	hRoot = m_treePOutValue.InsertItem(_T("输出量"),TVI_ROOT);
	if(OutPName == _T("全局变量"))
	{
		bool flag = false;
		int posinList = 0;
		for(vector<IOValue*>::iterator itero=m_pDoc->arryIOputDataG.begin();itero!=m_pDoc->arryIOputDataG.end();itero++)
		{
			if(!flag)
			{
				hChild = m_treePOutValue.InsertItem((*itero)->Name,hRoot,hRoot);
				flag   = true;
				vec_subMap.push_back(posinList);
			}
			else
			{
				hChild = m_treePOutValue.InsertItem((*itero)->Name,hRoot,hChild);
				vec_subMap.push_back(posinList);
			}
		}
	}
	else
	{
		for(vector<CPlace*>::iterator iterp = m_pDoc->Vector_Place.begin();iterp!=m_pDoc->Vector_Place.end();iterp++)
		{
			if((*iterp)->m_caption == OutPName)
			{
				bool flag = false;
				int posninList = 0;//每个参数在arryIOputData中的位置
				for(vector<IOValue*>::iterator iter=(*iterp)->arryIOputData.begin();iter!=(*iterp)->arryIOputData.end();iter++)
				{
					if((*iter)->IOType == "output" | (*iter)->IOType == "in/output")
					{
						if(!flag)
						{
							hChild = m_treePOutValue.InsertItem((*iter)->Name,hRoot,hRoot);
							flag = true;
							vec_subMap.push_back(posninList);
						}
						else
						{
							hChild = m_treePOutValue.InsertItem((*iter)->Name,hRoot,hChild);
							vec_subMap.push_back(posninList);
						}
					}
					posninList++;
				}
			}
		}
	}

}


void CDlgChooseOValue::OnChoosevalue_TreeClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CDlgChooseOValue::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	vec_OutValue.clear();
	HTREEITEM hChild = m_treePOutValue.GetChildItem(hRoot);
	int count = 0;
	while(hChild!=NULL)
	{
		if(m_treePOutValue.GetCheck(hChild))
		{
			//vec_OutValue.push_back(1);
			vec_OutValue.push_back(vec_subMap.at(count));
		}
		//else
		//{
		//	vec_OutValue.push_back(0);
		//}
		hChild = m_treePOutValue.GetNextItem(hChild,TVGN_NEXT);
		count++;
	}


	CDialogEx::OnOK();
}
