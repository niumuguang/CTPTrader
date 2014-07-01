#include "chstimer.h"

CHSTimer::CHSTimer()
{

}

CHSTimer::CHSTimer(char* DataStr, char* TimeStr, int MilliSecStr)
{
    m_StringTimer = TimeStr;
    m_StringDate = DataStr;
    m_MilliSecond = MilliSecStr;

    SplitTimeStr();
    SplitDateStr();
}

bool CHSTimer::isInTradeTime()
{
    if (m_Hour == 0 && m_Minute == 0 && m_Second == 0 && m_MilliSecond == 0)
    {
        return false;
    }
    if (m_Hour == 8)
    {
        if(m_Minute == 59)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    // 9:00 to 11:30
    else if(9 <= m_Hour && m_Hour < 12)
    {
        if (m_Hour== 11)
        {
            if (m_Minute > 30)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        else if (m_Hour == 10)
        {
            if (0 <= m_Minute && m_Minute < 16)
            {
                return true;
            }
            else if( 15<m_Minute && m_Minute<29)
            {
                return false;
            }
            else// 大于等于30分的
            {
                return true;
            }
        }
        else
        {
            return true;
        }
    }
    // 13:30 to 15:00
    else if ( 13 <= m_Hour && m_Hour <= 15)
    {
        if (m_Hour == 13)
        {
            // 25分以后为true
            if (0<=m_Minute && m_Minute<29)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        else if (m_Hour == 15)
        {
            if (m_Minute > 0)
            {
                return false;
            }
            else if(m_Minute == 0)
            {
                if (m_Second == 0)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        //////////////////////////////////////////////////////////////////////////
        // 14:00 返回 在时间范围
        else
        {
            return true;
        }
    }

    else if(m_Hour == 20)
    {
        if (m_Minute == 59)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    // 21:00 to 23:59
    else if(21 <= m_Hour && m_Hour < 24)
    {
        return true;
    }
    // 00:00 to 02:30
    else if (0 <= m_Hour && m_Hour < 3)
    {
        if (m_Hour == 2)
        {
            if ( m_Minute > 30 )
            {
                return false;
            }
            else
            {
                if (m_Minute == 30)
                {
                    if (m_Second == 0)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}
