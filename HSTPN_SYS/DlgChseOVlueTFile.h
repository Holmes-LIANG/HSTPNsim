#pragma once

#include "HSTPN_SYSDoc.h"
#include "afxwin.h"
#include "afxcmn.h"
class CHSTPN_SYSDoc;
// CDlgChseOVlueTFile 对话框

class CDlgChseOVlueTFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChseOVlueTFile)

public:
	CDlgChseOVlueTFile(CWnd* pParent = NULL);   // 标准构造函数
	CDlgChseOVlueTFile(CHSTPN_SYSDoc* pdoc, CWnd* pParent = NULL);
	virtual ~CDlgChseOVlueTFile();

// 对话框数据
	enum { IDD = IDD_DLGTOFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	
public:
	//----------成员变量---------------
	CHSTPN_SYSDoc      * m_pDoc;
	CComboBox            m_cmbPlace;       // combox 控件变量，用于提供用户选择库所的caption
	HTREEITEM            hRoot;            // 树的根节点的句柄
	vector<int>          vec_OutValue;     // 用于保存选中的输出值在arryIOputData中的位置
	CString              OutPName;         // 用于保存选中的输出库所的名字
	vector<int>          vec_subMap;       // 保存要输出的变量在arryIOputData中的位置
	int                  i_PposnOValue;    // 用于保存选中的库所的在vector_Places中的位置
	CTreeCtrl            m_treePOutValue;  // 库所输出变量的树形列表
	CEdit                m_edtFilePath;    // 编辑框变量，获取或显示要保存文件的文件名及路径

	CString               str_FilePath;     // 包含的文件的路径


public:
	//----------成员函数---------------

	
	
	virtual BOOL OnInitDialog();
	afx_msg void OnCmbSelchange_Chooseplace();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditFilepath();
	afx_msg void OnBnClicked_Chosefilepath();
	
	
};
