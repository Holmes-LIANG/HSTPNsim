// DlgChseOVlueTFile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgChseOVlueTFile.h"
//#include "afxdialogex.h"


// CDlgChseOVlueTFile �Ի���

IMPLEMENT_DYNAMIC(CDlgChseOVlueTFile, CDialogEx)

CDlgChseOVlueTFile::CDlgChseOVlueTFile(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgChseOVlueTFile::IDD, pParent)
{
	m_pDoc = NULL;
	OutPName = L"";
	i_PposnOValue = -1;
	str_FilePath = _T("");

	//if(m_pDatafile==NULL)
	//	m_pDatafile = new ofstream();
}

CDlgChseOVlueTFile::CDlgChseOVlueTFile(CHSTPN_SYSDoc* pdoc, CWnd* pParent/* = NULL*/)
	: CDialogEx(CDlgChseOVlueTFile::IDD, pParent)
{
	m_pDoc = pdoc;
	OutPName = L"";
	i_PposnOValue = -1;
	str_FilePath = _T("");

	//if(m_pDatafile==NULL)
	//	m_pDatafile = new ofstream();
}

CDlgChseOVlueTFile::~CDlgChseOVlueTFile()
{
}

void CDlgChseOVlueTFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_CHOSEPLACE, m_cmbPlace);
	DDX_Control(pDX, IDC_TREE_CHOSEVALUE, m_treePOutValue);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, m_edtFilePath);
}


BEGIN_MESSAGE_MAP(CDlgChseOVlueTFile, CDialogEx)
	ON_CBN_SELCHANGE(IDC_CMB_CHOSEPLACE, &CDlgChseOVlueTFile::OnCmbSelchange_Chooseplace)
	ON_BN_CLICKED(IDOK, &CDlgChseOVlueTFile::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_FILEPATH, &CDlgChseOVlueTFile::OnEnChangeEditFilepath)
	ON_BN_CLICKED(IDC_BTN_CHOSEFILEPATH, &CDlgChseOVlueTFile::OnBnClicked_Chosefilepath)
END_MESSAGE_MAP()


// CDlgChseOVlueTFile ��Ϣ�������


