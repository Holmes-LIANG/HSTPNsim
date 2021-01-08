#pragma once

#include "HSTPN_Message.h"
#include "DlgSMat_OutValue.h"
class CDlgSMat_OutValue;
#include "HSTPN_SYSView.h"

class CHSTPN_SYSView;


#include "DlgChseOVlueTFile.h"
class CDlgChseOVlueTFile;

class CCTRLShowMatrix : public CStatic
{
	DECLARE_DYNAMIC(CCTRLShowMatrix)

public:
	CCTRLShowMatrix();
	virtual ~CCTRLShowMatrix();

protected:
	DECLARE_MESSAGE_MAP()
public:
	//=================成员变量================================
	UINT                         ID;                // the ID of OutputModel
	CString                      m_caption;         // the name of OutputModel
	CPoint                       Position;          // the Position of OutputModel in View;
	enumOutputStyle              OutputStyle;       // the Style of OutputModel
	CDlgSMat_OutValue         *  m_pSMat_OutValue;  // the dialog to show OutValue
	vector<int>                  vec_OutValue;      // 用于保存选中的输出值在arryIOputData中的位置,vec_OutValue中的值为1则表示对应位置上的arryIOputData的值被设定为输出，要被记录
	CString                      OutPName;          // 用于保存选中的输出库所的名字
	vector<CString>              vec_OutValueName;  // 将vec_OutValue中元素值为1的变量名保存起来
	CHSTPN_SYSView            *  m_pParent;         // the pointor of Parent-class ,that is View-class
	int                          i_PposnOValue;     // 用于保存选中的库所的在vector_Places中的位置
	vector<VEC_DOUBLE*>          vec_OutputData;    // the output data of outputModel

	//CDlgChseOVlueTFile        * m_p_OVlueTFile; // the dialog to write the output data to file

	ofstream                   * m_pDatafile;      // 每个OUT_TOFILE类型的输出模块都包含文件
	CString                      str_FilePath;     // 包含的文件的路径

public:
	//================成员函数=================================
	virtual BOOL    Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, enumOutputStyle outputstyle, UINT nID = 0xffff);

	virtual void    DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void    PreSubclassWindow();
	afx_msg void    OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void    OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT UpdataList1(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMdDelete();
};


