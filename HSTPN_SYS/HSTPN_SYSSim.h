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
	static UINT             HSTPN_Simulation(LPVOID psimnet);                                                          // Petri网模型推演算法
	static void             UpdateIOValueofModel(vector<CPlace*> vector_place, CHSTPN_SYSDoc * m_pdoc, vector<CPlace*> PActived);                                        // 输入/输出列表值更新
	static UINT             HSTPN_SimEngine(LPVOID psimnet);                                                           // Petri网模型推演算法

	static vector<CString>  IsLtdNodeDetect(vector<CPlace*> vector_place,vector<CTransition*> vector_trans);           // 孤立点检查算法
	static UINT             ModelCheck(LPVOID m_modelcheckdata);                                                       // 孤立点检查算法

	static bool             isPureNet(vector<CPlace*> vector_place,vector<CTransition*> vector_trans);                 // 检测是否为纯网
	static UINT             IncdnceMatCompute(LPVOID psimnet);                                                         // 关联矩阵相关分析算法

	static void             UpdateOutputModel(vector<CPlace*> vector_place,vector<CCTRLShowMatrix*> vector_outputmodel,CHSTPN_SYSDoc * m_pDoc);

	static void             WriteDatatoFile(ofstream * file, const list<string> &vStr);

	static void             CloseAllDataFile(vector<CCTRLShowMatrix*> vector_outputmodel);                             // 仿真结束后，关闭OUT_TOFILE模块中所有的datafile

	static void             OpenAllDataFile(vector<CCTRLShowMatrix*> vector_outputmodel);                              // 仿真结束后，打开OUT_TOFILE模块中所有的datafile
	static void             InitPlaceLocalData(vector<CPlace*> vec_place, CHSTPN_SYSDoc * m_pdoc);
	
	static void             PassByValue(CHSTPN_SYSDoc * m_pdoc);
	//static void             UpdateMoveValue(CHSTPN_SYSDoc * m_pDoc);//更新动画所需的变量
	static bool startFlag;
	static vector<unordered_map<string, int>> getMoveData();
	static vector<unordered_map<string, int>> mapMoveData;
};


