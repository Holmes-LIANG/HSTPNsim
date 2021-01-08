// ArrowEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "ArrowEdit.h"
#include "Arrow.h"

class CArrow;
// CArrowEdit

IMPLEMENT_DYNAMIC(CArrowEdit, CEdit)

CArrowEdit::CArrowEdit()
{

}

CArrowEdit::~CArrowEdit()
{
}


BEGIN_MESSAGE_MAP(CArrowEdit, CEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, &CArrowEdit::OnEnChange)
END_MESSAGE_MAP()



// CArrowEdit 消息处理程序




void CArrowEdit::OnEnChange()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CEdit::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString str;
	this->GetWindowText(str);
	int a = _ttoi(str);

	this->m_pArrowBelong->num = a;
}


BOOL CArrowEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CArrow * _arrow)
{
	// TODO: 在此添加专用代码和/或调用基类
	this->m_pArrowBelong = _arrow;
	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}
