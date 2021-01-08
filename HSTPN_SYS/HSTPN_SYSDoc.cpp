
// HSTPN_SYSDoc.cpp : CHSTPN_SYSDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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

// CHSTPN_SYSDoc 构造/析构

CHSTPN_SYSDoc::CHSTPN_SYSDoc()
{
	// TODO: 在此添加一次性构造代码
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

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CHSTPN_SYSDoc 序列化

void CHSTPN_SYSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CHSTPN_SYSDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
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

// 搜索处理程序的支持
void CHSTPN_SYSDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
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

// CHSTPN_SYSDoc 诊断

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


// CHSTPN_SYSDoc 命令

// 打开操作
void CHSTPN_SYSDoc::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CHSTPN_SYSView* m_pHSTView;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);

	//====================打开操作=========================================================begin=========
	int n_SizeP = Vector_Place.size();
	int n_SizeT = Vector_Trans.size();
	int n_SizeA = arr_Arrow.GetSize();
	int n_SizeO = Vector_OutputModel.size();
	if(n_SizeP!=0 || n_SizeT!=0 || n_SizeA!=0 || n_SizeO!=0)                     // 首先判断当前视图是否为空，如果不为空，提醒用户进行保存;如果视图为空，那么直接弹出“打开对话框”
	{
		if(m_pHSTView->myDocument==NULL || m_pHSTView->bPNisChanged)
		{
			CDlgSaveNote m_dlgSaveNote;                                          // 当用户点击“新建”按钮时，先弹出一个提醒对话框
			int i_btn = m_dlgSaveNote.DoModal();
			if(1 == i_btn)                                                       // #define IDOK 1   当用户选择“保存”按钮时
			{
				// 1.首先保存文件
				if(m_pHSTView->myDocument==NULL)                                 // 判断仿真文件是否引进被保存
				{
					CFileDialog m_saveDlg(FALSE);                                // 创建保存文件对话框，若否，弹出“另存为”对话框
					m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//设置过滤器
					m_saveDlg.m_ofn.lpstrDefExt = L"hst";                        //添加缺省的扩展名
					if(IDOK == m_saveDlg.DoModal())
					{
						m_pHSTView->myDocument = new TiXmlDocument();
						WriteXMLFile(m_saveDlg.GetPathName(),m_pHSTView->myDocument);
					}
				}
				else                                                             // 若已被保存，则直接写入
				{
				//1019	TiXmlElement *RootElement = m_pHSTView->myDocument->RootElement();// 获取文件的根元素
				//1019	TiXmlAttribute *attribute1 = RootElement->FirstAttribute();  // 获取根元素的第一个属性，即文件路径名
				//1019	const char *filepathname = attribute1->Value();
				//1019	CString str_filepathname(filepathname);                      // 获取文件中保存的文件名
					if(m_pHSTView->myDocument!=NULL)
					{
						m_pHSTView->myDocument->Clear();
					}
					delete m_pHSTView->myDocument;                               // 此时应先释放myDocument的内存，因为下面要给它分配新的内存，如果不分配，内容会被重复写入到文件中，重新分配的话，相当于重新写入
					m_pHSTView->myDocument = new TiXmlDocument();
					WriteXMLFile(/*str_filepathname*/this->opening_filePath,m_pHSTView->myDocument);       // 写入文件
				}
				//2.清空视图类
				DeletePlcTrnsArcs(m_pHSTView);                                   // 然后清除视图内已有的信息：Vector_Place，Vector_Trans，arr_Arrow，将变量初始化
		
				m_pHSTView->Invalidate();
				m_pHSTView->UpdateWindow();

				//3. 打开文件
				CFileDialog m_saveDlg(TRUE);                                     // 创建打开文件对话框
				m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//设置过滤器
				m_saveDlg.m_ofn.lpstrDefExt = L"hst";                            // 添加缺省的扩展名

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

			if(3 == i_btn)                                                       // #define IDCANCEL 2   当用户选择“不保存”按钮时
			{
				//1.清空视图
				DeletePlcTrnsArcs(m_pHSTView);                                   // 直接清除视图内已有的信息：Vector_Place，Vector_Trans，arr_Arrow// 将变量恢复初始化
		
				m_pHSTView->Invalidate();
				m_pHSTView->UpdateWindow();

				//2. 打开文件
				CFileDialog m_saveDlg(TRUE);                                     // 创建打开文件对话框
				m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//设置过滤器
				m_saveDlg.m_ofn.lpstrDefExt = L"hst";                            // 添加缺省的扩展名

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
			                                                                     // 当用户选择的不是“保存”和“不保存”时，什么都不做，保持视图类为原来的状态
		}
		else
		{
			//1.清空视图
			DeletePlcTrnsArcs(m_pHSTView);                                       // 直接清除视图内已有的信息：Vector_Place，Vector_Trans，arr_Arrow// 将变量恢复初始化
		
			m_pHSTView->Invalidate();
			m_pHSTView->UpdateWindow();
			//2. 打开文件
			CFileDialog m_saveDlg(TRUE);                                         // 创建打开文件对话框
			m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//设置过滤器
			m_saveDlg.m_ofn.lpstrDefExt = L"hst";                                // 添加缺省的扩展名

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
	else                                                                         // 如果视图为空，直接弹出“打开”对话框
	{
		CFileDialog m_saveDlg(TRUE);                                             // 创建打开文件对话框
		m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//设置过滤器
		m_saveDlg.m_ofn.lpstrDefExt = L"hst";                                    // 添加缺省的扩展名

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
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_UPDATETREE1,0,0);// 目的发送至mainframe，再发送至dlgmodelchoose1，通知HSTPN结构树更新
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pParmListDlg->SendMessage(WM_MESSAGE_UPDATETREEOFMODEL,0,0);// 向模型编辑器发送WM_MESSAGE_UPDATETREEOFMODEL消息，更新模型树
	//====================打开操作===========================================================end=========

}

// 保存操作
void CHSTPN_SYSDoc::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	CHSTPN_SYSView* m_pHSTView;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);                                 // 在调试过程中发现，本实例中的结构，该文档应该对应了两个视图，第一个是
	//====================保存操作=========================================================begin=========
	if(m_pHSTView->myDocument==NULL)                                                // 判断仿真文件是否引进被保存
	{
		CFileDialog m_saveDlg(FALSE);                                               // 创建保存文件对话框，若否，弹出“另存为”对话框
		m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//设置过滤器
		m_saveDlg.m_ofn.lpstrDefExt = L"hst";                                       // 添加缺省的扩展名
		if(IDOK == m_saveDlg.DoModal())
		{
			m_pHSTView->myDocument = new TiXmlDocument();
			this->opening_filePath = m_saveDlg.GetPathName();

			WriteXMLFile(this->opening_filePath,m_pHSTView->myDocument);

			m_pHSTView->bPNisChanged = FALSE;
		}
	}
	else                                                                             // 若已被保存，则直接写入
	{
//1019		TiXmlElement *RootElement = m_pHSTView->myDocument->RootElement();           // 获取文件的根元素
//1019		TiXmlAttribute *attribute1 = RootElement->FirstAttribute();                  // 获取根元素的第一个属性，即文件路径名
//1019		const char *filepathname = attribute1->Value();
//1019		CString str_filepathname(filepathname);                                      // 获取文件中保存的文件名
		if(m_pHSTView->myDocument!=NULL)
		{
			m_pHSTView->myDocument->Clear();
		}
		delete m_pHSTView->myDocument;                                               // 此时应先释放myDocument的内存，因为下面要给它分配新的内存，如果不分配，内容会被重复写入到文件中，重新分配的话，相当于重新写入
		m_pHSTView->myDocument = new TiXmlDocument();
		WriteXMLFile(this->opening_filePath/*str_filepathname*/,m_pHSTView->myDocument);                       // 写入文件

		m_pHSTView->bPNisChanged = FALSE;

	}
	//====================保存操作=========================================================end===========

}

// 另存为操作
void CHSTPN_SYSDoc::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	CHSTPN_SYSView* m_pHSTView;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);
	//====================另存为操作=======================================================begin===========
	CFileDialog m_saveDlg(FALSE);                                                   // 创建保存文件对话框，若否，弹出“另存为”对话框
	m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0"; // 设置过滤器
	m_saveDlg.m_ofn.lpstrDefExt = L"hst";                                           // 添加缺省的扩展名
	if(IDOK == m_saveDlg.DoModal())
	{
		TiXmlDocument *myDocument1 = new TiXmlDocument();                           // 当点击“另存为”，应该重新申请一个对象，而不是用原来的对象；因为如果用原来的对象，再次点击保存时，发现当前文档已经变成“另存为”后的文档了
		WriteXMLFile(m_saveDlg.GetPathName(),myDocument1);
		if(myDocument1!=NULL)
		{
			m_pHSTView->myDocument->RootElement()->Clear();
		}
		delete myDocument1;
	}
	//====================另存为操作=========================================================end===========
}

