/**************************************************
*		�ļ���  ��WinAppender.cpp
*		��Ҫ��������дlog4cplus��Appender���append�ӿڣ�ʹ��־�������������

*		��ǰ�汾��1.0
*		����    ��ʯϲ
*		������ڣ�2010-11-10
*		�޶�˵����
****************************************************/
#include "StdAfx.h"
#include "WinAppender.h"

#include <iostream>
#include <log4cplus/helpers/property.h>
#include "../AppFrame/AppFrameDef.h"

using namespace std;
using namespace log4cplus::helpers;
using namespace AppFrame;
namespace log4cplus
{

    const int MaxLogContentLen = 65536;

    WinAppender::WinAppender(void)
    {
    }

    WinAppender::~WinAppender(void)
    {
        destructorImpl();
    }


    WinAppender::WinAppender(
        const log4cplus::helpers::Properties& properties)
        : Appender(properties)
    {
        
    }

    void WinAppender::close()
    {
        closed = true; 
    }

    void WinAppender::append(const log4cplus::spi::InternalLoggingEvent& event)
    {
        tostringstream buf;
        layout->formatAndAppend(buf, event);
        tstring sz = buf.str();

        HWND wnd = GetWnd(LOG);
        static DWORD last = timeGetTime();
        if (timeGetTime() - last > 300000 ||         // ����ʱ����
            (::GetWindowTextLength(wnd)) >= MaxLogContentLen)     // ���ݳ���64K
        {
            last = timeGetTime();
            ::SetWindowText(wnd, "");
        }
        char strText[MaxLogContentLen];
        ::GetWindowText(wnd,strText,MaxLogContentLen);
        strcat(strText,"\r\n");
        strcat(strText,sz.c_str());
        ::SetWindowText(wnd,strText);

        int nLine = ::SendMessage(wnd, EM_GETLINECOUNT, 0, 0);
        ::SendMessage(wnd, EM_LINESCROLL, 0, nLine);
    }
}



namespace
{
    template <typename Fac, typename Reg>
    static void reg_factory (Reg & reg)
    {
        reg.put (std::auto_ptr<typename Reg::product_type> (new Fac));
    }

} // namespace


void initializeWin32LogFactory()
{
    AppenderFactoryRegistry& reg = getAppenderFactoryRegistry();
    reg_factory<WinAppenderFactory> (reg);

}