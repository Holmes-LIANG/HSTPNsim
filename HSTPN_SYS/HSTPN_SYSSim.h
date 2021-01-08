#pragma once
#include "HSTPN_Message.h"
#include "HSTPN_SYSDoc.h"
#include "HSTPN_SYSView.h"
#include "MainFrm.h"
#include "HSTPN_SYSDoc.h"
#include <unordered_map>

class CMainFrame;
class CHSTPN_SYSDoc;
//#include "Frame_AnalyLog.h"
#include "WndAnalyLog.h"
#include "WndShowMatrix.h"
class CCTRLShowMatrix;

class CHSTPN_SYSSim : public CWnd
{
	DECLARE_DYNAMIC(CHSTPN_SYSSim)

public:
	CHSTPN_SYSSim();
	virtual ~CHSTPN_SYSSim();

protected:
	DECLARE_MESSAGE_MAP()
public:
	//static vector<CString> IsLtdNodeSet;
public:
	static UINT             HSTPN_Simulation(LPVOID psimnet);                                                          // Petri��ģ�������㷨
	static void             UpdateIOValueofModel(vector<CPlace*> vector_place, CHSTPN_SYSDoc * m_pdoc, vector<CPlace*> PActived);                                        // ����/����б�ֵ����
	static UINT             HSTPN_SimEngine(LPVOID psimnet);                                                           // Petri��ģ�������㷨

	static vector<CString>  IsLtdNodeDetect(vector<CPlace*> vector_place,vector<CTransition*> vector_trans);           // ���������㷨
	static UINT             ModelCheck(LPVOID m_modelcheckdata);                                                       // ���������㷨

	static bool             isPureNet(vector<CPlace*> vector_place,vector<CTransition*> vector_trans);                 // ����Ƿ�Ϊ����
	static UINT             IncdnceMatCompute(LPVOID psimnet);                                                         // ����������ط����㷨

	static void             UpdateOutputModel(vector<CPlace*> vector_place,vector<CCTRLShowMatrix*> vector_outputmodel,CHSTPN_SYSDoc * m_pDoc);

	static void             WriteDatatoFile(ofstream * file, const list<string> &vStr);

	static void             CloseAllDataFile(vector<CCTRLShowMatrix*> vector_outputmodel);                             // ��������󣬹ر�OUT_TOFILEģ�������е�datafile

	static void             OpenAllDataFile(vector<CCTRLShowMatrix*> vector_outputmodel);                              // ��������󣬴�OUT_TOFILEģ�������е�datafile
	static void             InitPlaceLocalData(vector<CPlace*> vec_place, CHSTPN_SYSDoc * m_pdoc);
	
	static void             PassByValue(CHSTPN_SYSDoc * m_pdoc);
	//static void             UpdateMoveValue(CHSTPN_SYSDoc * m_pDoc);//���¶�������ı���
	static bool startFlag;
	static vector<unordered_map<string, int>> getMoveData();
	static vector<unordered_map<string, int>> mapMoveData;
};