// 新建操作
void CHSTPN_SYSDoc::OnFileNew()
{
	// TODO: 在此添加命令处理程序代码
	CHSTPN_SYSView* m_pHSTView;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);

	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetWindowText(_T("New file-HSTPNSim"));

	//====================新建操作=========================================================begin=========
	// 当用户点击“新建”按钮时，首先弹出提示用户保存的对话框，根据用户选择操作
	int n_SizeP = Vector_Place.size();
	int n_SizeT = Vector_Trans.size();
	int n_SizeA = arr_Arrow.GetSize();
	int n_SizeO = Vector_OutputModel.size();
	// 首先判断当前视图是否为空，若不为空，提醒用户保存
	if(n_SizeP!=0 || n_SizeT!=0 || n_SizeA!=0 || n_SizeO!=0)
	{
		if(m_pHSTView->myDocument==NULL || m_pHSTView->bPNisChanged)
		{
			CDlgSaveNote m_dlgSaveNote;                                                      // 当用户点击“新建”按钮时，先弹出一个提醒对话框
			int i_btn = m_dlgSaveNote.DoModal();

			if(1 == i_btn)                                                                   // #define IDOK 1   当用户选择“保存”按钮时
			{
				// 1.首先保存文件
				if(m_pHSTView->myDocument==NULL)                                             // 判断仿真文件是否引进被保存
				{
					CFileDialog m_saveDlg(FALSE);                                            // 创建保存文件对话框，若否，弹出“另存为”对话框
					m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//设置过滤器
					m_saveDlg.m_ofn.lpstrDefExt = L"hst";                                    // 添加缺省的扩展名
					if(IDOK == m_saveDlg.DoModal())
					{
						m_pHSTView->myDocument = new TiXmlDocument();
						
						WriteXMLFile(m_saveDlg.GetPathName(),m_pHSTView->myDocument);
					}
				}
				else                                                                        // 若已被保存，则直接写入
				{
				//1019	TiXmlElement *RootElement = m_pHSTView->myDocument->RootElement();      // 获取文件的根元素
				//1019	TiXmlAttribute *attribute1 = RootElement->FirstAttribute();             // 获取根元素的第一个属性，即文件路径名
				//1019	const char *filepathname = attribute1->Value();
				//1019	CString str_filepathname(filepathname);                                 // 获取文件中保存的文件名
					if(m_pHSTView->myDocument!=NULL)
					{
						m_pHSTView->myDocument->Clear();
					}
					delete m_pHSTView->myDocument;                                          //此时应先释放myDocument的内存，因为下面要给它分配新的内存，如果不分配，内容会被重复写入到文件中，重新分配的话，相当于重新写入
					m_pHSTView->myDocument = new TiXmlDocument();
					WriteXMLFile(/*str_filepathname*/this->opening_filePath,m_pHSTView->myDocument);                  // 写入文件
				}
				//2.清空视图类
				DeletePlcTrnsArcs(m_pHSTView);                                              //然后清除视图内已有的信息：Vector_Place，Vector_Trans，arr_Arrow，将变量初始化
			}
		
			if(3 == i_btn)                                                                  // #define IDCANCEL 1   当用户选择“不保存”按钮时
			{
				DeletePlcTrnsArcs(m_pHSTView);                                              //直接清除视图内已有的信息：Vector_Place，Vector_Trans，arr_Arrow// 将变量恢复初始化
			}
			                                                                                // 当用户选择的不是“保存”和“不保存”时，什么都不做，保持视图类为原来的状态
		}
		else
		{
			DeletePlcTrnsArcs(m_pHSTView);
		}

	}
	
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_UPDATETREE1,0,0);      // 目的发送至mainframe，再发送至dlgmodelchoose1，通知HSTPN结构树更新
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pParmListDlg->SendMessage(WM_MESSAGE_UPDATETREEOFMODEL,0,0);// 向模型编辑器发送WM_MESSAGE_UPDATETREEOFMODEL消息，更新模型树
	m_pHSTView->Invalidate();
//	m_pHSTView->UpdateWindow();
	//====================新建操作=========================================================end===========
}

// 清空Vector_Place/Vector_Trans/arr_Arrow,恢复初始化
void CHSTPN_SYSDoc::DeletePlcTrnsArcs(CHSTPN_SYSView *m_pView)
{
	// 释放输入/输出列表
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

	// 释放Vector_Place中元素指向的内存，清空Vector_Place
	for(vector<CPlace*>::iterator iterP=Vector_Place.begin();iterP!=Vector_Place.end();iterP++)
	{
		(*iterP)->DestroyWindow();
		delete (*iterP);
	}
	Vector_Place.clear();
	// 释放Vector_Trans中元素指向的内存，清空Vector_Trans
	for(vector<CTransition*>::iterator iterT=Vector_Trans.begin();iterT!=Vector_Trans.end();iterT++)
	{
		(*iterT)->DestroyWindow();
		delete (*iterT);
	}
	Vector_Trans.clear();
	// 释放arr_Arrow中元素指向的内存，清空arr_Arrow
	int n_SizeArcs = arr_Arrow.GetSize();
	for(int i=0;i<n_SizeArcs;i++)
	{
		delete (CArrow*)arr_Arrow.GetAt(i);
	}
	arr_Arrow.RemoveAll();

	// 释放Vector_OutputModel中元素指向的内存，清空Vector_OutputModel
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

	// 将变量恢复初始化
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
		m_pView->myDocument->Clear();// 释放子节点内存
	}
	delete m_pView->myDocument; // 释放根节点本身

	m_pView->bPNisChanged             = TRUE;

	m_pView->u_SubScriptofOutputModel = 0;

}

