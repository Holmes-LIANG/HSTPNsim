#pragma once
#include "afxcmn.h"
#include "HSTPN_Message.h"
#include "Place.h"
#include "Transition.h"
#include "afxwin.h"
// CDlgModelParmList 对话框

#include "HSTPN_SYSDoc.h"
class CHSTPN_SYSDoc;

class CDlgModelParmList : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModelParmList)

public:
	CDlgModelParmList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgModelParmList();

// 对话框数据
	enum { IDD = IDD_DLG_DATALIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	CTreeCtrl           m_treeListofModel;      // 用于累出所有的库所，目的是设置其中的变量列表
	bool                b_isInitTree;
	HTREEITEM           hRoot;                  // 树的根节点的句柄

	CListCtrl           m_DataListAll;          // 用于声明库所或全局的变量,全局变量表
	CImageList          m_imageList;
	CEdit               m_edtvaluename1;        // 全局变量变量名控件变量
	CString             m_strvaluename1;        // 全局变量变量名str
	CEdit               m_edtValueInt1;         // 全局变量的初始值编辑框变量
	double              m_dblValueInt;          // 全局变量的初始值
	//vector<IOValue*>    arryIOputDataG;         // 全局变量表
	int                 m_rowforDG;             // m_row为m_List_IOput的行号,删除操作时用的
    int                 m_columnforDG;          // m_column为m_List_IOput的列号,删除操作时用的
	int                 m_rowG;                 // m_row为m_List_IOput的行号
    int                 m_columnG;              // m_column为m_List_IOput的列号

	afx_msg void        OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT     OnUpdateTreeofModel(WPARAM wParam, LPARAM lParam);
	virtual BOOL        OnInitDialog();
	afx_msg void        OnBtnaAddValueG();          // 全局表下，点击“添加”按钮
	afx_msg void        OnBtnDltValueG();           // 全局表下，点击“删除”按钮
	afx_msg void        OnClick_ListG(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void        OnDblclk_ListG(NMHDR *pNMHDR, LRESULT *pResult);
	void                InitCtrl_GetIO(int rowofchart, int columnofchart, CRect rc);
	afx_msg void        OnGetValueNameG();
	afx_msg void        OnGetValueIntG();

	afx_msg void        OnNMClick_ChoseNode(NMHDR *pNMHDR, LRESULT *pResult);


	CStatic             m_staticG;              // 全局变量表的名字表头
	CStatic             m_staticL;              // 局部变量表的名字表头
	CButton             m_btnAddG;              // 全局变量表下的添加按钮
	CButton             m_btnDeleteG;           // 全局变量表下的删除按钮
	CButton             m_btnAddL;              // 局部变量表下的添加按钮
	CButton             m_btnDeleteL;           // 局部变量表下的删除按钮

	CPlace *            p_Model;                // 显式的保存对应Model对象的指针

	afx_msg void        OnNMClick_ListLocal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void        OnNMDblclk_ListLocal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void        OnBtnaAddValueL();
	afx_msg void        OnBtnDltValueL();

	CListCtrl           m_ListLocal;            // 局部变量表控件
	CEdit               m_edtValueNameL;        // 局部变量表中局部变量名编辑框控件
	CComboBox           m_cmbSquenceL;          // 局部变量表中序号下拉列表框控件
	CComboBox           m_cmbIOtypeL;           // 局部变量表中IOType选择下拉列表框控件
	CComboBox           m_cmbIsQuoteG_L;        // 局部变量表中是否引用全局变量控制
	CComboBox           m_cmbGlobalNameL;       // 局部变量表中全局变量选择下拉列表控件
	CComboBox           m_cmbDataSourceL;       // 局部变量表中数据源库所选择下拉列表框控件
	CComboBox           m_cmbSourceValueL;      // 局部变量表中数据源变量选择下拉列表框控件
	CEdit               m_edtValueIntL;         // 局部变量表中变量初始值输入编辑框控件
	
	int                 m_rowforDL;             // m_row为m_List_IOput的行号,删除操作时用的
    int                 m_columnforDL;          // m_column为m_List_IOput的列号,删除操作时用的
	int                 m_rowL;                 // m_row为m_List_IOput的行号
    int                 m_columnL;              // m_column为m_List_IOput的列号

	void                InitCtrl_GetIOL(int rowofchart, int columnofchart, CRect rc);
	
	void reShowListLocal();// 刷新显示局部变量表
	
	afx_msg void OnBtnOK();
	afx_msg void OnGetVluName_L();
	afx_msg void OnGetSquence_L();
	afx_msg void OnGetIOtype_L();
	afx_msg void OnGetIsQuote_L();
	afx_msg void OnGetGlobalVlu_L();
	afx_msg void OnGetDataSource_L();
	afx_msg void OnGetDataSourceValue_L();
	afx_msg void OnGetValueInt_L();

	CHSTPN_SYSDoc * p_Doc;
	afx_msg void OnPaint();
	void reShowListGlobal();
};
