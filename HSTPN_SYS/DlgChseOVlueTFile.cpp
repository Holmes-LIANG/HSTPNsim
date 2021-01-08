// DlgChseOVlueTFile.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "DlgChseOVlueTFile.h"
//#include "afxdialogex.h"


// CDlgChseOVlueTFile 对话框

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


// CDlgChseOVlueTFile 消息处理程序


BOOL CDlgChseOVlueTFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_edtFilePath.SetWindowText(str_FilePath);

	int i_place = 1;
	m_cmbPlace.ResetContent();                     // 初始化“库所项”对应的下拉列表框
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
	if(OutPName == _T("全局变量"))
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

	// 将TreeCtrl的相应节点设置回选中状态
	vector<int> vec_OutValueTs;  // 先将vec_OutValue中保存的到arryIOputData中的映射，转换为到arryOutputData中的映射
	vec_OutValueTs.clear();
	if(this->OutPName == _T("全局变量")) // 如果输出的是全局变量
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
	else // 如果输出的是局部变量
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
	HTREEITEM   hChildItem = m_treePOutValue.GetChildItem(hRoot);// 获取第一个子节点，即获取第一个输出值
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
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgChseOVlueTFile::OnCmbSelchange_Chooseplace()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_cmbPlace.GetCurSel();  
	m_cmbPlace.SetCurSel(nSel);                     // 先设置选中项为当前项
	m_cmbPlace.GetWindowText(OutPName);        // 然后获取当前项的内容
	if(this->OutPName == _T("全局变量"))
	{
		this->i_PposnOValue = 0;
	}
	else
	{
		//----------获取OutPName表示的库所在Vector_Place 中的位置
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
	//----------初始化TreeControl-------------------
	m_treePOutValue.DeleteAllItems();
	HTREEITEM hChild;
	hRoot = m_treePOutValue.InsertItem(_T("输出量"),TVI_ROOT);
	if(this->OutPName == _T("全局变量"))
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
				int posninList = 0;//每个参数在arryIOputData中的位置
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

	//-- 在hst文件所在路径下创建一个同名的文件夹
/*	if(m_pDoc->opening_filePath != "")// 检验m_pDoc->opening_filePath是否为空，若不为空，这说明文件已保存
	{
		if (!PathIsDirectory(m_pDoc->opening_filePath + _T("-ToFile")))
		{
			::CreateDirectory(m_pDoc->opening_filePath + _T("-ToFile"),NULL);
			if (!PathIsDirectory(m_pDoc->opening_filePath + _T("-ToFile\\From-") + this->OutPName))//判断文件夹是否创建
			{
				::CreateDirectory(m_pDoc->opening_filePath + _T("-ToFile\\From-") + this->OutPName, NULL);//创建一个文件夹
			}
		}
		else
		{
			if (!PathIsDirectory(m_pDoc->opening_filePath + _T("-ToFile\\From-") + this->OutPName))//判断文件夹是否创建
			{
				::CreateDirectory(m_pDoc->opening_filePath + _T("-ToFile\\From-") + this->OutPName, NULL);//创建一个文件夹
			}
		}
		
	}
	else
	{
		MessageBox(_T("请先保存模型！"));
	}*/
}


void CDlgChseOVlueTFile::OnBnClickedOk()
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


void CDlgChseOVlueTFile::OnEnChangeEditFilepath()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CDlgChseOVlueTFile::OnBnClicked_Chosefilepath()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog m_saveDlg(FALSE);                                                   // 创建保存文件对话框，若否，弹出“另存为”对话框
	m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.csv)\0*.csv\0All Files(*.*)\0\0"; // 设置过滤器
	m_saveDlg.m_ofn.lpstrDefExt = L"csv"; 

	if(IDOK == m_saveDlg.DoModal())
	{
		USES_CONVERSION;
		const char* filename = T2A(m_saveDlg.GetPathName());
		//csvfile.WriteCsvData(filename,this->vec_listOputDat);

		str_FilePath = m_saveDlg.GetPathName();             // 获取文件路径

		m_edtFilePath.SetWindowText(str_FilePath);          // 将文件路径设置到编辑框

	}
}
