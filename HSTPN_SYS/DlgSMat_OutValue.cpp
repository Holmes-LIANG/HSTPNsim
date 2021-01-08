// DlgSMat_OutValue.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgSMat_OutValue.h"
//#include "afxdialogex.h"
#include <propkey.h>
#include <sstream>

// CDlgSMat_OutValue �Ի���

IMPLEMENT_DYNAMIC(CDlgSMat_OutValue, CDialogEx)

CDlgSMat_OutValue::CDlgSMat_OutValue(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSMat_OutValue::IDD, pParent)
{
	m_pDoc = NULL;
	OutPName = L"";
	i_PposnOValue = -1;
}

CDlgSMat_OutValue::CDlgSMat_OutValue(CHSTPN_SYSDoc *m_pdoc, CWnd* pParent/* = NULL*/)
	: CDialogEx(CDlgSMat_OutValue::IDD, pParent)
{
	m_pDoc = m_pdoc;
	OutPName = L"";
	i_PposnOValue = -1;
}

CDlgSMat_OutValue::~CDlgSMat_OutValue()
{
}

void CDlgSMat_OutValue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Chart_SMatrix);
}


BEGIN_MESSAGE_MAP(CDlgSMat_OutValue, CDialogEx)
	ON_COMMAND(ID_SMAT_SET, &CDlgSMat_OutValue::OnSmatSet)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_SAVEMAT, &CDlgSMat_OutValue::OnSavemat)
END_MESSAGE_MAP()


// CDlgSMat_OutValue ��Ϣ�������


// Set which OutValue will be shown,������ð�ť
void CDlgSMat_OutValue::OnSmatSet()
{
	// TODO: �ڴ���������������
	CDlgChooseOValue m_dlgChooseOValue(m_pDoc,this);
	m_dlgChooseOValue.OutPName = OutPName;
	m_dlgChooseOValue.vec_OutValue = vec_OutValue;
	m_dlgChooseOValue.i_PposnOValue = i_PposnOValue;
	if(IDOK == m_dlgChooseOValue.DoModal())
	{
		vec_OutValue = m_dlgChooseOValue.vec_OutValue;
		OutPName     = m_dlgChooseOValue.OutPName;
		i_PposnOValue = m_dlgChooseOValue.i_PposnOValue;

		
		int j = vec_OutValueName.size();// ��ÿ��ͨ������ѡ��ı�Ҫ����ı���ʱ����ɾ��ԭ������(����ɾ��)
		for(vector<CString>::iterator iter=vec_OutValueName.begin();iter!=vec_OutValueName.end();iter++)
		{
			m_Chart_SMatrix.DeleteColumn(j);
			j--;
		}
		m_Chart_SMatrix.DeleteColumn(0);

		SetChartHead(OutPName,vec_OutValue);// Ȼ���µ��в��뵽�����
		p_Parent->SendMessage(WM_MESSAGE_UPDATELIST1,0,0); //tell CCTRLShowMatrix-class to update vec_OutValue;
	}
}


BOOL CDlgSMat_OutValue::Create(UINT nIDTemplate,CHSTPN_SYSDoc* m_pdoc, CWnd* pParentWnd)
{
	// TODO: �ڴ����ר�ô����/����û���

	m_pDoc = m_pdoc;
	p_Parent = (CCTRLShowMatrix*)pParentWnd; 
	
	return CDialogEx::Create(nIDTemplate, pParentWnd);
}


