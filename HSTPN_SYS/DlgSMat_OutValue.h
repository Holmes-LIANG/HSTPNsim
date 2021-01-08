#pragma once
#include "HSTPN_Message.h"
#include <vector>
using namespace std;
#include "DlgChooseOValue.h"
#include "HSTPN_SYSDoc.h"
#include "afxcmn.h"
class CHSTPN_SYSDoc;

class CCTRLShowMatrix;

#include "CSVStream.h"

// CDlgSMat_OutValue 对话框

class CDlgSMat_OutValue : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSMat_OutValue)

public:
	CDlgSMat_OutValue(CWnd* pParent = NULL);   // 标准构造函数
	CDlgSMat_OutValue(CHSTPN_SYSDoc* m_pdoc, CWnd* pParent = NULL);
	virtual ~CDlgSMat_OutValue();

// 对话框数据
	enum { IDD = IDD_DLGSHOWMATRIX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//-----------成员变量-------------------
	CHSTPN_SYSDoc       * m_pDoc;
	vector<int>           vec_OutValue;     // 用于保存选中的输出值在arryIOputData中的位置,vec_OutValue中的值为1则表示对应位置上的arryIOputData的值被设定为输出，要被记录
	CString               OutPName;         // 用于保存选中的输出库所的名字
	CListCtrl             m_Chart_SMatrix;  // 用于展示矩阵变量的ListControl
	vector<CString>       vec_OutValueName; // 将vec_OutValue中元素值为1的变量名保存起来
	CCTRLShowMatrix     * p_Parent;         // 保存对话框的父窗口的指针
	int                   i_PposnOValue;    // 用于保存选中的库所的在vector_Places中的位置
	vector<VEC_DOUBLE*>   vec_OutputData;   // the output data of outputModel
	vector<list<string> > vec_listOputDat;  // 将vec_OutputData中的数据转换成string类型
public:
	afx_msg void OnSmatSet();
	virtual BOOL Create(UINT nIDTemplate,CHSTPN_SYSDoc* m_pdoc, CWnd* pParentWnd = NULL);
	
	
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void SetChartHead(CString Pname, vector<int> vec_outvalue);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSavemat();
};
