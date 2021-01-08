
// HSTPN_SYSDoc.cpp : CHSTPN_SYSDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "HSTPN_SYS.h"
#endif

#include "HSTPN_SYSDoc.h"

#include <propkey.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHSTPN_SYSDoc

IMPLEMENT_DYNCREATE(CHSTPN_SYSDoc, CDocument)

BEGIN_MESSAGE_MAP(CHSTPN_SYSDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CHSTPN_SYSDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CHSTPN_SYSDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CHSTPN_SYSDoc::OnFileSaveAs)
	ON_COMMAND(ID_FILE_NEW, &CHSTPN_SYSDoc::OnFileNew)
	ON_COMMAND(ID_DEBUG_START, &CHSTPN_SYSDoc::OnDebugStart)
	ON_COMMAND(IDT_MODELCHECK, &CHSTPN_SYSDoc::OnModelcheck)
	ON_COMMAND(ID_SIM_SUSPEND, &CHSTPN_SYSDoc::OnSimThreadSuspend)
	ON_COMMAND(ID_SIM_STOP, &CHSTPN_SYSDoc::OnSimThreadStop)
	ON_COMMAND(IDT_INCDMATANALY, &CHSTPN_SYSDoc::OnIncdmatanaly)
	ON_COMMAND(WM_MESSAGE_SCENE, &CHSTPN_SYSDoc::OnFileSceneOne)
END_MESSAGE_MAP()

// CHSTPN_SYSDoc ����/����

CHSTPN_SYSDoc::CHSTPN_SYSDoc()
{
	// TODO: �ڴ����һ���Թ������
	m_hstpnSim          = new CHSTPN_SYSSim();
	m_dlgModelCheck     = NULL;

	m_simThread         = NULL;
	b_SimSuspend        = FALSE;

	m_StopSimEvent      = new CEvent();

	m_IncdMatThread     = NULL;

	rectofModel.left    = 0;
	rectofModel.top     = 0;
	rectofModel.right   = 35;
	rectofModel.bottom  = 35;

	rectofTrans.left    = 0;
	rectofTrans.top     = 0;
	rectofTrans.right   = 25;
	rectofTrans.bottom  = 25;

	rectofOutputModel.left   = 0;
	rectofOutputModel.top    = 0;
	rectofOutputModel.right  = 70;
	rectofOutputModel.bottom = 35;

	opening_filePath         = "";

//	d_TimeStep               = 0.1;
//	d_SimTime                = DBL_MAX;
//	d_IntervalTime           = 200;

}

CHSTPN_SYSDoc::~CHSTPN_SYSDoc()
{
	if(m_hstpnSim!=NULL)
	{
	delete m_hstpnSim;
	m_hstpnSim = NULL;
	}

	if(m_dlgModelCheck!=NULL)
	{
		m_dlgModelCheck = NULL;
	}

	if(m_StopSimEvent!=NULL)
	{
		delete m_StopSimEvent;
		m_StopSimEvent = NULL;
	}

	if(m_simThread!=NULL)
	{
		m_simThread = NULL;
	}
	if(m_IncdMatThread!=NULL)
	{
		m_IncdMatThread=NULL;
	}

	if(this->arryIOputDataG.size()>0)
	{
		for(vector<IOValue*>::iterator iter=this->arryIOputDataG.begin();iter!=this->arryIOputDataG.end();iter++)
		{
			delete (*iter);
		}
	}
	
}

BOOL CHSTPN_SYSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CHSTPN_SYSDoc ���л�

void CHSTPN_SYSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CHSTPN_SYSDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CHSTPN_SYSDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CHSTPN_SYSDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CHSTPN_SYSDoc ���

#ifdef _DEBUG
void CHSTPN_SYSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHSTPN_SYSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHSTPN_SYSDoc ����

// �򿪲���
void CHSTPN_SYSDoc::OnFileOpen()
{
	// TODO: �ڴ���������������
	CHSTPN_SYSView* m_pHSTView;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);

	//====================�򿪲���=========================================================begin=========
	int n_SizeP = Vector_Place.size();
	int n_SizeT = Vector_Trans.size();
	int n_SizeA = arr_Arrow.GetSize();
	int n_SizeO = Vector_OutputModel.size();
	if(n_SizeP!=0 || n_SizeT!=0 || n_SizeA!=0 || n_SizeO!=0)                     // �����жϵ�ǰ��ͼ�Ƿ�Ϊ�գ������Ϊ�գ������û����б���;�����ͼΪ�գ���ôֱ�ӵ������򿪶Ի���
	{
		if(m_pHSTView->myDocument==NULL || m_pHSTView->bPNisChanged)
		{
			CDlgSaveNote m_dlgSaveNote;                                          // ���û�������½�����ťʱ���ȵ���һ�����ѶԻ���
			int i_btn = m_dlgSaveNote.DoModal();
			if(1 == i_btn)                                                       // #define IDOK 1   ���û�ѡ�񡰱��桱��ťʱ
			{
				// 1.���ȱ����ļ�
				if(m_pHSTView->myDocument==NULL)                                 // �жϷ����ļ��Ƿ�����������
				{
					CFileDialog m_saveDlg(FALSE);                                // ���������ļ��Ի������񣬵��������Ϊ���Ի���
					m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//���ù�����
					m_saveDlg.m_ofn.lpstrDefExt = L"hst";                        //���ȱʡ����չ��
					if(IDOK == m_saveDlg.DoModal())
					{
						m_pHSTView->myDocument = new TiXmlDocument();
						WriteXMLFile(m_saveDlg.GetPathName(),m_pHSTView->myDocument);
					}
				}
				else                                                             // ���ѱ����棬��ֱ��д��
				{
				//1019	TiXmlElement *RootElement = m_pHSTView->myDocument->RootElement();// ��ȡ�ļ��ĸ�Ԫ��
				//1019	TiXmlAttribute *attribute1 = RootElement->FirstAttribute();  // ��ȡ��Ԫ�صĵ�һ�����ԣ����ļ�·����
				//1019	const char *filepathname = attribute1->Value();
				//1019	CString str_filepathname(filepathname);                      // ��ȡ�ļ��б�����ļ���
					if(m_pHSTView->myDocument!=NULL)
					{
						m_pHSTView->myDocument->Clear();
					}
					delete m_pHSTView->myDocument;                               // ��ʱӦ���ͷ�myDocument���ڴ棬��Ϊ����Ҫ���������µ��ڴ棬��������䣬���ݻᱻ�ظ�д�뵽�ļ��У����·���Ļ����൱������д��
					m_pHSTView->myDocument = new TiXmlDocument();
					WriteXMLFile(/*str_filepathname*/this->opening_filePath,m_pHSTView->myDocument);       // д���ļ�
				}
				//2.�����ͼ��
				DeletePlcTrnsArcs(m_pHSTView);                                   // Ȼ�������ͼ�����е���Ϣ��Vector_Place��Vector_Trans��arr_Arrow����������ʼ��
		
				m_pHSTView->Invalidate();
				m_pHSTView->UpdateWindow();

				//3. ���ļ�
				CFileDialog m_saveDlg(TRUE);                                     // �������ļ��Ի���
				m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//���ù�����
				m_saveDlg.m_ofn.lpstrDefExt = L"hst";                            // ���ȱʡ����չ��

				CString str_filename;
				if(IDOK == m_saveDlg.DoModal())
				{
					m_pHSTView->myDocument = ReadXMLFile(m_saveDlg.GetPathName(),m_pHSTView->myDocument,m_pHSTView);
					this->opening_filePath = m_saveDlg.GetPathName();
					str_filename = m_saveDlg.GetPathName();
					((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetWindowText(str_filename + _T("-HSTPNSim"));
					
					m_pHSTView->bPNisChanged = FALSE;
				}
			}

			if(3 == i_btn)                                                       // #define IDCANCEL 2   ���û�ѡ�񡰲����桱��ťʱ
			{
				//1.�����ͼ
				DeletePlcTrnsArcs(m_pHSTView);                                   // ֱ�������ͼ�����е���Ϣ��Vector_Place��Vector_Trans��arr_Arrow// �������ָ���ʼ��
		
				m_pHSTView->Invalidate();
				m_pHSTView->UpdateWindow();

				//2. ���ļ�
				CFileDialog m_saveDlg(TRUE);                                     // �������ļ��Ի���
				m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//���ù�����
				m_saveDlg.m_ofn.lpstrDefExt = L"hst";                            // ���ȱʡ����չ��

				CString str_filename;
				if(IDOK == m_saveDlg.DoModal())
				{
					m_pHSTView->myDocument = ReadXMLFile(m_saveDlg.GetPathName(),m_pHSTView->myDocument,m_pHSTView);
					this->opening_filePath = m_saveDlg.GetPathName();
					str_filename = m_saveDlg.GetPathName();
					((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetWindowText(str_filename + _T("-HSTPNSim"));
					
					m_pHSTView->bPNisChanged = FALSE;
				}
			}
			                                                                     // ���û�ѡ��Ĳ��ǡ����桱�͡������桱ʱ��ʲô��������������ͼ��Ϊԭ����״̬
		}
		else
		{
			//1.�����ͼ
			DeletePlcTrnsArcs(m_pHSTView);                                       // ֱ�������ͼ�����е���Ϣ��Vector_Place��Vector_Trans��arr_Arrow// �������ָ���ʼ��
		
			m_pHSTView->Invalidate();
			m_pHSTView->UpdateWindow();
			//2. ���ļ�
			CFileDialog m_saveDlg(TRUE);                                         // �������ļ��Ի���
			m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//���ù�����
			m_saveDlg.m_ofn.lpstrDefExt = L"hst";                                // ���ȱʡ����չ��

			CString str_filename;
			if(IDOK == m_saveDlg.DoModal())
			{
				m_pHSTView->myDocument = ReadXMLFile(m_saveDlg.GetPathName(),m_pHSTView->myDocument,m_pHSTView);
				this->opening_filePath = m_saveDlg.GetPathName();
				str_filename = m_saveDlg.GetPathName();
				((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetWindowText(str_filename + _T("-HSTPNSim"));

				m_pHSTView->bPNisChanged = FALSE;
			}
		}

	}
	else                                                                         // �����ͼΪ�գ�ֱ�ӵ������򿪡��Ի���
	{
		CFileDialog m_saveDlg(TRUE);                                             // �������ļ��Ի���
		m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//���ù�����
		m_saveDlg.m_ofn.lpstrDefExt = L"hst";                                    // ���ȱʡ����չ��

		CString str_filename;

		if(IDOK == m_saveDlg.DoModal())
		{
			m_pHSTView->myDocument = ReadXMLFile(m_saveDlg.GetPathName(),m_pHSTView->myDocument,m_pHSTView);
			this->opening_filePath = m_saveDlg.GetPathName();
			str_filename = m_saveDlg.GetPathName();
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetWindowText(str_filename + _T("-HSTPNSim"));

			m_pHSTView->bPNisChanged = FALSE;
		}

		
	}

	m_pHSTView->Invalidate();
	m_pHSTView->UpdateWindow();
	m_pHSTView->Invalidate();
	m_pHSTView->UpdateWindow();
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_UPDATETREE1,0,0);// Ŀ�ķ�����mainframe���ٷ�����dlgmodelchoose1��֪ͨHSTPN�ṹ������
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pParmListDlg->SendMessage(WM_MESSAGE_UPDATETREEOFMODEL,0,0);// ��ģ�ͱ༭������WM_MESSAGE_UPDATETREEOFMODEL��Ϣ������ģ����
	//====================�򿪲���===========================================================end=========

}

// �������
void CHSTPN_SYSDoc::OnFileSave()
{
	// TODO: �ڴ���������������
	CHSTPN_SYSView* m_pHSTView;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);                                 // �ڵ��Թ����з��֣���ʵ���еĽṹ�����ĵ�Ӧ�ö�Ӧ��������ͼ����һ����
	//====================�������=========================================================begin=========
	if(m_pHSTView->myDocument==NULL)                                                // �жϷ����ļ��Ƿ�����������
	{
		CFileDialog m_saveDlg(FALSE);                                               // ���������ļ��Ի������񣬵��������Ϊ���Ի���
		m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//���ù�����
		m_saveDlg.m_ofn.lpstrDefExt = L"hst";                                       // ���ȱʡ����չ��
		if(IDOK == m_saveDlg.DoModal())
		{
			m_pHSTView->myDocument = new TiXmlDocument();
			this->opening_filePath = m_saveDlg.GetPathName();

			WriteXMLFile(this->opening_filePath,m_pHSTView->myDocument);

			m_pHSTView->bPNisChanged = FALSE;
		}
	}
	else                                                                             // ���ѱ����棬��ֱ��д��
	{
//1019		TiXmlElement *RootElement = m_pHSTView->myDocument->RootElement();           // ��ȡ�ļ��ĸ�Ԫ��
//1019		TiXmlAttribute *attribute1 = RootElement->FirstAttribute();                  // ��ȡ��Ԫ�صĵ�һ�����ԣ����ļ�·����
//1019		const char *filepathname = attribute1->Value();
//1019		CString str_filepathname(filepathname);                                      // ��ȡ�ļ��б�����ļ���
		if(m_pHSTView->myDocument!=NULL)
		{
			m_pHSTView->myDocument->Clear();
		}
		delete m_pHSTView->myDocument;                                               // ��ʱӦ���ͷ�myDocument���ڴ棬��Ϊ����Ҫ���������µ��ڴ棬��������䣬���ݻᱻ�ظ�д�뵽�ļ��У����·���Ļ����൱������д��
		m_pHSTView->myDocument = new TiXmlDocument();
		WriteXMLFile(this->opening_filePath/*str_filepathname*/,m_pHSTView->myDocument);                       // д���ļ�

		m_pHSTView->bPNisChanged = FALSE;

	}
	//====================�������=========================================================end===========

}

// ���Ϊ����
void CHSTPN_SYSDoc::OnFileSaveAs()
{
	// TODO: �ڴ���������������
	CHSTPN_SYSView* m_pHSTView;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);
	//====================���Ϊ����=======================================================begin===========
	CFileDialog m_saveDlg(FALSE);                                                   // ���������ļ��Ի������񣬵��������Ϊ���Ի���
	m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0"; // ���ù�����
	m_saveDlg.m_ofn.lpstrDefExt = L"hst";                                           // ���ȱʡ����չ��
	if(IDOK == m_saveDlg.DoModal())
	{
		TiXmlDocument *myDocument1 = new TiXmlDocument();                           // ����������Ϊ����Ӧ����������һ�����󣬶�������ԭ���Ķ�����Ϊ�����ԭ���Ķ����ٴε������ʱ�����ֵ�ǰ�ĵ��Ѿ���ɡ����Ϊ������ĵ���
		WriteXMLFile(m_saveDlg.GetPathName(),myDocument1);
		if(myDocument1!=NULL)
		{
			m_pHSTView->myDocument->RootElement()->Clear();
		}
		delete myDocument1;
	}
	//====================���Ϊ����=========================================================end===========
}

// �½�����
void CHSTPN_SYSDoc::OnFileNew()
{
	// TODO: �ڴ���������������
	CHSTPN_SYSView* m_pHSTView;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);

	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetWindowText(_T("New file-HSTPNSim"));

	//====================�½�����=========================================================begin=========
	// ���û�������½�����ťʱ�����ȵ�����ʾ�û�����ĶԻ��򣬸����û�ѡ�����
	int n_SizeP = Vector_Place.size();
	int n_SizeT = Vector_Trans.size();
	int n_SizeA = arr_Arrow.GetSize();
	int n_SizeO = Vector_OutputModel.size();
	// �����жϵ�ǰ��ͼ�Ƿ�Ϊ�գ�����Ϊ�գ������û�����
	if(n_SizeP!=0 || n_SizeT!=0 || n_SizeA!=0 || n_SizeO!=0)
	{
		if(m_pHSTView->myDocument==NULL || m_pHSTView->bPNisChanged)
		{
			CDlgSaveNote m_dlgSaveNote;                                                      // ���û�������½�����ťʱ���ȵ���һ�����ѶԻ���
			int i_btn = m_dlgSaveNote.DoModal();

			if(1 == i_btn)                                                                   // #define IDOK 1   ���û�ѡ�񡰱��桱��ťʱ
			{
				// 1.���ȱ����ļ�
				if(m_pHSTView->myDocument==NULL)                                             // �жϷ����ļ��Ƿ�����������
				{
					CFileDialog m_saveDlg(FALSE);                                            // ���������ļ��Ի������񣬵��������Ϊ���Ի���
					m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//���ù�����
					m_saveDlg.m_ofn.lpstrDefExt = L"hst";                                    // ���ȱʡ����չ��
					if(IDOK == m_saveDlg.DoModal())
					{
						m_pHSTView->myDocument = new TiXmlDocument();
						
						WriteXMLFile(m_saveDlg.GetPathName(),m_pHSTView->myDocument);
					}
				}
				else                                                                        // ���ѱ����棬��ֱ��д��
				{
				//1019	TiXmlElement *RootElement = m_pHSTView->myDocument->RootElement();      // ��ȡ�ļ��ĸ�Ԫ��
				//1019	TiXmlAttribute *attribute1 = RootElement->FirstAttribute();             // ��ȡ��Ԫ�صĵ�һ�����ԣ����ļ�·����
				//1019	const char *filepathname = attribute1->Value();
				//1019	CString str_filepathname(filepathname);                                 // ��ȡ�ļ��б�����ļ���
					if(m_pHSTView->myDocument!=NULL)
					{
						m_pHSTView->myDocument->Clear();
					}
					delete m_pHSTView->myDocument;                                          //��ʱӦ���ͷ�myDocument���ڴ棬��Ϊ����Ҫ���������µ��ڴ棬��������䣬���ݻᱻ�ظ�д�뵽�ļ��У����·���Ļ����൱������д��
					m_pHSTView->myDocument = new TiXmlDocument();
					WriteXMLFile(/*str_filepathname*/this->opening_filePath,m_pHSTView->myDocument);                  // д���ļ�
				}
				//2.�����ͼ��
				DeletePlcTrnsArcs(m_pHSTView);                                              //Ȼ�������ͼ�����е���Ϣ��Vector_Place��Vector_Trans��arr_Arrow����������ʼ��
			}
		
			if(3 == i_btn)                                                                  // #define IDCANCEL 1   ���û�ѡ�񡰲����桱��ťʱ
			{
				DeletePlcTrnsArcs(m_pHSTView);                                              //ֱ�������ͼ�����е���Ϣ��Vector_Place��Vector_Trans��arr_Arrow// �������ָ���ʼ��
			}
			                                                                                // ���û�ѡ��Ĳ��ǡ����桱�͡������桱ʱ��ʲô��������������ͼ��Ϊԭ����״̬
		}
		else
		{
			DeletePlcTrnsArcs(m_pHSTView);
		}

	}
	
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_UPDATETREE1,0,0);      // Ŀ�ķ�����mainframe���ٷ�����dlgmodelchoose1��֪ͨHSTPN�ṹ������
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pParmListDlg->SendMessage(WM_MESSAGE_UPDATETREEOFMODEL,0,0);// ��ģ�ͱ༭������WM_MESSAGE_UPDATETREEOFMODEL��Ϣ������ģ����
	m_pHSTView->Invalidate();
//	m_pHSTView->UpdateWindow();
	//====================�½�����=========================================================end===========
}