BOOL CDlgSMat_OutValue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Chart_SMatrix.ModifyStyle(0,LVS_REPORT);               // ����Ϊ����ģʽ
	m_Chart_SMatrix.SetExtendedStyle(m_Chart_SMatrix.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_FULLROWSELECT);// �����+��ѡ��
	
	SetWindowText(_T("Value From-px"));

	m_Chart_SMatrix.InsertColumn(0,L"���");                     // �����ͷ
	m_Chart_SMatrix.SetColumnWidth(0, 50);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgSMat_OutValue::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetClientRect(&rect);
	if(m_Chart_SMatrix.GetSafeHwnd()!=NULL)
	{
		m_Chart_SMatrix.SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

void CDlgSMat_OutValue::SetChartHead(CString Pname, vector<int> vec_outvalue)
{
	vec_OutValueName.clear();
	if(Pname == _T("ȫ�ֱ���"))
	{
		for(vector<int>::iterator iter=vec_outvalue.begin();iter!=vec_outvalue.end();iter++)
		{
			vec_OutValueName.push_back(m_pDoc->arryIOputDataG.at(*iter)->Name);
		}
	}
	else
	{
		for(vector<CPlace*>::iterator iterp=m_pDoc->Vector_Place.begin();iterp!=m_pDoc->Vector_Place.end();iterp++)
		{
			if((*iterp)->m_caption == Pname)
			{
				for(vector<int>::iterator iter=vec_outvalue.begin();iter!=vec_outvalue.end();iter++)
				{
					vec_OutValueName.push_back((*iterp)->arryIOputData.at((*iter))->Name);
				}
			}
		}
	}

	m_Chart_SMatrix.InsertColumn(0,L"���");                     // �����ͷ
	m_Chart_SMatrix.SetColumnWidth(0, 50);

	if(vec_OutValueName.size()!=0)
	{
		int i = 1;
		for(vector<CString>::iterator iter=vec_OutValueName.begin();iter!=vec_OutValueName.end();iter++)
		{
			m_Chart_SMatrix.InsertColumn(i,(*iter)); 
			m_Chart_SMatrix.SetColumnWidth(i, 50);
			i++;
		}
	}
}


void CDlgSMat_OutValue::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	vec_OutputData = p_Parent->vec_OutputData;
	vec_listOputDat.clear();
	for(vector<VEC_DOUBLE*>::iterator itervv=vec_OutputData.begin();itervv!=vec_OutputData.end();itervv++)
	{
		list<string> list_value;
		for(vector<double*>::iterator iterv=(*itervv)->begin();iterv!=(*itervv)->end();iterv++)
		{
			ostringstream os_value;
			if(os_value << (**iterv))
			{
				list_value.push_back(os_value.str());
			}
		}
		vec_listOputDat.push_back(list_value);
	}

	bool flag = false;
	if(vec_OutputData.size()!=0)
	{
		if(vec_OutputData.at(0)->size()!=0)
		{
			flag = true;
		}
	}

	m_Chart_SMatrix.DeleteAllItems();// ��������֮ǰ��ɾ�����е���

	int i = vec_OutValueName.size();
	if(vec_OutValueName.size()!=0)// ���vec_OutValueName.size()��Ϊ�㣬˵���ϴ��Ѿ����ù���ͷ����Ҫɾ��������������
	{
		//int i = 1;
		for(vector<CString>::iterator iter=vec_OutValueName.begin();iter!=vec_OutValueName.end();iter++)
		{
			m_Chart_SMatrix.DeleteColumn(i);
			i--;
		}
		m_Chart_SMatrix.DeleteColumn(0);

		i=1;
		m_Chart_SMatrix.InsertColumn(0,L"���");                     // �����ͷ
		m_Chart_SMatrix.SetColumnWidth(0, 50);
		for(vector<CString>::iterator iter=vec_OutValueName.begin();iter!=vec_OutValueName.end();iter++)
		{
			m_Chart_SMatrix.InsertColumn(i,(*iter)); 
			m_Chart_SMatrix.SetColumnWidth(i, 50);
			i++;
		}
	}
	else
	{
		m_Chart_SMatrix.DeleteColumn(0);    // ��ɾ��������룬��ֹ���ֶ���ظ��ı�ͷ
		m_Chart_SMatrix.InsertColumn(0,L"���");                     // �����ͷ
		m_Chart_SMatrix.SetColumnWidth(0, 50);
	}

	if(flag)// flagΪ�棬��ʾvec_OutputData�б��洢��ֵ����Ҫ�ٴ洢��һ��ֵ��ͬʱ�������У���д����š�
	{
		int flag_first = false;
		int row = 0;//hang
		int column = 1;//lie
		CString str_row;
		CString str_value;
		for(vector<VEC_DOUBLE*>::iterator itervv=vec_OutputData.begin();itervv!=vec_OutputData.end();itervv++)
		{
			row = 0;
			for(vector<double*>::iterator iterv=(*itervv)->begin();iterv!=(*itervv)->end();iterv++)
			{	
				if(!flag_first)
				{
					str_row.Format(L"%d",row+1);
					m_Chart_SMatrix.InsertItem(row,str_row);
					str_value.Format(L"%.2f",(**iterv));
					m_Chart_SMatrix.SetItemText(row,column,str_value);
				}
				else
				{
					str_value.Format(L"%.2f",(**iterv));
					m_Chart_SMatrix.SetItemText(row,column,str_value);
				}
				row++;
			}
			column++;
			flag_first = true;
		}
	}/**/


	SetWindowText(_T("Value From-")+this->OutPName);
}

// save vec_OutValue to *.csv
void CDlgSMat_OutValue::OnSavemat()
{
	// TODO: �ڴ���������������
	CFileDialog m_saveDlg(FALSE);                                                   // ���������ļ��Ի������񣬵��������Ϊ���Ի���
	m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.csv)\0*.csv\0All Files(*.*)\0\0"; // ���ù�����
	m_saveDlg.m_ofn.lpstrDefExt = L"csv";                                           // ���ȱʡ����չ��

	CCSVStream csvfile;

	if(IDOK == m_saveDlg.DoModal())
	{
		USES_CONVERSION;
		const char* filename = T2A(m_saveDlg.GetPathName());
		csvfile.WriteCsvData(filename,this->vec_listOputDat);

		
	}
}
