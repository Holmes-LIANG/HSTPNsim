#include "StdAfx.h"
#include "CSVStream.h"


CCSVStream::CCSVStream(void)
{  
}


CCSVStream::~CCSVStream(void)
{
}

const bool CCSVStream::IsCsvFile(const char* lpszFilename)
{
	/// 0���ж��ļ����Ƿ���Ч
	if (NULL == lpszFilename || 4 > strlen(lpszFilename)) 
		return false;
	/// ���ر���
	string _strFileName(lpszFilename);
	size_t _iLen = _strFileName.length();
	string _strSuff(_strFileName.substr(_iLen - 4, 4));//ȡ�����4���ַ�
	/// ת��ΪСд�����Ҫת��Ϊ��д��tolower -> toupper ��
	transform(_strSuff.begin(), _strSuff.end(), _strSuff.begin(), tolower);
	/// 1���ж��Ƿ���CSV�ļ�
	return (0 == _strSuff.compare(".csv"));
}

const int CCSVStream::ReadCsvData(const char* lpszFilename, vector<list<string> > &vlStr)
{
	/// 1���ж��Ƿ���CSV�ļ�
	if (! IsCsvFile(lpszFilename)) 
		return XC_ERR_INVALID_FILE_NAME;

	/// 2����CSV�ļ�
	ifstream _streamFromFile(lpszFilename);
	///    �жϴ��ļ��Ƿ�ɹ�
	if (!_streamFromFile) 
		return (-errno);
	/// �洢��ȡ���ļ�����
	string _strIn("");

	/// 3����ȡһ��
	while (getline(_streamFromFile, _strIn))// ��ȡһ�У����浽_strIn��
	{
		/// ÿ�е�Դ�ַ���
		const char* _pcSrc = _strIn.c_str();// ��stringת��Ϊconst char*
		/// �洢һ�С�,'�ָ�������ĸ���Ԫ��
		list<string> _ltStr;
		/// Parse values in this line
		while (*_pcSrc != '\0')
		{
			/// string to hold this value
			string _strElem(""); 
			/// ���ÿ���ַ�����
			if (*_pcSrc == '"') 
			{
				/// Bump past opening quote
				_pcSrc++;
				/// Parse quoted value
				while (*_pcSrc != '\0') 
				{
					/// Test for quote character
					if (*_pcSrc == '"') 
					{
						/// Found one quote
						_pcSrc++;
						// If pair of quotes, keep one
						// Else interpret as end of value
						if (*_pcSrc != '"') 
						{
							_pcSrc++;
							break;
						}
					}
					/// Add this character to value
					_strElem.push_back(*_pcSrc++);
				}
			}
			else 
			{
				// Parse unquoted value
				while (*_pcSrc != '\0' && *_pcSrc != ',') 
					_strElem.push_back(*_pcSrc++);
				// Advance to next character (if not already end of string)
				if (*_pcSrc != '\0')
					_pcSrc++;
			}
			/// Add this string to container
			_ltStr.push_back(_strElem);
		}
		/// �������һ���ļ��������õ�Ԫ���б���ӵ�������
		vlStr.push_back(_ltStr);
		/// ���㣬��ֹ�´η����ɵ����ݡ�
		_strIn.assign("");
	}


	return XC_ERR_NONE;
}


const int CCSVStream::WriteCsvData(const char* lpszFilename, const vector<list<string> > &vlStr)
{
	/// 1���ж��Ƿ���CSV�ļ�
	if (! IsCsvFile(lpszFilename)) 
		return XC_ERR_INVALID_FILE_NAME;

	/// 2����CSV�ļ�
	ofstream _streamToFile(lpszFilename);
	///    �жϴ��ļ��Ƿ�ɹ�
	if (!_streamToFile) 
		return (-errno);
	/// ���ر���
	static TCHAR chQuote = '"';
	static TCHAR chComma = ',';
	/// Loop through each list of string in vector 
	for (vector<list<string> >::const_iterator vIt = vlStr.begin(); vIt != vlStr.end(); vIt ++) 
	{
		/// Loop through each string in list 
		for (list<string>::const_iterator lIt = vIt->begin(); lIt != vIt->end(); lIt ++) 
		{
			/// Separate this value from previous
			if (vIt->begin() != lIt) 
				_streamToFile.put(chComma);
			/// ����string�п�����,��"����������Ҫ�����װ��
			bool bComma = (lIt->find(chComma) != lIt->npos);
			bool bQuote = (lIt->find(chQuote) != lIt->npos);
			/// ��ĺ���,��"�����
			if (bComma || bQuote) 
			{
				_streamToFile.put(chQuote);


				if (bQuote) 
				{
					for (string::const_iterator chIt = lIt->begin(); chIt != lIt->end(); chIt ++ ) 
					{
						// Pairs of quotes interpreted as single quote
						if (chQuote == *chIt) 
						_streamToFile.put(chQuote);


						_streamToFile.put(*chIt);
					}
				}
				else 
				{
					_streamToFile << *lIt;
				}


				_streamToFile.put(chQuote);
			}
			else
			{
				_streamToFile << *lIt;
			}
		}
		/// ����
		_streamToFile << endl;
	}
	/// 
return XC_ERR_NONE;
}