#ifndef CHSTIMER_H
#define CHSTIMER_H

#include <QString>
#include <QStringList>
#include <vector>

using namespace std;

class CHSTimer
{
public:
    CHSTimer();

    CHSTimer( char* DataStr, char* TimeStr, int MilliSecStr );

public:
    bool isInTradeTime();

private:
    void SplitTimeStr(){
        int StringLen = m_StringTimer.length();
        //
        QString temp_Hour = m_StringTimer.mid(0,2);
        m_Hour= temp_Hour.toInt();
        QString temp_Minute = m_StringTimer.mid(3,2);
        m_Minute = temp_Minute.toInt();
        QString temp_Second = m_StringTimer.mid(6,2);
        m_Second = temp_Second.toInt();

    };

    void SplitDateStr(){
        vector<QString> res;

        QString strArr = m_StringDate;
        QString strSplit = "-";
        QStringList* arr = new QStringList;
        int iStart = 0 , iFind = 0;
        do
        {

            iFind = strArr.indexOf(strSplit,iStart);
            if(iFind == -1)
                arr->append(strArr.mid(iStart));
                //arr->Add(strArr.Mid(iStart));
            else
                arr->append(strArr.mid(iStart , iFind - iStart));
            iStart = iFind + 1;
        }while(iFind != -1);

        //arr->GetCount()
        for (int i=0; i<arr->length();i++)
        {
            res.push_back(arr->at(i));
        }
        // 用- 分割

        QString tempStr = res.at(0);
        //QString temp_Year = m_StringDate.Mid(0,4);
        m_Year= m_StringDate.mid(0,4).toInt();
        //QString temp_Month = m_StringDate.Mid(5,2);
        m_Month = m_StringDate.mid(4,2).toInt();
        //QString temp_Day = m_StringDate.Mid(8,2);
        m_Day = m_StringDate.mid(6,2).toInt();
    }

    bool InFiveMinute(CHSTimer otherTimer){
        if (otherTimer.m_Hour == m_Hour)
        {
            // 同一小时之内
            if ( m_Minute = otherTimer.m_Minute)
            {
            }
            else if (m_Minute != otherTimer.m_Minute)
            {

            }
        }
        else if ( abs(otherTimer.m_Hour - m_Hour) == 1 )
        {
            // 跨小时
        }

    }

public:
    int m_Year;
    int m_Month;
    int m_Day;
    int m_Hour;
    int m_Minute;
    int m_Second;
    int m_MilliSecond;

    QString m_StringTimer;
    QString m_StringDate;

};

#endif // CHSTIMER_H