// ���Vector_Place/Vector_Trans/arr_Arrow,�ָ���ʼ��
void CHSTPN_SYSDoc::DeletePlcTrnsArcs(CHSTPN_SYSView *m_pView)
{
	// �ͷ�����/����б�
	for(vector<CPlace*>::iterator iterP=Vector_Place.begin();iterP!=Vector_Place.end();iterP++)
	{
		for(vector<IOValue*>::iterator iter3=(*iterP)->arryIOputData.begin();iter3!=(*iterP)->arryIOputData.end();iter3++)
		{
			delete (*iter3);
		}
		(*iterP)->arryIOputData.clear();
		(*iterP)->arryInputofLua.clear();
		(*iterP)->arryOutputofLua.clear();
	}

	// �ͷ�Vector_Place��Ԫ��ָ����ڴ棬���Vector_Place
	for(vector<CPlace*>::iterator iterP=Vector_Place.begin();iterP!=Vector_Place.end();iterP++)
	{
		(*iterP)->DestroyWindow();
		delete (*iterP);
	}
	Vector_Place.clear();
	// �ͷ�Vector_Trans��Ԫ��ָ����ڴ棬���Vector_Trans
	for(vector<CTransition*>::iterator iterT=Vector_Trans.begin();iterT!=Vector_Trans.end();iterT++)
	{
		(*iterT)->DestroyWindow();
		delete (*iterT);
	}
	Vector_Trans.clear();
	// �ͷ�arr_Arrow��Ԫ��ָ����ڴ棬���arr_Arrow
	int n_SizeArcs = arr_Arrow.GetSize();
	for(int i=0;i<n_SizeArcs;i++)
	{
		delete (CArrow*)arr_Arrow.GetAt(i);
	}
	arr_Arrow.RemoveAll();

	// �ͷ�Vector_OutputModel��Ԫ��ָ����ڴ棬���Vector_OutputModel
	// release the vec_OutputData of OutputofModel
	for(vector<CCTRLShowMatrix*>::iterator iterO=Vector_OutputModel.begin();iterO!=Vector_OutputModel.end();iterO++)
	{
		for(vector<VEC_DOUBLE*>::iterator itervv=(*iterO)->vec_OutputData.begin();itervv!=(*iterO)->vec_OutputData.end();itervv++)
		{
			for(vector<double*>::iterator iterv=(*itervv)->begin();iterv!=(*itervv)->end();iterv++)
			{
				delete (*iterv);
			}
			(*itervv)->clear();
			delete (*itervv);
		}
	}
	// release the OutputofModel
	for(vector<CCTRLShowMatrix*>::iterator iterO=Vector_OutputModel.begin();iterO!=Vector_OutputModel.end();iterO++)
	{
		(*iterO)->DestroyWindow();
		delete(*iterO);
	}
	Vector_OutputModel.clear();

	// �������ָ���ʼ��
	m_pView->u_whichSubscript      = 0;
	m_pView->u_SubScriptofPlace    = 0;  
	m_pView->u_SubScriptofTrans    = 0;
	m_pView->b_DrawLine            = FALSE;
	m_pView->m_bIsLButtonDown	   = FALSE;
	m_pView->m_ptOffset.x		   = 0;
	m_pView->m_ptOffset.y		   = 0;
	m_pView->m_ptLBtnDown.x        = 0;
	m_pView->m_ptLBtnDown.y        = 0;
	m_pView->u_PlaceorTrans_Arrow  = 0;
	m_pView->myDocument            = NULL;

	if(m_pView->myDocument != NULL)
	{
		m_pView->myDocument->Clear();// �ͷ��ӽڵ��ڴ�
	}
	delete m_pView->myDocument; // �ͷŸ��ڵ㱾��

	m_pView->bPNisChanged             = TRUE;

	m_pView->u_SubScriptofOutputModel = 0;

}

