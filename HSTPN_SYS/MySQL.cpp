//
#include "stdafx.h"
//#include "MySQL.h"
//
//#include <stdio.h>  
//#include <WinSock.h>  //һ��Ҫ�������
//#include <iostream>
//// Ψһ��һ�� CMySQL ����
//CMySQL mySQL;
//
//CMySQL::CMySQL()
//{
//}
//
//
//CMySQL::~CMySQL()
//{
//}
//
//void CMySQL::ConnectMySQL() {
//	mysql_init(&mysql); //��ʼ��mysql�ṹ
//
//	const char host[] = "localhost";
//	const char user[] = "root";
//	const char psw[] = "123456";
//	const char table[] = "test";
//	const int port = 3306;
//
//	if (!mysql_real_connect(&mysql, host, user, psw, table, 3306, NULL, 0))
//	{
//		AfxMessageBox(L"\n�������ݿ�ʱ��������!\n");
//	}
//	else {
//		AfxMessageBox(L"\n�������ݿ�ɹ�!\n");
//	}
//	//mysql_close(&mysql); //�ͷ����ݿ�
//}
//
//bool CMySQL::QueryDatabase1()
//{
//	strcpy(query, "select * from mytalbe"); //ִ�в�ѯ��䣬�����ǲ�ѯ���У�user�Ǳ��������ü����ţ���strcpyҲ����  
//	mysql_query(&mysql, "set names gbk"); //���ñ����ʽ��SET NAMES GBKҲ�У�������cmd����������  
//										  //����0 ��ѯ�ɹ�������1��ѯʧ��  
//	if (mysql_query(&mysql, query)) {        // ִ��ָ��Ϊһ���ս�β���ַ�����SQL��ѯ��	
//		printf("Query failed (%s)\n", mysql_error(&mysql));
//		AfxMessageBox(L"\nquery failed!\n");
//		return false;
//	}
//	else {
//		printf("query success\n");
//		AfxMessageBox(L"\nquery success!\n");
//	}
//	//��ȡ�����  
//	if (!(res = mysql_store_result(&mysql)))    //���sql�������󷵻صĽ����  
//	{
//		printf("Couldn't get result from %s\n", mysql_error(&mysql));
//		return false;
//	}
//
//	//��ӡ��������  
//	printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));
//
//	//��ȡ�ֶε���Ϣ  
//	char *str_field[32];  //����һ���ַ�������洢�ֶ���Ϣ
//	for (int i = 0; i < 4; i++)   //����֪�ֶ�����������»�ȡ�ֶ��� 
//	{
//		str_field[i] = mysql_fetch_field(res)->name;	//����һ�������ֶνṹ�����顣
//	}
//	for (int i = 0; i < 4; i++)   //��ӡ�ֶ�  
//		printf("%10s\t", str_field[i]);
//	printf("\n");
//	//��ӡ��ȡ������  
//	while (column = mysql_fetch_row(res))   //����֪�ֶ���������£���ȡ����ӡ��һ��  
//	{
//		printf("%10s\t%10s\t%10s\t%10s\n", column[0], column[1], column[2], column[3]);  //column�������� 
//
//	}
//	return true;
//}
//
////��������  
//bool CMySQL::InsertData()
//{
//	//������취ʵ���ֶ��ڿ���̨�ֶ�����ָ��  
//	strcpy(query, "insert into mytalbe values ('wu', 'M', '1994-09-07','NEU');");
//
//	if (mysql_query(&mysql, query))        //ִ��SQL���  
//	{
//		printf("Query failed (%s)\n", mysql_error(&mysql));
//		return false;
//	}
//	else
//	{
//		printf("Insert success\n");
//		return true;
//	}
//}
//
////�޸�����  
//bool CMySQL::ModifyData()
//{
//	strcpy(query, "update mytalbe set name='zhao' where name='wu'");
//	if (mysql_query(&mysql, query))        //ִ��SQL���  
//	{
//		printf("Query failed (%s)\n", mysql_error(&mysql));
//		return false;
//	}
//	else
//	{
//		printf("Insert success\n");
//		return true;
//	}
//}
////ɾ������  
//bool CMySQL::DeleteData()
//{
//	strcpy(query, "delete from mytalbe where name='zhao'"); 
//	if (mysql_query(&mysql, query))        //ִ��SQL���  
//	{
//		printf("Query failed (%s)\n", mysql_error(&mysql));
//		return false;
//	}
//	else
//	{
//		printf("Insert success\n");
//		return true;
//	}
//}