BOOL CDlgChseOVlueTFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_edtFilePath.SetWindowText(str_FilePath);

	int i_place = 1;
	m_cmbPlace.ResetContent();                     // ��ʼ�����������Ӧ�������б��
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
	if(OutPName == _T("ȫ�ֱ���"))
	{
		bool flag = false;
		int posninList = 0;
		for(vector<IOValue*>::iterator iterg=m_pDoc->arryIOputDataG.begin();iterg!=m_pDoc->arryIOputDataG.end();iterg++)
		{
			if(!flag)
			{
				hChild = m_treePOutValue.InsertItem((*iterg)->Name, hRoot, hRoot);
				flag = true;
				vec_subMap.push_back(posninList);
			}
			else
			{
				hChild = m_treePOutValue.InsertItem((*iterg)->Name, hRoot, hChild);
				vec_subMap.push_back(posninList);
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

	// ��TreeCtrl����Ӧ�ڵ����û�ѡ��״̬
	vector<int> vec_OutValueTs;  // �Ƚ�vec_OutValue�б���ĵ�arryIOputData�е�ӳ�䣬ת��Ϊ��arryOutputData�е�ӳ��
	vec_OutValueTs.clear();
	if(this->OutPName == _T("ȫ�ֱ���")) // ����������ȫ�ֱ���
	{
		int m = 0;
		int n = 0;
		int n_o = 0;
		for(vector<IOValue*>::iterator iterg=m_pDoc->arryIOputDataG.begin();iterg!=m_pDoc->arryIOputDataG.end();iterg++)
		{
			if(m<vec_OutValue.size())
			{
				if(this->vec_OutValue[m]==n)
				{
					vec_OutValueTs.push_back(n_o);
					m++;
				}
				if((*iterg)->IOType == "output" | (*iterg)->IOType=="in/output")
				{
					n_o++;
				}
				n++;
			}
		}
	}
	else // ���������Ǿֲ�����
	{
		int m = 0;
		int n = 0;
		int n_o = 0;
		if(this->i_PposnOValue!=-1 && m_pDoc->Vector_Place.size()!=0)
		{
			for(vector<IOValue*>::iterator iter=m_pDoc->Vector_Place.at(this->i_PposnOValue)->arryIOputData.begin();
										iter!=m_pDoc->Vector_Place.at(this->i_PposnOValue)->arryIOputData.end();iter++)
			{
				if(m<vec_OutValue.size())
				{
					if(this->vec_OutValue[m]==n)
					{
						vec_OutValueTs.push_back(n_o);
						m++;
					}
					if((*iter)->IOType == "output" | (*iter)->IOType=="in/output")
					{
						n_o++;
					}
					n++;
				}
			}
		}
	}
	m_treePOutValue.ModifyStyle( TVS_CHECKBOXES, 0 );
	m_treePOutValue.ModifyStyle( 0, TVS_CHECKBOXES );
	//m_treePOutValue.SetCheck(hRoot, TRUE);
	HTREEITEM   hChildItem = m_treePOutValue.GetChildItem(hRoot);// ��ȡ��һ���ӽڵ㣬����ȡ��һ�����ֵ
	int count = 0;
	int i = 0;
	int nsize = vec_OutValueTs.size();
	while(hChildItem!=NULL && i<nsize)
	{
		if(vec_OutValueTs[i] == count)
		{
			m_treePOutValue.SetCheck(hChildItem, TRUE);
			i++;
		}
		count++;
		hChildItem = m_treePOutValue.GetNextSiblingItem(hChildItem);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgChseOVlueTFile::OnCmbSelchange_Chooseplace()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel = m_cmbPlace.GetCurSel();  
	m_cmbPlace.SetCurSel(nSel);                     // ������ѡ����Ϊ��ǰ��
	m_cmbPlace.GetWindowText(OutPName);        // Ȼ���ȡ��ǰ�������
	if(this->OutPName == _T("ȫ�ֱ���"))
	{
		this->i_PposnOValue = 0;
	}
	else
	{
		//----------��ȡOutPName��ʾ�Ŀ�����Vector_Place �е�λ��
		int posnofP = 0;
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
	if(this->OutPName == _T("ȫ�ֱ���"))
	{
		bool flag = false;
		int posninList = 0;
		for(vector<IOValue*>::iterator iterg=m_pDoc->arryIOputDataG.begin();iterg!=m_pDoc->arryIOputDataG.end();iterg++)
		{
			if(!flag)
			{
				hChild = m_treePOutValue.InsertItem((*iterg)->Name, hRoot, hRoot);
				flag = true;
				vec_subMap.push_back(posninList);
			}
			else
			{
				hChild = m_treePOutValue.InsertItem((*iterg)->Name, hRoot, hChild);
				vec_subMap.push_back(posninList);
			}
			posninList++;
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
					if((*iter)->IOType == "output" | (*iter)->IOType=="in/output")
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

	//-- ��hst�ļ�����·���´���һ��ͬ�����ļ���
/*	if(m_pDoc->opening_filePath != "")// ����m_pDoc->opening_filePath�Ƿ�Ϊ�գ�����Ϊ�գ���˵���ļ��ѱ���
	{
		if (!PathIsDirectory(m_pDoc->opening_filePath + _T("-ToFile")))
		{
			::CreateDirectory(m_pDoc->opening_filePath + _T("-ToFile"),NULL);
			if (!PathIsDirectory(m_pDoc->opening_filePath + _T("-ToFile\\From-") + this->OutPName))//�ж��ļ����Ƿ񴴽�
			{
				::CreateDirectory(m_pDoc->opening_filePath + _T("-ToFile\\From-") + this->OutPName, NULL);//����һ���ļ���
			}
		}
		else
		{
			if (!PathIsDirectory(m_pDoc->opening_filePath + _T("-ToFile\\From-") + this->OutPName))//�ж��ļ����Ƿ񴴽�
			{
				::CreateDirectory(m_pDoc->opening_filePath + _T("-ToFile\\From-") + this->OutPName, NULL);//����һ���ļ���
			}
		}
		
	}
	else
	{
		MessageBox(_T("���ȱ���ģ�ͣ�"));
	}*/
}


void CDlgChseOVlueTFile::OnBnClickedOk()
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


void CDlgChseOVlueTFile::OnEnChangeEditFilepath()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CDlgChseOVlueTFile::OnBnClicked_Chosefilepath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog m_saveDlg(FALSE);                                                   // ���������ļ��Ի������񣬵��������Ϊ���Ի���
	m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.csv)\0*.csv\0All Files(*.*)\0\0"; // ���ù�����
	m_saveDlg.m_ofn.lpstrDefExt = L"csv"; 

	if(IDOK == m_saveDlg.DoModal())
	{
		USES_CONVERSION;
		const char* filename = T2A(m_saveDlg.GetPathName());
		//csvfile.WriteCsvData(filename,this->vec_listOputDat);

		str_FilePath = m_saveDlg.GetPathName();             // ��ȡ�ļ�·��

		m_edtFilePath.SetWindowText(str_FilePath);          // ���ļ�·�����õ��༭��

	}
}