// �����ļ�д�����
void CHSTPN_SYSDoc::WriteXMLFile(CString filePath_Name,TiXmlDocument *myDocument)
{
	// 

	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration("1.0","","");// 2.д��һ����������
	if(NULL == pDeclaration) return;
	myDocument->LinkEndChild(pDeclaration);                            // ���ӵ��ĵ�����

	TiXmlElement *RootElement = new TiXmlElement("HSTPnet");           // 3.����һ����Ԫ�ز����ӡ�
    myDocument->LinkEndChild(RootElement);                             // ���ӵ��ĵ�����
	// -----------------����ȫ�ֱ�����------------------------------------------------------------------Begin-----------------------------
	TiXmlElement *ListGlobal = new TiXmlElement("ListGlobal");
	RootElement->LinkEndChild(ListGlobal);
	int n_countg = 0;
	for(vector<IOValue*>::iterator iterG=arryIOputDataG.begin();iterG!=arryIOputDataG.end();iterG++)
	{
		USES_CONVERSION;
		TiXmlElement *IOputDataG = new TiXmlElement("IOputDataG");
		ListGlobal->LinkEndChild(IOputDataG);

		char * name;
		name = T2A((*iterG)->Name);
		IOputDataG->SetAttribute("Name",name);
			
		char * iotype;
		iotype = T2A((*iterG)->IOType);
		IOputDataG->SetAttribute("IOType",iotype);
		
		//int initvalue = ((CHSTPN_SYSView*)(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1)))->vec_IOputDataG.at(n_countg)->Value * 10000;
		int initvalue = (*iterG)->InitValue;
		/*if(this->vec_IOputDataG.at(n_countg)!=NULL)
		{
			initvalue = this->vec_IOputDataG.at(n_countg)->Value * 10000;
		}
		else
		{
			initvalue = (*iterG)->Value * 10000; 
		}*/
		IOputDataG->SetAttribute("InitValue",initvalue);
		
		n_countg++;
		
	}
	// -------------------------------------------------------------------------------------------------End-------------------------------
	
	//------------------���������Ϣ--------------------------------------------------------------------Begin-----------------------------
	TiXmlElement *PlacesElement = new TiXmlElement("Places");                                           // ����һ��PlacesԪ�ز����ӡ�
    RootElement->LinkEndChild(PlacesElement);
	int n_countP = 0;
	for(vector<CPlace*>::iterator iterP=Vector_Place.begin();iterP!=Vector_Place.end();iterP++)
	{
		TiXmlElement *PlaceElement = new TiXmlElement("Place");                                         // ���δ���һ��Place�ڵ�
		PlacesElement->LinkEndChild(PlaceElement);

		char * caption;
		USES_CONVERSION;
		caption = T2A((*iterP)->m_caption);
		PlaceElement->SetAttribute("m_caption",caption);                                               // Place���ԣ�m_caption

		PlaceElement->SetAttribute("u_SubscriptofModel",(*iterP)->u_SubscriptofModel);                 // Place���ԣ�u_SubscriptofModel

		int placestyle;
		if((*iterP)->PlaceStyle == STYLE_DISCRETE) placestyle=1;
		if((*iterP)->PlaceStyle == STYLE_CNTNUS)   placestyle=2;
		if((*iterP)->PlaceStyle == STYLE_RAND)     placestyle=3;
		if((*iterP)->PlaceStyle == STYLE_TIME)     placestyle=4;
		if((*iterP)->PlaceStyle == STYLE_TCTRL)    placestyle=5;
		if((*iterP)->PlaceStyle == STYLE_LCTRL)    placestyle=6;
		if((*iterP)->PlaceStyle == STYLE_DISIC)    placestyle=7;
		PlaceElement->SetAttribute("PlaceStyle",placestyle);                                           // Place���ԣ�PlaceStyle

		PlaceElement->SetAttribute("x",(*iterP)->Position.x);                                          // Place���ԣ�Position.x
		PlaceElement->SetAttribute("y",(*iterP)->Position.y);                                          // Place���ԣ�Position.y
                          
		char * bactivation;
		if((*iterP)->bActivation) bactivation="true"; else bactivation="false";
		PlaceElement->SetAttribute("bActivation",bactivation);                                         // Place���ԣ�bActivation
                               
		char * benable;
		if((*iterP)->bEnable) benable="true"; else benable="false";
		PlaceElement->SetAttribute("bEnable",benable);                                                 // Place���ԣ�bEnable

		
		char * bisassigned;
		if((*iterP)->bisAssigned) bisassigned="true"; else bisassigned="false";
		PlaceElement->SetAttribute("bisAssigned",bisassigned);                                         //Place���ԣ�bisAssigned

		char * ischoseSocket;
		if((*iterP)->ischoseSocket) ischoseSocket="true"; else ischoseSocket="false";                  //Place���ԣ�ischoseSocket
		PlaceElement->SetAttribute("ischoseSocket",ischoseSocket);
		//-----------------------------
		char * ischose;
		if((*iterP)->ischoseupdown) ischose = "true" ;else ischose = "false";
		PlaceElement->SetAttribute("ischoseupdown",ischose);

		int downline = (*iterP)->dTime_downline*10000;
		PlaceElement->SetAttribute("dTime_downline",downline);
		int upline = (*iterP)->dTime_upline*10000;
		PlaceElement->SetAttribute("dTime_upline",upline);

		char * ischoseusr;
		if((*iterP)->ischoseProgRand) ischoseusr = "true"; else ischoseusr = "false";
		PlaceElement->SetAttribute("ischoseProgRand",ischoseusr);

		TiXmlElement *S_LuaElement = new TiXmlElement("S_Luafile"); 
		PlaceElement->LinkEndChild(S_LuaElement);
		char * S_function;
		if((*iterP)->PlaceStyle==STYLE_RAND/* && (*iterP)->Lua_fileName!=""*/ && (*iterP)->ischoseProgRand)
		{
			// ��ȡLua�ļ��е�����
			CString luafilepath = _T("LuaFile\\S_") + (*iterP)->m_caption + _T(".lua");
			CFile luafile(luafilepath,CFile::modeRead);                                            // ��Lua�ļ�����LuaFile�ļ�����
			DWORD dwFileLen;
			dwFileLen = (DWORD)luafile.GetLength();                                                // ��ȡ�ļ������ݵĳ���
			S_function = new char[dwFileLen + 1];                                                    // Ϊ�ַ���function�����ڴ�
			S_function[dwFileLen] = 0;                                                               // ���һ���ַ�Ӧ��Ϊ\0
			luafile.Read(S_function,dwFileLen);                                                      // ������д���ַ���function��
			luafile.Close();

		}
		else
		{
			S_function = "";
		}
		TiXmlText *S_luafuc = new TiXmlText(S_function);
		S_LuaElement->LinkEndChild(S_luafuc);

		//-----------------------------

		TiXmlElement *dProbltyElement = new TiXmlElement("dProblty");                                  //Place���ԣ�dProblty
		PlaceElement->LinkEndChild(dProbltyElement);
		CString str_dproblty;
		str_dproblty.Format(L"%f",(*iterP)->dProblty);
		char * dproblty;
		dproblty = T2A(str_dproblty);
		dProbltyElement->LinkEndChild(new TiXmlText(dproblty));

		TiXmlElement *dTimeElement = new TiXmlElement("dTime");                                       //Place���ԣ�dTime
		PlaceElement->LinkEndChild(dTimeElement);
		CString str_dtime;
		str_dtime.Format(L"%f",(*iterP)->dTime);
		char * dtime;
		dtime = T2A(str_dtime);
		dTimeElement->LinkEndChild(new TiXmlText(dtime));
		
		if(((*iterP)->PlaceStyle==STYLE_DISIC || (*iterP)->PlaceStyle==STYLE_CNTNUS)&&(*iterP)->ischoseSocket)
		{
			TiXmlElement *ipadrsElement = new TiXmlElement("ipadrs");                                  //Place���ԣ�ipadrs
			PlaceElement->LinkEndChild(ipadrsElement);
			CString str_ipadrs;
			str_ipadrs = (*iterP)->ipadrs.c_str();
			char * ipadrs;
			ipadrs = T2A(str_ipadrs);
			ipadrsElement->LinkEndChild(new TiXmlText(ipadrs));

			TiXmlElement *portElement = new TiXmlElement("port");                                       //Place���ԣ�port
			PlaceElement->LinkEndChild(portElement);
			CString str_port;
			str_port.Format(L"%d",(*iterP)->port);
			char * port;
			port = T2A(str_port);
			portElement->LinkEndChild(new TiXmlText(port));

			TiXmlElement *inputElement = new TiXmlElement("num_input");                                       //Place���ԣ�num_input
			PlaceElement->LinkEndChild(inputElement);
			CString str_input;
			str_input.Format(L"%d",(*iterP)->num_input);
			char * num_input;
			num_input = T2A(str_input);
			inputElement->LinkEndChild(new TiXmlText(num_input));

			TiXmlElement *outputElement = new TiXmlElement("num_output");                                       //Place���ԣ�num_output
			PlaceElement->LinkEndChild(outputElement);
			CString str_output;
			str_output.Format(L"%d",(*iterP)->num_output);
			char * num_output;
			num_output = T2A(str_output);
			outputElement->LinkEndChild(new TiXmlText(num_output));
		}



		TiXmlElement *Lua_fileNameElement = new TiXmlElement("Lua_fileName");                        //Place���ԣ�Lua_fileName
		PlaceElement->LinkEndChild(Lua_fileNameElement);
		char *lua_filename;
		lua_filename = T2A((*iterP)->Lua_fileName);
		Lua_fileNameElement->LinkEndChild(new TiXmlText(lua_filename));

		TiXmlElement *pt1Element = new TiXmlElement("pt1");                                         //Place���ԣ�pt1
		PlaceElement->LinkEndChild(pt1Element);
		pt1Element->SetAttribute("x",(*iterP)->pt1.x);
		pt1Element->SetAttribute("y",(*iterP)->pt1.y);

		TiXmlElement *pt2Element = new TiXmlElement("pt2");                                         //Place���ԣ�pt2
		PlaceElement->LinkEndChild(pt2Element);
		pt2Element->SetAttribute("x",(*iterP)->pt2.x);
		pt2Element->SetAttribute("y",(*iterP)->pt2.y);

		TiXmlElement *pt3Element = new TiXmlElement("pt3");                                        //Place���ԣ�pt3
		PlaceElement->LinkEndChild(pt3Element);
		pt3Element->SetAttribute("x",(*iterP)->pt3.x);
		pt3Element->SetAttribute("y",(*iterP)->pt3.y);

		TiXmlElement *pt4Element = new TiXmlElement("pt4");                                       //Place���ԣ�pt4
		PlaceElement->LinkEndChild(pt4Element);
		pt4Element->SetAttribute("x",(*iterP)->pt4.x);
		pt4Element->SetAttribute("y",(*iterP)->pt4.y);

		TiXmlElement *arryPlacePreElement = new TiXmlElement("arryPlacePre");                     //Place���ԣ�arryPlacePre
		PlaceElement->LinkEndChild(arryPlacePreElement);
		for(vector<CTransition*>::iterator itert1=(*iterP)->arryPlacePre.begin();itert1!=(*iterP)->arryPlacePre.end();itert1++)
		{
			TiXmlElement *pretransitionElement = new TiXmlElement("pretransition");
			arryPlacePreElement->LinkEndChild(pretransitionElement);
			pretransitionElement->SetAttribute("u_SubscriptofTrans",(*itert1)->u_SubscriptofTrans);
		}
		TiXmlElement *arryPlacePstElement = new TiXmlElement("arryPlacePst");                     //Place���ԣ�arryPlacePst
		PlaceElement->LinkEndChild(arryPlacePstElement);
		for(vector<CTransition*>::iterator itert2=(*iterP)->arryPlacePst.begin();itert2!=(*iterP)->arryPlacePst.end();itert2++)
		{
			TiXmlElement *psttransitionElement = new TiXmlElement("psttransition");
			arryPlacePstElement->LinkEndChild(psttransitionElement);
			psttransitionElement->SetAttribute("u_SubscriptofTrans",(*itert2)->u_SubscriptofTrans);
		}

		TiXmlElement *bisUserDef_probElement = new TiXmlElement("bisUserDef_prob");               // Place���ԣ�bisUserDef_prob
		PlaceElement->LinkEndChild(bisUserDef_probElement);
		char * bisuserdef_prob;
		if((*iterP)->bisUserDef_prob) bisuserdef_prob="true"; else bisuserdef_prob="false";
		bisUserDef_probElement->LinkEndChild(new TiXmlText(bisuserdef_prob));

		TiXmlElement *bisUserDef_timeElement = new TiXmlElement("bisUserDef_time");               // Place���ԣ�bisUserDef_time
		PlaceElement->LinkEndChild(bisUserDef_timeElement);
		char * bisuserdef_time;
		if((*iterP)->bisUserDef_time) bisuserdef_time="true"; else bisuserdef_time="false";
		bisUserDef_timeElement->LinkEndChild(new TiXmlText(bisuserdef_time));

		TiXmlElement *bisUserDef_ctrlElement = new TiXmlElement("bisUserDef_ctrl");               // Place���ԣ�bisUserDef_ctrl
		PlaceElement->LinkEndChild(bisUserDef_ctrlElement);
		char * bisuserdef_ctrl;
		if((*iterP)->bisUserDef_ctrl) bisuserdef_ctrl="true"; else bisuserdef_ctrl="false";
		bisUserDef_ctrlElement->LinkEndChild(new TiXmlText(bisuserdef_ctrl));

		TiXmlElement *str_Inputport_probElement = new TiXmlElement("str_Inputport_prob");         // Place���ԣ�str_Inputport_prob
		PlaceElement->LinkEndChild(str_Inputport_probElement);
		char * inputport_prob;
		inputport_prob=T2A((*iterP)->str_Inputport_prob);
		str_Inputport_probElement->LinkEndChild(new TiXmlText(inputport_prob));

		TiXmlElement *str_Inputport_timeElement = new TiXmlElement("str_Inputport_time");         // Place���ԣ�str_Inputport_time
		PlaceElement->LinkEndChild(str_Inputport_timeElement);
		char * inputport_time;
		inputport_time=T2A((*iterP)->str_Inputport_time);
		str_Inputport_timeElement->LinkEndChild(new TiXmlText(inputport_time));

		TiXmlElement *str_Inputport_ctrlElement = new TiXmlElement("str_Inputport_ctrl");         // Place���ԣ�str_Inputport_ctrl
		PlaceElement->LinkEndChild(str_Inputport_ctrlElement);
		char * inputport_ctrl;
		inputport_ctrl=T2A((*iterP)->str_Inputport_ctrl);
		str_Inputport_ctrlElement->LinkEndChild(new TiXmlText(inputport_ctrl));


		TiXmlElement *arryIOputDataElement = new TiXmlElement("arryIOputData");                    // Place���ԣ�arryIOputData
		PlaceElement->LinkEndChild(arryIOputDataElement);
		int n_countio = 0;
		for(vector<IOValue*>::iterator iterIO1=(*iterP)->arryIOputData.begin();iterIO1!=(*iterP)->arryIOputData.end();iterIO1++)
		{
			TiXmlElement * IOputDataElement = new TiXmlElement("IOputData");
			arryIOputDataElement->LinkEndChild(IOputDataElement);
			char * name;
			name = T2A((**iterIO1).Name);
			IOputDataElement->SetAttribute("Name",name);
			char sequence[10];
			_itoa_s((**iterIO1).Sequence,sequence,10,10);
			IOputDataElement->SetAttribute("Sequence",sequence);
			char * iotype;
			iotype = T2A((**iterIO1).IOType);
			IOputDataElement->SetAttribute("IOType",iotype);

			char * isquoteg;
			if((*iterIO1)->isQuoteG) isquoteg = "true"; else isquoteg = "false";
			IOputDataElement->SetAttribute("isQuoteG", isquoteg);

			char * globalvluname;
			globalvluname = T2A((*iterIO1)->GlobalVluName);
			IOputDataElement->SetAttribute("GlobalVluName", globalvluname);


			char * frommodel;
			frommodel = T2A((**iterIO1).FromModel);
			IOputDataElement->SetAttribute("FromModel",frommodel);
			char * frommdvluname;
			frommdvluname = T2A((**iterIO1).FromMDVluName);
			IOputDataElement->SetAttribute("FromMDVluName",frommdvluname);

			//int value = ((CHSTPN_SYSView*)(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1)))->vec_Model_saveInitSet.at(n_countP)->arryIOputData.at(n_countio)->Value * 10000;
			int value = (*iterIO1)->InitValue;
			/*if(this->vec_Model_saveInitSet.at(n_countP)->arryIOputData.at(n_countio)!=NULL)
			{
				value = this->vec_Model_saveInitSet.at(n_countP)->arryIOputData.at(n_countio)->Value * 10000;
			}
			else
			{
				value = (*iterIO1)->Value * 10000;
			}*/
			IOputDataElement->SetAttribute("InitValue",value);

			n_countio++;
		}

		TiXmlElement *arryInputofLuaElement = new TiXmlElement("arryInputofLua");                  // Place���ԣ�arryInputofLua
		PlaceElement->LinkEndChild(arryInputofLuaElement);
		n_countio = 0;
		for(vector<IOValue*>::iterator iterInLua1=(*iterP)->arryInputofLua.begin();iterInLua1!=(*iterP)->arryInputofLua.end();iterInLua1++)
		{
			TiXmlElement * InputofLuaElement = new TiXmlElement("InputofLua");
			arryInputofLuaElement->LinkEndChild(InputofLuaElement);
			char * name;
			name = T2A((**iterInLua1).Name);
			InputofLuaElement->SetAttribute("Name",name);
			char sequence[10];
			_itoa_s((**iterInLua1).Sequence,sequence,10,10);
			InputofLuaElement->SetAttribute("Sequence",sequence);
			char * iotype;
			iotype = T2A((**iterInLua1).IOType);
			InputofLuaElement->SetAttribute("IOType",iotype);

			char * isquoteg;
			if((*iterInLua1)->isQuoteG) isquoteg = "true"; else isquoteg = "false";
			InputofLuaElement->SetAttribute("isQuoteG", isquoteg);

			char * globalvluname;
			globalvluname = T2A((*iterInLua1)->GlobalVluName);
			InputofLuaElement->SetAttribute("GlobalVluName", globalvluname);

			char * frommodel;
			frommodel = T2A((**iterInLua1).FromModel);
			InputofLuaElement->SetAttribute("FromModel",frommodel);
			char * frommdvluname;
			frommdvluname = T2A((**iterInLua1).FromMDVluName);
			InputofLuaElement->SetAttribute("FromMDVluName",frommdvluname);

			//int value = ((CHSTPN_SYSView*)(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1)))->vec_Model_saveInitSet.at(n_countP)->arryInputofLua.at(n_countio)->Value * 10000;
			int value = (*iterInLua1)->InitValue;
			/*if(this->vec_Model_saveInitSet.at(n_countP)->arryInputofLua.at(n_countio)!=NULL)
			{
			int value = this->vec_Model_saveInitSet.at(n_countP)->arryInputofLua.at(n_countio)->Value * 10000;
			}
			else
			{
				value = (*iterInLua1)->Value * 10000;
			}*/
			InputofLuaElement->SetAttribute("InitValue",value);

			n_countio++;
		}

		TiXmlElement *arryOutputofLuaElement = new TiXmlElement("arryOutputofLua");               // Place���ԣ�arryOutputofLua
		PlaceElement->LinkEndChild(arryOutputofLuaElement);
		n_countio = 0;
		for(vector<IOValue*>::iterator iterOutLua1=(*iterP)->arryOutputofLua.begin();iterOutLua1!=(*iterP)->arryOutputofLua.end();iterOutLua1++)
		{
			TiXmlElement * OutputofLuaElement = new TiXmlElement("OutputofLua");
			arryOutputofLuaElement->LinkEndChild(OutputofLuaElement);
			char * name;
			name = T2A((**iterOutLua1).Name);
			OutputofLuaElement->SetAttribute("Name",name);
			char sequence[10];
			_itoa_s((**iterOutLua1).Sequence,sequence,10,10);                                      // ��һ��10��sequence[10]�е�10���ڶ���10�Ǳ�ʾʮ����
			OutputofLuaElement->SetAttribute("Sequence",sequence);
			char * iotype;
			iotype = T2A((**iterOutLua1).IOType);
			OutputofLuaElement->SetAttribute("IOType",iotype);

			char * isquoteg;
			if((*iterOutLua1)->isQuoteG) isquoteg = "true"; else isquoteg = "false";
			OutputofLuaElement->SetAttribute("isQuoteG", isquoteg);

			char * globalvluname;
			globalvluname = T2A((*iterOutLua1)->GlobalVluName);
			OutputofLuaElement->SetAttribute("GlobalVluName", globalvluname);

			char * frommodel;
			frommodel = T2A((**iterOutLua1).FromModel);
			OutputofLuaElement->SetAttribute("FromModel",frommodel);
			char * frommdvluname;
			frommdvluname = T2A((**iterOutLua1).FromMDVluName);
			OutputofLuaElement->SetAttribute("FromMDVluName",frommdvluname);

			//int value = ((CHSTPN_SYSView*)(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1)))->vec_Model_saveInitSet.at(n_countP)->arryOutputofLua.at(n_countio)->Value * 10000;
			int value = (*iterOutLua1)->InitValue;
			/*if(this->vec_Model_saveInitSet.at(n_countP)->arryOutputofLua.at(n_countio)!=NULL)
			{
				value = this->vec_Model_saveInitSet.at(n_countP)->arryOutputofLua.at(n_countio)->Value * 10000;
			}
			else
			{
				value = (*iterOutLua1)->Value * 10000;
			}*/
			OutputofLuaElement->SetAttribute("InitValue",value);

			n_countio++;
		}

		TiXmlElement *Lua_functionElement = new TiXmlElement("Lua_function");                      // Place���ԣ�Lua_function ע���������Ǳ��������Ӧ��Lua�ļ��е�����
		PlaceElement->LinkEndChild(Lua_functionElement);
		char * function;
		if(((*iterP)->PlaceStyle==STYLE_CNTNUS || (*iterP)->PlaceStyle==STYLE_DISIC) && (*iterP)->Lua_fileName!="")
		{
			// ��ȡLua�ļ��е�����
			CString luafilepath = _T("LuaFile\\") + (*iterP)->Lua_fileName + _T(".lua");
			CFile luafile(luafilepath,CFile::modeRead);                                            // ��Lua�ļ�����LuaFile�ļ�����
			DWORD dwFileLen;
			dwFileLen = (DWORD)luafile.GetLength();                                                // ��ȡ�ļ������ݵĳ���
			function = new char[dwFileLen + 1];                                                    // Ϊ�ַ���function�����ڴ�
			function[dwFileLen] = 0;                                                               // ���һ���ַ�Ӧ��Ϊ\0
			luafile.Read(function,dwFileLen);                                                      // ������д���ַ���function��
			luafile.Close();

		}
		else
		{
			function = "";
		}
		TiXmlText *luafuc = new TiXmlText(function);
		Lua_functionElement->LinkEndChild(luafuc);

		TiXmlElement *CtrlStyleElement = new TiXmlElement("CtrlPlaceStyle");                      // Place���ԣ�CtrlPlaceStyle
		PlaceElement->LinkEndChild(CtrlStyleElement);
		if((*iterP)->CtrlPlaceStyle == TCTRL)
		{
			CtrlStyleElement->LinkEndChild(new TiXmlText("TCTRL"));
		}
		else
		{
			CtrlStyleElement->LinkEndChild(new TiXmlText("LCTRL"));
		}

//		delete function;
//		function = NULL;
		n_countP++;

	}
	//------------------���������Ϣ--------------------------------------------------------------------End-------------------------------


	//------------------�����Ǩ��Ϣ--------------------------------------------------------------------Begin-----------------------------
	TiXmlElement *TranstionsElement = new TiXmlElement("Transitions");                                  // ����һ��TransitionsԪ�ز����ӡ�
	RootElement->LinkEndChild(TranstionsElement);
	for(vector<CTransition*>::iterator iterT=Vector_Trans.begin();iterT!=Vector_Trans.end();iterT++)
	{
		TiXmlElement *TransitionElement = new TiXmlElement("Transition");                              // ���δ���һ��Transition�ڵ�
		TranstionsElement->LinkEndChild(TransitionElement);

		char * caption;
		USES_CONVERSION;
		caption = T2A((*iterT)->m_caption);
		TransitionElement->SetAttribute("m_caption",caption);                                           // Transition���ԣ�m_caption

		TransitionElement->SetAttribute("u_SubscriptofTrans",(*iterT)->u_SubscriptofTrans);             // Transition���ԣ�u_SubscriptofTrans

		TransitionElement->SetAttribute("x",(*iterT)->Position.x);                                      // Transition���ԣ�Position.x
		TransitionElement->SetAttribute("y",(*iterT)->Position.y);                                      // Transition���ԣ�Position.y

		char * bfire;
		if((*iterT)->bFire) bfire="true"; else bfire="false";
		TransitionElement->SetAttribute("bFire",bfire);                                                 // Transition���ԣ�bFire

		TiXmlElement *pt1Element = new TiXmlElement("pt1");                                             // Transition���ԣ�pt1
		TransitionElement->LinkEndChild(pt1Element);
		pt1Element->SetAttribute("x",(*iterT)->pt1.x);
		pt1Element->SetAttribute("y",(*iterT)->pt1.y);

		TiXmlElement *pt2Element = new TiXmlElement("pt2");                                             // Transition���ԣ�pt2
		TransitionElement->LinkEndChild(pt2Element);
		pt2Element->SetAttribute("x",(*iterT)->pt2.x);
		pt2Element->SetAttribute("y",(*iterT)->pt2.y);

		TiXmlElement *arryTransPreElement = new TiXmlElement("arryTransPre");                           // Transition���ԣ�arryTransPre
		TransitionElement->LinkEndChild(arryTransPreElement);
		for(vector<CPlace*>::iterator iterp1=(*iterT)->arryTransPre.begin();iterp1!=(*iterT)->arryTransPre.end();iterp1++)
		{
			TiXmlElement *preplaceElement = new TiXmlElement("preplae");
			arryTransPreElement->LinkEndChild(preplaceElement);
			preplaceElement->SetAttribute("u_SubscriptofModel",(*iterp1)->u_SubscriptofModel);
		}

		TiXmlElement *arryTransPstElement = new TiXmlElement("arryTransPst");                            // Transition���ԣ�arryTransPst
		TransitionElement->LinkEndChild(arryTransPstElement);
		for(vector<CPlace*>::iterator iterp2=(*iterT)->arryTransPst.begin();iterp2!=(*iterT)->arryTransPst.end();iterp2++)
		{
			TiXmlElement *pstplaceElement = new TiXmlElement("pstplae");
			arryTransPstElement->LinkEndChild(pstplaceElement);
			pstplaceElement->SetAttribute("u_SubscriptofModel",(*iterp2)->u_SubscriptofModel);
		}
	}
	//------------------�����Ǩ��Ϣ--------------------------------------------------------------------End-------------------------------


	//------------------����������Ϣ------------------------------------------------------------------Begin-----------------------------
	TiXmlElement *ArcsElement = new TiXmlElement("Arcs");                                                  // ����һ��ArcsԪ�ز�����
	RootElement->LinkEndChild(ArcsElement);
	int nCount = arr_Arrow.GetSize();                                                                      // ��ȡҪ��������򻡵ĸ���
	for(int i=0;i<nCount;i++)                                                                              // �����򻡵���Ϣ����д���ļ�
	{
		char* disic; 
		TiXmlElement *ArcElement = new TiXmlElement("Arc");
		ArcsElement->LinkEndChild(ArcElement);
		ArcElement->SetAttribute("whichModel_Start",((CArrow*)arr_Arrow.GetAt(i))->whichModel_Start);      // Arc����1��whichModel_Start
		ArcElement->SetAttribute("u_SubScriptofPlace",((CArrow*)arr_Arrow.GetAt(i))->u_SubScriptofPlace);  // Arc����2��u_SubScriptofPlace
		ArcElement->SetAttribute("u_SubScriptofTrans",((CArrow*)arr_Arrow.GetAt(i))->u_SubScriptofTrans);  // Arc����3��u_SubScriptofTrans
		ArcElement->SetAttribute("pt_startx",((CArrow*)arr_Arrow.GetAt(i))->pt_start.x);
		ArcElement->SetAttribute("pt_starty",((CArrow*)arr_Arrow.GetAt(i))->pt_start.y);
		ArcElement->SetAttribute("pt_endx",((CArrow*)arr_Arrow.GetAt(i))->pt_end.x);
		ArcElement->SetAttribute("pt_endy",((CArrow*)arr_Arrow.GetAt(i))->pt_end.y);
		ArcElement->SetAttribute("u_arrow_id",((CArrow*)arr_Arrow.GetAt(i))->u_arrow_id);
		if (((CArrow*)arr_Arrow.GetAt(i))->is_disic)
			disic = "true"; else disic = "false";
		ArcElement->SetAttribute("isdisic",disic);
		ArcElement->SetAttribute("num",((CArrow*)arr_Arrow.GetAt(i))->num);
	}
	//------------------����������Ϣ------------------------------------------------------------------End-------------------------------

	//-----------------�������ģ�����Ϣ---------------------------------------------------------------Begin-----------------------------
	TiXmlElement *OutModelsElement = new TiXmlElement("OutModels");                                  // ����һ��OutModelsԪ�ز����ӡ�
	RootElement->LinkEndChild(OutModelsElement);
	for(vector<CCTRLShowMatrix*>::iterator itero=Vector_OutputModel.begin();itero!=Vector_OutputModel.end();itero++)
	{
		TiXmlElement *OutmodelElement = new TiXmlElement("OutModel");                               // ���δ���һ��OutModel�ڵ�
		OutModelsElement->LinkEndChild(OutmodelElement);

		char * caption;
		USES_CONVERSION;
		caption = T2A((*itero)->m_caption);
		OutmodelElement->SetAttribute("m_caption",caption);                                        // OutModel����1��m_caption

		//char id[10];
		//_itoa_s((*itero)->ID,id,10,10);
		OutmodelElement->SetAttribute("ID",(*itero)->ID);                                          // OutModel����2��ID

		OutmodelElement->SetAttribute("x",(*itero)->Position.x);                                   // OutModel����3��x
		OutmodelElement->SetAttribute("y",(*itero)->Position.y);                                   // OutModel����4��y

		int outputstyle;
		if((*itero)->OutputStyle==OUT_SHOWMATRIX) outputstyle = 1;
		if((*itero)->OutputStyle==OUT_TOFILE) outputstyle = 2;
		if((*itero)->OutputStyle==OUT_ACCUMULATOR) outputstyle = 3;
		OutmodelElement->SetAttribute("OutputStyle",outputstyle);                                  // OutModel����5��OutputStyle

		TiXmlElement *OutPNameElement = new TiXmlElement("OutPName");                              // OutModel����6��OutPName
		OutmodelElement->LinkEndChild(OutPNameElement);
		char * outpname;
		outpname=T2A((*itero)->OutPName);
		OutPNameElement->LinkEndChild(new TiXmlText(outpname));

		TiXmlElement *vec_OutValueNameElement = new TiXmlElement("vec_OutValueName");              // OutModel����7��vec_OutValueName
		OutmodelElement->LinkEndChild(vec_OutValueNameElement);
		for(vector<CString>::iterator iters=(*itero)->vec_OutValueName.begin();iters!=(*itero)->vec_OutValueName.end();iters++)
		{
			TiXmlElement *OutValueNameElement = new TiXmlElement("OutValueName");
			vec_OutValueNameElement->LinkEndChild(OutValueNameElement);
			char * outvaluenaem;
			outvaluenaem=T2A((*iters));
			OutValueNameElement->LinkEndChild(new TiXmlText(outvaluenaem));
		}

		TiXmlElement *i_PposnOValueElement = new TiXmlElement("i_PposnOValue");                   // OutModel����8��i_PposnOValue
		OutmodelElement->LinkEndChild(i_PposnOValueElement);
		char i_psn[10];
		_itoa_s((*itero)->i_PposnOValue,i_psn,10);
		char * i_posnovalue = i_psn;
		i_PposnOValueElement->LinkEndChild(new TiXmlText(i_posnovalue));

		TiXmlElement *vec_OutValueElement = new TiXmlElement("vec_OutValue");                     // OutModel����9��vec_OutValue
		OutmodelElement->LinkEndChild(vec_OutValueElement);
		for(vector<int>::iterator iter=(*itero)->vec_OutValue.begin();iter!=(*itero)->vec_OutValue.end();iter++)
		{
			TiXmlElement *OutValueElement = new TiXmlElement("OutValue");
			vec_OutValueElement->LinkEndChild(OutValueElement);
			char outvalue[10];
			_itoa_s((*iter),outvalue,10);
			//char * i_posnovalue = i_psn;
			OutValueElement->LinkEndChild(new TiXmlText(outvalue));
		}

		TiXmlElement *FilePathElement = new TiXmlElement("str_FilePath");                     // OutModel����10��str_FilePath
		OutmodelElement->LinkEndChild(FilePathElement);
		char * filepath = T2A((*itero)->str_FilePath);	
		FilePathElement->LinkEndChild(new TiXmlText(filepath));
	}
	//-----------------�������ģ�����Ϣ---------------------------------------------------------------End-------------------------------

	char * str_filename;
	USES_CONVERSION;                                   // ���ú�����T2A��W2A��֧��ATL��MFC�е��ַ�ת��
    str_filename = T2A(filePath_Name); 
	RootElement->SetAttribute("Name",str_filename);                        // д���ļ���

	RootElement->SetAttribute("Step",((CHSTPN_SYSView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1))->d_TimeStep * 10000);   // ���沽��
	if(((CHSTPN_SYSView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1))->d_SimTime > 65535)
	{
		RootElement->SetAttribute("SimTime",65535);
	}
	else
	{
		RootElement->SetAttribute("SimTime",((CHSTPN_SYSView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1))->d_SimTime);  
	}// �������ʱ��
	RootElement->SetAttribute("IntervalTime",((CHSTPN_SYSView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1))->d_IntervalTime);        // ������ʱ��

	myDocument->SaveFile(str_filename);                                    // ���浽�ļ�

	if(RootElement!=NULL)
	{
		RootElement->Clear();
	}

}

