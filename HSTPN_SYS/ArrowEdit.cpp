// ArrowEdit.cpp : ʵ���ļ�
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



// CArrowEdit ��Ϣ�������




void CArrowEdit::OnEnChange()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CEdit::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	this->GetWindowText(str);
	int a = _ttoi(str);

	this->m_pArrowBelong->num = a;
}


BOOL CArrowEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CArrow * _arrow)
{
	// TODO: �ڴ����ר�ô����/����û���
	this->m_pArrowBelong = _arrow;
	return CEdit::Create(dwStyle, rect, pParentWnd, nID);
}
