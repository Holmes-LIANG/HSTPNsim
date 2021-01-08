#pragma once

#include "Arrow.h"
class CArrow;

class CArrowEdit : public CEdit
{
	DECLARE_DYNAMIC(CArrowEdit)

public:
	CArrowEdit();
	virtual ~CArrowEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChange();
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CArrow * arrow);
	CArrow * m_pArrowBelong;
};