// �����ļ���ȡ����
TiXmlDocument* CHSTPN_SYSDoc::ReadXMLFile1(CString filePath_Name,TiXmlDocument *myDocument,CHSTPN_SYSView *m_pView)
{
	
	UINT max_subPlace = 0;
	UINT max_subTrans = 0;                         // �ҳ���ģ���п����ͱ�Ǩ������±�
	UINT max_subOuts  = 0;
	char *filepath_name;
	USES_CONVERSION;
	filepath_name = T2A(filePath_Name);
//	filepath_name = CString2char(filePath_Name);  // �˴���������CString2char��������Ϊ���ǲ�ϣ��char*ָ����ڴ汻�ͷţ�һ���ͷţ��ļ����ʻᷢ������
	myDocument = new TiXmlDocument(filepath_name);
	myDocument->LoadFile();// �����ļ�

	// ��ȡ�ӿ�ԭ��!!!!!!!!!!!!!!!!!!!!!!!!!!5.17
	CPoint pt_ViewportOrg;
	CClientDC dc((CWnd*)m_pView);
	m_pView->OnPrepareDC(&dc);
	pt_ViewportOrg = dc.GetViewportOrg();

	TiXmlElement *RootElement = myDocument->RootElement();                                                    // ��ȡ��Ԫ��--HSTPnet
	//--------------------------------------��ȡ������Ϣ-----------------------------------------------------Begin--------
	TiXmlElement *PlacesNode = RootElement->FirstChildElement();                                              // ��ȡPlaces�ڵ�

	//CRect rectofModel(0,0,40,40);//(0,0,40,60);                                                             // �������ģ�͵Ĵ�С
	TiXmlElement *PlaceNode = PlacesNode->FirstChildElement();                                                // ��ȡ��һ�������ڵ�
	for(;PlaceNode!=NULL;PlaceNode=PlaceNode->NextSiblingElement())                                           // �������еĿ����ڵ�
	{
		CPlace *place = new CPlace();                                                                         // ������һ��������������Ϊ������ڴ�
		TiXmlAttribute * attribute1 = PlaceNode->FirstAttribute();
		attribute1=attribute1->Next();
		attribute1=attribute1->Next();
		CString placestyle1(attribute1->Value());
		if(placestyle1 == "1") place->PlaceStyle  = STYLE_DISCRETE;
		if(placestyle1 == "2") place->PlaceStyle = STYLE_CNTNUS;
		if(placestyle1 == "3") place->PlaceStyle = STYLE_RAND;
		if(placestyle1 == "4") place->PlaceStyle = STYLE_TIME;
		if(placestyle1 == "5") place->PlaceStyle = STYLE_TCTRL;
		if(placestyle1 == "6") place->PlaceStyle = STYLE_LCTRL;
		if(placestyle1 == "7") place->PlaceStyle = STYLE_DISIC;      // ���Ȼ�ȡPlaceStyle���ԣ�����������ģ��
		place->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofModel,m_pView,place->PlaceStyle);
		place->m_thisModel = place;
		
		//====��ȡm_caption��u_SubscriptofModel��PlaceStyle��x��y��bActivation��bEnable��bisAssigned���⼸����������Ϊ����������
		TiXmlAttribute * attributeofPlace = PlaceNode->FirstAttribute();                                      // ��ȡ�����ĵ�1������ֵ--m_caption
		place->m_caption = attributeofPlace->Value();
		attributeofPlace = attributeofPlace->Next();                                                          // ��ȡ�����ĵ�2������ֵ--u_SubscriptofModel
		place->u_SubscriptofModel = atoi(attributeofPlace->Value());
		if(place->u_SubscriptofModel > max_subPlace)
			max_subPlace = place->u_SubscriptofModel;

		attributeofPlace = attributeofPlace->Next();                                                          // ��ȡ�����ĵ�3������ֵ--PlaceStyle
		CString placestyle(attributeofPlace->Value());
		if(placestyle == "1") place->PlaceStyle  = STYLE_DISCRETE;
		if(placestyle == "2") place->PlaceStyle = STYLE_CNTNUS;
		if(placestyle == "3") place->PlaceStyle = STYLE_RAND;
		if(placestyle == "4") place->PlaceStyle = STYLE_TIME;
		if(placestyle == "5") place->PlaceStyle = STYLE_TCTRL;
		if(placestyle == "6") place->PlaceStyle = STYLE_LCTRL;
		if(placestyle == "7") place->PlaceStyle = STYLE_DISIC;
		
		attributeofPlace = attributeofPlace->Next();                                                          // ��ȡ�����ĵ�4������ֵ--x
		place->Position.x = atoi(attributeofPlace->Value());
		attributeofPlace = attributeofPlace->Next();                                                          // ��ȡ�����ĵ�5������ֵ--y
		place->Position.y = atoi(attributeofPlace->Value());
		attributeofPlace = attributeofPlace->Next();                                                          // ��ȡ�����ĵ�6������ֵ--bActivation
		CString bactivation(attributeofPlace->Value());
		if(bactivation=="true") place->bActivation=TRUE; else place->bActivation=FALSE;

		attributeofPlace = attributeofPlace->Next();                                                          // ��ȡ�����ĵ�7������ֵ--bEnable
		CString benable(attributeofPlace->Value());
		if(benable=="true") place->bEnable=TRUE; else place->bEnable=FALSE;

		attributeofPlace = attributeofPlace->Next();                                                          // ��ȡ�����ĵ�8������ֵ--bisAssigned
		CString bassigned(attributeofPlace->Value());
		if(bassigned=="true") place->bisAssigned=TRUE; else place->bisAssigned=FALSE;

		attributeofPlace = attributeofPlace->Next();                                                          // ��ȡ�����ĵ�9������ֵ--ischoseSocket
		CString ischoseSocket(attributeofPlace->Value());
		if(ischoseSocket=="true") place->ischoseSocket=TRUE; else place->ischoseSocket=FALSE;
		//-------------------------
		attributeofPlace = attributeofPlace->Next(); 
		CString ischose(attributeofPlace->Value());
		if(ischose=="true") place->ischoseupdown = true; else place->ischoseupdown = false;

		attributeofPlace = attributeofPlace->Next(); 
		place->dTime_downline = (atoi(attributeofPlace->Value()))/10000.0;

		attributeofPlace = attributeofPlace->Next(); 
		place->dTime_upline = (atoi(attributeofPlace->Value()))/10000.0;

		attributeofPlace = attributeofPlace->Next(); 
		CString ischoseusr(attributeofPlace->Value());
		if(ischoseusr =="true") place->ischoseProgRand = true; else place->ischoseProgRand = false;

		TiXmlElement *elementofPlace = PlaceNode->FirstChildElement();
		const char* S_luafunc = elementofPlace->GetText();
		if(place->ischoseProgRand && place->PlaceStyle == STYLE_RAND)                              // �����⺯����Lua��������Ϊ��,�ҿ���������Ϊ��������ʱ����д���ļ�
		{        
			CString Lua_fileName_1 = _T("LuaFile\\S_") + place->m_caption + _T(".lua");
			CFile file(Lua_fileName_1,CFile::modeCreate | CFile::modeWrite);
			file.Write(S_luafunc,strlen(S_luafunc));
			file.Close();
		}

		//-------------------------

		//��ȡdProblty��dTime��Lua_fileName��pt1��pt2��pt3��pt4��arryPlacePre��arryPlacePst��bisUserDef_prob��bisUserDef_time��bisUserDef_ctrl
		//str_Inputport_prob��str_Inputport_time��str_Inputport_ctrl��arryInputData��arryOutputData��arryIOputData��arryInputofLua��arryOutputofLua��Lua_function
		//TiXmlElement *elementofPlace = PlaceNode->FirstChildElement();                                       // ��ȡ�����ĵ�1���ӽڵ�--dProblty
		elementofPlace = elementofPlace->NextSiblingElement();
		place->dProblty = atof(elementofPlace->GetText());

		elementofPlace = elementofPlace->NextSiblingElement();                                               // ��ȡ�����ĵ�2���ӽڵ�--dTime
		place->dTime = atof(elementofPlace->GetText());
		place->d_TimeTS = place->dTime;//++6.15

		elementofPlace = elementofPlace->NextSiblingElement();                                               // ��ȡ�����ĵ�3���ӽڵ�--ipadrs
		place->ipadrs = elementofPlace->GetText();

		elementofPlace = elementofPlace->NextSiblingElement();                                               // ��ȡ�����ĵ�4���ӽڵ�--port
		place->port = atoi(elementofPlace->GetText());

		elementofPlace = elementofPlace->NextSiblingElement();                                               // ��ȡ�����ĵ�5���ӽڵ�--num_input
		place->num_input = atoi(elementofPlace->GetText());

		elementofPlace = elementofPlace->NextSiblingElement();                                               // ��ȡ�����ĵ�6���ӽڵ�--num_output
		place->num_output = atoi(elementofPlace->GetText());

		elementofPlace = elementofPlace->NextSiblingElement();                                               // ��ȡ�����ĵ�4���ӽڵ�--Lua_fileName
		place->Lua_fileName = elementofPlace->GetText(); 

		elementofPlace = elementofPlace->NextSiblingElement();                                               // ��ȡ�����ĵ�4���ӽڵ�--pt1
		TiXmlAttribute *attributeofpt1 = elementofPlace->FirstAttribute();                                   // ��ȡpt1�ĵ�һ������x
		place->pt1.x = atoi(attributeofpt1->Value());
		attributeofpt1 = attributeofpt1->Next();                                                             // ��ȡpt1�ĵڶ�������y
		place->pt1.y = atoi(attributeofpt1->Value());

		elementofPlace = elementofPlace->NextSiblingElement();                                               // ��ȡ�����ĵ�5���ӽڵ�--pt2
		TiXmlAttribute *attributeofpt2 = elementofPlace->FirstAttribute();                                   // ��ȡpt2�ĵ�һ������x
		place->pt2.x = atoi(attributeofpt2->Value());
		attributeofpt2 = attributeofpt2->Next();                                                             // ��ȡpt2�ĵڶ�������y
		place->pt2.y = atoi(attributeofpt2->Value());

		elementofPlace = elementofPlace->NextSiblingElement();                                              // ��ȡ�����ĵ�6���ӽڵ�--pt3
		TiXmlAttribute *attributeofpt3 = elementofPlace->FirstAttribute();                                  // ��ȡpt3�ĵ�һ������x
		place->pt3.x = atoi(attributeofpt3->Value());
		attributeofpt3 = attributeofpt3->Next();                                                            // ��ȡpt3�ĵڶ�������y
		place->pt3.y = atoi(attributeofpt3->Value());

		elementofPlace = elementofPlace->NextSiblingElement();                                              // ��ȡ�����ĵ�7���ӽڵ�--pt4
		TiXmlAttribute *attributeofpt4 = elementofPlace->FirstAttribute();                                  // ��ȡpt4�ĵ�һ������x
		place->pt4.x = atoi(attributeofpt4->Value());
		attributeofpt4 = attributeofpt4->Next();                                                            // ��ȡpt4�ĵڶ�������y
		place->pt4.y = atoi(attributeofpt4->Value());

		elementofPlace = elementofPlace->NextSiblingElement();                                              // ��ȡ�����ĵ�8���ӽڵ�--arryPlacePre--�����ȡ
		elementofPlace = elementofPlace->NextSiblingElement();                                              // ��ȡ�����ĵ�9���ӽڵ�--arryPlacePst--�����ȡ

		elementofPlace = elementofPlace->NextSiblingElement();                                              // ��ȡ�����ĵ�10���ӽڵ�--bisUserDef_prob
		CString bisuserdef_prob(elementofPlace->GetText());
		if(bisuserdef_prob=="true") place->bisUserDef_prob=TRUE; else place->bisUserDef_prob=FALSE;

		elementofPlace = elementofPlace->NextSiblingElement();                                              // ��ȡ�����ĵ�11���ӽڵ�--bisUserDef_time
		CString bisuserdef_time(elementofPlace->GetText());
		if(bisuserdef_time=="true") place->bisUserDef_time=TRUE; else place->bisUserDef_time=FALSE;

		elementofPlace = elementofPlace->NextSiblingElement();                                              // ��ȡ�����ĵ�12���ӽڵ�--bisUserDef_ctrl
		CString bisuserdef_ctrl(elementofPlace->GetText());
		if(bisuserdef_ctrl=="true") place->bisUserDef_ctrl=TRUE; else place->bisUserDef_ctrl=FALSE;

		elementofPlace = elementofPlace->NextSiblingElement();                                              // ��ȡ�����ĵ�13���ӽڵ�--str_Inputport_prob
		place->str_Inputport_prob = elementofPlace->GetText();

		elementofPlace = elementofPlace->NextSiblingElement();                                              // ��ȡ�����ĵ�14���ӽڵ�--str_Inputport_time
		place->str_Inputport_time = elementofPlace->GetText();

		elementofPlace = elementofPlace->NextSiblingElement();                                              // ��ȡ�����ĵ�15���ӽڵ�--str_Inputport_ctrl
		place->str_Inputport_ctrl = elementofPlace->GetText();

		
		elementofPlace = elementofPlace->NextSiblingElement();                                             // ��ȡ�����ĵ�18���ӽڵ�--arryIOputData
		TiXmlElement *elementofIOputData = elementofPlace->FirstChildElement();                            // ��ȡarryIOputData�ĵ�һ��ֵ�Ľڵ�
		for(;elementofIOputData!=NULL;elementofIOputData = elementofIOputData->NextSiblingElement())       // ����arryIOputData�е����б����Ľڵ�
		{
			IOValue *ioputdata = new IOValue();//
			TiXmlAttribute *attributeofioputdata = elementofIOputData->FirstAttribute();                   // ��ȡIOputData�ĵ�1������--Name
			ioputdata->Name = attributeofioputdata->Value();
			attributeofioputdata = attributeofioputdata->Next();                                           // ��ȡIOputData�ĵ�2������--Sequence
			ioputdata->Sequence = atoi(attributeofioputdata->Value());
			  
			attributeofioputdata = attributeofioputdata->Next();                                           // ��ȡIOputData�ĵ�3������--IOType
			ioputdata->IOType = attributeofioputdata->Value();
			attributeofioputdata = attributeofioputdata->Next();                                           // ��ȡIOputData�ĵ�4������--FromModel
			ioputdata->FromModel = attributeofioputdata->Value();
			attributeofioputdata = attributeofioputdata->Next();                                           // ��ȡIOputData�ĵ�5������--FromMDVluName
			ioputdata->FromMDVluName = attributeofioputdata->Value();

			ioputdata->Value = 0;

			place->arryIOputData.push_back(ioputdata);
		}


		elementofPlace = elementofPlace->NextSiblingElement();                                             // ��ȡ�����ĵ�19���ӽڵ�--arryInputofLua

		elementofPlace = elementofPlace->NextSiblingElement();                                             // ��ȡ�����ĵ�20���ӽڵ�--arryOutputofLua
		
		vector<IOValue*> InputofLuaTS;
		vector<IOValue*> OutputofLuaTS;
		place->arryInputofLua.clear();
		place->arryOutputofLua.clear();
		for(vector<IOValue*>::iterator iter=place->arryIOputData.begin();iter!=place->arryIOputData.end();iter++)
		{
			if((**iter).IOType == "input")
			{
				if((**iter).Sequence!=0)
				{
					InputofLuaTS.push_back(*iter);
					place->arryInputofLua.push_back(*iter);
				}
			}
			if((**iter).IOType == "output")
			{
				if((**iter).Sequence!=0)
				{
							/*if((**iter).FromModel == "����1" || (**iter).FromModel == "����2" || (**iter).FromModel == "����3" || (**iter).FromModel == "����4" 
								|| (**iter).FromModel == "����5" || (**iter).FromModel == "����6" || (**iter).FromModel == "����7" || (**iter).FromModel == "����8" 
								|| (**iter).FromModel == "����9" || (**iter).FromModel == "����10" || (**iter).FromModel == "������ʶ" || (**iter).FromModel == "������ֵ"
								|| (**iter).FromModel == "���沽��" || (**iter).FromModel == "����11"|| (**iter).FromModel == "����12" || (**iter).FromModel == "����13" || (**iter).FromModel == "����14" 
								|| (**iter).FromModel == "����15" || (**iter).FromModel == "����16" || (**iter).FromModel == "����17" || (**iter).FromModel == "����18" 
								|| (**iter).FromModel == "����19" || (**iter).FromModel == "����20" 
								|| (**iter).FromModel=="����21" || (**iter).FromModel=="����22"|| (**iter).FromModel=="����23" || (**iter).FromModel=="����24"
							|| (**iter).FromModel=="����25" || (**iter).FromModel=="����26"|| (**iter).FromModel=="����27" || (**iter).FromModel=="����28"
							|| (**iter).FromModel=="����29" || (**iter).FromModel=="����30"|| (**iter).FromModel=="����31" || (**iter).FromModel=="����32"
							|| (**iter).FromModel=="����33" || (**iter).FromModel=="����34"|| (**iter).FromModel=="����35" || (**iter).FromModel=="����36"
							|| (**iter).FromModel=="����37" || (**iter).FromModel=="����38"|| (**iter).FromModel=="����39" || (**iter).FromModel=="����40"
							|| (**iter).FromModel=="����41" || (**iter).FromModel=="����42"|| (**iter).FromModel=="����43" || (**iter).FromModel=="����44"
							|| (**iter).FromModel=="����45" || (**iter).FromModel=="����46"|| (**iter).FromModel=="����47" || (**iter).FromModel=="����48"
							|| (**iter).FromModel=="����49" || (**iter).FromModel=="����50"
							)
							{
								OutputofLuaTS.push_back(*iter);
								place->arryOutputofLua.push_back(*iter);
							}*/
					OutputofLuaTS.push_back(*iter);
					place->arryOutputofLua.push_back(*iter);
				}
			}
			if((*iter)->IOType == "in/output")
			{
				if((*iter)->Sequence != 0)
				{
					InputofLuaTS.push_back(*iter);
					OutputofLuaTS.push_back(*iter);
					place->arryInputofLua.push_back(*iter);
					place->arryOutputofLua.push_back(*iter);
				}
			}
		}
		for(vector<IOValue*>::iterator iter_o = OutputofLuaTS.begin();iter_o!=OutputofLuaTS.end();iter_o++)
		{
			place->arryOutputofLua.at((**iter_o).Sequence-1) = *iter_o;
		}
		for(vector<IOValue*>::iterator iter_i = InputofLuaTS.begin();iter_i!=InputofLuaTS.end();iter_i++)
		{
			place->arryInputofLua.at((**iter_i).Sequence-1) = *iter_i;
		}

		elementofPlace = elementofPlace->NextSiblingElement();                                        // ��ȡ�����ĵ�21���ӽڵ�--Lua_function
		const char* luafunc = elementofPlace->GetText();
		if(place->Lua_fileName!="" && place->PlaceStyle == STYLE_CNTNUS)                              // �����⺯����Lua��������Ϊ��,�ҿ���������Ϊ��������ʱ����д���ļ�
		{        
			CString Lua_fileName_1 = _T("LuaFile\\") + place->Lua_fileName + _T(".lua");
			CFile file(Lua_fileName_1,CFile::modeCreate | CFile::modeWrite);
			file.Write(luafunc,strlen(luafunc));
			file.Close();
		}

		elementofPlace = elementofPlace->NextSiblingElement();          // ��ȡ����������
		CString ctrlstyle(elementofPlace->GetText());
		if(ctrlstyle == "TCTRL")
		{
			place->CtrlPlaceStyle = TCTRL;
		}
		else
		{
			place->CtrlPlaceStyle = LCTRL;
		}

		place->MoveWindow(place->Position.x + pt_ViewportOrg.x,place->Position.y + pt_ViewportOrg.y,rectofModel.Width(),rectofModel.Height());
		place->ShowWindow(SW_SHOW);                                                                  // ��ʾģ��

		Vector_Place.push_back(place);
	}

	//--------------------------------------��ȡ������Ϣ-----------------------------------------------------End----------

	//--------------------------------------��ȡ��Ǩ��Ϣ-----------------------------------------------------Begin--------
	TiXmlElement *TransitionsNode = PlacesNode->NextSiblingElement();                                       // ��ȡTransitions�ڵ�

	//CRect rectofTrans(0,0,15,35);//(0,0,20,50);                                                           // �����Ǩģ�͵Ĵ�С
	TiXmlElement *TransitionNode = TransitionsNode->FirstChildElement();                                    // ��ȡ��һ����Ǩ�ڵ�
	for(;TransitionNode!=NULL;TransitionNode=TransitionNode->NextSiblingElement())                          // �������еı�Ǩ�ڵ�
	{
		CTransition *trans = new CTransition();                                                             // ����ÿ����Ǩ��Ϣǰ����Ϊ��Ǩ�����ڴ�
		trans->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofTrans,m_pView);
		

		//====��ȡm_caption��u_SubscriptofTrans��Position.x��Position.y��bFire���⼸����������ΪTransition������
		TiXmlAttribute * attributeofTrans = TransitionNode->FirstAttribute();                               // ��ȡ���һ������ֵ--m_caption
		trans->m_caption = attributeofTrans->Value();
		attributeofTrans = attributeofTrans->Next();                                                        // ��ȡ���2������ֵ--u_SubscriptofTrans
		trans->u_SubscriptofTrans = atoi(attributeofTrans->Value());
		if(trans->u_SubscriptofTrans > max_subTrans)
			max_subTrans = trans->u_SubscriptofTrans;


		attributeofTrans = attributeofTrans->Next();                                                        // ��ȡ���3������ֵ--Position.x
		trans->Position.x = atoi(attributeofTrans->Value());
		attributeofTrans = attributeofTrans->Next();                                                        // ��ȡ���4������ֵ--Position.y
		trans->Position.y = atoi(attributeofTrans->Value());
		attributeofTrans = attributeofTrans->Next();                                                        // ��ȡ���5������ֵ--bFire
		CString bfire(attributeofTrans->Value());
		if(bfire == "true") trans->bFire=TRUE; else trans->bFire=FALSE;

		//====��ȡpt1��pt2��arryTransPre��arryTransPst���⼸����������ΪTransition���ӽڵ�
		TiXmlElement * elementofTrans  = TransitionNode->FirstChildElement();                               // ��ȡ��һ���ӽڵ�--pt1
		TiXmlAttribute * attributeofpt1 = elementofTrans->FirstAttribute();                                 // ��ȡpt1�ĵ�һ������x
		trans->pt1.x = atoi(attributeofpt1->Value());

		attributeofpt1 = attributeofpt1->Next();                                                            // ��ȡpt1�ĵڶ�������y
		trans->pt1.y = atoi(attributeofpt1->Value());

		elementofTrans = elementofTrans->NextSiblingElement();                                              // ��ȡ�ڶ����ӽڵ� pt2
		TiXmlAttribute * attributeofpt2 = elementofTrans->FirstAttribute();                                 // ��ȡpt2�ĵ�һ������x
		trans->pt2.x = atoi(attributeofpt2->Value());

		attributeofpt2 = attributeofpt2->Next();                                                            // ��ȡpt2�ĵڶ�������y
		trans->pt2.y = atoi(attributeofpt2->Value());
		 
		elementofTrans = elementofTrans->NextSiblingElement();                                              // ��ȡ�������ӽڵ� arryTransPre--�����ȡ
		elementofTrans = elementofTrans->NextSiblingElement();                                              // ��ȡ���ĸ��ӽڵ� arryTransPst--�����ȡ

		trans->MoveWindow(trans->Position.x + pt_ViewportOrg.x,trans->Position.y + pt_ViewportOrg.y,rectofTrans.Width(),rectofTrans.Height());    // �ƶ�ģ�͵�ָ��λ��
		trans->ShowWindow(SW_SHOW);                                                                         // ��ʾģ��

		Vector_Trans.push_back(trans);                                                                      // ��󣬽���Ǩ���浽Vector_Trans������
	}
	//--------------------------------------��ȡ��Ǩ��Ϣ-----------------------------------------------------End----------

	//--------------------------------------��ȡ������Ϣ---------------------------------------------------Begin--------
	TiXmlElement *ArcsNode = TransitionsNode->NextSiblingElement();                                        // ��ȡArcs�ڵ�
	TiXmlElement *ArcNode = ArcsNode->FirstChildElement();                                                 // ��ȡ��һ�����򻡽ڵ�
	for(;ArcNode!=NULL;ArcNode = ArcNode->NextSiblingElement())                                            // �������е����򻡽ڵ�
	{
		CPoint ptstart;
		CPoint ptend;
		TiXmlAttribute *attributeofArc = ArcNode->FirstAttribute();                                        // ��ȡ���򻡵ĵ�һ������--whichModel_Start
		int whichmodelstart = atoi(attributeofArc->Value());
		attributeofArc = attributeofArc->Next();                                                           // ������򻡵ĵڶ�������--u_SubScriptofPlace
		int subofplace = atoi(attributeofArc->Value());
		attributeofArc = attributeofArc->Next();                                                           // ������򻡵ĵ���������--u_SubScriptofTrans
		int suboftrans = atoi(attributeofArc->Value());
		attributeofArc = attributeofArc->Next();
		ptstart.x = atoi(attributeofArc->Value());
		attributeofArc = attributeofArc->Next();
		ptstart.y = atoi(attributeofArc->Value());
		attributeofArc = attributeofArc->Next();
		ptend.x = atoi(attributeofArc->Value());
		attributeofArc = attributeofArc->Next();
		ptend.y = atoi(attributeofArc->Value());
		attributeofArc = attributeofArc->Next();
		int arrowid = atoi(attributeofArc->Value());
		attributeofArc = attributeofArc->Next();
		int isdisic = atoi(attributeofArc->Value());

		// ������ֵ��ֵ��arr_Arrow�����е�Ԫ��
		CArrow *arrow = new CArrow(subofplace,suboftrans,whichmodelstart,ptstart,ptend,arrowid,isdisic);
		arr_Arrow.Add(arrow);
		// ���¿����ͱ�Ǩ��ǰ�úͺ���			
		vector<CPlace*>::iterator iterp=FindPlace(subofplace);
		vector<CTransition*>::iterator itert=FindTrans(suboftrans);
		switch(whichmodelstart)
		{
		case 1:// ��ʼģ��Ϊ����������� �����ĺ��úͱ�Ǩ��ǰ��
			(*iterp)->arryPlacePst.push_back(*itert);
			(*itert)->arryTransPre.push_back(*iterp);
			(*iterp)->arryArrowPst.push_back(arrow);
			(*itert)->arryArrowPre.push_back(arrow);
			break;
		case 2:// ��ʼģ��Ϊ��Ǩ������ ��Ǩ�ĺ��úͿ�����ǰ��
			(*itert)->arryTransPst.push_back(*iterp);
			(*iterp)->arryPlacePre.push_back(*itert);
			(*itert)->arryArrowPst.push_back(arrow);
			(*iterp)->arryArrowPre.push_back(arrow);
			break;
		default:
			break;
		}
	}
	//--------------------------------------��ȡ������Ϣ---------------------------------------------------End----------

	//--------------------------------------��ȡ���ģ����Ϣ-------------------------------------------------Begin--------
	TiXmlElement *OutModelsNode = ArcsNode->NextSiblingElement();                                               // ��ȡOutModels�ڵ�
	TiXmlElement *OutModelNode  = OutModelsNode->FirstChildElement();                                           // ��ȡ��һ�����ģ��ڵ�
	for(;OutModelNode!=NULL;OutModelNode = OutModelNode->NextSiblingElement())                                  // �������е����ģ��ڵ�
	{
		CCTRLShowMatrix * m_OutputModel = new CCTRLShowMatrix();
		TiXmlAttribute * attributeofOuts = OutModelNode->FirstAttribute();
		attributeofOuts = attributeofOuts->Next();
		attributeofOuts = attributeofOuts->Next();
		attributeofOuts = attributeofOuts->Next();
		attributeofOuts = attributeofOuts->Next();
		CString outputstyle1(attributeofOuts->Value());
		if(outputstyle1 == "1") m_OutputModel->OutputStyle = OUT_SHOWMATRIX;
		if(outputstyle1 == "2") m_OutputModel->OutputStyle = OUT_TOFILE;
		if(outputstyle1 == "3") m_OutputModel->OutputStyle = OUT_ACCUMULATOR;

		m_OutputModel->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofOutputModel,m_pView,m_OutputModel->OutputStyle);

		//===��ȡm_caption��ID��Position��OutputStyle����==
		attributeofOuts = OutModelNode->FirstAttribute();                                     // ��ȡ���һ������ֵ--m_caption
		m_OutputModel->m_caption = attributeofOuts->Value();

		attributeofOuts = attributeofOuts->Next();                                                             //��ȡ����ֵ��ID
		m_OutputModel->ID = atoi(attributeofOuts->Value());
		if(m_OutputModel->ID > max_subOuts)
			max_subOuts = m_OutputModel->ID;

		attributeofOuts = attributeofOuts->Next();                                                             //��ȡ����ֵ��x
		m_OutputModel->Position.x = atoi(attributeofOuts->Value());
		attributeofOuts = attributeofOuts->Next();                                                             //��ȡ����ֵ��y
		m_OutputModel->Position.y = atoi(attributeofOuts->Value());

		attributeofOuts = attributeofOuts->Next();                                                             //��ȡ����ֵ��OutputStyle
		CString outputstyle(attributeofOuts->Value());
		if(outputstyle == "1") m_OutputModel->OutputStyle = OUT_SHOWMATRIX;
		if(outputstyle == "2") m_OutputModel->OutputStyle = OUT_TOFILE;
		if(outputstyle == "3") m_OutputModel->OutputStyle = OUT_ACCUMULATOR;
		
		//===��ȡOutPName��vec_OutValueName��i_PposnOValue��vec_OutValue����==
		TiXmlElement * elementofOuts  = OutModelNode->FirstChildElement();                                     // ��ȡ����ֵ��OutPName
		m_OutputModel->OutPName = elementofOuts->GetText();

		elementofOuts  = elementofOuts->NextSiblingElement();                                                    // ��ȡ����ֵ��vec_OutValueName
		TiXmlElement * elementofvOVN  = elementofOuts->FirstChildElement();
		for(;elementofvOVN!=NULL;elementofvOVN=elementofvOVN->NextSiblingElement())
		{
			CString outvaluename(elementofvOVN->GetText());
			m_OutputModel->vec_OutValueName.push_back(outvaluename);
		}

		elementofOuts  = elementofOuts->NextSiblingElement();                                                    // ��ȡ����ֵ��i_PposnOValue
		//m_OutputModel->i_PposnOValue = atoi(elementofOuts->Value());
		m_OutputModel->i_PposnOValue = atoi(elementofOuts->GetText());

		elementofOuts  = elementofOuts->NextSiblingElement();                                                    // ��ȡ����ֵ��vec_OutValue
		TiXmlElement * elementofvOV  = elementofOuts->FirstChildElement();
		for(;elementofvOV!=NULL;elementofvOV=elementofvOV->NextSiblingElement())
		{
			int outvalue = atoi(elementofvOV->GetText());
			m_OutputModel->vec_OutValue.push_back(outvalue);
		}

		m_OutputModel->vec_OutputData.clear();
		int nsize = m_OutputModel->vec_OutValueName.size();
		for(int i=0;i<nsize;i++)
		{
			VEC_DOUBLE* m_Value = new VEC_DOUBLE;
			m_OutputModel->vec_OutputData.push_back(m_Value);
		}

		elementofOuts  = elementofOuts->NextSiblingElement();                                                    // ��ȡ����ֵ��str_FilePath
		m_OutputModel->str_FilePath = elementofOuts->GetText();

		if(m_OutputModel->OutputStyle == OUT_TOFILE)
		{
			m_OutputModel->m_pDatafile = new ofstream(m_OutputModel->str_FilePath);
		}

		m_OutputModel->MoveWindow(m_OutputModel->Position.x + pt_ViewportOrg.x,m_OutputModel->Position.y + pt_ViewportOrg.y,rectofOutputModel.Width(),rectofOutputModel.Height());
		m_OutputModel->ShowWindow(SW_SHOW);

		Vector_OutputModel.push_back(m_OutputModel);
	}
	//--------------------------------------��ȡ���ģ����Ϣ-------------------------------------------------End----------

	if(max_subPlace==0)
	{
		m_pView->u_SubScriptofPlace = 1;//0;
	}
	else
	{
		m_pView->u_SubScriptofPlace = max_subPlace + 1;
	}
	if(max_subTrans==0)
	{
		m_pView->u_SubScriptofTrans = 1;//0;
	}
	else
	{
		m_pView->u_SubScriptofTrans = max_subTrans + 1;
	}
	if(max_subOuts==0)
	{
		m_pView->u_SubScriptofOutputModel = 1;//0;
	}
	else
	{
		m_pView->u_SubScriptofOutputModel = max_subOuts + 1;
	}

    if(RootElement!=NULL)
	{
		RootElement->Clear();
	}

	return myDocument;
}

