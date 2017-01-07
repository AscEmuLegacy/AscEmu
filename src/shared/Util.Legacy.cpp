/*
 * AscEmu Framework based on ArcEmu MMORPG Server
 * Copyright (C) 2014-2016 AscEmu Team <http://www.ascemu.org/>
 * Copyright (C) 2008-2012 ArcEmu Team <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Util.hpp"

int32 GetTimePeriodFromString(const char* str)
{
    uint32 time_to_ban = 0;
    char* p = (char*)str;
    uint32 multiplier;
    std::string number_temp;
    uint32 multipliee;
    number_temp.reserve(10);
    while(*p != 0)
    {
        // always starts with a number.
        if(!isdigit(*p))
            break;
        number_temp.clear();
        while(isdigit(*p) && *p != 0)
        {
            number_temp += *p;
            ++p;
        }
        // try to find a letter
        if(*p != 0)
        {
            // check the type
            switch(tolower(*p))
            {
                case 'y':
                    multiplier = TIME_YEAR;        // eek!
                    break;
                case 'm':
                    multiplier = TIME_MONTH;
                    break;
                case 'd':
                    multiplier = TIME_DAY;
                    break;
                case 'h':
                    multiplier = TIME_HOUR;
                    break;
                default:
                    return -1;
                    break;
            }
            ++p;
        }
        else
            multiplier = TIME_MINUTE; // Defaults to MINUTES, if no letter is given
        multipliee = atoi(number_temp.c_str());
        time_to_ban += (multiplier * multipliee);
    }
    return time_to_ban;
}

const char* szDayNames[7] =
{
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

const char* szMonthNames[12] =
{
    "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

void MakeIntString(char* buf, int num)
{
    if(num < 10)
    {
        buf[0] = '0';
        //itoa(num, &buf[1], 10);
        sprintf(&buf[1], "%u", num);
    }
    else
    {
        //itoa(num,buf,10);
        sprintf(buf, "%u", num);
    }
}

void MakeIntStringNoZero(char* buf, int num)
{
    //itoa(num,buf,10);
    sprintf(buf, "%u", num);
}

std::string ConvertTimeStampToString(uint32 timestamp)
{
    int seconds = (int)timestamp;
    int mins = 0;
    int hours = 0;
    int days = 0;
    int months = 0;
    int years = 0;
    if(seconds >= 60)
    {
        mins = seconds / 60;
        if(mins)
        {
            seconds -= mins * 60;
            if(mins >= 60)
            {
                hours = mins / 60;
                if(hours)
                {
                    mins -= hours * 60;
                    if(hours >= 24)
                    {
                        days = hours / 24;
                        if(days)
                        {
                            hours -= days * 24;
                            if(days >= 30)
                            {
                                months = days / 30;
                                if(months)
                                {
                                    days -= months * 30;
                                    if(months >= 12)
                                    {
                                        years = months / 12;
                                        if(years)
                                        {
                                            months -= years * 12;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    char szTempBuf[100];
    std::string szResult;

    if(years)
    {
        MakeIntStringNoZero(szTempBuf, years);
        szResult += szTempBuf;
        szResult += " years, ";
    }

    if(months)
    {
        MakeIntStringNoZero(szTempBuf, months);
        szResult += szTempBuf;
        szResult += " months, ";
    }

    if(days)
    {
        MakeIntStringNoZero(szTempBuf, days);
        szResult += szTempBuf;
        szResult += " days, ";
    }

    if(hours)
    {
        MakeIntStringNoZero(szTempBuf, hours);
        szResult += szTempBuf;
        szResult += " hours, ";
    }

    if(mins)
    {
        MakeIntStringNoZero(szTempBuf, mins);
        szResult += szTempBuf;
        szResult += " minutes, ";
    }

    if(seconds)
    {
        MakeIntStringNoZero(szTempBuf, seconds);
        szResult += szTempBuf;
        szResult += " seconds";
    }

    return szResult;
}

uint32 DecimalToMask(uint32 dec)
{
    return ((uint32)1 << (dec - 1));
}

#ifdef WIN32
static char _StringConversionStorage[2048];
#endif

// win32 only
// cebernic added it
// for multilanguage supports
// --------------------------------------------------------------------------------------------------
SERVER_DECL const char* _StringToUTF8(const char*   pASCIIBuf)
{
#ifdef WIN32
    DWORD     UniCodeLen = MultiByteToWideChar(CP_ACP,   0,   pASCIIBuf,   -1,   0,   0);
    std::vector <wchar_t>   vWCH(UniCodeLen);
    MultiByteToWideChar(CP_ACP,   0,   pASCIIBuf,   -1,   &vWCH[0],   UniCodeLen);
    DWORD   dwUtf8Len = WideCharToMultiByte(CP_UTF8,   0,   &vWCH[0],   UniCodeLen   ,   NULL,   NULL,   NULL,   NULL);
    ASSERT(dwUtf8Len + 1 < 2048);
    memset(_StringConversionStorage, 0, (sizeof(char)*dwUtf8Len) + 1);
    WideCharToMultiByte(CP_UTF8,   0,   &vWCH[0],   UniCodeLen   ,   _StringConversionStorage,   dwUtf8Len,   NULL,   NULL);
    return &_StringConversionStorage[0];
#else
    return &pASCIIBuf[0];
#endif
}
SERVER_DECL const char* _StringToANSI(const char*   pUtf8Buf)
{
#ifdef WIN32
    DWORD   UniCodeLen = MultiByteToWideChar(CP_UTF8,   0,   pUtf8Buf,   -1,   NULL, 0);
    std::vector <wchar_t>   vWCH(UniCodeLen);
    MultiByteToWideChar(CP_UTF8,   0,   pUtf8Buf,   -1,   &vWCH[0]   ,   UniCodeLen);
    DWORD   dwASCIILen = WideCharToMultiByte(CP_ACP,   0,   &vWCH[0],   UniCodeLen   ,   NULL   , NULL   ,   NULL,   NULL);
    ASSERT(dwASCIILen + 1 < 2048);
    memset(_StringConversionStorage, 0, (sizeof(char)*dwASCIILen) + 1);
    WideCharToMultiByte(CP_ACP,   0,   &vWCH[0],   UniCodeLen   ,   _StringConversionStorage,   dwASCIILen,   NULL,   NULL);
    return &_StringConversionStorage[0];
#else
    return &pUtf8Buf[0];
#endif
}

SERVER_DECL bool _IsStringUTF8(const char* str)
{
    int   i;
    unsigned char cOctets;  // octets to go in this UTF-8 encoded character
    unsigned char chr;
    bool  bAllAscii = TRUE;
    long iLen = (long)strlen(str);

    cOctets = 0;
    for(i = 0; i < iLen; i++)
    {

        chr = (unsigned char)str[i];

        if((chr & 0x80) != 0) bAllAscii = FALSE;

        if(cOctets == 0)
        {
            if(chr >= 0x80)
            {
                do
                {
                    chr <<= 1;
                    cOctets++;
                }
                while((chr & 0x80) != 0);

                cOctets--;
                if(cOctets == 0) return FALSE;
            }
        }
        else
        {
            if((chr & 0xC0) != 0x80)
                return FALSE;

            cOctets--;
        }
    }
    if(cOctets > 0)
        return FALSE;
    if(bAllAscii)
        return FALSE;
    return TRUE;

}

volatile long Sync_Add(volatile long* value)
{
#ifdef WIN32
    return InterlockedIncrement(value);
#else
    return __sync_add_and_fetch(value, 1);
#endif
}

volatile long Sync_Sub(volatile long* value)
{
#ifdef WIN32
    return InterlockedDecrement(value);
#else
    return __sync_sub_and_fetch(value, 1);
#endif
}