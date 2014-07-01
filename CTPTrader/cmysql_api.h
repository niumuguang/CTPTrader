#ifndef CMYSQL_API_H
#define CMYSQL_API_H

#include "string.h"
#include <mysql/mysql.h>
#include <iostream>
#include <CTP/api/trade/win/public/ThostFtdcUserApiStruct.h>

using namespace std;

class CMySQL_Api
{
private:
    CMySQL_Api();
public:
    CMySQL_Api(string username, string password, string database,
               string tablename);
    CMySQL_Api(string ipaddress, int port, string username,
               string password, string database,string tablename);

    ~CMySQL_Api();

private:
    string m_user;
    string m_passwd;
    string m_database;
    string m_table;
    string m_address;
    int m_port;
    MYSQL m_mysql;

public:
    bool IsConnect();
    int Testing();

    bool WriteDB(CThostFtdcDepthMarketDataField* DepthMarketData);
    char* String2CharPt(string inputData);
    string Int2String(int inputData);
    string Double2String(double inputData);
};

#endif // CMYSQL_API_H