TiXmlDocument* CHSTPN_SYSDoc::ReadXMLFile(CString filePath_Name,TiXmlDocument *myDocument,CHSTPN_SYSView *m_pView)
{
	UINT max_subPlace = 0;
	UINT max_subTrans = 0;                          // �ҳ���ģ���п����ͱ�Ǩ������±�
	UINT max_subOuts  = 0;
	char *filepath_name;
	USES_CONVERSION;
	filepath_name = T2A(filePath_Name);
	myDocument = new TiXmlDocument(filepath_name);
	myDocument->LoadFile();                         // �����ļ�

	// ��ȡ�ӿ�ԭ��
	CPoint pt_ViewportOrg;
	CClientDC dc((CWnd*)m_pView);
	m_pView->OnPrepareDC(&dc);
	pt_ViewportOrg = dc.GetViewportOrg();

	TiXmlElement *RootElement = myDocument->RootElement();                                                    // ��ȡ��Ԫ��--HSTPnet
	TiXmlElement *PTONode = RootElement->FirstChildElement();
	CString PTONodeName;
	for(;PTONode!=NULL;PTONode=PTONode->NextSiblingElement())// ���������ӽڵ�"ListGlobal"/"Places"/"Transitions"/"Arcs"/"OutModels"
	{
		PTONodeName = PTONode->Value();
		//-----------------------------��ȡȫ�ֱ�������Ϣ----------------------------------------------------------------------------------------
		if(PTONodeName == "ListGlobal")
		{
			TiXmlElement * IOputDataGNode = PTONode->FirstChildElement();
			for(;IOputDataGNode!=NULL;IOputDataGNode=IOputDataGNode->NextSiblingElement())
			{
				IOValue * ioputdata = new IOValue();
				TiXmlAttribute * AttributeofIOG = IOputDataGNode->FirstAttribute(); // ����ÿ�����������нṹ��Name/IOType/InitValue
				CString AttributeofIOGName;
				for(;AttributeofIOG!=NULL;AttributeofIOG=AttributeofIOG->Next())
				{
					AttributeofIOGName = AttributeofIOG->Name();
					if(AttributeofIOGName == "Name")
					{
						ioputdata->Name = AttributeofIOG->Value();
					}
					if(AttributeofIOGName == "IOType")
					{
						ioputdata->IOType = AttributeofIOG->Value();
					}
					if(AttributeofIOGName == "InitValue")
					{
						ioputdata->Value = (atoi(AttributeofIOG->Value()))/10000.0;
						ioputdata->InitValue = ioputdata->Value;
					}
				}
				this->arryIOputDataG.push_back(ioputdata);
			}

		}
		//-----------------------------��ȡ������Ϣ----------------------------------------------------------------------------------------------
		if(PTONodeName == "Places")
		{
			TiXmlElement * PlaceNode = PTONode->FirstChildElement();
			for(;PlaceNode!=NULL;PlaceNode=PlaceNode->NextSiblingElement())// ����ÿһ������Ԫ��
			{
				CPlace * place = new CPlace();
				TiXmlAttribute * AttributeofP = PlaceNode->FirstAttribute();
				CString AttributeofPName;
				for(;AttributeofP!=NULL;AttributeofP=AttributeofP->Next())//��������������m_caption/u_SubscriptofModel/PlaceStyle/x/y/bActivation/bEnable/bisAssigned/ischoseSocket/ischoseupdown/dTime_downline/dTime_upline/ischoseProgRand
				{
					AttributeofPName = AttributeofP->Name();
					if(AttributeofPName == "m_caption")
					{
						place->m_caption = AttributeofP->Value();
					}
					if(AttributeofPName == "u_SubscriptofModel")
					{
						place->u_SubscriptofModel = atoi(AttributeofP->Value());
						if(place->u_SubscriptofModel > max_subPlace)
							max_subPlace = place->u_SubscriptofModel;
					}
					if(AttributeofPName == "PlaceStyle")
					{
						CString placestyle(AttributeofP->Value());
						if(placestyle == "1") place->PlaceStyle  = STYLE_DISCRETE;
						if(placestyle == "2") place->PlaceStyle  = STYLE_CNTNUS;
						if(placestyle == "3") place->PlaceStyle  = STYLE_RAND;
						if(placestyle == "4") place->PlaceStyle  = STYLE_TIME;
						if(placestyle == "5") place->PlaceStyle  = STYLE_TCTRL;
						if(placestyle == "6") place->PlaceStyle  = STYLE_LCTRL;
						if(placestyle == "7") place->PlaceStyle  = STYLE_DISIC;
					}
					if(AttributeofPName == "x")
					{
						place->Position.x = atoi(AttributeofP->Value());
					}
					if(AttributeofPName == "y")
					{
						place->Position.y = atoi(AttributeofP->Value());
					}
					if(AttributeofPName == "bActivation")
					{
						CString bactivation(AttributeofP->Value());
						if(bactivation=="true") place->bActivation=true; else place->bActivation=false;
					}
					if(AttributeofPName == "bEnable")
					{
						CString benable(AttributeofP->Value());
						if(benable=="true") place->bEnable=true; else place->bEnable=false;
					}
					if(AttributeofPName == "bisAssigned")
					{
						CString bassigned(AttributeofP->Value());
						if(bassigned=="true") place->bisAssigned=true; else place->bisAssigned=false;
					}
					if(AttributeofPName == "ischoseSocket")
					{
						CString ischoseSocket(AttributeofP->Value());
						if(ischoseSocket=="true") place->ischoseSocket=true; else place->ischoseSocket=false;
					}
					if(AttributeofPName == "ischoseupdown")
					{
						CString ischose(AttributeofP->Value());
						if(ischose=="true") place->ischoseupdown = true; else place->ischoseupdown = false;
					}
					if(AttributeofPName == "dTime_downline")
					{
						place->dTime_downline = (atoi(AttributeofP->Value()))/10000.0;
					}
					if(AttributeofPName == "dTime_upline")
					{
						place->dTime_upline = (atoi(AttributeofP->Value()))/10000.0;
					}
					if(AttributeofPName == "ischoseProgRand")
					{
						CString ischoseusr(AttributeofP->Value());
						if(ischoseusr =="true") place->ischoseProgRand = true; else place->ischoseProgRand = false;
					}	
				}
				TiXmlElement * ChildofP = PlaceNode->FirstChildElement();
				CString ChildofPName;
				for(;ChildofP!=NULL;ChildofP=ChildofP->NextSiblingElement())// �����������ӽڵ�
				{
					ChildofPName = ChildofP->Value();
					if(ChildofPName == "S_Luafile")
					{
						const char* S_luafunc = ChildofP->GetText();
						if(place->ischoseProgRand && place->PlaceStyle == STYLE_RAND)  // �����⺯����Lua��������Ϊ��,�ҿ���������Ϊ��������ʱ����д���ļ�
						{        
							CString Lua_fileName_1 = _T("LuaFile\\S_") + place->m_caption + _T(".lua");
							CFile file(Lua_fileName_1,CFile::modeCreate | CFile::modeWrite);
							file.Write(S_luafunc,strlen(S_luafunc));
							file.Close();
						}
					}
					if(ChildofPName == "dProblty")
					{
						place->dProblty = atof(ChildofP->GetText());
					}
					if(ChildofPName == "dTime")
					{
						place->dTime = atof(ChildofP->GetText());
						place->d_TimeTS = place->dTime;
					}
					if(ChildofPName == "ipadrs")
					{
						place->ipadrs = ChildofP->GetText();
					}
					if(ChildofPName == "port")
					{
						place->port = atoi(ChildofP->GetText());
					}
					if(ChildofPName == "num_input")
					{
						place->num_input = atoi(ChildofP->GetText());
					}
					if(ChildofPName == "num_output")
					{
						place->num_output = atoi(ChildofP->GetText());
					}
					if(ChildofPName == "Lua_fileName")
					{
						place->Lua_fileName = ChildofP->GetText();
					}
					if(ChildofPName == "pt1")
					{
						TiXmlAttribute *attributeofpt1 = ChildofP->FirstAttribute();                                   // ��ȡpt1�ĵ�һ������x
						place->pt1.x = atoi(attributeofpt1->Value());
						attributeofpt1 = attributeofpt1->Next();                                                       // ��ȡpt1�ĵڶ�������y
						place->pt1.y = atoi(attributeofpt1->Value());
					}
					if(ChildofPName == "pt2")
					{
						TiXmlAttribute *attributeofpt2 = ChildofP->FirstAttribute();                                  // ��ȡpt2�ĵ�һ������x
						place->pt2.x = atoi(attributeofpt2->Value());
						attributeofpt2 = attributeofpt2->Next();                                                      // ��ȡpt2�ĵڶ�������y
						place->pt2.y = atoi(attributeofpt2->Value());
					}
					if(ChildofPName == "pt3")
					{
						TiXmlAttribute *attributeofpt3 = ChildofP->FirstAttribute();                                  // ��ȡpt3�ĵ�һ������x
						place->pt3.x = atoi(attributeofpt3->Value());
						attributeofpt3 = attributeofpt3->Next();                                                      // ��ȡpt3�ĵڶ�������y
						place->pt3.y = atoi(attributeofpt3->Value());
					}
					if(ChildofPName == "pt4")
					{
						TiXmlAttribute *attributeofpt4 = ChildofP->FirstAttribute();                                  // ��ȡpt4�ĵ�һ������x
						place->pt4.x = atoi(attributeofpt4->Value());
						attributeofpt4 = attributeofpt4->Next();                                                      // ��ȡpt4�ĵڶ�������y
						place->pt4.y = atoi(attributeofpt4->Value());
					}
					if(ChildofPName == "arryPlacePre")
					{}
					if(ChildofPName == "arryPlacePst")
					{}
					if(ChildofPName == "bisUserDef_prob")
					{
						CString bisuserdef_prob(ChildofP->GetText());
						if(bisuserdef_prob=="true") place->bisUserDef_prob=true; else place->bisUserDef_prob=false;
					}
					if(ChildofPName == "bisUserDef_time")
					{
						CString bisuserdef_time(ChildofP->GetText());
						if(bisuserdef_time=="true") place->bisUserDef_time=true; else place->bisUserDef_time=false;
					}
					if(ChildofPName == "bisUserDef_ctrl")
					{
						CString bisuserdef_ctrl(ChildofP->GetText());
						if(bisuserdef_ctrl=="true") place->bisUserDef_ctrl=true; else place->bisUserDef_ctrl=false;
					}
					if(ChildofPName == "str_Inputport_prob")
					{
						place->str_Inputport_prob = ChildofP->GetText();
					}
					if(ChildofPName == "str_Inputport_time")
					{
						place->str_Inputport_time = ChildofP->GetText();
					}
					if(ChildofPName == "str_Inputport_ctrl")
					{
						place->str_Inputport_ctrl = ChildofP->GetText();
					}
					if(ChildofPName == "arryIOputData")
					{
						TiXmlElement *elementofIOputData = ChildofP->FirstChildElement();
						for(;elementofIOputData!=NULL;elementofIOputData = elementofIOputData->NextSiblingElement())       // ����arryIOputData�е����б����Ľڵ�
						{
							IOValue *ioputdata = new IOValue();
							//������������ Name/Sequence/IOType/isQuoteG/GlobalVluName/FromModel/FromMDVluName/InitValue
							TiXmlAttribute *attributeofioputdata = elementofIOputData->FirstAttribute();
							CString attributeofioputdataName;
							for(;attributeofioputdata!=NULL;attributeofioputdata=attributeofioputdata->Next())
							{
								attributeofioputdataName = attributeofioputdata->Name();
								if(attributeofioputdataName == "Name")
								{
									ioputdata->Name = attributeofioputdata->Value();
								}
								if(attributeofioputdataName == "Sequence")
								{
									ioputdata->Sequence = atoi(attributeofioputdata->Value());
								}
								if(attributeofioputdataName == "IOType")
								{
									ioputdata->IOType = attributeofioputdata->Value();
								}
								if(attributeofioputdataName == "isQuoteG")
								{
									CString isquotegstr(attributeofioputdata->Value());
									if(isquotegstr == "true") ioputdata->isQuoteG=true; else ioputdata->isQuoteG = false;
								}
								if(attributeofioputdataName == "GlobalVluName")
								{
									ioputdata->GlobalVluName = attributeofioputdata->Value();
								}
								if(attributeofioputdataName == "FromModel")
								{
									ioputdata->FromModel = attributeofioputdata->Value();
								}
								if(attributeofioputdataName == "FromMDVluName")
								{
									ioputdata->FromMDVluName = attributeofioputdata->Value();
								}
								if(attributeofioputdataName == "InitValue")
								{
									ioputdata->Value = (atoi(attributeofioputdata->Value()))/10000.0;
									ioputdata->InitValue = ioputdata->Value;
								}
							}
							place->arryIOputData.push_back(ioputdata);
						}
					}
					if(place->arryIOputData.size()!=0)
					{
						vector<IOValue*> InputofLuaTS;
						vector<IOValue*> OutputofLuaTS;
						place->arryInputofLua.clear();
						place->arryOutputofLua.clear();
						for(vector<IOValue*>::iterator iter=place->arryIOputData.begin();iter!=place->arryIOputData.end();iter++)
						{
							if((*iter)->IOType == "input")
						    {
							    if((*iter)->Sequence != 0)
							    {
								    InputofLuaTS.push_back(*iter);
								    place->arryInputofLua.push_back(*iter);
							    }
						    }
						    if((*iter)->IOType == "output")
						    {
							    if((*iter)->Sequence != 0)
							    {
								    OutputofLuaTS.push_back(*iter);
								    place->arryOutputofLua.push_back(*iter);
							    }
						    }
						    if((*iter)->IOType == "in/output")
						    {
							    if((*iter)->Sequence != 0)
							    {
								    InputofLuaTS.push_back(*iter);
								    OutputofLuaTS.push_back(*iter);
								    place->arryInputofLua.push_back(*iter);
								    place->arryOutputofLua.push_back(*iter);
							    }
						    }
						}
						// ��������
					    for(vector<IOValue*>::iterator iter_i=InputofLuaTS.begin();iter_i!=InputofLuaTS.end();iter_i++)
					    {
						    if(place->arryInputofLua.at((*iter_i)->Sequence-1) != NULL)
							    place->arryInputofLua.at((*iter_i)->Sequence-1) = *iter_i;
					    }
					    for(vector<IOValue*>::iterator iter_o=OutputofLuaTS.begin();iter_o!=OutputofLuaTS.end();iter_o++)
					    {
						    if(place->arryOutputofLua.at((*iter_o)->Sequence-1)!=NULL)
							    place->arryOutputofLua.at((*iter_o)->Sequence-1) = (*iter_o);
					    }
					}
					if(ChildofPName == "arryInputofLua")
					{
					}
					if(ChildofPName == "arryOutputofLua")
					{
					}
					if(ChildofPName == "Lua_function")
					{
						const char* luafunc = ChildofP->GetText();
						if(place->Lua_fileName!="" && place->PlaceStyle == STYLE_CNTNUS)                              // �����⺯����Lua��������Ϊ��,�ҿ���������Ϊ��������ʱ����д���ļ�
						{        
							CString Lua_fileName_1 = _T("LuaFile\\") + place->Lua_fileName + _T(".lua");
							CFile file(Lua_fileName_1,CFile::modeCreate | CFile::modeWrite);
							file.Write(luafunc,strlen(luafunc));
							file.Close();
						}
					}
					if(ChildofPName == "CtrlPlaceStyle")
					{
						CString ctrlstyle(ChildofP->GetText());
						if(ctrlstyle == "TCTRL")
						{
							place->CtrlPlaceStyle = TCTRL;
						}
						else
						{
							place->CtrlPlaceStyle = LCTRL;
						}
					}
				}

				place->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofModel,m_pView,place->PlaceStyle);
				place->m_thisModel = place;
				place->MoveWindow(place->Position.x + pt_ViewportOrg.x,place->Position.y + pt_ViewportOrg.y,rectofModel.Width(),rectofModel.Height());
				place->ShowWindow(SW_SHOW);                                                                  // ��ʾģ��

				Vector_Place.push_back(place);
			}
		}
		//-----------------------------��ȡ��Ǩ��Ϣ----------------------------------------------------------------------------------------------
		if(PTONodeName == "Transitions")
		{
			TiXmlElement *TransitionNode = PTONode->FirstChildElement();
			for(;TransitionNode!=NULL;TransitionNode=TransitionNode->NextSiblingElement()) // �������еı�Ǩ
			{
				CTransition *trans = new CTransition();
				// ������Ǩ���е�����m_caption/u_SubscriptofTrans/x/y/bFire
				TiXmlAttribute * attributeofTrans = TransitionNode->FirstAttribute();
				CString attributeofTransName;
				for(;attributeofTrans!=NULL;attributeofTrans=attributeofTrans->Next())
				{
					attributeofTransName = attributeofTrans->Name();
					if(attributeofTransName == "m_caption")
					{
						trans->m_caption = attributeofTrans->Value();
					}
					if(attributeofTransName == "u_SubscriptofTrans")
					{
						trans->u_SubscriptofTrans = atoi(attributeofTrans->Value());
						if(trans->u_SubscriptofTrans > max_subTrans)
							max_subTrans = trans->u_SubscriptofTrans;
					}
					if(attributeofTransName == "x")
					{
						trans->Position.x = atoi(attributeofTrans->Value());
					}
					if(attributeofTransName == "y")
					{
						trans->Position.y = atoi(attributeofTrans->Value());
					}
					if(attributeofTransName == "bFire")
					{
						CString bfire(attributeofTrans->Value());
						if(bfire == "true") trans->bFire=true; else trans->bFire=false;
					}
				}
				// ������Ǩ���е��ӽڵ�pt1/pt2/arryTransPre/arryTransPst
				TiXmlElement * elementofTrans  = TransitionNode->FirstChildElement();
				CString elementofTransName;
				for(;elementofTrans!=NULL;elementofTrans=elementofTrans->NextSiblingElement())
				{
					elementofTransName = elementofTrans->Value();
					if(elementofTransName == "pt1")
					{
						TiXmlAttribute * attributeofpt1 = elementofTrans->FirstAttribute();                                 // ��ȡpt1�ĵ�һ������x
						trans->pt1.x = atoi(attributeofpt1->Value());
						attributeofpt1 = attributeofpt1->Next();                                                            // ��ȡpt1�ĵڶ�������y
						trans->pt1.y = atoi(attributeofpt1->Value());
					}
					if(elementofTransName == "pt2")
					{
						TiXmlAttribute * attributeofpt2 = elementofTrans->FirstAttribute();                                 // ��ȡpt2�ĵ�һ������x
						trans->pt2.x = atoi(attributeofpt2->Value());
						attributeofpt2 = attributeofpt2->Next();                                                            // ��ȡpt2�ĵڶ�������y
						trans->pt2.y = atoi(attributeofpt2->Value());
					}
					if(elementofTransName == "arryTransPre")
					{}
					if(elementofTransName == "arryTransPst")
					{}
				}
				trans->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofTrans,m_pView);
				trans->MoveWindow(trans->Position.x + pt_ViewportOrg.x,trans->Position.y + pt_ViewportOrg.y,rectofTrans.Width(),rectofTrans.Height());    // �ƶ�ģ�͵�ָ��λ��
				trans->ShowWindow(SW_SHOW);                                                                         // ��ʾģ��

				Vector_Trans.push_back(trans); 
			}
		}
		//-----------------------------��ȡ������Ϣ--------------------------------------------------------------------------------------------
		if(PTONodeName == "Arcs")
		{
			TiXmlElement *ArcNode = PTONode->FirstChildElement();
			for(;ArcNode!=NULL;ArcNode=ArcNode->NextSiblingElement()) // �������е�����
			{
				// �������򻡵�����whichModel_Start/u_SubScriptofPlace/u_SubScriptofTrans
				TiXmlAttribute *attributeofArc = ArcNode->FirstAttribute();
				CString attributeofArcName;
				int whichmodelstart;
				int subofplace;
				int suboftrans;
				CPoint ptstart;
				CPoint ptend;
				int arrowid;
				bool isdisic = false;
				int num1;
				for(;attributeofArc!=NULL;attributeofArc=attributeofArc->Next())
				{
					attributeofArcName = attributeofArc->Name();
					if(attributeofArcName == "whichModel_Start")
					{
						whichmodelstart = atoi(attributeofArc->Value());
					}
					if(attributeofArcName == "u_SubScriptofPlace")
					{
						subofplace = atoi(attributeofArc->Value());
					}
					if(attributeofArcName == "u_SubScriptofTrans")
					{
						suboftrans = atoi(attributeofArc->Value());
					}
					if(attributeofArcName == "pt_startx")
					{
						ptstart.x = atoi(attributeofArc->Value());
					}
					if(attributeofArcName == "pt_starty")
					{
						ptstart.y = atoi(attributeofArc->Value());
					}
					if(attributeofArcName == "pt_endx")
					{
						ptend.x = atoi(attributeofArc->Value());
					}
					if(attributeofArcName == "pt_starty")
					{
						ptend.y = atoi(attributeofArc->Value());
					}
					if(attributeofArcName == "u_arrow_id")
					{
						arrowid = atoi(attributeofArc->Value());
					}
					if(attributeofArcName == "isdisic")
					{
						CString isdisic1(attributeofArc->Value());
						if(isdisic1 == "true") isdisic=true; else isdisic=false;
					}
					if(attributeofArcName == "num")
					{
						num1 = atoi(attributeofArc->Value());
					}
				}
				// ������ֵ��ֵ��arr_Arrow�����е�Ԫ��
				CArrow *arrow = new CArrow(subofplace,suboftrans,whichmodelstart,ptstart,ptend,arrowid,isdisic);
				if (isdisic)
				{
					CString str;
					str.Format(_T("%d"),num1);
					arrow->m_pEdit->SetWindowText(str);
					arrow->num = num1;
				}
				arr_Arrow.Add(arrow);

				// ���¿����ͱ�Ǩ��ǰ�úͺ���			
				vector<CPlace*>::iterator iterp=FindPlace(subofplace);
				vector<CTransition*>::iterator itert=FindTrans(suboftrans);
				switch(whichmodelstart)
				{
				case 1:// ��ʼģ��Ϊ����������� �����ĺ��úͱ�Ǩ��ǰ��
					(*iterp)->arryPlacePst.push_back(*itert);
					(*itert)->arryTransPre.push_back(*iterp);
					(*iterp)->arryArrowPst.push_back(arrow);
					(*itert)->arryArrowPre.push_back(arrow);
					break;
				case 2:// ��ʼģ��Ϊ��Ǩ������ ��Ǩ�ĺ��úͿ�����ǰ��
					(*itert)->arryTransPst.push_back(*iterp);
					(*iterp)->arryPlacePre.push_back(*itert);
					(*itert)->arryArrowPst.push_back(arrow);
					(*iterp)->arryArrowPre.push_back(arrow);
					break;
				default:
					break;
				}
			}
		}
		//-----------------------------��ȡ����ڵ���Ϣ------------------------------------------------------------------------------------------
		if(PTONodeName == "OutModels")
		{
			TiXmlElement *OutModelNode  = PTONode->FirstChildElement();
			for(;OutModelNode!=NULL;OutModelNode = OutModelNode->NextSiblingElement()) // �������е�����ڵ�
			{
				CCTRLShowMatrix * m_OutputModel = new CCTRLShowMatrix();
				// ��ȡ����ڵ������m_caption/ID/x/y/OutputStyle
				TiXmlAttribute * attributeofOuts = OutModelNode->FirstAttribute();
				CString attributeofOutsName;
				for(;attributeofOuts!=NULL;attributeofOuts=attributeofOuts->Next())
				{
					attributeofOutsName = attributeofOuts->Name();
					if(attributeofOutsName == "m_caption")
					{
						m_OutputModel->m_caption = attributeofOuts->Value();
					}
					if(attributeofOutsName == "ID")
					{
						m_OutputModel->ID = atoi(attributeofOuts->Value());
						if(m_OutputModel->ID > max_subOuts)
							max_subOuts = m_OutputModel->ID;
					}
					if(attributeofOutsName == "x")
					{
						m_OutputModel->Position.x = atoi(attributeofOuts->Value());
					}
					if(attributeofOutsName == "y")
					{
						m_OutputModel->Position.y = atoi(attributeofOuts->Value());
					}
					if(attributeofOutsName == "OutputStyle")
					{
						CString outputstyle(attributeofOuts->Value());
						if(outputstyle == "1") m_OutputModel->OutputStyle = OUT_SHOWMATRIX;
						if(outputstyle == "2") m_OutputModel->OutputStyle = OUT_TOFILE;
						if(outputstyle == "3") m_OutputModel->OutputStyle = OUT_ACCUMULATOR;
					}
				}
				// �������ݽڵ���ӽڵ�OutPName/vec_OutValueName/i_PposnOValue/vec_OutValue/str_FilePath
				TiXmlElement * elementofOuts  = OutModelNode->FirstChildElement();
				CString elementofOutsName;
				for(;elementofOuts!=NULL;elementofOuts=elementofOuts->NextSiblingElement())
				{
					elementofOutsName = elementofOuts->Value();
					if(elementofOutsName == "OutPName")
					{
						m_OutputModel->OutPName = elementofOuts->GetText();
					}
					if(elementofOutsName == "vec_OutValueName")
					{
						TiXmlElement * elementofvOVN  = elementofOuts->FirstChildElement();
						for(;elementofvOVN!=NULL;elementofvOVN=elementofvOVN->NextSiblingElement())
						{
							CString outvaluename(elementofvOVN->GetText());
							m_OutputModel->vec_OutValueName.push_back(outvaluename);
						}
					}
					if(elementofOutsName == "i_PposnOValue")
					{
						m_OutputModel->i_PposnOValue = atoi(elementofOuts->GetText());
					}
					if(elementofOutsName == "vec_OutValue")
					{
						TiXmlElement * elementofvOV  = elementofOuts->FirstChildElement();
						for(;elementofvOV!=NULL;elementofvOV=elementofvOV->NextSiblingElement())
						{
							int outvalue = atoi(elementofvOV->GetText());
							m_OutputModel->vec_OutValue.push_back(outvalue);
						}
						m_OutputModel->vec_OutputData.clear();
						int nsize = m_OutputModel->vec_OutValueName.size();
						for(int i=0;i<nsize;i++)
						{
							VEC_DOUBLE* m_Value = new VEC_DOUBLE;
							m_OutputModel->vec_OutputData.push_back(m_Value);
						}
					}
					if(elementofOutsName == "str_FilePath")
					{
						m_OutputModel->str_FilePath = elementofOuts->GetText();
					}
				}
				m_OutputModel->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofOutputModel,m_pView,m_OutputModel->OutputStyle);
				if(m_OutputModel->OutputStyle == OUT_TOFILE)
				{
					m_OutputModel->m_pDatafile = new ofstream(m_OutputModel->str_FilePath);
				}

				m_OutputModel->MoveWindow(m_OutputModel->Position.x + pt_ViewportOrg.x,m_OutputModel->Position.y + pt_ViewportOrg.y,rectofOutputModel.Width(),rectofOutputModel.Height());
				m_OutputModel->ShowWindow(SW_SHOW);

				Vector_OutputModel.push_back(m_OutputModel);/**/
			}
		}
	}
	if(max_subPlace==0)
	{
		m_pView->u_SubScriptofPlace = 1;//0;
	}
	else
	{
		m_pView->u_SubScriptofPlace = max_subPlace + 1;
	}
	if(max_subTrans==0)
	{
		m_pView->u_SubScriptofTrans = 1;//0;
	}
	else
	{
		m_pView->u_SubScriptofTrans = max_subTrans + 1;
	}
	if(max_subOuts==0)
	{
		m_pView->u_SubScriptofOutputModel = 1;//0;
	}
	else
	{
		m_pView->u_SubScriptofOutputModel = max_subOuts + 1;
	}

    if(RootElement!=NULL)
	{
		RootElement->Clear();
	}
	
	// ������е�ToFile�ͽڵ�������ļ��Ƿ���ڣ�����������򴴽���
	for(vector<CCTRLShowMatrix*>::iterator itero=Vector_OutputModel.begin();itero!=Vector_OutputModel.end();itero++)
	{
		if((*itero)->OutputStyle == OUT_TOFILE)
		{
			if(!PathIsDirectory((*itero)->str_FilePath))
			{
				CreateMultipleDirectory((*itero)->str_FilePath);
				//::CreateDirectory((*itero)->str_FilePath, NULL);
			}
		}
	}

	return myDocument;
}


