#pragma once

#include "HSTPN_SYS.h"

class CMySQL
{
public:
	CMySQL();

	~CMySQL();

	MYSQL mysql;//mySql

	MYSQL_FIELD *fd;  //字段列数组
	char field[32][32];  //存字段名二维数组
	MYSQL_RES *res; //这个结构代表返回行的一个查询结果集
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列
	char query[150]; //查询语句

	void ConnectMySQL();    //连接数据库
	void FreeConnect();
 
	bool InsertData();		//增
	bool DeleteData();		//删
	bool ModifyData();		//改
	bool QueryDatabase1();  //查询1  
	bool QueryDatabase2();  //查询2 
};

extern CMySQL mySQL;