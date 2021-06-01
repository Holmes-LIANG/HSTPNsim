//
#include "stdafx.h"
//#include "MySQL.h"
//
//#include <stdio.h>  
//#include <WinSock.h>  //一定要包含这个
//#include <iostream>
//// 唯一的一个 CMySQL 对象
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
//	mysql_init(&mysql); //初始化mysql结构
//
//	const char host[] = "localhost";
//	const char user[] = "root";
//	const char psw[] = "123456";
//	const char table[] = "test";
//	const int port = 3306;
//
//	if (!mysql_real_connect(&mysql, host, user, psw, table, 3306, NULL, 0))
//	{
//		AfxMessageBox(L"\n连接数据库时发生错误!\n");
//	}
//	else {
//		AfxMessageBox(L"\n连接数据库成功!\n");
//	}
//	//mysql_close(&mysql); //释放数据库
//}
//
//bool CMySQL::QueryDatabase1()
//{
//	strcpy(query, "select * from mytalbe"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
//	mysql_query(&mysql, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
//										  //返回0 查询成功，返回1查询失败  
//	if (mysql_query(&mysql, query)) {        // 执行指定为一个空结尾的字符串的SQL查询。	
//		printf("Query failed (%s)\n", mysql_error(&mysql));
//		AfxMessageBox(L"\nquery failed!\n");
//		return false;
//	}
//	else {
//		printf("query success\n");
//		AfxMessageBox(L"\nquery success!\n");
//	}
//	//获取结果集  
//	if (!(res = mysql_store_result(&mysql)))    //获得sql语句结束后返回的结果集  
//	{
//		printf("Couldn't get result from %s\n", mysql_error(&mysql));
//		return false;
//	}
//
//	//打印数据行数  
//	printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));
//
//	//获取字段的信息  
//	char *str_field[32];  //定义一个字符串数组存储字段信息
//	for (int i = 0; i < 4; i++)   //在已知字段数量的情况下获取字段名 
//	{
//		str_field[i] = mysql_fetch_field(res)->name;	//返回一个所有字段结构的数组。
//	}
//	for (int i = 0; i < 4; i++)   //打印字段  
//		printf("%10s\t", str_field[i]);
//	printf("\n");
//	//打印获取的数据  
//	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
//	{
//		printf("%10s\t%10s\t%10s\t%10s\n", column[0], column[1], column[2], column[3]);  //column是列数组 
//
//	}
//	return true;
//}
//
////插入数据  
//bool CMySQL::InsertData()
//{
//	//可以想办法实现手动在控制台手动输入指令  
//	strcpy(query, "insert into mytalbe values ('wu', 'M', '1994-09-07','NEU');");
//
//	if (mysql_query(&mysql, query))        //执行SQL语句  
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
////修改数据  
//bool CMySQL::ModifyData()
//{
//	strcpy(query, "update mytalbe set name='zhao' where name='wu'");
//	if (mysql_query(&mysql, query))        //执行SQL语句  
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
////删除数据  
//bool CMySQL::DeleteData()
//{
//	strcpy(query, "delete from mytalbe where name='zhao'"); 
//	if (mysql_query(&mysql, query))        //执行SQL语句  
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