bool CHSTPN_SYSDoc::CreateMultipleDirectory(const CString szPath)  
{  
    CString strDir(szPath);    //���Ҫ������Ŀ¼�ַ��� 
    if (strDir.GetAt(strDir.GetLength() - 1) != _T('\\'))  //ȷ����'\'��β�Դ������һ��Ŀ¼  
    {  
        strDir.AppendChar(_T('\\'));  
    }  
    vector<CString> vPath;              //���ÿһ��Ŀ¼�ַ���  
    CString strTemp;                    //һ����ʱ����,���Ŀ¼�ַ���  
    bool bSuccess = false;              //�ɹ���־  
                                        //����Ҫ�������ַ���  
    for (int i = 0; i < strDir.GetLength(); ++i)  
    {  
        if (strDir.GetAt(i) != _T('\\'))  
        {                               //�����ǰ�ַ�����'\\'  
            strTemp.AppendChar(strDir.GetAt(i));  
        }  
        else  
        {                                //�����ǰ�ַ���'\\'  
            vPath.push_back(strTemp);    //����ǰ����ַ�����ӵ�������  
            strTemp.AppendChar(_T('\\'));  
        }  
    }  
                                         //�������Ŀ¼������,����ÿ��Ŀ¼  
    vector<CString>::const_iterator vIter;  
    for (vIter = vPath.begin(); vIter != vPath.end()-1; vIter++)  
    {  
        if (!PathIsDirectory(*vIter))    //�жϵ�ǰĿ¼ʱ����ڣ������ھʹ���  
       {  
                                         //���CreateDirectoryִ�гɹ�,����true,���򷵻�false  
            bSuccess = CreateDirectory(*vIter, NULL) ? true : false;  
        }         
    } 

	// ������Ŀ¼�󣬴���һ��CSV�ļ�
	CFile file;
	file.Open(szPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
//	file.Close();
    return bSuccess;  
}  




// ���ҿ���
vector<CPlace*>::iterator CHSTPN_SYSDoc::FindPlace( UINT id)
{
	vector<CPlace*>::iterator iter = Vector_Place.begin(); // ����һ��������
	int nSize = Vector_Place.size();                       // ��ȡvector_place�Ĵ�С
	nSize = (int)floor(nSize/2.0);                              // �����־j
	int j = nSize;
	iter = iter + j;                                       // ���������ƽ����м�λ��

	do{
		if(id >= (*iter)->u_SubscriptofModel)                  //ҪѰ�ҵ�Ԫ�����м�Ԫ�ص��ұ�
		{
			if(nSize>=10)
			{
				nSize = (int)floor(nSize/2.0);
				j = nSize;
				iter = iter + j;
			}
			else
			{
				for(;iter!=Vector_Place.end();iter++)
				{
					if((*iter)->u_SubscriptofModel == id)
						return iter;
				}
			}
		}
		else                                                   //ҪѰ�ҵ�Ԫ�����м�Ԫ�ص����
		{
			if(nSize>=10)
			{
				nSize = (int)floor(nSize/2.0);
				j = nSize;
				iter = iter - j;
			}
			else
			{
				for(;iter!=Vector_Place.begin();iter--)
				{
					if((*iter)->u_SubscriptofModel == id)
						return iter;
				}
				// ����������û���ҵ�����ֻʣ��һ���ˣ���Ϊ��һ��û�б�������
				vector<CPlace*>::iterator iter1 = Vector_Place.begin();
				if((*iter1)->u_SubscriptofModel == id)
				{
					return iter1;
				}
			}

		}
	}while(id != (*iter)->u_SubscriptofModel);

	return iter;
}

// ���ұ�Ǩ
vector<CTransition*>::iterator CHSTPN_SYSDoc::FindTrans(UINT id)
{
	vector<CTransition*>::iterator iter = Vector_Trans.begin(); // ����һ��������
	int nSize = Vector_Trans.size();                       // ��ȡvector_Trans�Ĵ�С
	nSize = (int)floor(nSize/2.0);                              // �����־j
	int j = nSize;
	iter = iter + j;                                       // ���������ƽ����м�λ��

	do{
		if(id >= (*iter)->u_SubscriptofTrans)                  //ҪѰ�ҵ�Ԫ�����м�Ԫ�ص��ұ�
		{
			if(nSize>=10)
			{
				nSize = (int)floor(nSize/2.0);
				j = nSize;
				iter = iter + j;
			}
			else
			{
				for(;iter!=Vector_Trans.end();iter++)
				{
					if((*iter)->u_SubscriptofTrans == id)
						return iter;
				}
			}
		}
		else                                                   //ҪѰ�ҵ�Ԫ�����м�Ԫ�ص����
		{
			if(nSize>=10)
			{
				nSize = (int)floor(nSize/2.0);
				j = nSize;
				iter = iter - j;
			}
			else
			{
				for(;iter!=Vector_Trans.begin();iter--)
				{
					if((*iter)->u_SubscriptofTrans == id)
						return iter;
				}
				// ����������û���ҵ�����ֻʣ��һ���ˣ���Ϊ��һ��û�б�������
				vector<CTransition*>::iterator iter1 = Vector_Trans.begin();
				if((*iter1)->u_SubscriptofTrans == id)
				{
					return iter1;
				}
			}

		}
	}while(id != (*iter)->u_SubscriptofTrans);

	return iter;
}

// �����̣߳���ʼ����
void CHSTPN_SYSDoc::OnDebugStart()
{
	// TODO: �ڴ���������������
	// =============���ٷ����߳�===================

	if(Vector_Place.size()==0 || Vector_Trans.size()==0)// ����������ǨΪ�գ��򲻿�ʼ�����������û����ܴ��ڹ������
	{
		CHSTPN_SYSView* m_pHSTView;
		POSITION pos = GetFirstViewPosition();
		GetNextView(pos);
		m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);
		m_pHSTView->MessageBox(L"û��ģ�ͻ���ܴ��ڹ�����㣬�����ģ�ͼ��!");
	}
	else                                                // �����Ϊ�գ���ʼ
	{
		CHSTPN_SYSView* m_pHSTView;
		POSITION pos = GetFirstViewPosition();
		GetNextView(pos);
		m_pHSTView   = (CHSTPN_SYSView*)GetNextView(pos);

		m_hstpnData.vector_place        = Vector_Place;
		m_hstpnData.vector_trans        = Vector_Trans;
		m_hstpnData.arr_arrow.Copy(arr_Arrow);
		m_hstpnData.m_stopsimevent      = m_StopSimEvent;
		m_hstpnData.m_timestep          = m_pHSTView->d_TimeStep;
		m_hstpnData.m_simtime           = m_pHSTView->d_SimTime;
		m_hstpnData.m_intervaltime      = m_pHSTView->d_IntervalTime;
		m_hstpnData.vector_outputmodel  = Vector_OutputModel;
		m_hstpnData.m_pmainframe        = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
		m_hstpnData.m_pDoc              = this;
		
		if(m_pHSTView->d_SimTime >= 1.7e+300)
		{
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_wndStatusBar.EnablePaneProgressBar(1,2147483647,true);// ���÷�����ȵ����Χ
		}
		else
		{
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_wndStatusBar.EnablePaneProgressBar(1,m_pHSTView->d_SimTime,true);// ���÷�����ȵ����Χ
		}
		if(!b_SimSuspend)// ����ͣ״̬�£������з���
		{
			m_simThread = AfxBeginThread(m_hstpnSim->HSTPN_SimEngine/*HSTPN_Simulation*/,(LPVOID)&m_hstpnData);// �������̣߳����г�����档����1 �̵߳���ں���,����һ��Ҫ����: UINT MyThreadFunction( LPVOID pParam ); ����2 �������̵߳Ĳ���, ע����������Ϊ:LPVOID, �������ǿ��Դ���һ���ṹ�����̡߳�
		}
	}

}

