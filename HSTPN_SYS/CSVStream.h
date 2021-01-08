#pragma once
#include <string>
#include <list>
#include <vector>
using namespace std;
#include <windows.h>
#include <algorithm>
#include <fstream>
/// �޴�
#define XC_ERR_NONE 0
/// ��Ч�ļ������ָ���ļ���ʽ
#define XC_ERR_INVALID_FILE_NAME (-1)

class CCSVStream
{
public:
	CCSVStream(void);
	~CCSVStream(void);


	/*
	* �������ܣ���ȡCSV�ļ����������е����ݣ�Ȼ��洢�������С�
	* ����������
	*     [in] lpszFilename - ����ȡ��CSV�ļ���
	*     [in, out] vlStr - �洢�������CSV����
	* ����ֵ��
	*     �������
	* ע�⣺������Ϊ����Ը�⣨Ŀǰ���������������vector<list<string>>������vector<list<string> >��
	*/
	const int ReadCsvData(const char* lpszFilename, vector<list<string> > &vlStr);
	/* 
	* �������ܣ��������е����ݾ����Ű桢���֡���ӷָ�����д�뵽CSV�ļ���
	* ����������
	*     [in] lpszFilename - ����ȡ��CSV�ļ���
	*     [in] vlStr - �洢�������CSV����
	* ����ֵ��
	*     �������
	* ע�⣺������Ϊ����Ը�⣨Ŀǰ���������������vector<list<string>>������vector<list<string> >��
	*/
	const int WriteCsvData(const char* lpszFilename, const vector<list<string> > &vlStr);

private:
	/// �ж��Ƿ���CSV�ļ�
	const bool IsCsvFile(const char* lpszFilename);

};

