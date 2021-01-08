#pragma once

#include "HSTPN_SYS.h"

class CMySQL
{
public:
	CMySQL();

	~CMySQL();

	MYSQL mysql;//mySql

	MYSQL_FIELD *fd;  //�ֶ�������
	char field[32][32];  //���ֶ�����ά����
	MYSQL_RES *res; //����ṹ�������е�һ����ѯ�����
	MYSQL_ROW column; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���
	char query[150]; //��ѯ���

	void ConnectMySQL();    //�������ݿ�
	void FreeConnect();
 
	bool InsertData();		//��
	bool DeleteData();		//ɾ
	bool ModifyData();		//��
	bool QueryDatabase1();  //��ѯ1  
	bool QueryDatabase2();  //��ѯ2 
};

extern CMySQL mySQL;