// ����ģ�ͼ��Ի���
void CHSTPN_SYSDoc::OnModelcheck()
{
	// TODO: �ڴ���������������
	CHSTPN_SYSView* m_pHSTView;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);
	//---����ģ�ͼ��Ի���-----
	if(m_dlgModelCheck!=NULL)
	{
		m_dlgModelCheck->DestroyWindow();
		delete m_dlgModelCheck;
		m_dlgModelCheck = NULL;
	}
	if(m_dlgModelCheck == NULL)
	{
		m_hstpnData.vector_place = Vector_Place;
		m_hstpnData.vector_trans = Vector_Trans;
		m_hstpnData.arr_arrow.Copy(arr_Arrow);

		m_dlgModelCheck = new CDlgModelCheck();
		m_dlgModelCheck->Create((LPCTSTR)IDD_DLGMODELCHECK, m_hstpnSim, (LPVOID)&m_hstpnData,m_pHSTView);
	}
	m_dlgModelCheck->ShowWindow(SW_SHOW);
}

// ��ͣ����
void CHSTPN_SYSDoc::OnSimThreadSuspend()
{
	// TODO: �ڴ���������������
	if(Vector_Place.size()==0 || Vector_Trans.size()==0)// ����������ǨΪ�գ��򲻿�ʼ�����������û����ܴ��ڹ������
	{
		CHSTPN_SYSView* m_pHSTView;
		POSITION pos = GetFirstViewPosition();
		GetNextView(pos);
		m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);
		m_pHSTView->MessageBox(L"û��ģ�ͻ���ܴ��ڹ�����㣬�����ģ�ͼ��");
	}
	else
	{
		if(m_simThread!=NULL)
		{
			if(!b_SimSuspend)//����߳�û�б���ͣ������ͣ�߳�
			{
				m_simThread->SuspendThread();
				b_SimSuspend = TRUE;
			}
			else            //����̱߳���ͣ�ˣ�������߳�
			{
				m_simThread->ResumeThread();
				b_SimSuspend = FALSE;
			}
		}
	}
}