// 仿真文件写入操作
void CHSTPN_SYSDoc::WriteXMLFile(CString filePath_Name,TiXmlDocument *myDocument)
{
	// 

	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration("1.0","","");// 2.写第一行声明文字
	if(NULL == pDeclaration) return;
	myDocument->LinkEndChild(pDeclaration);                            // 链接到文档对象

	TiXmlElement *RootElement = new TiXmlElement("HSTPnet");           // 3.创建一个根元素并连接。
    myDocument->LinkEndChild(RootElement);                             // 链接到文档对象
	// -----------------保存全局变量表------------------------------------------------------------------Begin-----------------------------
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
	
	//------------------保存库所信息--------------------------------------------------------------------Begin-----------------------------
	TiXmlElement *PlacesElement = new TiXmlElement("Places");                                           // 创建一个Places元素并连接。
    RootElement->LinkEndChild(PlacesElement);
	int n_countP = 0;
	for(vector<CPlace*>::iterator iterP=Vector_Place.begin();iterP!=Vector_Place.end();iterP++)
	{
		TiXmlElement *PlaceElement = new TiXmlElement("Place");                                         // 依次创建一个Place节点
		PlacesElement->LinkEndChild(PlaceElement);

		char * caption;
		USES_CONVERSION;
		caption = T2A((*iterP)->m_caption);
		PlaceElement->SetAttribute("m_caption",caption);                                               // Place属性：m_caption

		PlaceElement->SetAttribute("u_SubscriptofModel",(*iterP)->u_SubscriptofModel);                 // Place属性：u_SubscriptofModel

		int placestyle;
		if((*iterP)->PlaceStyle == STYLE_DISCRETE) placestyle=1;
		if((*iterP)->PlaceStyle == STYLE_CNTNUS)   placestyle=2;
		if((*iterP)->PlaceStyle == STYLE_RAND)     placestyle=3;
		if((*iterP)->PlaceStyle == STYLE_TIME)     placestyle=4;
		if((*iterP)->PlaceStyle == STYLE_TCTRL)    placestyle=5;
		if((*iterP)->PlaceStyle == STYLE_LCTRL)    placestyle=6;
		if((*iterP)->PlaceStyle == STYLE_DISIC)    placestyle=7;
		PlaceElement->SetAttribute("PlaceStyle",placestyle);                                           // Place属性：PlaceStyle

		PlaceElement->SetAttribute("x",(*iterP)->Position.x);                                          // Place属性：Position.x
		PlaceElement->SetAttribute("y",(*iterP)->Position.y);                                          // Place属性：Position.y
                          
		char * bactivation;
		if((*iterP)->bActivation) bactivation="true"; else bactivation="false";
		PlaceElement->SetAttribute("bActivation",bactivation);                                         // Place属性：bActivation
                               
		char * benable;
		if((*iterP)->bEnable) benable="true"; else benable="false";
		PlaceElement->SetAttribute("bEnable",benable);                                                 // Place属性：bEnable

		
		char * bisassigned;
		if((*iterP)->bisAssigned) bisassigned="true"; else bisassigned="false";
		PlaceElement->SetAttribute("bisAssigned",bisassigned);                                         //Place属性：bisAssigned

		char * ischoseSocket;
		if((*iterP)->ischoseSocket) ischoseSocket="true"; else ischoseSocket="false";                  //Place属性：ischoseSocket
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
			// 读取Lua文件中的内容
			CString luafilepath = _T("LuaFile\\S_") + (*iterP)->m_caption + _T(".lua");
			CFile luafile(luafilepath,CFile::modeRead);                                            // 打开Lua文件，在LuaFile文件夹下
			DWORD dwFileLen;
			dwFileLen = (DWORD)luafile.GetLength();                                                // 获取文件中内容的长度
			S_function = new char[dwFileLen + 1];                                                    // 为字符串function分配内存
			S_function[dwFileLen] = 0;                                                               // 最后一个字符应该为\0
			luafile.Read(S_function,dwFileLen);                                                      // 将内容写入字符串function中
			luafile.Close();

		}
		else
		{
			S_function = "";
		}
		TiXmlText *S_luafuc = new TiXmlText(S_function);
		S_LuaElement->LinkEndChild(S_luafuc);

		//-----------------------------

		TiXmlElement *dProbltyElement = new TiXmlElement("dProblty");                                  //Place属性：dProblty
		PlaceElement->LinkEndChild(dProbltyElement);
		CString str_dproblty;
		str_dproblty.Format(L"%f",(*iterP)->dProblty);
		char * dproblty;
		dproblty = T2A(str_dproblty);
		dProbltyElement->LinkEndChild(new TiXmlText(dproblty));

		TiXmlElement *dTimeElement = new TiXmlElement("dTime");                                       //Place属性：dTime
		PlaceElement->LinkEndChild(dTimeElement);
		CString str_dtime;
		str_dtime.Format(L"%f",(*iterP)->dTime);
		char * dtime;
		dtime = T2A(str_dtime);
		dTimeElement->LinkEndChild(new TiXmlText(dtime));
		
		if(((*iterP)->PlaceStyle==STYLE_DISIC || (*iterP)->PlaceStyle==STYLE_CNTNUS)&&(*iterP)->ischoseSocket)
		{
			TiXmlElement *ipadrsElement = new TiXmlElement("ipadrs");                                  //Place属性：ipadrs
			PlaceElement->LinkEndChild(ipadrsElement);
			CString str_ipadrs;
			str_ipadrs = (*iterP)->ipadrs.c_str();
			char * ipadrs;
			ipadrs = T2A(str_ipadrs);
			ipadrsElement->LinkEndChild(new TiXmlText(ipadrs));

			TiXmlElement *portElement = new TiXmlElement("port");                                       //Place属性：port
			PlaceElement->LinkEndChild(portElement);
			CString str_port;
			str_port.Format(L"%d",(*iterP)->port);
			char * port;
			port = T2A(str_port);
			portElement->LinkEndChild(new TiXmlText(port));

			TiXmlElement *inputElement = new TiXmlElement("num_input");                                       //Place属性：num_input
			PlaceElement->LinkEndChild(inputElement);
			CString str_input;
			str_input.Format(L"%d",(*iterP)->num_input);
			char * num_input;
			num_input = T2A(str_input);
			inputElement->LinkEndChild(new TiXmlText(num_input));

			TiXmlElement *outputElement = new TiXmlElement("num_output");                                       //Place属性：num_output
			PlaceElement->LinkEndChild(outputElement);
			CString str_output;
			str_output.Format(L"%d",(*iterP)->num_output);
			char * num_output;
			num_output = T2A(str_output);
			outputElement->LinkEndChild(new TiXmlText(num_output));
		}



		TiXmlElement *Lua_fileNameElement = new TiXmlElement("Lua_fileName");                        //Place属性：Lua_fileName
		PlaceElement->LinkEndChild(Lua_fileNameElement);
		char *lua_filename;
		lua_filename = T2A((*iterP)->Lua_fileName);
		Lua_fileNameElement->LinkEndChild(new TiXmlText(lua_filename));

		TiXmlElement *pt1Element = new TiXmlElement("pt1");                                         //Place属性：pt1
		PlaceElement->LinkEndChild(pt1Element);
		pt1Element->SetAttribute("x",(*iterP)->pt1.x);
		pt1Element->SetAttribute("y",(*iterP)->pt1.y);

		TiXmlElement *pt2Element = new TiXmlElement("pt2");                                         //Place属性：pt2
		PlaceElement->LinkEndChild(pt2Element);
		pt2Element->SetAttribute("x",(*iterP)->pt2.x);
		pt2Element->SetAttribute("y",(*iterP)->pt2.y);

		TiXmlElement *pt3Element = new TiXmlElement("pt3");                                        //Place属性：pt3
		PlaceElement->LinkEndChild(pt3Element);
		pt3Element->SetAttribute("x",(*iterP)->pt3.x);
		pt3Element->SetAttribute("y",(*iterP)->pt3.y);

		TiXmlElement *pt4Element = new TiXmlElement("pt4");                                       //Place属性：pt4
		PlaceElement->LinkEndChild(pt4Element);
		pt4Element->SetAttribute("x",(*iterP)->pt4.x);
		pt4Element->SetAttribute("y",(*iterP)->pt4.y);

		TiXmlElement *arryPlacePreElement = new TiXmlElement("arryPlacePre");                     //Place属性：arryPlacePre
		PlaceElement->LinkEndChild(arryPlacePreElement);
		for(vector<CTransition*>::iterator itert1=(*iterP)->arryPlacePre.begin();itert1!=(*iterP)->arryPlacePre.end();itert1++)
		{
			TiXmlElement *pretransitionElement = new TiXmlElement("pretransition");
			arryPlacePreElement->LinkEndChild(pretransitionElement);
			pretransitionElement->SetAttribute("u_SubscriptofTrans",(*itert1)->u_SubscriptofTrans);
		}
		TiXmlElement *arryPlacePstElement = new TiXmlElement("arryPlacePst");                     //Place属性：arryPlacePst
		PlaceElement->LinkEndChild(arryPlacePstElement);
		for(vector<CTransition*>::iterator itert2=(*iterP)->arryPlacePst.begin();itert2!=(*iterP)->arryPlacePst.end();itert2++)
		{
			TiXmlElement *psttransitionElement = new TiXmlElement("psttransition");
			arryPlacePstElement->LinkEndChild(psttransitionElement);
			psttransitionElement->SetAttribute("u_SubscriptofTrans",(*itert2)->u_SubscriptofTrans);
		}

		TiXmlElement *bisUserDef_probElement = new TiXmlElement("bisUserDef_prob");               // Place属性：bisUserDef_prob
		PlaceElement->LinkEndChild(bisUserDef_probElement);
		char * bisuserdef_prob;
		if((*iterP)->bisUserDef_prob) bisuserdef_prob="true"; else bisuserdef_prob="false";
		bisUserDef_probElement->LinkEndChild(new TiXmlText(bisuserdef_prob));

		TiXmlElement *bisUserDef_timeElement = new TiXmlElement("bisUserDef_time");               // Place属性：bisUserDef_time
		PlaceElement->LinkEndChild(bisUserDef_timeElement);
		char * bisuserdef_time;
		if((*iterP)->bisUserDef_time) bisuserdef_time="true"; else bisuserdef_time="false";
		bisUserDef_timeElement->LinkEndChild(new TiXmlText(bisuserdef_time));

		TiXmlElement *bisUserDef_ctrlElement = new TiXmlElement("bisUserDef_ctrl");               // Place属性：bisUserDef_ctrl
		PlaceElement->LinkEndChild(bisUserDef_ctrlElement);
		char * bisuserdef_ctrl;
		if((*iterP)->bisUserDef_ctrl) bisuserdef_ctrl="true"; else bisuserdef_ctrl="false";
		bisUserDef_ctrlElement->LinkEndChild(new TiXmlText(bisuserdef_ctrl));

		TiXmlElement *str_Inputport_probElement = new TiXmlElement("str_Inputport_prob");         // Place属性：str_Inputport_prob
		PlaceElement->LinkEndChild(str_Inputport_probElement);
		char * inputport_prob;
		inputport_prob=T2A((*iterP)->str_Inputport_prob);
		str_Inputport_probElement->LinkEndChild(new TiXmlText(inputport_prob));

		TiXmlElement *str_Inputport_timeElement = new TiXmlElement("str_Inputport_time");         // Place属性：str_Inputport_time
		PlaceElement->LinkEndChild(str_Inputport_timeElement);
		char * inputport_time;
		inputport_time=T2A((*iterP)->str_Inputport_time);
		str_Inputport_timeElement->LinkEndChild(new TiXmlText(inputport_time));

		TiXmlElement *str_Inputport_ctrlElement = new TiXmlElement("str_Inputport_ctrl");         // Place属性：str_Inputport_ctrl
		PlaceElement->LinkEndChild(str_Inputport_ctrlElement);
		char * inputport_ctrl;
		inputport_ctrl=T2A((*iterP)->str_Inputport_ctrl);
		str_Inputport_ctrlElement->LinkEndChild(new TiXmlText(inputport_ctrl));


		TiXmlElement *arryIOputDataElement = new TiXmlElement("arryIOputData");                    // Place属性：arryIOputData
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

		TiXmlElement *arryInputofLuaElement = new TiXmlElement("arryInputofLua");                  // Place属性：arryInputofLua
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

		TiXmlElement *arryOutputofLuaElement = new TiXmlElement("arryOutputofLua");               // Place属性：arryOutputofLua
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
			_itoa_s((**iterOutLua1).Sequence,sequence,10,10);                                      // 第一个10是sequence[10]中的10，第二个10是表示十进制
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

		TiXmlElement *Lua_functionElement = new TiXmlElement("Lua_function");                      // Place属性：Lua_function 注：该属性是保存库所对应的Lua文件中的内容
		PlaceElement->LinkEndChild(Lua_functionElement);
		char * function;
		if(((*iterP)->PlaceStyle==STYLE_CNTNUS || (*iterP)->PlaceStyle==STYLE_DISIC) && (*iterP)->Lua_fileName!="")
		{
			// 读取Lua文件中的内容
			CString luafilepath = _T("LuaFile\\") + (*iterP)->Lua_fileName + _T(".lua");
			CFile luafile(luafilepath,CFile::modeRead);                                            // 打开Lua文件，在LuaFile文件夹下
			DWORD dwFileLen;
			dwFileLen = (DWORD)luafile.GetLength();                                                // 获取文件中内容的长度
			function = new char[dwFileLen + 1];                                                    // 为字符串function分配内存
			function[dwFileLen] = 0;                                                               // 最后一个字符应该为\0
			luafile.Read(function,dwFileLen);                                                      // 将内容写入字符串function中
			luafile.Close();

		}
		else
		{
			function = "";
		}
		TiXmlText *luafuc = new TiXmlText(function);
		Lua_functionElement->LinkEndChild(luafuc);

		TiXmlElement *CtrlStyleElement = new TiXmlElement("CtrlPlaceStyle");                      // Place属性：CtrlPlaceStyle
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
	//------------------保存库所信息--------------------------------------------------------------------End-------------------------------


	//------------------保存变迁信息--------------------------------------------------------------------Begin-----------------------------
	TiXmlElement *TranstionsElement = new TiXmlElement("Transitions");                                  // 创建一个Transitions元素并连接。
	RootElement->LinkEndChild(TranstionsElement);
	for(vector<CTransition*>::iterator iterT=Vector_Trans.begin();iterT!=Vector_Trans.end();iterT++)
	{
		TiXmlElement *TransitionElement = new TiXmlElement("Transition");                              // 依次创建一个Transition节点
		TranstionsElement->LinkEndChild(TransitionElement);

		char * caption;
		USES_CONVERSION;
		caption = T2A((*iterT)->m_caption);
		TransitionElement->SetAttribute("m_caption",caption);                                           // Transition属性：m_caption

		TransitionElement->SetAttribute("u_SubscriptofTrans",(*iterT)->u_SubscriptofTrans);             // Transition属性：u_SubscriptofTrans

		TransitionElement->SetAttribute("x",(*iterT)->Position.x);                                      // Transition属性：Position.x
		TransitionElement->SetAttribute("y",(*iterT)->Position.y);                                      // Transition属性：Position.y

		char * bfire;
		if((*iterT)->bFire) bfire="true"; else bfire="false";
		TransitionElement->SetAttribute("bFire",bfire);                                                 // Transition属性：bFire

		TiXmlElement *pt1Element = new TiXmlElement("pt1");                                             // Transition属性：pt1
		TransitionElement->LinkEndChild(pt1Element);
		pt1Element->SetAttribute("x",(*iterT)->pt1.x);
		pt1Element->SetAttribute("y",(*iterT)->pt1.y);

		TiXmlElement *pt2Element = new TiXmlElement("pt2");                                             // Transition属性：pt2
		TransitionElement->LinkEndChild(pt2Element);
		pt2Element->SetAttribute("x",(*iterT)->pt2.x);
		pt2Element->SetAttribute("y",(*iterT)->pt2.y);

		TiXmlElement *arryTransPreElement = new TiXmlElement("arryTransPre");                           // Transition属性：arryTransPre
		TransitionElement->LinkEndChild(arryTransPreElement);
		for(vector<CPlace*>::iterator iterp1=(*iterT)->arryTransPre.begin();iterp1!=(*iterT)->arryTransPre.end();iterp1++)
		{
			TiXmlElement *preplaceElement = new TiXmlElement("preplae");
			arryTransPreElement->LinkEndChild(preplaceElement);
			preplaceElement->SetAttribute("u_SubscriptofModel",(*iterp1)->u_SubscriptofModel);
		}

		TiXmlElement *arryTransPstElement = new TiXmlElement("arryTransPst");                            // Transition属性：arryTransPst
		TransitionElement->LinkEndChild(arryTransPstElement);
		for(vector<CPlace*>::iterator iterp2=(*iterT)->arryTransPst.begin();iterp2!=(*iterT)->arryTransPst.end();iterp2++)
		{
			TiXmlElement *pstplaceElement = new TiXmlElement("pstplae");
			arryTransPstElement->LinkEndChild(pstplaceElement);
			pstplaceElement->SetAttribute("u_SubscriptofModel",(*iterp2)->u_SubscriptofModel);
		}
	}
	//------------------保存变迁信息--------------------------------------------------------------------End-------------------------------


	//------------------保存有向弧信息------------------------------------------------------------------Begin-----------------------------
	TiXmlElement *ArcsElement = new TiXmlElement("Arcs");                                                  // 创建一个Arcs元素并连接
	RootElement->LinkEndChild(ArcsElement);
	int nCount = arr_Arrow.GetSize();                                                                      // 获取要保存的有向弧的个数
	for(int i=0;i<nCount;i++)                                                                              // 将有向弧的信息依次写入文件
	{
		char* disic; 
		TiXmlElement *ArcElement = new TiXmlElement("Arc");
		ArcsElement->LinkEndChild(ArcElement);
		ArcElement->SetAttribute("whichModel_Start",((CArrow*)arr_Arrow.GetAt(i))->whichModel_Start);      // Arc属性1：whichModel_Start
		ArcElement->SetAttribute("u_SubScriptofPlace",((CArrow*)arr_Arrow.GetAt(i))->u_SubScriptofPlace);  // Arc属性2：u_SubScriptofPlace
		ArcElement->SetAttribute("u_SubScriptofTrans",((CArrow*)arr_Arrow.GetAt(i))->u_SubScriptofTrans);  // Arc属性3：u_SubScriptofTrans
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
	//------------------保存有向弧信息------------------------------------------------------------------End-------------------------------

	//-----------------保存输出模块的信息---------------------------------------------------------------Begin-----------------------------
	TiXmlElement *OutModelsElement = new TiXmlElement("OutModels");                                  // 创建一个OutModels元素并连接。
	RootElement->LinkEndChild(OutModelsElement);
	for(vector<CCTRLShowMatrix*>::iterator itero=Vector_OutputModel.begin();itero!=Vector_OutputModel.end();itero++)
	{
		TiXmlElement *OutmodelElement = new TiXmlElement("OutModel");                               // 依次创建一个OutModel节点
		OutModelsElement->LinkEndChild(OutmodelElement);

		char * caption;
		USES_CONVERSION;
		caption = T2A((*itero)->m_caption);
		OutmodelElement->SetAttribute("m_caption",caption);                                        // OutModel属性1：m_caption

		//char id[10];
		//_itoa_s((*itero)->ID,id,10,10);
		OutmodelElement->SetAttribute("ID",(*itero)->ID);                                          // OutModel属性2：ID

		OutmodelElement->SetAttribute("x",(*itero)->Position.x);                                   // OutModel属性3：x
		OutmodelElement->SetAttribute("y",(*itero)->Position.y);                                   // OutModel属性4：y

		int outputstyle;
		if((*itero)->OutputStyle==OUT_SHOWMATRIX) outputstyle = 1;
		if((*itero)->OutputStyle==OUT_TOFILE) outputstyle = 2;
		if((*itero)->OutputStyle==OUT_ACCUMULATOR) outputstyle = 3;
		OutmodelElement->SetAttribute("OutputStyle",outputstyle);                                  // OutModel属性5：OutputStyle

		TiXmlElement *OutPNameElement = new TiXmlElement("OutPName");                              // OutModel属性6：OutPName
		OutmodelElement->LinkEndChild(OutPNameElement);
		char * outpname;
		outpname=T2A((*itero)->OutPName);
		OutPNameElement->LinkEndChild(new TiXmlText(outpname));

		TiXmlElement *vec_OutValueNameElement = new TiXmlElement("vec_OutValueName");              // OutModel属性7：vec_OutValueName
		OutmodelElement->LinkEndChild(vec_OutValueNameElement);
		for(vector<CString>::iterator iters=(*itero)->vec_OutValueName.begin();iters!=(*itero)->vec_OutValueName.end();iters++)
		{
			TiXmlElement *OutValueNameElement = new TiXmlElement("OutValueName");
			vec_OutValueNameElement->LinkEndChild(OutValueNameElement);
			char * outvaluenaem;
			outvaluenaem=T2A((*iters));
			OutValueNameElement->LinkEndChild(new TiXmlText(outvaluenaem));
		}

		TiXmlElement *i_PposnOValueElement = new TiXmlElement("i_PposnOValue");                   // OutModel属性8：i_PposnOValue
		OutmodelElement->LinkEndChild(i_PposnOValueElement);
		char i_psn[10];
		_itoa_s((*itero)->i_PposnOValue,i_psn,10);
		char * i_posnovalue = i_psn;
		i_PposnOValueElement->LinkEndChild(new TiXmlText(i_posnovalue));

		TiXmlElement *vec_OutValueElement = new TiXmlElement("vec_OutValue");                     // OutModel属性9：vec_OutValue
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

		TiXmlElement *FilePathElement = new TiXmlElement("str_FilePath");                     // OutModel属性10：str_FilePath
		OutmodelElement->LinkEndChild(FilePathElement);
		char * filepath = T2A((*itero)->str_FilePath);	
		FilePathElement->LinkEndChild(new TiXmlText(filepath));
	}
	//-----------------保存输出模块的信息---------------------------------------------------------------End-------------------------------

	char * str_filename;
	USES_CONVERSION;                                   // 调用函数，T2A和W2A均支持ATL和MFC中的字符转换
    str_filename = T2A(filePath_Name); 
	RootElement->SetAttribute("Name",str_filename);                        // 写入文件名

	RootElement->SetAttribute("Step",((CHSTPN_SYSView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1))->d_TimeStep * 10000);   // 保存步长
	if(((CHSTPN_SYSView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1))->d_SimTime > 65535)
	{
		RootElement->SetAttribute("SimTime",65535);
	}
	else
	{
		RootElement->SetAttribute("SimTime",((CHSTPN_SYSView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1))->d_SimTime);  
	}// 保存仿真时间
	RootElement->SetAttribute("IntervalTime",((CHSTPN_SYSView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_splitterWnd1.GetPane(0,1))->d_IntervalTime);        // 保存间隔时间

	myDocument->SaveFile(str_filename);                                    // 保存到文件

	if(RootElement!=NULL)
	{
		RootElement->Clear();
	}

}

// 仿真文件读取操作
TiXmlDocument* CHSTPN_SYSDoc::ReadXMLFile1(CString filePath_Name,TiXmlDocument *myDocument,CHSTPN_SYSView *m_pView)
{
	
	UINT max_subPlace = 0;
	UINT max_subTrans = 0;                         // 找出打开模型中库所和变迁的最大下标
	UINT max_subOuts  = 0;
	char *filepath_name;
	USES_CONVERSION;
	filepath_name = T2A(filePath_Name);
//	filepath_name = CString2char(filePath_Name);  // 此处不能利用CString2char函数，因为我们不希望char*指向的内存被释放，一旦释放，文件访问会发生错误
	myDocument = new TiXmlDocument(filepath_name);
	myDocument->LoadFile();// 加载文件

	// 获取视口原点!!!!!!!!!!!!!!!!!!!!!!!!!!5.17
	CPoint pt_ViewportOrg;
	CClientDC dc((CWnd*)m_pView);
	m_pView->OnPrepareDC(&dc);
	pt_ViewportOrg = dc.GetViewportOrg();

	TiXmlElement *RootElement = myDocument->RootElement();                                                    // 获取根元素--HSTPnet
	//--------------------------------------读取库所信息-----------------------------------------------------Begin--------
	TiXmlElement *PlacesNode = RootElement->FirstChildElement();                                              // 获取Places节点

	//CRect rectofModel(0,0,40,40);//(0,0,40,60);                                                             // 定义库所模型的大小
	TiXmlElement *PlaceNode = PlacesNode->FirstChildElement();                                                // 获取第一个库所节点
	for(;PlaceNode!=NULL;PlaceNode=PlaceNode->NextSiblingElement())                                           // 遍历所有的库所节点
	{
		CPlace *place = new CPlace();                                                                         // 先声明一个库所变量，并为其分配内存
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
		if(placestyle1 == "7") place->PlaceStyle = STYLE_DISIC;      // 首先获取PlaceStyle属性，并创建库所模型
		place->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofModel,m_pView,place->PlaceStyle);
		place->m_thisModel = place;
		
		//====获取m_caption，u_SubscriptofModel，PlaceStyle，x，y，bActivation，bEnable，bisAssigned；这几个变量保存为库所的属性
		TiXmlAttribute * attributeofPlace = PlaceNode->FirstAttribute();                                      // 获取库所的第1个属性值--m_caption
		place->m_caption = attributeofPlace->Value();
		attributeofPlace = attributeofPlace->Next();                                                          // 获取库所的第2个属性值--u_SubscriptofModel
		place->u_SubscriptofModel = atoi(attributeofPlace->Value());
		if(place->u_SubscriptofModel > max_subPlace)
			max_subPlace = place->u_SubscriptofModel;

		attributeofPlace = attributeofPlace->Next();                                                          // 获取库所的第3个属性值--PlaceStyle
		CString placestyle(attributeofPlace->Value());
		if(placestyle == "1") place->PlaceStyle  = STYLE_DISCRETE;
		if(placestyle == "2") place->PlaceStyle = STYLE_CNTNUS;
		if(placestyle == "3") place->PlaceStyle = STYLE_RAND;
		if(placestyle == "4") place->PlaceStyle = STYLE_TIME;
		if(placestyle == "5") place->PlaceStyle = STYLE_TCTRL;
		if(placestyle == "6") place->PlaceStyle = STYLE_LCTRL;
		if(placestyle == "7") place->PlaceStyle = STYLE_DISIC;
		
		attributeofPlace = attributeofPlace->Next();                                                          // 获取库所的第4个属性值--x
		place->Position.x = atoi(attributeofPlace->Value());
		attributeofPlace = attributeofPlace->Next();                                                          // 获取库所的第5个属性值--y
		place->Position.y = atoi(attributeofPlace->Value());
		attributeofPlace = attributeofPlace->Next();                                                          // 获取库所的第6个属性值--bActivation
		CString bactivation(attributeofPlace->Value());
		if(bactivation=="true") place->bActivation=TRUE; else place->bActivation=FALSE;

		attributeofPlace = attributeofPlace->Next();                                                          // 获取库所的第7个属性值--bEnable
		CString benable(attributeofPlace->Value());
		if(benable=="true") place->bEnable=TRUE; else place->bEnable=FALSE;

		attributeofPlace = attributeofPlace->Next();                                                          // 获取库所的第8个属性值--bisAssigned
		CString bassigned(attributeofPlace->Value());
		if(bassigned=="true") place->bisAssigned=TRUE; else place->bisAssigned=FALSE;

		attributeofPlace = attributeofPlace->Next();                                                          // 获取库所的第9个属性值--ischoseSocket
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
		if(place->ischoseProgRand && place->PlaceStyle == STYLE_RAND)                              // 如果检测函数的Lua函数名不为空,且库所的类型为连续库所时，才写入文件
		{        
			CString Lua_fileName_1 = _T("LuaFile\\S_") + place->m_caption + _T(".lua");
			CFile file(Lua_fileName_1,CFile::modeCreate | CFile::modeWrite);
			file.Write(S_luafunc,strlen(S_luafunc));
			file.Close();
		}

		//-------------------------

		//获取dProblty、dTime、Lua_fileName、pt1、pt2、pt3、pt4、arryPlacePre、arryPlacePst、bisUserDef_prob、bisUserDef_time、bisUserDef_ctrl
		//str_Inputport_prob、str_Inputport_time、str_Inputport_ctrl、arryInputData、arryOutputData、arryIOputData、arryInputofLua、arryOutputofLua、Lua_function
		//TiXmlElement *elementofPlace = PlaceNode->FirstChildElement();                                       // 获取库所的第1个子节点--dProblty
		elementofPlace = elementofPlace->NextSiblingElement();
		place->dProblty = atof(elementofPlace->GetText());

		elementofPlace = elementofPlace->NextSiblingElement();                                               // 获取库所的第2个子节点--dTime
		place->dTime = atof(elementofPlace->GetText());
		place->d_TimeTS = place->dTime;//++6.15

		elementofPlace = elementofPlace->NextSiblingElement();                                               // 获取库所的第3个子节点--ipadrs
		place->ipadrs = elementofPlace->GetText();

		elementofPlace = elementofPlace->NextSiblingElement();                                               // 获取库所的第4个子节点--port
		place->port = atoi(elementofPlace->GetText());

		elementofPlace = elementofPlace->NextSiblingElement();                                               // 获取库所的第5个子节点--num_input
		place->num_input = atoi(elementofPlace->GetText());

		elementofPlace = elementofPlace->NextSiblingElement();                                               // 获取库所的第6个子节点--num_output
		place->num_output = atoi(elementofPlace->GetText());

		elementofPlace = elementofPlace->NextSiblingElement();                                               // 获取库所的第4个子节点--Lua_fileName
		place->Lua_fileName = elementofPlace->GetText(); 

		elementofPlace = elementofPlace->NextSiblingElement();                                               // 获取库所的第4个子节点--pt1
		TiXmlAttribute *attributeofpt1 = elementofPlace->FirstAttribute();                                   // 获取pt1的第一个属性x
		place->pt1.x = atoi(attributeofpt1->Value());
		attributeofpt1 = attributeofpt1->Next();                                                             // 获取pt1的第二个属性y
		place->pt1.y = atoi(attributeofpt1->Value());

		elementofPlace = elementofPlace->NextSiblingElement();                                               // 获取库所的第5个子节点--pt2
		TiXmlAttribute *attributeofpt2 = elementofPlace->FirstAttribute();                                   // 获取pt2的第一个属性x
		place->pt2.x = atoi(attributeofpt2->Value());
		attributeofpt2 = attributeofpt2->Next();                                                             // 获取pt2的第二个属性y
		place->pt2.y = atoi(attributeofpt2->Value());

		elementofPlace = elementofPlace->NextSiblingElement();                                              // 获取库所的第6个子节点--pt3
		TiXmlAttribute *attributeofpt3 = elementofPlace->FirstAttribute();                                  // 获取pt3的第一个属性x
		place->pt3.x = atoi(attributeofpt3->Value());
		attributeofpt3 = attributeofpt3->Next();                                                            // 获取pt3的第二个属性y
		place->pt3.y = atoi(attributeofpt3->Value());

		elementofPlace = elementofPlace->NextSiblingElement();                                              // 获取库所的第7个子节点--pt4
		TiXmlAttribute *attributeofpt4 = elementofPlace->FirstAttribute();                                  // 获取pt4的第一个属性x
		place->pt4.x = atoi(attributeofpt4->Value());
		attributeofpt4 = attributeofpt4->Next();                                                            // 获取pt4的第二个属性y
		place->pt4.y = atoi(attributeofpt4->Value());

		elementofPlace = elementofPlace->NextSiblingElement();                                              // 获取库所的第8个子节点--arryPlacePre--无需获取
		elementofPlace = elementofPlace->NextSiblingElement();                                              // 获取库所的第9个子节点--arryPlacePst--无需获取

		elementofPlace = elementofPlace->NextSiblingElement();                                              // 获取库所的第10个子节点--bisUserDef_prob
		CString bisuserdef_prob(elementofPlace->GetText());
		if(bisuserdef_prob=="true") place->bisUserDef_prob=TRUE; else place->bisUserDef_prob=FALSE;

		elementofPlace = elementofPlace->NextSiblingElement();                                              // 获取库所的第11个子节点--bisUserDef_time
		CString bisuserdef_time(elementofPlace->GetText());
		if(bisuserdef_time=="true") place->bisUserDef_time=TRUE; else place->bisUserDef_time=FALSE;

		elementofPlace = elementofPlace->NextSiblingElement();                                              // 获取库所的第12个子节点--bisUserDef_ctrl
		CString bisuserdef_ctrl(elementofPlace->GetText());
		if(bisuserdef_ctrl=="true") place->bisUserDef_ctrl=TRUE; else place->bisUserDef_ctrl=FALSE;

		elementofPlace = elementofPlace->NextSiblingElement();                                              // 获取库所的第13个子节点--str_Inputport_prob
		place->str_Inputport_prob = elementofPlace->GetText();

		elementofPlace = elementofPlace->NextSiblingElement();                                              // 获取库所的第14个子节点--str_Inputport_time
		place->str_Inputport_time = elementofPlace->GetText();

		elementofPlace = elementofPlace->NextSiblingElement();                                              // 获取库所的第15个子节点--str_Inputport_ctrl
		place->str_Inputport_ctrl = elementofPlace->GetText();

		
		elementofPlace = elementofPlace->NextSiblingElement();                                             // 获取库所的第18个子节点--arryIOputData
		TiXmlElement *elementofIOputData = elementofPlace->FirstChildElement();                            // 获取arryIOputData的第一个值的节点
		for(;elementofIOputData!=NULL;elementofIOputData = elementofIOputData->NextSiblingElement())       // 遍历arryIOputData中的所有变量的节点
		{
			IOValue *ioputdata = new IOValue();//
			TiXmlAttribute *attributeofioputdata = elementofIOputData->FirstAttribute();                   // 获取IOputData的第1个属性--Name
			ioputdata->Name = attributeofioputdata->Value();
			attributeofioputdata = attributeofioputdata->Next();                                           // 获取IOputData的第2个属性--Sequence
			ioputdata->Sequence = atoi(attributeofioputdata->Value());
			  
			attributeofioputdata = attributeofioputdata->Next();                                           // 获取IOputData的第3个属性--IOType
			ioputdata->IOType = attributeofioputdata->Value();
			attributeofioputdata = attributeofioputdata->Next();                                           // 获取IOputData的第4个属性--FromModel
			ioputdata->FromModel = attributeofioputdata->Value();
			attributeofioputdata = attributeofioputdata->Next();                                           // 获取IOputData的第5个属性--FromMDVluName
			ioputdata->FromMDVluName = attributeofioputdata->Value();

			ioputdata->Value = 0;

			place->arryIOputData.push_back(ioputdata);
		}


		elementofPlace = elementofPlace->NextSiblingElement();                                             // 获取库所的第19个子节点--arryInputofLua

		elementofPlace = elementofPlace->NextSiblingElement();                                             // 获取库所的第20个子节点--arryOutputofLua
		
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
							/*if((**iter).FromModel == "参数1" || (**iter).FromModel == "参数2" || (**iter).FromModel == "参数3" || (**iter).FromModel == "参数4" 
								|| (**iter).FromModel == "参数5" || (**iter).FromModel == "参数6" || (**iter).FromModel == "参数7" || (**iter).FromModel == "参数8" 
								|| (**iter).FromModel == "参数9" || (**iter).FromModel == "参数10" || (**iter).FromModel == "布尔标识" || (**iter).FromModel == "概率阈值"
								|| (**iter).FromModel == "仿真步长" || (**iter).FromModel == "参数11"|| (**iter).FromModel == "参数12" || (**iter).FromModel == "参数13" || (**iter).FromModel == "参数14" 
								|| (**iter).FromModel == "参数15" || (**iter).FromModel == "参数16" || (**iter).FromModel == "参数17" || (**iter).FromModel == "参数18" 
								|| (**iter).FromModel == "参数19" || (**iter).FromModel == "参数20" 
								|| (**iter).FromModel=="参数21" || (**iter).FromModel=="参数22"|| (**iter).FromModel=="参数23" || (**iter).FromModel=="参数24"
							|| (**iter).FromModel=="参数25" || (**iter).FromModel=="参数26"|| (**iter).FromModel=="参数27" || (**iter).FromModel=="参数28"
							|| (**iter).FromModel=="参数29" || (**iter).FromModel=="参数30"|| (**iter).FromModel=="参数31" || (**iter).FromModel=="参数32"
							|| (**iter).FromModel=="参数33" || (**iter).FromModel=="参数34"|| (**iter).FromModel=="参数35" || (**iter).FromModel=="参数36"
							|| (**iter).FromModel=="参数37" || (**iter).FromModel=="参数38"|| (**iter).FromModel=="参数39" || (**iter).FromModel=="参数40"
							|| (**iter).FromModel=="参数41" || (**iter).FromModel=="参数42"|| (**iter).FromModel=="参数43" || (**iter).FromModel=="参数44"
							|| (**iter).FromModel=="参数45" || (**iter).FromModel=="参数46"|| (**iter).FromModel=="参数47" || (**iter).FromModel=="参数48"
							|| (**iter).FromModel=="参数49" || (**iter).FromModel=="参数50"
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

		elementofPlace = elementofPlace->NextSiblingElement();                                        // 获取库所的第21个子节点--Lua_function
		const char* luafunc = elementofPlace->GetText();
		if(place->Lua_fileName!="" && place->PlaceStyle == STYLE_CNTNUS)                              // 如果检测函数的Lua函数名不为空,且库所的类型为连续库所时，才写入文件
		{        
			CString Lua_fileName_1 = _T("LuaFile\\") + place->Lua_fileName + _T(".lua");
			CFile file(Lua_fileName_1,CFile::modeCreate | CFile::modeWrite);
			file.Write(luafunc,strlen(luafunc));
			file.Close();
		}

		elementofPlace = elementofPlace->NextSiblingElement();          // 获取库所的属性
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
		place->ShowWindow(SW_SHOW);                                                                  // 显示模型

		Vector_Place.push_back(place);
	}

	//--------------------------------------读取库所信息-----------------------------------------------------End----------

	//--------------------------------------读取变迁信息-----------------------------------------------------Begin--------
	TiXmlElement *TransitionsNode = PlacesNode->NextSiblingElement();                                       // 获取Transitions节点

	//CRect rectofTrans(0,0,15,35);//(0,0,20,50);                                                           // 定义变迁模型的大小
	TiXmlElement *TransitionNode = TransitionsNode->FirstChildElement();                                    // 获取第一个变迁节点
	for(;TransitionNode!=NULL;TransitionNode=TransitionNode->NextSiblingElement())                          // 遍历所有的变迁节点
	{
		CTransition *trans = new CTransition();                                                             // 保存每个变迁信息前，先为变迁分配内存
		trans->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofTrans,m_pView);
		

		//====获取m_caption，u_SubscriptofTrans，Position.x，Position.y，bFire；这几个变量保存为Transition的属性
		TiXmlAttribute * attributeofTrans = TransitionNode->FirstAttribute();                               // 获取其第一个属性值--m_caption
		trans->m_caption = attributeofTrans->Value();
		attributeofTrans = attributeofTrans->Next();                                                        // 获取其第2个属性值--u_SubscriptofTrans
		trans->u_SubscriptofTrans = atoi(attributeofTrans->Value());
		if(trans->u_SubscriptofTrans > max_subTrans)
			max_subTrans = trans->u_SubscriptofTrans;


		attributeofTrans = attributeofTrans->Next();                                                        // 获取其第3个属性值--Position.x
		trans->Position.x = atoi(attributeofTrans->Value());
		attributeofTrans = attributeofTrans->Next();                                                        // 获取其第4个属性值--Position.y
		trans->Position.y = atoi(attributeofTrans->Value());
		attributeofTrans = attributeofTrans->Next();                                                        // 获取其第5个属性值--bFire
		CString bfire(attributeofTrans->Value());
		if(bfire == "true") trans->bFire=TRUE; else trans->bFire=FALSE;

		//====获取pt1、pt2、arryTransPre、arryTransPst；这几个变量保存为Transition的子节点
		TiXmlElement * elementofTrans  = TransitionNode->FirstChildElement();                               // 获取第一个子节点--pt1
		TiXmlAttribute * attributeofpt1 = elementofTrans->FirstAttribute();                                 // 获取pt1的第一个属性x
		trans->pt1.x = atoi(attributeofpt1->Value());

		attributeofpt1 = attributeofpt1->Next();                                                            // 获取pt1的第二个属性y
		trans->pt1.y = atoi(attributeofpt1->Value());

		elementofTrans = elementofTrans->NextSiblingElement();                                              // 获取第二个子节点 pt2
		TiXmlAttribute * attributeofpt2 = elementofTrans->FirstAttribute();                                 // 获取pt2的第一个属性x
		trans->pt2.x = atoi(attributeofpt2->Value());

		attributeofpt2 = attributeofpt2->Next();                                                            // 获取pt2的第二个属性y
		trans->pt2.y = atoi(attributeofpt2->Value());
		 
		elementofTrans = elementofTrans->NextSiblingElement();                                              // 获取第三个子节点 arryTransPre--无需读取
		elementofTrans = elementofTrans->NextSiblingElement();                                              // 获取第四个子节点 arryTransPst--无需读取

		trans->MoveWindow(trans->Position.x + pt_ViewportOrg.x,trans->Position.y + pt_ViewportOrg.y,rectofTrans.Width(),rectofTrans.Height());    // 移动模型到指定位置
		trans->ShowWindow(SW_SHOW);                                                                         // 显示模型

		Vector_Trans.push_back(trans);                                                                      // 最后，将变迁保存到Vector_Trans集合中
	}
	//--------------------------------------读取变迁信息-----------------------------------------------------End----------

	//--------------------------------------读取有向弧信息---------------------------------------------------Begin--------
	TiXmlElement *ArcsNode = TransitionsNode->NextSiblingElement();                                        // 获取Arcs节点
	TiXmlElement *ArcNode = ArcsNode->FirstChildElement();                                                 // 获取第一个有向弧节点
	for(;ArcNode!=NULL;ArcNode = ArcNode->NextSiblingElement())                                            // 遍历所有的有向弧节点
	{
		CPoint ptstart;
		CPoint ptend;
		TiXmlAttribute *attributeofArc = ArcNode->FirstAttribute();                                        // 获取有向弧的第一个属性--whichModel_Start
		int whichmodelstart = atoi(attributeofArc->Value());
		attributeofArc = attributeofArc->Next();                                                           // 获得有向弧的第二个属性--u_SubScriptofPlace
		int subofplace = atoi(attributeofArc->Value());
		attributeofArc = attributeofArc->Next();                                                           // 获得有向弧的第三个属性--u_SubScriptofTrans
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

		// 将属性值赋值给arr_Arrow集合中的元素
		CArrow *arrow = new CArrow(subofplace,suboftrans,whichmodelstart,ptstart,ptend,arrowid,isdisic);
		arr_Arrow.Add(arrow);
		// 更新库所和变迁的前置和后置			
		vector<CPlace*>::iterator iterp=FindPlace(subofplace);
		vector<CTransition*>::iterator itert=FindTrans(suboftrans);
		switch(whichmodelstart)
		{
		case 1:// 开始模型为库所，则更新 库所的后置和变迁的前置
			(*iterp)->arryPlacePst.push_back(*itert);
			(*itert)->arryTransPre.push_back(*iterp);
			(*iterp)->arryArrowPst.push_back(arrow);
			(*itert)->arryArrowPre.push_back(arrow);
			break;
		case 2:// 开始模型为变迁，更新 变迁的后置和库所的前置
			(*itert)->arryTransPst.push_back(*iterp);
			(*iterp)->arryPlacePre.push_back(*itert);
			(*itert)->arryArrowPst.push_back(arrow);
			(*iterp)->arryArrowPre.push_back(arrow);
			break;
		default:
			break;
		}
	}
	//--------------------------------------读取有向弧信息---------------------------------------------------End----------

	//--------------------------------------读取输出模块信息-------------------------------------------------Begin--------
	TiXmlElement *OutModelsNode = ArcsNode->NextSiblingElement();                                               // 获取OutModels节点
	TiXmlElement *OutModelNode  = OutModelsNode->FirstChildElement();                                           // 获取第一个输出模块节点
	for(;OutModelNode!=NULL;OutModelNode = OutModelNode->NextSiblingElement())                                  // 遍历所有的输出模块节点
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

		//===获取m_caption、ID、Position、OutputStyle属性==
		attributeofOuts = OutModelNode->FirstAttribute();                                     // 获取其第一个属性值--m_caption
		m_OutputModel->m_caption = attributeofOuts->Value();

		attributeofOuts = attributeofOuts->Next();                                                             //获取属性值：ID
		m_OutputModel->ID = atoi(attributeofOuts->Value());
		if(m_OutputModel->ID > max_subOuts)
			max_subOuts = m_OutputModel->ID;

		attributeofOuts = attributeofOuts->Next();                                                             //获取属性值：x
		m_OutputModel->Position.x = atoi(attributeofOuts->Value());
		attributeofOuts = attributeofOuts->Next();                                                             //获取属性值：y
		m_OutputModel->Position.y = atoi(attributeofOuts->Value());

		attributeofOuts = attributeofOuts->Next();                                                             //获取属性值：OutputStyle
		CString outputstyle(attributeofOuts->Value());
		if(outputstyle == "1") m_OutputModel->OutputStyle = OUT_SHOWMATRIX;
		if(outputstyle == "2") m_OutputModel->OutputStyle = OUT_TOFILE;
		if(outputstyle == "3") m_OutputModel->OutputStyle = OUT_ACCUMULATOR;
		
		//===获取OutPName、vec_OutValueName、i_PposnOValue、vec_OutValue属性==
		TiXmlElement * elementofOuts  = OutModelNode->FirstChildElement();                                     // 获取属性值：OutPName
		m_OutputModel->OutPName = elementofOuts->GetText();

		elementofOuts  = elementofOuts->NextSiblingElement();                                                    // 获取属性值：vec_OutValueName
		TiXmlElement * elementofvOVN  = elementofOuts->FirstChildElement();
		for(;elementofvOVN!=NULL;elementofvOVN=elementofvOVN->NextSiblingElement())
		{
			CString outvaluename(elementofvOVN->GetText());
			m_OutputModel->vec_OutValueName.push_back(outvaluename);
		}

		elementofOuts  = elementofOuts->NextSiblingElement();                                                    // 获取属性值：i_PposnOValue
		//m_OutputModel->i_PposnOValue = atoi(elementofOuts->Value());
		m_OutputModel->i_PposnOValue = atoi(elementofOuts->GetText());

		elementofOuts  = elementofOuts->NextSiblingElement();                                                    // 获取属性值：vec_OutValue
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

		elementofOuts  = elementofOuts->NextSiblingElement();                                                    // 获取属性值：str_FilePath
		m_OutputModel->str_FilePath = elementofOuts->GetText();

		if(m_OutputModel->OutputStyle == OUT_TOFILE)
		{
			m_OutputModel->m_pDatafile = new ofstream(m_OutputModel->str_FilePath);
		}

		m_OutputModel->MoveWindow(m_OutputModel->Position.x + pt_ViewportOrg.x,m_OutputModel->Position.y + pt_ViewportOrg.y,rectofOutputModel.Width(),rectofOutputModel.Height());
		m_OutputModel->ShowWindow(SW_SHOW);

		Vector_OutputModel.push_back(m_OutputModel);
	}
	//--------------------------------------读取输出模块信息-------------------------------------------------End----------

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
	UINT max_subTrans = 0;                          // 找出打开模型中库所和变迁的最大下标
	UINT max_subOuts  = 0;
	char *filepath_name;
	USES_CONVERSION;
	filepath_name = T2A(filePath_Name);
	myDocument = new TiXmlDocument(filepath_name);
	myDocument->LoadFile();                         // 加载文件

	// 获取视口原点
	CPoint pt_ViewportOrg;
	CClientDC dc((CWnd*)m_pView);
	m_pView->OnPrepareDC(&dc);
	pt_ViewportOrg = dc.GetViewportOrg();

	TiXmlElement *RootElement = myDocument->RootElement();                                                    // 获取根元素--HSTPnet
	TiXmlElement *PTONode = RootElement->FirstChildElement();
	CString PTONodeName;
	for(;PTONode!=NULL;PTONode=PTONode->NextSiblingElement())// 遍历二级子节点"ListGlobal"/"Places"/"Transitions"/"Arcs"/"OutModels"
	{
		PTONodeName = PTONode->Value();
		//-----------------------------读取全局变量表信息----------------------------------------------------------------------------------------
		if(PTONodeName == "ListGlobal")
		{
			TiXmlElement * IOputDataGNode = PTONode->FirstChildElement();
			for(;IOputDataGNode!=NULL;IOputDataGNode=IOputDataGNode->NextSiblingElement())
			{
				IOValue * ioputdata = new IOValue();
				TiXmlAttribute * AttributeofIOG = IOputDataGNode->FirstAttribute(); // 遍历每个遍历的所有结构项Name/IOType/InitValue
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
		//-----------------------------读取库所信息----------------------------------------------------------------------------------------------
		if(PTONodeName == "Places")
		{
			TiXmlElement * PlaceNode = PTONode->FirstChildElement();
			for(;PlaceNode!=NULL;PlaceNode=PlaceNode->NextSiblingElement())// 遍历每一个库所元素
			{
				CPlace * place = new CPlace();
				TiXmlAttribute * AttributeofP = PlaceNode->FirstAttribute();
				CString AttributeofPName;
				for(;AttributeofP!=NULL;AttributeofP=AttributeofP->Next())//遍历库所的属性m_caption/u_SubscriptofModel/PlaceStyle/x/y/bActivation/bEnable/bisAssigned/ischoseSocket/ischoseupdown/dTime_downline/dTime_upline/ischoseProgRand
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
				for(;ChildofP!=NULL;ChildofP=ChildofP->NextSiblingElement())// 遍历库所的子节点
				{
					ChildofPName = ChildofP->Value();
					if(ChildofPName == "S_Luafile")
					{
						const char* S_luafunc = ChildofP->GetText();
						if(place->ischoseProgRand && place->PlaceStyle == STYLE_RAND)  // 如果检测函数的Lua函数名不为空,且库所的类型为连续库所时，才写入文件
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
						TiXmlAttribute *attributeofpt1 = ChildofP->FirstAttribute();                                   // 获取pt1的第一个属性x
						place->pt1.x = atoi(attributeofpt1->Value());
						attributeofpt1 = attributeofpt1->Next();                                                       // 获取pt1的第二个属性y
						place->pt1.y = atoi(attributeofpt1->Value());
					}
					if(ChildofPName == "pt2")
					{
						TiXmlAttribute *attributeofpt2 = ChildofP->FirstAttribute();                                  // 获取pt2的第一个属性x
						place->pt2.x = atoi(attributeofpt2->Value());
						attributeofpt2 = attributeofpt2->Next();                                                      // 获取pt2的第二个属性y
						place->pt2.y = atoi(attributeofpt2->Value());
					}
					if(ChildofPName == "pt3")
					{
						TiXmlAttribute *attributeofpt3 = ChildofP->FirstAttribute();                                  // 获取pt3的第一个属性x
						place->pt3.x = atoi(attributeofpt3->Value());
						attributeofpt3 = attributeofpt3->Next();                                                      // 获取pt3的第二个属性y
						place->pt3.y = atoi(attributeofpt3->Value());
					}
					if(ChildofPName == "pt4")
					{
						TiXmlAttribute *attributeofpt4 = ChildofP->FirstAttribute();                                  // 获取pt4的第一个属性x
						place->pt4.x = atoi(attributeofpt4->Value());
						attributeofpt4 = attributeofpt4->Next();                                                      // 获取pt4的第二个属性y
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
						for(;elementofIOputData!=NULL;elementofIOputData = elementofIOputData->NextSiblingElement())       // 遍历arryIOputData中的所有变量的节点
						{
							IOValue *ioputdata = new IOValue();
							//遍历所有属性 Name/Sequence/IOType/isQuoteG/GlobalVluName/FromModel/FromMDVluName/InitValue
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
						// 重新排序
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
						if(place->Lua_fileName!="" && place->PlaceStyle == STYLE_CNTNUS)                              // 如果检测函数的Lua函数名不为空,且库所的类型为连续库所时，才写入文件
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
				place->ShowWindow(SW_SHOW);                                                                  // 显示模型

				Vector_Place.push_back(place);
			}
		}
		//-----------------------------读取变迁信息----------------------------------------------------------------------------------------------
		if(PTONodeName == "Transitions")
		{
			TiXmlElement *TransitionNode = PTONode->FirstChildElement();
			for(;TransitionNode!=NULL;TransitionNode=TransitionNode->NextSiblingElement()) // 遍历所有的变迁
			{
				CTransition *trans = new CTransition();
				// 遍历变迁所有的属性m_caption/u_SubscriptofTrans/x/y/bFire
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
				// 遍历变迁所有的子节点pt1/pt2/arryTransPre/arryTransPst
				TiXmlElement * elementofTrans  = TransitionNode->FirstChildElement();
				CString elementofTransName;
				for(;elementofTrans!=NULL;elementofTrans=elementofTrans->NextSiblingElement())
				{
					elementofTransName = elementofTrans->Value();
					if(elementofTransName == "pt1")
					{
						TiXmlAttribute * attributeofpt1 = elementofTrans->FirstAttribute();                                 // 获取pt1的第一个属性x
						trans->pt1.x = atoi(attributeofpt1->Value());
						attributeofpt1 = attributeofpt1->Next();                                                            // 获取pt1的第二个属性y
						trans->pt1.y = atoi(attributeofpt1->Value());
					}
					if(elementofTransName == "pt2")
					{
						TiXmlAttribute * attributeofpt2 = elementofTrans->FirstAttribute();                                 // 获取pt2的第一个属性x
						trans->pt2.x = atoi(attributeofpt2->Value());
						attributeofpt2 = attributeofpt2->Next();                                                            // 获取pt2的第二个属性y
						trans->pt2.y = atoi(attributeofpt2->Value());
					}
					if(elementofTransName == "arryTransPre")
					{}
					if(elementofTransName == "arryTransPst")
					{}
				}
				trans->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER|SS_NOTIFY, rectofTrans,m_pView);
				trans->MoveWindow(trans->Position.x + pt_ViewportOrg.x,trans->Position.y + pt_ViewportOrg.y,rectofTrans.Width(),rectofTrans.Height());    // 移动模型到指定位置
				trans->ShowWindow(SW_SHOW);                                                                         // 显示模型

				Vector_Trans.push_back(trans); 
			}
		}
		//-----------------------------读取有向弧信息--------------------------------------------------------------------------------------------
		if(PTONodeName == "Arcs")
		{
			TiXmlElement *ArcNode = PTONode->FirstChildElement();
			for(;ArcNode!=NULL;ArcNode=ArcNode->NextSiblingElement()) // 遍历所有的有向弧
			{
				// 遍历有向弧的属性whichModel_Start/u_SubScriptofPlace/u_SubScriptofTrans
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
				// 将属性值赋值给arr_Arrow集合中的元素
				CArrow *arrow = new CArrow(subofplace,suboftrans,whichmodelstart,ptstart,ptend,arrowid,isdisic);
				if (isdisic)
				{
					CString str;
					str.Format(_T("%d"),num1);
					arrow->m_pEdit->SetWindowText(str);
					arrow->num = num1;
				}
				arr_Arrow.Add(arrow);

				// 更新库所和变迁的前置和后置			
				vector<CPlace*>::iterator iterp=FindPlace(subofplace);
				vector<CTransition*>::iterator itert=FindTrans(suboftrans);
				switch(whichmodelstart)
				{
				case 1:// 开始模型为库所，则更新 库所的后置和变迁的前置
					(*iterp)->arryPlacePst.push_back(*itert);
					(*itert)->arryTransPre.push_back(*iterp);
					(*iterp)->arryArrowPst.push_back(arrow);
					(*itert)->arryArrowPre.push_back(arrow);
					break;
				case 2:// 开始模型为变迁，更新 变迁的后置和库所的前置
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
		//-----------------------------读取输出节点信息------------------------------------------------------------------------------------------
		if(PTONodeName == "OutModels")
		{
			TiXmlElement *OutModelNode  = PTONode->FirstChildElement();
			for(;OutModelNode!=NULL;OutModelNode = OutModelNode->NextSiblingElement()) // 遍历所有的输出节点
			{
				CCTRLShowMatrix * m_OutputModel = new CCTRLShowMatrix();
				// 获取输出节点的属性m_caption/ID/x/y/OutputStyle
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
				// 遍历数据节点的子节点OutPName/vec_OutValueName/i_PposnOValue/vec_OutValue/str_FilePath
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
	
	// 检测所有的ToFile型节点关联的文件是否存在，如果不存在则创建它
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
    CString strDir(szPath);    //存放要创建的目录字符串 
    if (strDir.GetAt(strDir.GetLength() - 1) != _T('\\'))  //确保以'\'结尾以创建最后一个目录  
    {  
        strDir.AppendChar(_T('\\'));  
    }  
    vector<CString> vPath;              //存放每一层目录字符串  
    CString strTemp;                    //一个临时变量,存放目录字符串  
    bool bSuccess = false;              //成功标志  
                                        //遍历要创建的字符串  
    for (int i = 0; i < strDir.GetLength(); ++i)  
    {  
        if (strDir.GetAt(i) != _T('\\'))  
        {                               //如果当前字符不是'\\'  
            strTemp.AppendChar(strDir.GetAt(i));  
        }  
        else  
        {                                //如果当前字符是'\\'  
            vPath.push_back(strTemp);    //将当前层的字符串添加到数组中  
            strTemp.AppendChar(_T('\\'));  
        }  
    }  
                                         //遍历存放目录的数组,创建每层目录  
    vector<CString>::const_iterator vIter;  
    for (vIter = vPath.begin(); vIter != vPath.end()-1; vIter++)  
    {  
        if (!PathIsDirectory(*vIter))    //判断当前目录时候存在，不存在就创建  
       {  
                                         //如果CreateDirectory执行成功,返回true,否则返回false  
            bSuccess = CreateDirectory(*vIter, NULL) ? true : false;  
        }         
    } 

	// 创建完目录后，创建一个CSV文件
	CFile file;
	file.Open(szPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
//	file.Close();
    return bSuccess;  
}  




// 查找库所
vector<CPlace*>::iterator CHSTPN_SYSDoc::FindPlace( UINT id)
{
	vector<CPlace*>::iterator iter = Vector_Place.begin(); // 定义一个迭代器
	int nSize = Vector_Place.size();                       // 获取vector_place的大小
	nSize = (int)floor(nSize/2.0);                              // 定义标志j
	int j = nSize;
	iter = iter + j;                                       // 将迭代器推进到中间位置

	do{
		if(id >= (*iter)->u_SubscriptofModel)                  //要寻找的元素在中间元素的右边
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
		else                                                   //要寻找的元素在中间元素的左边
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
				// 如果倒序变量没有找到，就只剩第一个了，因为第一个没有被遍历到
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

// 查找变迁
vector<CTransition*>::iterator CHSTPN_SYSDoc::FindTrans(UINT id)
{
	vector<CTransition*>::iterator iter = Vector_Trans.begin(); // 定义一个迭代器
	int nSize = Vector_Trans.size();                       // 获取vector_Trans的大小
	nSize = (int)floor(nSize/2.0);                              // 定义标志j
	int j = nSize;
	iter = iter + j;                                       // 将迭代器推进到中间位置

	do{
		if(id >= (*iter)->u_SubscriptofTrans)                  //要寻找的元素在中间元素的右边
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
		else                                                   //要寻找的元素在中间元素的左边
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
				// 如果倒序变量没有找到，就只剩第一个了，因为第一个没有被遍历到
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

// 创建线程，开始仿真
void CHSTPN_SYSDoc::OnDebugStart()
{
	// TODO: 在此添加命令处理程序代码
	// =============开辟仿真线程===================

	if(Vector_Place.size()==0 || Vector_Trans.size()==0)// 如果库所或变迁为空，则不开始，而是提醒用户可能存在孤立结点
	{
		CHSTPN_SYSView* m_pHSTView;
		POSITION pos = GetFirstViewPosition();
		GetNextView(pos);
		m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);
		m_pHSTView->MessageBox(L"没有模型或可能存在孤立结点，请进行模型检测!");
	}
	else                                                // 如果不为空，则开始
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
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_wndStatusBar.EnablePaneProgressBar(1,2147483647,true);// 设置仿真进度的最大范围
		}
		else
		{
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_wndStatusBar.EnablePaneProgressBar(1,m_pHSTView->d_SimTime,true);// 设置仿真进度的最大范围
		}
		if(!b_SimSuspend)// 在暂停状态下，不进行仿真
		{
			m_simThread = AfxBeginThread(m_hstpnSim->HSTPN_SimEngine/*HSTPN_Simulation*/,(LPVOID)&m_hstpnData);// 开辟新线程，进行程序仿真。参数1 线程的入口函数,声明一定要如下: UINT MyThreadFunction( LPVOID pParam ); 参数2 传递入线程的参数, 注意它的类型为:LPVOID, 所以我们可以传递一个结构体入线程。
		}
	}

}

