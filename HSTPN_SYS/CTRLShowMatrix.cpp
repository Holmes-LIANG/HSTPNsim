// CTRLShowMatrix.cpp : 实现文件
//

#include "stdafx.h"
#include "HSTPN_SYS.h"
#include "CTRLShowMatrix.h"


// CCTRLShowMatrix

IMPLEMENT_DYNAMIC(CCTRLShowMatrix, CStatic)

CCTRLShowMatrix::CCTRLShowMatrix()
{
	m_pSMat_OutValue = NULL;
	OutPName = L"";
	i_PposnOValue = -1;
	m_pDatafile = NULL;
}

CCTRLShowMatrix::~CCTRLShowMatrix()
{
	if(m_pSMat_OutValue != NULL)
	{
		delete m_pSMat_OutValue;
		m_pSMat_OutValue = NULL;
	}
	if(m_pDatafile != NULL)
	{
		delete m_pDatafile;
		m_pDatafile = NULL;
	}
}


BEGIN_MESSAGE_MAP(CCTRLShowMatrix, CStatic)

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_MESSAGE_UPDATELIST1,UpdataList1)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_MD_DELETE3, &CCTRLShowMatrix::OnMdDelete)
END_MESSAGE_MAP()



// CCTRLShowMatrix 消息处理程序

// reWrite the virtual function of Create
BOOL CCTRLShowMatrix::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,enumOutputStyle outputstyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类
	this->OutputStyle = outputstyle;
	this->m_pParent = (CHSTPN_SYSView*)pParentWnd;

	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}

// reDraw the CStatic-ctrl
void CCTRLShowMatrix::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{

	// TODO:  添加您的代码以绘制指定项
	CRect rcWnd;  
    GetWindowRect(&rcWnd);  
    ScreenToClient(&rcWnd);  
    CString strText;  
    GetWindowText(strText);
	if(this->OutputStyle == OUT_SHOWMATRIX)
	{  
		CBitmap bmp;  
		bmp.LoadBitmap(IDB_BMPSHOWMATRIX2);  
		BITMAP bmpInfo;  
		bmp.GetBitmap(&bmpInfo);  
		CDC *pdc = GetWindowDC();  
		CDC dcMem;  
		dcMem.CreateCompatibleDC(pdc);  
		CBitmap bmpBg;  
		bmpBg.CreateCompatibleBitmap(pdc,rcWnd.Width(),rcWnd.Height());  
		dcMem.SelectObject(&bmpBg);  
  
		CDC dcDraw;  
		dcDraw.CreateCompatibleDC(pdc);  
		dcDraw.SelectObject(&bmp);  
		dcMem.StretchBlt(0,0,rcWnd.Width(),rcWnd.Height(),&dcDraw,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY);

		/*CFont font;  
		VERIFY(font.CreateFont(  
			12,                        // nHeight  
			0,                         // nWidth  
			0,                         // nEscapement  
			0,                         // nOrientation  
			FW_NORMAL,                 // nWeight  
			FALSE,                     // bItalic  
			FALSE,                     // bUnderline  
			0,                         // cStrikeOut  
			ANSI_CHARSET,              // nCharSet  
			OUT_DEFAULT_PRECIS,        // nOutPrecision  
			CLIP_DEFAULT_PRECIS,       // nClipPrecision  
			DEFAULT_QUALITY,           // nQuality  
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily  
			_T("宋体")));                 // lpszFacename  */
  
	   // dcMem.SelectObject(&font);  
	   // dcMem.SetBkMode(TRANSPARENT);  
	   // dcMem.SetTextColor(RGB(255,0,0));  
	   // dcMem.DrawText(strText,&rcWnd,DT_CENTER);  
     
		BLENDFUNCTION bf = {0};  
		bf.AlphaFormat = AC_SRC_OVER ;  
		bf.BlendFlags = 0;  
		bf.BlendOp = AC_SRC_OVER ;  
		bf.SourceConstantAlpha = 255;//125;  
		pdc->AlphaBlend(0,0,rcWnd.Width(),rcWnd.Height(),&dcMem,0,0,rcWnd.Width(),rcWnd.Height(),bf);  
		bmp.DeleteObject();  
		ReleaseDC(pdc);
	}
	else
	{
		CBitmap bmp;  
		bmp.LoadBitmap(IDB_BMPTOFILE);  
		BITMAP bmpInfo;  
		bmp.GetBitmap(&bmpInfo);  
		CDC *pdc = GetWindowDC();  
		CDC dcMem;  
		dcMem.CreateCompatibleDC(pdc);  
		CBitmap bmpBg;  
		bmpBg.CreateCompatibleBitmap(pdc,rcWnd.Width(),rcWnd.Height());  
		dcMem.SelectObject(&bmpBg);  
  
		CDC dcDraw;  
		dcDraw.CreateCompatibleDC(pdc);  
		dcDraw.SelectObject(&bmp);  
		dcMem.StretchBlt(0,0,rcWnd.Width(),rcWnd.Height(),&dcDraw,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY); 
     
		BLENDFUNCTION bf = {0};  
		bf.AlphaFormat = AC_SRC_OVER ;  
		bf.BlendFlags = 0;  
		bf.BlendOp = AC_SRC_OVER ;  
		bf.SourceConstantAlpha = 255;//125;  
		pdc->AlphaBlend(0,0,rcWnd.Width(),rcWnd.Height(),&dcMem,0,0,rcWnd.Width(),rcWnd.Height(),bf);  
		bmp.DeleteObject();  
		ReleaseDC(pdc);
	}
}


