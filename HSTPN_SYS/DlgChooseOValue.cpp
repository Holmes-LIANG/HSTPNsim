// DlgChooseOValue.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgChooseOValue.h"
//#include "afxdialogex.h"


// CDlgChooseOValue �Ի���

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


// CDlgChooseOValue ��Ϣ�������


BOOL CDlgChooseOValue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//-----------��ʼ��DropDownList�ؼ�------------
	int i_place = 1;
	m_cmbPlace.ResetContent();
	m_cmbPlace.InsertString(0,_T("ȫ�ֱ���"));
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
	hRoot = m_treePOutValue.InsertItem(_T("�����"),TVI_ROOT);
	if(OutPName == _T("ȫ�ֱ���")) // ���������ѡ��ġ������Ϊ��ȫ�ֱ�����
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
	else                           // ���������ѡ��ġ������Ϊ����Ŀ�����
	{
		for(vector<CPlace*>::iterator iterp = m_pDoc->Vector_Place.begin();iterp!=m_pDoc->Vector_Place.end();iterp++)
		{
			if((*iterp)->m_caption == OutPName)
			{
				bool flag = false;
				int posninList = 0;  // ÿ��������arryIOputData�е�λ�ã��洢��vec_subMap��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

// do when change the text of comobox
void CDlgChooseOValue::OnCmbSelchange_Chooseplace()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CString str_placename;
	int nSel = m_cmbPlace.GetCurSel();  
	m_cmbPlace.SetCurSel(nSel);                     // ������ѡ����Ϊ��ǰ��
	m_cmbPlace.GetWindowText(OutPName);        // Ȼ���ȡ��ǰ�������
	//----------��ȡOutPName��ʾ�Ŀ�����Vector_Place �е�λ��
	int posnofP = 0;
	if(OutPName == _T("ȫ�ֱ���"))
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
	//----------��ʼ��TreeControl-------------------
	m_treePOutValue.DeleteAllItems();
	HTREEITEM hChild;
	hRoot = m_treePOutValue.InsertItem(_T("�����"),TVI_ROOT);
	if(OutPName == _T("ȫ�ֱ���"))
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
				int posninList = 0;//ÿ��������arryIOputData�е�λ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CDlgChooseOValue::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
