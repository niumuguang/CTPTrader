#include "cmysql_api.h"

#include <CTP/api/trade/win/public/ThostFtdcUserApiStruct.h>
//#include "CShareFun.h"
#include <qstring.h>
#include <chstimer.h>
//#include <config.h>


using namespace std;

CMySQL_Api::CMySQL_Api()
{
    mysql_init(&m_mysql);
}

CMySQL_Api::CMySQL_Api(string username, string password, string database, string tablename)
{
    mysql_init(&m_mysql);
    m_user = username;
    m_passwd = password;
    m_database = database;
    m_table = tablename;
    //this->String2CharPt()
    m_address = "localhost";
    mysql_real_connect(&m_mysql, m_address.c_str(), m_user.c_str(),
                       m_passwd.c_str(), m_database.c_str(), 3306, NULL, 0);

}

/****************************/
CMySQL_Api::CMySQL_Api(string ipaddress, int port,
                       string username, string password,
                       string database, string tablename)
{
    mysql_init(&m_mysql);
    m_user = username;
    m_passwd = password;
    m_database = database;
    m_table = tablename;
    m_address = ipaddress;
    m_port = port;
    mysql_real_connect(&m_mysql, m_address.c_str(), m_user.c_str(),
                       m_passwd.c_str(),
                       m_database.c_str(), port, NULL, 0);

}

CMySQL_Api::~CMySQL_Api()
{
    mysql_close(&m_mysql);
}


bool CMySQL_Api::IsConnect()
{
    if( !&m_mysql )
    {
        return false;
    }
    else
    {
        return true;
    }
}

int CMySQL_Api::Testing()
{
    string sql = "insert into ag1409 (InstrumentID) values ('java1');";
    int res = mysql_query(&m_mysql, sql.c_str());
    return res;
}


bool CMySQL_Api::WriteDB(CThostFtdcDepthMarketDataField *DepthMarketData)
{
    //HSTime tmpTime;
    int res = 0;
    CHSTimer tmpTime(DepthMarketData->TradingDay,DepthMarketData->UpdateTime,
                     DepthMarketData->UpdateMillisec);


    string sq = "insert into "+(string)DepthMarketData->InstrumentID+" (InstrumentID,TradingDay,UpdateTime,UpdateMillisec,LastPrice,AskPrice1,BidPrice1,AskVolume1,BidVolume1,AskPrice2,BidPrice2,AskVolume2,BidVolume2,AskPrice3,BidPrice3,AskVolume3,BidVolume3,AskPrice4,BidPrice4,AskVolume4,BidVolume4,AskPrice5,BidPrice5,AskVolume5,BidVolume5,AveragePrice,ClosePrice,CurrDelta,ExchangeID,ExchangeInstID,HighestPrice,LowestPrice,UpperLimitPrice,LowerLimitPrice,OpenInterest,OpenPrice,PreClosePrice,PreDelta,PreOpenInterest,PreSettlementPrice,Volume) values ('" +
            (string)DepthMarketData->InstrumentID +"','"+DepthMarketData->TradingDay+"','"+
            DepthMarketData->UpdateTime+"'," + this->Int2String(DepthMarketData->UpdateMillisec) +"," +
            this->Int2String(DepthMarketData->LastPrice) + ","+
            Int2String(DepthMarketData->AskPrice1) + ","+ Int2String(DepthMarketData->BidPrice1) + ","+Int2String(DepthMarketData->AskVolume1) + "," +
            Int2String(DepthMarketData->BidVolume1) + "," +Int2String(DepthMarketData->AskPrice2) + "," +Int2String(DepthMarketData->BidPrice2) + "," +
            Int2String(DepthMarketData->AskVolume2) + "," +Int2String(DepthMarketData->BidVolume2) + "," +Int2String(DepthMarketData->AskPrice3) + "," +
            Int2String(DepthMarketData->BidPrice3) + "," +Int2String(DepthMarketData->AskVolume3) + "," +Int2String(DepthMarketData->BidVolume3) + "," +
            Int2String(DepthMarketData->AskPrice4) + "," +Int2String(DepthMarketData->BidPrice4) + "," +Int2String(DepthMarketData->AskVolume4) + "," +
            Int2String(DepthMarketData->BidVolume4) + "," + Int2String(DepthMarketData->AskPrice5) + "," +Int2String(DepthMarketData->BidPrice5) + "," +
            Int2String(DepthMarketData->AskVolume5) + "," +Int2String(DepthMarketData->BidVolume5) + "," +
            Int2String(DepthMarketData->AveragePrice) + "," +Int2String(DepthMarketData->ClosePrice) + "," +Double2String(DepthMarketData->CurrDelta)+ ",'" +
            DepthMarketData->ExchangeID + "','" + DepthMarketData->ExchangeInstID + "'," + Int2String(DepthMarketData->HighestPrice) + "," +
            Int2String(DepthMarketData->LowestPrice) + "," +Int2String(DepthMarketData->UpperLimitPrice) + "," +
            Int2String(DepthMarketData->LowerLimitPrice) + "," +Int2String(DepthMarketData->OpenInterest) + "," +
            Int2String(DepthMarketData->OpenPrice) + "," +Int2String(DepthMarketData->PreClosePrice) + "," +
            Double2String(DepthMarketData->PreDelta) + "," + Int2String(DepthMarketData->PreOpenInterest) + "," +
            Int2String(DepthMarketData->PreSettlementPrice) + "," + Int2String(DepthMarketData->Volume) +
            ");";

    if(tmpTime.isInTradeTime() == true)
    {
        res = mysql_query(&m_mysql, sq.c_str());
    }
    return (bool)res;
}

char *CMySQL_Api::String2CharPt(string inputData)
{
    string str=inputData;
    char * pstr=new char [str.length()+1];
    return pstr;
}

string CMySQL_Api::Int2String(int inputData)
{
    string res = "0";
    if( inputData < -1147480000)
    {
        return res;
    }
    int n = inputData;
    QString qs = QString::number(n, 10);
    res = qs.toStdString();
    return res;
}

string CMySQL_Api::Double2String(double inputData)
{
    string res = "0";
    if( inputData < -1147480000 )
    {
        return res;
    }
    double num = inputData;
    QString data = QString("%1").arg(num);
    res = data.toStdString();
    return res;
}