void CCTRLShowMatrix::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	DWORD dwStyle = GetStyle();  
    SetWindowLong(GetSafeHwnd(),GWL_STYLE,dwStyle | SS_OWNERDRAW  ); 
	CStatic::PreSubclassWindow();
}

// tell the View-class which model should be moved
void CCTRLShowMatrix::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	(AfxGetApp()->m_pMainWnd)->SendMessage(WM_MESSAGE_MOVEMODEL1,this->ID,3);//Lparm中的1表示模型为库所

	CStatic::OnLButtonDown(nFlags, point);
}

// double-cliced to show the matrix-show window
void CCTRLShowMatrix::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(this->OutputStyle == OUT_SHOWMATRIX)
	{
		if(m_pSMat_OutValue == NULL)
		{
			CHSTPN_SYSDoc* m_pDoc = ((CHSTPN_SYSDoc*)((CHSTPN_SYSView*)(this->GetParent()))->GetDocument());
			m_pSMat_OutValue = new CDlgSMat_OutValue();
			m_pSMat_OutValue->Create(IDD_DLGSHOWMATRIX,m_pDoc,this);
		
			m_pSMat_OutValue->OutPName = this->OutPName;
			m_pSMat_OutValue->vec_OutValueName = this->vec_OutValueName;
			m_pSMat_OutValue->i_PposnOValue = this->i_PposnOValue;
			m_pSMat_OutValue->vec_OutValue = this->vec_OutValue;
			/*vec_OutputData.clear();
			int nsize = vec_OutValueName.size();
			for(int i=0;i<nsize;i++)
			{
				VEC_DOUBLE* m_Value = new VEC_DOUBLE;
				vec_OutputData.push_back(m_Value);

			}*/
		}
		m_pSMat_OutValue->ShowWindow(SW_SHOW);
	}
	else
	{
		CHSTPN_SYSDoc* m_pDoc = ((CHSTPN_SYSDoc*)((CHSTPN_SYSView*)(this->GetParent()))->GetDocument());
		CDlgChseOVlueTFile m_dlg_ChooseValue(m_pDoc);
		m_dlg_ChooseValue.OutPName = this->OutPName;
		m_dlg_ChooseValue.vec_OutValue = this->vec_OutValue;
		m_dlg_ChooseValue.i_PposnOValue = this->i_PposnOValue;
		m_dlg_ChooseValue.str_FilePath = this->str_FilePath;

		/*if(m_pDatafile == NULL)//为m_pDatafile文件变量分配内存
		{
			m_pDatafile = new ofstream();
		}*/

		if(IDOK == m_dlg_ChooseValue.DoModal())
		{
			this->OutPName = m_dlg_ChooseValue.OutPName;
			this->vec_OutValue = m_dlg_ChooseValue.vec_OutValue;
			this->i_PposnOValue = m_dlg_ChooseValue.i_PposnOValue;
			if(this->str_FilePath != m_dlg_ChooseValue.str_FilePath)
			{
				if(m_pDatafile != NULL)
				{
					delete m_pDatafile;
					m_pDatafile = NULL;
				}
				USES_CONVERSION;
				const char* filename = T2A(m_dlg_ChooseValue.str_FilePath);

				m_pDatafile = new ofstream(filename);
				
				this->str_FilePath = m_dlg_ChooseValue.str_FilePath;
			}
			if(str_FilePath == "")
			{
				MessageBox(_T("未选择文件保存路径！"));
			}
			
			this->m_caption = L"From-" + OutPName;
			m_pParent->Invalidate();
			m_pParent->UpdateWindow();
		}
	}

	CStatic::OnLButtonDblClk(nFlags, point);
}

LRESULT CCTRLShowMatrix::UpdataList1(WPARAM wParam, LPARAM lParam)
{
	OutPName = m_pSMat_OutValue->OutPName;
	vec_OutValue = m_pSMat_OutValue->vec_OutValue;
	vec_OutValueName = m_pSMat_OutValue->vec_OutValueName;
	i_PposnOValue =m_pSMat_OutValue->i_PposnOValue;

	// update the m_caption of outputmodel
	this->m_caption = L"From-" + OutPName;
	m_pParent->Invalidate();
	m_pParent->UpdateWindow();

	// if vec_Output is not empty, release the element of vec_Output,first.
	for(vector<VEC_DOUBLE*>::iterator itervv=vec_OutputData.begin();itervv!=vec_OutputData.end();itervv++)
	{
		for(vector<double*>::iterator iterv=(*itervv)->begin();iterv!=(*itervv)->end();iterv++)
		{
			delete (*iterv);
		}
	}
	// the bulid the struct of vec_OutputData
	vec_OutputData.clear();
	int nsize = vec_OutValueName.size();
	for(int i=0;i<nsize;i++)
	{
		VEC_DOUBLE* m_Value = new VEC_DOUBLE;
		vec_OutputData.push_back(m_Value);

	}
	return 0;
}



void CCTRLShowMatrix::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值IDR_MDEDIT_POPUP3
	CMenu m_MDeditpopup;
	m_MDeditpopup.LoadMenuW(IDR_MDEDIT_POPUP3);

	ClientToScreen(&point);

	m_MDeditpopup.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);//GetParent());//创建一个浮动的菜单
	//CStatic::OnRButtonUp(nFlags, point);
}

// 删除操作
void CCTRLShowMatrix::OnMdDelete()
{
	// TODO: 在此添加命令处理程序代码
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_MDDELETE1,this->ID,3);
}