// ��������
void CHSTPN_SYSDoc::OnSimThreadStop()
{
	CHSTPN_SYSView* m_pHSTView;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);
	// TODO: �ڴ���������������
	if(Vector_Place.size()==0 || Vector_Trans.size()==0)// ����������ǨΪ�գ��򲻿�ʼ�����������û����ܴ��ڹ������
	{
		m_pHSTView->MessageBox(L"û��ģ�ͻ���ܴ��ڹ�����㣬�����ģ�ͼ��");
	}
	else
	{
		if(m_simThread!=NULL)
		{
			if(!b_SimSuspend)
			{
				SetEvent(m_StopSimEvent->m_hObject);// �����¼�Ϊ���ź�״̬��Ҳ������CHSTPN_SYSSim���HSTPN_Simulation()�����У����ú����˳��������̽���
				m_simThread = NULL;
			}
			else
			{
				/*CHSTPN_SYSView* m_pHSTView;
				POSITION pos = GetFirstViewPosition();
				GetNextView(pos);
				m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);
				m_pHSTView->MessageBox(L"��������״̬�µ��������ť");*/
				m_simThread->ResumeThread();//�����ʱΪ��ͣ״̬���Ȼظ�����
				b_SimSuspend = FALSE;
				SetEvent(m_StopSimEvent->m_hObject);// �ٽ����߳�
				m_simThread = NULL;
			}
			//m_pHSTView->OnRestorePnset();// ����View���OnRestorePnset()��������Petri�����ûس�ʼ״̬
		}
	}
}

// �����̣߳���ʼ�����������
void CHSTPN_SYSDoc::OnIncdmatanaly()
{
	// TODO: �ڴ���������������

	if(Vector_Place.size()==0 || Vector_Trans.size()==0)// ����������ǨΪ�գ��򲻿�ʼ�����������û����ܴ��ڹ������
	{
		CHSTPN_SYSView* m_pHSTView;
		POSITION pos = GetFirstViewPosition();
		GetNextView(pos);
		m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);
		m_pHSTView->MessageBox(L"û��ģ�ͻ���ܴ��ڹ�����㣬�����ģ�ͼ��");
	}
	else
	{
		m_hstpnData.vector_place = Vector_Place;
		m_hstpnData.vector_trans = Vector_Trans;
		m_hstpnData.arr_arrow.Copy(arr_Arrow);
		m_hstpnData.m_stopsimevent = NULL;

		m_IncdMatThread = AfxBeginThread(m_hstpnSim->IncdnceMatCompute,(LPVOID)&m_hstpnData);// �������̣߳����й����������
	}

}


void CHSTPN_SYSDoc::OnFileSceneOne() {
	// TODO: �ڴ���������������
	CHSTPN_SYSView* m_pHSTView;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);

	//====================�򿪲���=========================================================begin=========
	int n_SizeP = Vector_Place.size();
	int n_SizeT = Vector_Trans.size();
	int n_SizeA = arr_Arrow.GetSize();
	int n_SizeO = Vector_OutputModel.size();

	CFileDialog m_saveDlg(TRUE);                                             // �������ļ��Ի���
	m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//���ù�����
	m_saveDlg.m_ofn.lpstrDefExt = L"hst";                                    // ���ȱʡ����չ��

	CString str_filename;
	str_filename = "modelOne.hst";
	m_pHSTView->myDocument = ReadXMLFile(str_filename, m_pHSTView->myDocument, m_pHSTView);
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetWindowText(str_filename + _T("-HSTPNSim"));
	m_pHSTView->bPNisChanged = FALSE;

	m_pHSTView->Invalidate();
	m_pHSTView->UpdateWindow();
	m_pHSTView->Invalidate();
	m_pHSTView->UpdateWindow();
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_UPDATETREE1, 0, 0);// Ŀ�ķ�����mainframe���ٷ�����dlgmodelchoose1��֪ͨHSTPN�ṹ������
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pParmListDlg->SendMessage(WM_MESSAGE_UPDATETREEOFMODEL, 0, 0);// ��ģ�ͱ༭������WM_MESSAGE_UPDATETREEOFMODEL��Ϣ������ģ����
	
}