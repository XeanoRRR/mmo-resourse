#include "StdAfx.h"

void LoadLog(const char* name, const char* file, uint state)
{
    static DWORD start = timeGetTime(), delta = 0;
    using namespace AppFrame;
   // LogLevel lvl = Error;
    const char* s = "GAME_COMMON_FAILED";
    switch (state)
    {
    case 0:
        start = timeGetTime();
        //lvl = Info;
        s = "GAME_COMMON_ING";
        break;
    case 1: // ����������ģ�鶼���ڼ���ǰ�����־
        delta = timeGetTime() - start;
        start = timeGetTime();
        Log4c::Trace(ROOT_MODULE, "                                               elapse time: %u.%03u (sec)",delta / 1000, delta % 1000);
         //lvl = Standard;
         s = "GAME_COMMON_OK";
        break;
    case 2:
       // lvl = Warning;
        break;
    }
    Log4c::Trace(ROOT_MODULE, "%s %s (%s) %s",
         GetText("GAME_COMMON_LOAD"),
         GetText(name),
         file,
         GetText(s));
}