// 创建模型检查对话框
void CHSTPN_SYSDoc::OnModelcheck()
{
	// TODO: 在此添加命令处理程序代码
	CHSTPN_SYSView* m_pHSTView;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);
	//---创建模型检查对话框-----
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

// 暂停仿真
void CHSTPN_SYSDoc::OnSimThreadSuspend()
{
	// TODO: 在此添加命令处理程序代码
	if(Vector_Place.size()==0 || Vector_Trans.size()==0)// 如果库所或变迁为空，则不开始，而是提醒用户可能存在孤立结点
	{
		CHSTPN_SYSView* m_pHSTView;
		POSITION pos = GetFirstViewPosition();
		GetNextView(pos);
		m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);
		m_pHSTView->MessageBox(L"没有模型或可能存在孤立结点，请进行模型检测");
	}
	else
	{
		if(m_simThread!=NULL)
		{
			if(!b_SimSuspend)//如果线程没有被暂停，则暂停线程
			{
				m_simThread->SuspendThread();
				b_SimSuspend = TRUE;
			}
			else            //如果线程被暂停了，则继续线程
			{
				m_simThread->ResumeThread();
				b_SimSuspend = FALSE;
			}
		}
	}
}

// 结束仿真
void CHSTPN_SYSDoc::OnSimThreadStop()
{
	CHSTPN_SYSView* m_pHSTView;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);
	// TODO: 在此添加命令处理程序代码
	if(Vector_Place.size()==0 || Vector_Trans.size()==0)// 如果库所或变迁为空，则不开始，而是提醒用户可能存在孤立结点
	{
		m_pHSTView->MessageBox(L"没有模型或可能存在孤立结点，请进行模型检测");
	}
	else
	{
		if(m_simThread!=NULL)
		{
			if(!b_SimSuspend)
			{
				SetEvent(m_StopSimEvent->m_hObject);// 设置事件为有信号状态，也就是在CHSTPN_SYSSim类的HSTPN_Simulation()函数中，设置函数退出，即进程结束
				m_simThread = NULL;
			}
			else
			{
				/*CHSTPN_SYSView* m_pHSTView;
				POSITION pos = GetFirstViewPosition();
				GetNextView(pos);
				m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);
				m_pHSTView->MessageBox(L"请在运行状态下点击结束按钮");*/
				m_simThread->ResumeThread();//如果此时为暂停状态，先回复仿真
				b_SimSuspend = FALSE;
				SetEvent(m_StopSimEvent->m_hObject);// 再结束线程
				m_simThread = NULL;
			}
			//m_pHSTView->OnRestorePnset();// 调用View类的OnRestorePnset()方法，将Petri网设置回初始状态
		}
	}
}

