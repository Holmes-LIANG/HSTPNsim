#pragma once


// CScene �Ի���

class CScene : public CDialogEx
{
	DECLARE_DYNAMIC(CScene)

public:

	virtual ~CScene();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SCENE };
#endif
//private:
//	CScene(CWnd* pParent = nullptr);   // ��׼���캯��
//
//	CScene(const CScene&);
//	CScene& operator=(const CScene&);
//public:
//	static CScene& getInstance() {
//		static CScene instance;
//		return instance;
//	}
private:
	CScene* pSceneDlg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButtonOne();
	afx_msg void OnBnClickedButtonTwo();
	afx_msg void OnBnClickedButtonThree();
	afx_msg void OnBnClickedButtonFour();
			
	void CreatSceneDlg();
};