// 开辟线程，开始关联矩阵求解
void CHSTPN_SYSDoc::OnIncdmatanaly()
{
	// TODO: 在此添加命令处理程序代码

	if(Vector_Place.size()==0 || Vector_Trans.size()==0)// 如果库所或变迁为空，则不开始，而是提醒用户可能存在孤立结点
	{
		CHSTPN_SYSView* m_pHSTView;
		POSITION pos = GetFirstViewPosition();
		GetNextView(pos);
		m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);
		m_pHSTView->MessageBox(L"没有模型或可能存在孤立结点，请进行模型检测");
	}
	else
	{
		m_hstpnData.vector_place = Vector_Place;
		m_hstpnData.vector_trans = Vector_Trans;
		m_hstpnData.arr_arrow.Copy(arr_Arrow);
		m_hstpnData.m_stopsimevent = NULL;

		m_IncdMatThread = AfxBeginThread(m_hstpnSim->IncdnceMatCompute,(LPVOID)&m_hstpnData);// 开辟新线程，进行关联矩阵计算
	}

}


void CHSTPN_SYSDoc::OnFileSceneOne() {
	// TODO: 在此添加命令处理程序代码
	CHSTPN_SYSView* m_pHSTView;
	POSITION pos = GetFirstViewPosition();
	GetNextView(pos);
	m_pHSTView = (CHSTPN_SYSView*)GetNextView(pos);

	//====================打开操作=========================================================begin=========
	int n_SizeP = Vector_Place.size();
	int n_SizeT = Vector_Trans.size();
	int n_SizeA = arr_Arrow.GetSize();
	int n_SizeO = Vector_OutputModel.size();

	CFileDialog m_saveDlg(TRUE);                                             // 创建打开文件对话框
	m_saveDlg.m_ofn.lpstrFilter = L"HSTPN Files(*.hst)\0*.hst\0All Files(*.*)\0\0";//设置过滤器
	m_saveDlg.m_ofn.lpstrDefExt = L"hst";                                    // 添加缺省的扩展名

	CString str_filename;
	str_filename = "modelOne.hst";
	m_pHSTView->myDocument = ReadXMLFile(str_filename, m_pHSTView->myDocument, m_pHSTView);
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SetWindowText(str_filename + _T("-HSTPNSim"));
	m_pHSTView->bPNisChanged = FALSE;

	m_pHSTView->Invalidate();
	m_pHSTView->UpdateWindow();
	m_pHSTView->Invalidate();
	m_pHSTView->UpdateWindow();
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->SendMessage(WM_MESSAGE_UPDATETREE1, 0, 0);// 目的发送至mainframe，再发送至dlgmodelchoose1，通知HSTPN结构树更新
	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pParmListDlg->SendMessage(WM_MESSAGE_UPDATETREEOFMODEL, 0, 0);// 向模型编辑器发送WM_MESSAGE_UPDATETREEOFMODEL消息，更新模型树
	
}